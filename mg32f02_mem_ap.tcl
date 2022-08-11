# OpenOCD/Tcl Library for Megawin MG32F02 chips
# (C) 2022, reug@mail.ru
# Version: 2.1.0

set MEM_STA     0x4D000000
set MEM_INT     0x4D000004
set MEM_KEY     0x4D00000C
set MEM_CR0     0x4D000010
set MEM_CR1     0x4D000014
set MEM_SKEY    0x4D00001C
set MEM_IAPSZ   0x4D000028

# Gets MEM_STA register
proc mem_status {} {
  global MEM_STA
  return [read_memory $MEM_STA 32 1]
}

# Print MEM_STA and flags
proc print_memstatus {} {
  global MEM_STA MEM_KEY
  set w [read_memory $MEM_STA 32 1]
  set f {}
  if [expr {$w & 0x00010000}] {lappend f "IAPSEF"}
  if [expr {$w & 0x00000040}] {lappend f "RPEF"}
  if [expr {$w & 0x00000020}] {lappend f "WPEF"}
  if [expr {$w & 0x00000010}] {lappend f "IAEF"}
  if [expr {$w & 0x00000002}] {lappend f "EOPF"}
  if [expr {$w & 0x00000001}] {lappend f "FBUSYF"}

  set w [read_memory $MEM_KEY 32 1]
  if {$w & 0x0000FFFF} {set k1 "LOCK"} {set k1 "UNLOCK"}
  if {$w & 0xFFFF0000} {set k2 "LOCK"} {set k2 "UNLOCK"}

  echo "MEM status:$w flags:\{$f\} key=$k1 key2=$k2"
}

# Print important fields in OB0, ORx and MEM
proc print_memcfg {} {
  global MEM_CR0 MEM_CR1
  set w [read_memory 0x1FF10000 32 1]
  set bootms [expr {~$w & 3}]
  set lock [expr {($w >> 2) & 1}]
  set w [read_memory 0x1FF10004 32 1]
  set iaps [expr {(~$w >> 8) & 0xFFFF}]
  set w [read_memory 0x1FF10008 32 1]
  set isps [expr {(~$w >> 8) & 0xFFFF}]
  echo "OB0: BOOT_MS=$bootms LOCK_DIS=$lock IAP_SIZE=$iaps ISP_SIZE=$isps"

  set w [read_memory 0x4FF00010 32 1]
  set bootms [expr {$w & 3}]
  set lock [expr {($w >> 8) & 1}]
  set w [read_memory 0x4FF00014 32 1]
  set iaps [expr {($w >> 8) & 0xFFFF}]
  set w [read_memory 0x4FF00018 32 1]
  set isps [expr {($w >> 8) & 0xFFFF}]
  echo "ORx: BOOT_MS=$bootms LOCK_DIS=$lock IAP_SIZE=$iaps ISP_SIZE=$isps"

  set w [read_memory $MEM_CR0 32 1]
  set bootms [expr {($w >> 16) & 3}]
  set w [read_memory $MEM_CR1 32 1]
  set iren [expr {($w >> 9) & 1}]
  set iwen [expr {($w >> 8) & 1}]
  echo "MEM: BOOT_MS=$bootms ISP_REN=$iren ISP_WEN=$iwen"
}


# Unlock Register Protection by KEY
proc mem_unlock {} {
  global MEM_KEY
  write_memory $MEM_KEY 32 0xA217
}


# Unlock Register Protection by KEY2
proc mem_unlock2 {} {
  global MEM_KEY
  write_memory $MEM_KEY 32 0xA217A217
}


# Lock Register Protection
proc mem_lock {} {
  global MEM_KEY
  write_memory $MEM_KEY 32 0x11111111
}


# Initial sequence. mode=1 - write, mode=2 - erase
proc mem_ap_init {mode} {
  global MEM_CR0 MEM_CR1

  mem_unlock

  # Enable memory controller (MEM_EN=1)
  set w [read_memory $MEM_CR0 32 1]
  write_memory $MEM_CR0 32 [expr {$w | 1}]

  # Set Flash Write Enable (MEM_AP_WEN=1)
  set w [read_memory $MEM_CR1 32 1]
  write_memory $MEM_CR1 32 [expr {$w | 1}]

  # Set Flash Access Mode (MEM_MDS = 1 or 2)
  set w [read_memory $MEM_CR0 32 1]
  write_memory $MEM_CR0 32 [expr {$w | ($mode << 4)}]
}


# Setup for write. mode - FW_SINGLE or FW_MULTI
proc mem_ap_setwrite {mode} {
  global MEM_SKEY

  # Unlock Flash Access Sequence Key (MEM_SKEY) = 0x46
  write_memory $MEM_SKEY 32 0x46

  # (5) Single Write Access or Multiple Write Access
  #   (a) Single Write Enable (MEM_SKEY) = 0xB9
  #   (b) Multiple Write Enable ~ (MEM_SKEY) = 0xBE
  write_memory $MEM_SKEY 32 $mode
}


# Lock flash access for multiply write
proc mem_ap_lock {} {
  global MEM_SKEY
  write_memory $MEM_SKEY 32 0x64
  mem_lock
}


# ISP read enable
proc mem_isp_ren {} {
  global MEM_CR0 MEM_CR1
  mem_unlock2
  # Enable memory controller (MEM_EN=1)
  set w [read_memory $MEM_CR0 32 1]
  write_memory $MEM_CR0 32 [expr {$w | 1}]  

  # Set MEM_ISP_REN bit
  set w [read_memory $MEM_CR1 32 1]
  write_memory $MEM_CR1 32 [expr {$w | (1 << 9)}]

}


# Read binary file to global list 'DATA'.
# Return: number of bytes
proc read_binfile {fname {size 0}} {
  global DATA
  if {$size==0} {set size [file size $fname]}
  if {($size % 2) != 0} {
    error "size must be multiply of 2"
  }
  # Do file complement to 4 bytes
  set fadd [expr {$size % 4}]

  echo "Reading $size bytes..."
  set f [open $fname rb]
  set DATA {}
  set n [expr {$size/4}]
  for {set i 0} {$i<$n} {incr i} {
    set v [read $f 4]
    lappend DATA [unpack $v -uintle 0 32]
  }
  if {$fadd==2} {
    echo "Warning: appended 2 bytes (zero)"
    set v [read $f 2]
    lappend DATA [unpack $v -uintle 0 16]
  }
  close $f
  return $size
}


# Flash binary file to AP area.
# fname - filename, addr - target's begin address, size - bytes to flash
proc mem_ap_flash {fname addr {size 0}} {
  global DATA
  if {($addr % 1024) != 0} {
    error "addr must be aligned to 1K page"
  }
  set nb [read_binfile $fname $size]
  halt
  echo "Setup for erase..."
  # Init for erase operation
  if [catch {mem_ap_init 2}] {
    print_memstatus
    return
  }
  # Setup for multibyte
  if [catch {mem_ap_setwrite 0xBE}] {
    print_memstatus
    return
  }
  # Number of pages to erase:
  set np [expr {$nb/1024 + ($nb%1024 ? 1 : 0)}]
  echo "Pages: $np"
  for {set i 0} {$i < $np} {incr i} {
    set a [expr {$addr+$i*1024}]
    echo "addr: [format "0x%08X" $a]"
    if [catch {write_memory $a 32 0xffffffff}] {
      print_memstatus
      break
    }
  }
  reset
  halt
  echo "Write..."
  # Init for write operation
  if [catch {mem_ap_init 1}] {
    print_memstatus
    return
  }
  # Setup for multibyte
  if [catch {mem_ap_setwrite 0xBE}] {
    print_memstatus
    return
  }
  set a $addr
  set nw [expr {$nb/4}]
  for {set i 0} {$i < $nw} {incr i} {
    set w [lindex $DATA $i]
    if [catch {write_memory $a 32 $w}] {
      mem_status_print
      break
    }
    incr a 4
  }
  mem_ap_lock
  sleep 250
  reset
  echo "Done"
}

