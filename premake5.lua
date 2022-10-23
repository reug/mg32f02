-- Подготовка Makefile:
--   premake5 gmake
-- Сборка:
--   make <цель>
-- Сборка с выводом командных строк:
--   make <цель> verbose=1

-- Chips: "MG32F02A032" или "MG32F02A128"
function setup_paths(CHIP)

-- Device Family Pack path:
  DFP_PATH =  "/opt/arm/megawin/DFP/Device/MG32x02z/"

-- Development Kit Drivers path
  DRV_PATH =  "/opt/arm/megawin/DFP/Device/MG32x02z/MG32F02A_Driver/"

-- Development Kit Middlewares path
  MID_PATH =  "/opt/arm/megawin/DFP/Device/MG32x02z/MG32F02A_Middleware/"

  DFP_INC = {DFP_PATH.."Common/Initial/Include", DFP_PATH..CHIP.."/Include", DFP_PATH.."Package/"..CHIP}

  DRV_INC =   DRV_PATH.."Include"
  DRV_SRC =   DRV_PATH.."Source/"

  MID_INC =   MID_PATH.."Include"
  MID_SRC =   MID_PATH.."Source/"

  defines {CHIP}

  includedirs {DFP_INC, DRV_INC, MID_INC}

end

workspace "Megawin"

-- MCPU name:
  MCPU = "cortex-m0"

  configurations {"Debug"}
  symbols "On"

-- CMSIS paths:
  CMSIS_PATH = "/opt/arm/CMSIS/"
  CMSIS_INC = {CMSIS_PATH.."CMSIS/Core/Include"}

-- Linker script dir
  LDSCRIPT_PATH = "ld"

-- GCC toolchain prefix
  GCCPREFIX = "/opt/gcc-arm/bin/arm-none-eabi-"

  includedirs {".", "src", CMSIS_INC}
  buildoptions {"-mthumb", "-mcpu="..MCPU, "-Wall", "-O3", "-g", "-fno-common",
    "-ffunction-sections", "-fdata-sections", "-ffreestanding"}
    --"-ffunction-sections", "-fdata-sections", "-ffreestanding", "-flto"}
  linkoptions {
    "-mthumb", "-mcpu="..MCPU,
    "-L"..LDSCRIPT_PATH,
    "-Wl,--undefined=arm_stack_area",
    "-Wl,-Map,%{cfg.buildtarget.relpath}.map"
    --"-flto"
  }
  targetdir "bin"
  postbuildcommands {
    GCCPREFIX.."objcopy -O binary %{cfg.buildtarget.relpath} %{cfg.buildtarget.relpath}.bin",
    --GCCPREFIX.."objdump -Mforce-thumb -d -l -S -G %{cfg.buildtarget.relpath} > %{cfg.buildtarget.relpath}.lst",
    GCCPREFIX.."objdump -Mforce-thumb -d -S -G %{cfg.buildtarget.relpath} > %{cfg.buildtarget.relpath}.lst",
    GCCPREFIX.."size %{cfg.buildtarget.relpath}"
  }

  gccprefix (GCCPREFIX)


project "svr32"
  kind "ConsoleApp"
  language "C"
  setup_paths ("MG32F02A032")
  defines {"HWCF_A032"}
  files {"src/init.c", "src/startup.c", "src/main.c", "src/svr.c", "src/ulib.c"}
  linkoptions {"-nostdlib"}
  linkoptions { "-Wl,--gc-sections"}
  linkoptions {"-T mg32f02a032_svr.ld"}

project "svr64"
  kind "ConsoleApp"
  language "C"
  setup_paths ("MG32F02A128")
  defines {"HWCF_A064"}
  files {"src/init.c", "src/startup.c", "src/main.c", "src/svr.c", "src/ulib.c"}
  linkoptions {"-nostdlib"}
  linkoptions { "-Wl,--gc-sections"}
  linkoptions {"-T mg32f02a064_svr.ld"}

project "app"
  kind "ConsoleApp"
  language "C"
  setup_paths ("MG32F02A128")
  defines {"HWCF_A064"}
  defines {"I2C_DEBUG"}
  --files {"src/app.c", "src/utils.c", "test/adc_test.c", "src/uart.c", "src/adc.c", "src/cmp.c", "src/init.c", "src/ulib.c"}
  --files {"src/app.c", "src/utils.c", "src/uart.c", "test/st_rtc_test.c", "src/rtc.c", "src/init.c", "src/ulib.c"}
  --files {"src/app.c", "src/utils.c", "src/uart.c", "test/wdt_test.c", "src/wdt.c", "test/st_rtc_test.c", "src/rtc.c","src/init.c", "src/ulib.c"}
  --files {"src/app.c", "src/utils.c", "src/uart.c", "test/timer_test.c", "src/timer.c", "src/init.c", "src/ulib.c"}
  --files {"src/app.c", "src/utils.c", "src/uart.c", "test/timer_test.c", "src/timer.c", "src/init.c", "src/ulib.c", MID_SRC.."MG32x02z_GPL_MID.c"}
  --files {"src/app.c", "src/utils.c", "src/uart.c", "test/timer_test.c", "src/timer.c", "src/init.c", "src/ulib.c", "src/nco.c", MID_SRC.."MG32x02z_GPL_MID.c"}
  --files {"src/app.c", "src/utils.c", "src/uart.c", "test/i2c_test.c", "src/i2c.c", "src/init.c", "src/ulib.c", MID_SRC.."MG32x02z_GPL_MID.c"}
  files {"src/app.c", "src/utils.c", "src/uart.c", "test/i2c_test.c", "src/i2c.c", "src/init.c", "src/ulib.c", MID_SRC.."MG32x02z_GPL_MID.c", "ic/ds3231.c"}
  linkoptions {"-nostdlib"}
  linkoptions { "-Wl,--gc-sections"}
  linkoptions {"-T mg32f02a064_app.ld"}

project "clock"
  kind "ConsoleApp"
  language "C"
  setup_paths ("MG32F02A128")
  defines {"HWCF_A064"}
  defines {"I2C_DEBUG"}
  files {"src/app_clock.c", "src/utils.c", "src/uart.c", "test/i2c_test.c", "src/init.c", "src/ulib.c", MID_SRC.."MG32x02z_GPL_MID.c", "ic/tm1637.c", "ic/ds3231.c", "src/i2c.c"}
  linkoptions {"-nostdlib"}
  linkoptions { "-Wl,--gc-sections"}
  linkoptions {"-T mg32f02a064_app.ld"}

project "slave"
  kind "ConsoleApp"
  language "C"
  setup_paths ("MG32F02A032")
  defines {"HWCF_A032"}
  defines {"I2C_DEBUG"}
  files {"src/app_slave.c", "src/utils.c", "src/uart.c", "src/init.c", "src/ulib.c", MID_SRC.."MG32x02z_GPL_MID.c", "ic/ds3231.c", "src/i2c.c"}
  linkoptions {"-nostdlib"}
  linkoptions { "-Wl,--gc-sections"}
  linkoptions {"-T mg32f02a032_app.ld"}

-- SPI Master for MG32F02A064
project "spim"
  kind "ConsoleApp"
  language "C"
  setup_paths ("MG32F02A128")
  defines {"HWCF_A064"}
  defines {"SPI_DEBUG"}
  files {"src/app.c", "src/utils.c", "src/uart.c", "test/spi_test.c", "src/spi.c", "src/init.c", "src/ulib.c", MID_SRC.."MG32x02z_GPL_MID.c", "ic/enc28j60.c", "src/ethernet.c", "src/dma.c"}
  linkoptions {"-nostdlib"}
  linkoptions { "-Wl,--gc-sections"}
  linkoptions {"-T mg32f02a064_app.ld"}
