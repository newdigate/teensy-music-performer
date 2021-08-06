#include <Arduino.h>
#include <MIDI.h>
#include <ST7735_t3.h>

#pragma region midi setup
#ifdef BUILD_FOR_LINUX
    #include <RtMidiMIDI.h>
    #include <RtMidiTransport.h>
    MIDI_CREATE_RTMIDI_INSTANCE(RtMidiMIDI, rtMIDI,  MIDI);
#else
    MIDI_CREATE_DEFAULT_INSTANCE();
#endif
#pragma endregion

#pragma region display setup
#ifdef BUILD_FOR_LINUX
    #include <st7735_opengl.h>
    #include <st7735_opengl_main.h>
    st7735_opengl tft = st7735_opengl(true, 10);
#else
    #define TFT_CS   6  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
    #define TFT_DC    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
    #define TFT_RST   255 // RST can use any pin
    ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);
#endif
#pragma endregion

#pragma region handle midi input
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    Serial.printf("Its alive...ch:%d pitch:%d vel:%d\n", channel, pitch, velocity);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
}
#pragma endregion

void setup()
{
    Serial.begin(9600);

    tft.initR(INITR_GREENTAB);
    tft.fillScreen(ST7735_GREEN);
    
    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop()
{
    MIDI.read();
}

#ifdef BUILD_FOR_LINUX45555
int main() {
    setup();
    while(true) {        
        loop();
        delayMicroseconds(100);
    }
}
#endif