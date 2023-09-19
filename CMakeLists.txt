#此文件从模板自动生成! 请勿更改!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.23)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(my_libarary C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(HALRES/Inc FATFS/Target FATFS/App Middlewares/Third_Party/FatFs/src Format_analysis
        LVppDisplayer lvvglpp/src LVGL LVGL/src/ LVGL/examples/porting/ MyApplication/Inc BSP/touch BSP LCD other_library MyLibrary/Inc ConnectingBridge/Inc Core/Inc Drivers/STM32F4xx_HAL_Driver/Inc Drivers/STM32F4xx_HAL_Driver/Inc/Legacy Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/CMSIS/Include)

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx)

file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*" "ConnectingBridge/Src/*.*" "Format_analysis/*.*" "HALRES/*.*"
        "Middlewares/Third_Party/FatFs/src/*.*" "FATFS/App/*.*" "FATFS/Target/*.*"
        "MyLibrary/Src/*.*""other_library/*.*" "LCD/lcd.c" "BSP/*.*" "BSP/touch/*.*"
        "MyApplication/Src/*.*" "LVGL/*.*" "ManageLVGL/Src/*.*"
        "lvvglpp/src/lvglpp/core/*.*" "LVppDisplayer/*.*"
        #        "src/lvglpp/core/event.cpp"
        #        "src/lvglpp/core/indev.cpp"
        #        "src/lvglpp/core/object.cpp"
        #        "src/lvglpp/core/group.cpp"
        #        "src/lvglpp/core/theme.cpp"

        "lvvglpp/src/lvglpp/draw/*.*"
        #        "src/lvglpp/draw/image.cpp"
        #        "src/lvglpp/draw/mask.cpp"

        "lvvglpp/src/lvglpp/font/font.cpp"

        "lvvglpp/src/lvglpp/misc/anim.cpp"
        "lvvglpp/src/lvglpp/misc/area.cpp"
        "lvvglpp/src/lvglpp/misc/color.cpp"
        "lvvglpp/src/lvglpp/misc/fs.cpp"
        "lvvglpp/src/lvglpp/misc/style.cpp"
        "lvvglpp/src/lvglpp/misc/timer.cpp"

        "lvvglpp/src/lvglpp/widgets/animimg/animimg.cpp"
        "lvvglpp/src/lvglpp/widgets/arc/arc.cpp"
        "lvvglpp/src/lvglpp/widgets/bar/bar.cpp"
        "lvvglpp/src/lvglpp/widgets/btnmatrix/btnmatrix.cpp"
        "lvvglpp/src/lvglpp/widgets/calendar/calendar.cpp"
        "lvvglpp/src/lvglpp/widgets/canvas/canvas.cpp"
        "lvvglpp/src/lvglpp/widgets/chart/chart.cpp"
        "lvvglpp/src/lvglpp/widgets/checkbox/checkbox.cpp"
        "lvvglpp/src/lvglpp/widgets/colorwheel/colorwheel.cpp"
        "lvvglpp/src/lvglpp/widgets/dropdown/dropdown.cpp"
        "lvvglpp/src/lvglpp/widgets/image/image.cpp"
        "lvvglpp/src/lvglpp/widgets/imgbtn/imgbtn.cpp"
        "lvvglpp/src/lvglpp/widgets/keyboard/keyboard.cpp"
        "lvvglpp/src/lvglpp/widgets/label/label.cpp"
        "lvvglpp/src/lvglpp/widgets/led/led.cpp"
        "lvvglpp/src/lvglpp/widgets/line/line.cpp"
        "lvvglpp/src/lvglpp/widgets/list/list.cpp"
        "lvvglpp/src/lvglpp/widgets/menu/menu.cpp"
        "lvvglpp/src/lvglpp/widgets/meter/meter.cpp"
        "lvvglpp/src/lvglpp/widgets/msgbox/msgbox.cpp"
        "lvvglpp/src/lvglpp/widgets/roller/roller.cpp"
        "lvvglpp/src/lvglpp/widgets/slider/slider.cpp"
        "lvvglpp/src/lvglpp/widgets/span/span.cpp"
        "lvvglpp/src/lvglpp/widgets/spinbox/spinbox.cpp"
        "lvvglpp/src/lvglpp/widgets/spinner/spinner.cpp"
        "lvvglpp/src/lvglpp/widgets/table/table.cpp"
        "lvvglpp/src/lvglpp/widgets/tabview/tabview.cpp"
        "lvvglpp/src/lvglpp/widgets/textarea/textarea.cpp"
        "lvvglpp/src/lvglpp/widgets/tileview/tileview.cpp"
        "lvvglpp/src/lvglpp/widgets/win/win.cpp""src/lvglpp/core/display.cpp"
        "lvvglpp/src/lvglpp/core/event.cpp"
        "lvvglpp/src/lvglpp/core/indev.cpp"
        "lvvglpp/src/lvglpp/core/object.cpp"
        "lvvglpp/src/lvglpp/core/group.cpp"
        "lvvglpp/src/lvglpp/core/theme.cpp"

        "lvvglpp/src/lvglpp/draw/desc.cpp"
        "lvvglpp/src/lvglpp/draw/image.cpp"
        "lvvglpp/src/lvglpp/draw/mask.cpp"

        "lvvglpp/src/lvglpp/font/font.cpp"

        #        "lvvglpp/src/lvglpp/misc/*.*"
        #        "lvvglpp/src/lvglpp/misc/area.cpp"
        #        "lvvglpp/src/lvglpp/misc/color.cpp"
        #        "lvvglpp/src/lvglpp/misc/fs.cpp"
        #        "lvvglpp/src/lvglpp/misc/style.cpp"
        #        "lvvglpp/src/lvglpp/misc/timer.cpp"

        "lvvglpp/src/lvglpp/widgets/animimg/animimg.cpp"
        "lvvglpp/src/lvglpp/widgets/arc/arc.cpp"
        "lvvglpp/src/lvglpp/widgets/bar/bar.cpp"
        "lvvglpp/src/lvglpp/widgets/btnmatrix/btnmatrix.cpp"
        "lvvglpp/src/lvglpp/widgets/calendar/calendar.cpp"
        "lvvglpp/src/lvglpp/widgets/canvas/canvas.cpp"
        "lvvglpp/src/lvglpp/widgets/chart/chart.cpp"
        "lvvglpp/src/lvglpp/widgets/checkbox/checkbox.cpp"
        "lvvglpp/src/lvglpp/widgets/colorwheel/colorwheel.cpp"
        "lvvglpp/src/lvglpp/widgets/dropdown/dropdown.cpp"
        "lvvglpp/src/lvglpp/widgets/image/image.cpp"
        "lvvglpp/src/lvglpp/widgets/imgbtn/imgbtn.cpp"
        "lvvglpp/src/lvglpp/widgets/keyboard/keyboard.cpp"
        "lvvglpp/src/lvglpp/widgets/label/label.cpp"
        "lvvglpp/src/lvglpp/widgets/led/led.cpp"
        "lvvglpp/src/lvglpp/widgets/line/line.cpp"
        "lvvglpp/src/lvglpp/widgets/list/list.cpp"
        "lvvglpp/src/lvglpp/widgets/menu/menu.cpp"
        "lvvglpp/src/lvglpp/widgets/meter/meter.cpp"
        "lvvglpp/src/lvglpp/widgets/msgbox/msgbox.cpp"
        "lvvglpp/src/lvglpp/widgets/roller/roller.cpp"
        "lvvglpp/src/lvglpp/widgets/slider/slider.cpp"
        "lvvglpp/src/lvglpp/widgets/span/span.cpp"
        "lvvglpp/src/lvglpp/widgets/spinbox/spinbox.cpp"
        "lvvglpp/src/lvglpp/widgets/spinner/spinner.cpp"
        "lvvglpp/src/lvglpp/widgets/table/table.cpp"
        "lvvglpp/src/lvglpp/widgets/tabview/tabview.cpp"
        "lvvglpp/src/lvglpp/widgets/textarea/textarea.cpp"
        "lvvglpp/src/lvglpp/widgets/tileview/tileview.cpp"
        "lvvglpp/src/lvglpp/widgets/win/win.cpp")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407ZGTX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
