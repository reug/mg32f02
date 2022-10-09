# OpenOCD/Tcl Library for Megawin MG32F02 chips
# (C) 2022, reug@mail.ru

set VERSION "2.2.3"

set MEM_STA     0x4D000000
set MEM_INT     0x4D000004
set MEM_KEY     0x4D00000C
set MEM_CR0     0x4D000010
set MEM_CR1     0x4D000014
set MEM_SKEY    0x4D00001C
set MEM_IAPSZ   0x4D000028

set OOCD_INIT_RESET {}

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
  write_memory $MEM_CR0 32 [expr {($w & 0xFFFFFF0F) | ($mode << 4)}]
}


proc mem_ap_skey {byte} {
  global MEM_SKEY
  set w [read_memory $MEM_SKEY 32 1]
  write_memory $MEM_SKEY 32 [expr {($w & 0xFFFFFF00) | $byte}]
}


# Setup for single write
proc mem_ap_setwrite_single {} {
  mem_ap_skey 0x46
  mem_ap_skey 0xB9
}


# Setup for multiple write
proc mem_ap_setwrite_multiple {} {
  mem_ap_skey 0x46
  mem_ap_skey 0xBE
}


# Lock flash access for multiple write
proc mem_ap_lock {} {
  mem_ap_skey 0x64
  mem_lock
}

# Waiting ready state
proc mem_ap_wait {} {
  global MEM_STA
  # Check MEM_EOPF flag...
  while {!([read_memory $MEM_STA 32 1] & 2)} {after 1}
  # Clear MEM_EOPF flag:
  write_memory $MEM_STA 32 2  
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


# Override OpenOCD internal procedure
proc init_reset {mode} {
  global OOCD_INIT_RESET
  set OOCD_INIT_RESET $mode
}


# Verify memory with file. Returns 0 if success, amount of error words else
proc mem_ap_verify {fname addr {size 0}} {
  global DATA
  set ew 0
  set nb [read_binfile $fname $size]
  set a $addr
  set nw [expr {$nb/4}]
  for {set i 0} {$i < $nw} {incr i} {
    set w [lindex $DATA $i]
    if [catch {
      if {[read_memory $a 32 1] != $w} {incr ew}
    }] {
      mem_status_print
      break
    }
    incr a 4
  }
  return $ew
}


# Flash binary file to AP area.
# fname - filename, addr - target's begin address, size - bytes to flash
proc mem_ap_flash {fname addr {size 0}} {
  global DATA OOCD_INIT_RESET PAGESIZE
  if {$PAGESIZE > 0} {set PS $PAGESIZE} {
    echo "PAGESIZE is not set! Using 512"
    set PS 512
  }
  if {($addr % $PS) != 0} {
    error "addr must be aligned to $PS bytes page"
  }

  set nb [read_binfile $fname $size]
  halt

  echo "ERASE..."
  # Init for erase operation
  if [catch {mem_ap_init 2}] {
    print_memstatus
    return
  }
  # Setup for multibyte
  #if [catch {mem_ap_setwrite_multiple}] {
    #print_memstatus
    #return
  #}
  # Number of pages to erase:
  set np [expr {$nb/$PS + ($nb % $PS ? 1 : 0)}]
  echo "Pages to erase: $np"
  for {set i 0} {$i < $np} {incr i} {
    set a [expr {$addr + $i * $PS}]
    echo "page $i, addr: [format "0x%08X" $a]"
    if [catch {
      mem_ap_setwrite_single
      poll off
      write_memory $a 32 0xffffffff
      # Delay for erase operation: 10 for A064/128, 20 for A032 (page 2 times larger).
      after 20
      poll on
      #mem_ap_wait
    }] {
      print_memstatus
      break
    }
  }

  #reset
  ##set OOCD_INIT_RESET {}
  #echo "Waiting MCU ready after reset..."
  #vwait OOCD_INIT_RESET
  #halt

  echo "WRITE..."
  # Init for write operation
  if [catch {mem_ap_init 1}] {
    print_memstatus
    return
  }
  # Setup for multibyte
  if [catch {mem_ap_setwrite_multiple}] {
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

  sleep 10
  mem_ap_lock
  reset
  echo "Waiting MCU ready after reset..."
  vwait OOCD_INIT_RESET
  echo "VERIFY..."
  set ew [mem_ap_verify $fname $addr]
  if {$ew} {echo "VERIFY ERROR!!! $ew words mismatch!!!"} {echo "Success"}
  echo "Done"
}

echo "MG32F02 Tcl library Version $VERSION by reug"
echo "PAGESIZE: $PAGESIZE"
