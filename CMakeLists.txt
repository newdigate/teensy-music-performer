cmake_minimum_required(VERSION 3.5)
project(teensy_music_performer)
set(CMAKE_CXX_STANDARD 14)
if (NOT DEFINED TEENSY_VERSION)
    set(BUILD_FOR_LINUX 1)
    add_definitions(-DBUILD_FOR_LINUX)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (NOT DEFINED BUILD_FOR_LINUX)    
    import_arduino_library(cores ${COREPATH} avr debug util)
    import_arduino_library(SPI ${DEPSPATH}/SPI)
    import_arduino_library(SdFat ${DEPSPATH}/SdFat/src common DigitalIO ExFatLib FatLib FsLib iostream SdCard SpiDriver)
    import_arduino_library(SD ${DEPSPATH}/SD/src)
    import_arduino_library(SerialFlash ${DEPSPATH}/SerialFlash util)
    import_arduino_library(Wire ${DEPSPATH}/Wire utility)
    import_arduino_library(arm_math ${DEPSPATH}/arm_math/src)
    import_arduino_library(Audio ${DEPSPATH}/Audio utility)   
    import_arduino_library(MIDI ${DEPSPATH}/MIDI/src)   
    add_subdirectory(src)
else()

    find_package(teensy_x86_stubs)
    include_directories(${teensy_x86_stubs_INCLUDE_DIR})

    find_package(teensy_x86_sd_stubs)
    include_directories(${teensy_x86_sd_stubs_INCLUDE_DIR})

    find_package(teensy_audio_x86_stubs)
    include_directories(${teensy_audio_x86_stubs_INCLUDE_DIR})

    find_package(teensy_audio_x86_stubs_soundio)
    include_directories(${teensy_audio_x86_stubs_soundio_INCLUDE_DIR})

    find_package(teensy_x86_midi_stubs)
    include_directories(${teensy_x86_midi_stubs_INCLUDE_DIR})

    find_package(teensy_x86_midi_stubs_rtmidi)
    include_directories(${teensy_x86_midi_stubs_rtmidi_INCLUDE_DIR})

    find_package(teensy_st7735_linux_stubs)
    include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})

    find_package(teensy_st7735_linux_opengl)
    include_directories(${teensy_st7735_linux_extras_opengl_INCLUDE_DIR})

    include(cmake/opengl.cmake.in)

    include_directories(/usr/local/include/rtmidi)

    add_executable(musicperformer musicperformer.cpp)
    target_link_libraries(musicperformer -L/usr/local/lib/ rtmidi)
    target_link_libraries(musicperformer ${teensy_st7735_linux_extras_opengl_LIBS} ${teensy_st7735_linux_stubs_LIBS} ${teensy_x86_midi_stubs_LIBS} ${teensy_x86_stubs_LIBS} ${LIBS})
endif()