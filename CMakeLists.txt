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
    import_arduino_library(TeensyVariablePlayback ${DEPSPATH}/teensy-variable-playback src)
    import_arduino_library(midi_smf_reader ${DEPSPATH}/midi-smf-reader/src)
    import_arduino_library(teensypolyphony ${DEPSPATH}/teensy-polyphony/src)
    import_arduino_library(sampleflashloader ${DEPSPATH}/teensy-sample-flashloader/src)
    import_arduino_library(teensyquencer ${DEPSPATH}/teensy-quencer/src)
    import_arduino_library(ST7735_t3 ${DEPSPATH}/ST7735_t3)
    import_arduino_library(arduino_tftpianoview ${DEPSPATH}/arduino-tftpianoview/src)
    import_arduino_library(teensy_audio_display_components ${DEPSPATH}/teensy-audio-display-components/src)
    import_arduino_library(midi_smf_writer ${DEPSPATH}/midi-smf-writer/src)

    teensy_add_executable(musicperformer musicperformer.cpp)
    teensy_target_link_libraries(musicperformer  midi_smf_writer  teensy_audio_display_components  arduino_tftpianoview ST7735_t3 teensypolyphony midi_smf_reader teensyquencer sampleflashloader TeensyVariablePlayback Audio arm_math SD SdFat  SPI SerialFlash  Wire  cores )
    target_link_libraries(musicperformer.elf m stdc++)
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

    find_package(midi_smf_writer)
    include_directories(${midi_smf_writer_INCLUDE_DIR})

    include(cmake/opengl.cmake.in)

    include_directories(/usr/local/include/rtmidi)

    add_executable(musicperformer musicperformer.cpp)
    target_link_libraries(musicperformer -L/usr/local/lib/ rtmidi)
    target_link_libraries(musicperformer ${midi_smf_writer_LIBS} ${teensy_st7735_linux_extras_opengl_LIBS} ${teensy_st7735_linux_stubs_LIBS} ${teensy_x86_midi_stubs_LIBS} ${teensy_x86_sd_stubs_LIBS} ${teensy_x86_stubs_LIBS} ${LIBS})
endif()