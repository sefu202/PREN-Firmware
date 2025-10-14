# --------------------------
# STM32F746/Cortex-M7 Toolchain
# --------------------------

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Compiler IDs
set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# Toolchain prefix
set(TOOLCHAIN_PREFIX                arm-none-eabi-)

# Compilers and tools
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)
set(CMAKE_OBJDUMP                   ${TOOLCHAIN_PREFIX}objdump)

# Executable suffixes
set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

# Try compile type
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# --------------------------
# MCU specific flags
# --------------------------
set(MCU_FLAGS "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -DSTM32F746xx -D__FPU_PRESENT=1 -D__FPU_USED=1 -D__ARM_ARCH_7EM__=1")

# C/C++ flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -x assembler-with-cpp -MMD -MP")

# Debug/Release flags
set(CMAKE_C_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

# Linker flags
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T\"${CMAKE_SOURCE_DIR}/STM32F746XX_FLASH.ld\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")

# Standard libraries to link
set(TOOLCHAIN_LINK_LIBRARIES "m")
