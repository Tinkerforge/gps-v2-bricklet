CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

SET(PROJECT_NAME gps-v2-bricklet)
SET(CHIP_FAMILY SAM0)
SET(CHIP SAMD09D14A)
SET(MCU cortex-m0plus\ -mthumb)
PROJECT(${PROJECT_NAME})

# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
#SET(OPTIMIZATION_LEVEL 0)
#SET(DEBUG "-g -ggdb")

SET(OPTIMIZATION_LEVEL s)
SET(DEBUG "")

SET(CMAKE_BUILD_TYPE None)

INCLUDE_DIRECTORIES(
	"${PROJECT_SOURCE_DIR}/src/"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/asf_includes"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/thirdparty/CMSIS/Include/"
)

# find source files
SET(SOURCES
	"${PROJECT_SOURCE_DIR}/src/main.c"
	"${PROJECT_SOURCE_DIR}/src/minmea.c"
	"${PROJECT_SOURCE_DIR}/src/firefly_x1.c"
	"${PROJECT_SOURCE_DIR}/src/communication.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/protocols/tfp/tfp.c"
#	"${PROJECT_SOURCE_DIR}/src/bricklib2/logging/logging.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/tinydma.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/bootloader.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/startup/startup_samd09.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/uartbb/uartbb.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/hal/system_timer/system_timer.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/common/utils/interrupt/interrupt_sam_nvic.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/port/port.c"
#	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/wdt/wdt.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf_overwrite/sam0/drivers/system/clock/clock_samd09_d10_d11/clock.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/system/clock/clock_samd09_d10_d11/gclk.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/system/interrupt/system_interrupt.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/system/pinmux/pinmux.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/utils/syscalls/gcc/syscalls.c"
#	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/sercom/usart/usart.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf_overwrite/sam0/drivers/sercom/sercom.c"
	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/sam0/drivers/sercom/spi/spi.c"
#	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/common/utils/stdio/write.c"
#	"${PROJECT_SOURCE_DIR}/src/bricklib2/asf/common/utils/stdio/read.c"
)

MESSAGE(STATUS "\nFound following source files:\n ${SOURCES}\n")

# define executable
ADD_EXECUTABLE(${PROJECT_NAME}.elf ${SOURCES})
SET_TARGET_PROPERTIES(${PROJECT_NAME}.elf PROPERTIES LINKER_LANGUAGE C)

# add library aeabi_idiv
ADD_LIBRARY(aeabi_idiv "${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/libgcc/aeabi_idiv.c")
SET_TARGET_PROPERTIES(aeabi_idiv PROPERTIES COMPILE_FLAGS "-mcpu=${MCU} -O${OPTIMIZATION_LEVEL}")

# add library aeabi_uidiv
ADD_LIBRARY(aeabi_uidiv "${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/libgcc/aeabi_uidiv.c")
SET_TARGET_PROPERTIES(aeabi_uidiv PROPERTIES COMPILE_FLAGS "-mcpu=${MCU} -O${OPTIMIZATION_LEVEL}")

# add library aeabi_idivmod
ADD_LIBRARY(aeabi_idivmod "${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/libgcc/aeabi_idivmod.c")
SET_TARGET_PROPERTIES(aeabi_idivmod PROPERTIES COMPILE_FLAGS "-mcpu=${MCU} -O${OPTIMIZATION_LEVEL}")

# add library aeabi_uidivmod
ADD_LIBRARY(aeabi_uidivmod "${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/libgcc/aeabi_uidivmod.c")
SET_TARGET_PROPERTIES(aeabi_uidivmod PROPERTIES COMPILE_FLAGS "-mcpu=${MCU} -O${OPTIMIZATION_LEVEL}")

TARGET_LINK_LIBRARIES(${PROJECT_NAME}.elf aeabi_idiv aeabi_uidiv aeabi_idivmod aeabi_uidivmod -lc_nano)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}.elf aeabi_idiv aeabi_idivmod -lc_nano)

# define compile flags
SET_TARGET_PROPERTIES(${PROJECT_NAME}.elf PROPERTIES COMPILE_FLAGS
	"${DEBUG} -mcpu=${MCU} -std=gnu99 -Wall --specs=nano.specs -mlong-calls -ffunction-sections -fdata-sections -O${OPTIMIZATION_LEVEL}"
)

#define linker flags
SET_TARGET_PROPERTIES(${PROJECT_NAME}.elf PROPERTIES LINK_FLAGS
	"-nostartfiles -mcpu=${MCU} -Wl,--gc-sections -T\"${PROJECT_SOURCE_DIR}/src/bricklib2/linker_script/samd09_firmware_with_brickletboot.ld\" "
)

# touch main.c to make sure that all of the custom POST_BUILD commands are definitely called
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   PRE_BUILD
                   COMMAND touch ${PROJECT_SOURCE_DIR}/src/main.c)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf 
                   POST_BUILD 
                   COMMAND ${CMAKE_OBJCOPY} -S -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD
                   COMMAND ${CMAKE_SIZE} ${OBJECT} ${PROJECT_NAME}.elf)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD
                   COMMAND ${PROJECT_SOURCE_DIR}/src/bricklib2/bootloader/crc32.py ${PROJECT_NAME}.bin > ${PROJECT_NAME}-checksum.bin)
                   
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD
                   COMMAND cat ${PROJECT_NAME}.bin ${PROJECT_NAME}-checksum.bin > ${PROJECT_NAME}-with-checksum.bin)
                   
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD
                   COMMAND cp ${PROJECT_SOURCE_DIR}/src/configs/config_custom_bootloader.h ${PROJECT_SOURCE_DIR}/../../brickletboot/software/src/configs)
                   
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD 
                   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/../../brickletboot/software/build
                   COMMAND make clean && make)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD
                   COMMAND rm ${PROJECT_SOURCE_DIR}/../../brickletboot/software/src/configs/config_custom_bootloader.h)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf
                   POST_BUILD
                   COMMAND cat ${PROJECT_SOURCE_DIR}/../../brickletboot/software/build/brickletboot-with-checksum.bin ${PROJECT_NAME}-with-checksum.bin > ${PROJECT_NAME}-with-bootloader.bin)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf POST_BUILD COMMAND 
                   ${CMAKE_OBJDUMP} -D
                   ${PROJECT_NAME}.elf > statistics.objdump)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf POST_BUILD COMMAND 
                   ${CMAKE_NM} --print-size --size-sort --radix=d
                   ${PROJECT_NAME}.elf > statistics.function_sizes)

# add preprocessor defines
ADD_DEFINITIONS(-D__${CHIP}__ -D__${CHIP_FAMILY}__ -Dflash -Dprintf=iprintf)
ADD_DEFINITIONS(-DNOSTARTFILES) # No start files (check this for every project, we don't call __libc_init_array if this is defined)