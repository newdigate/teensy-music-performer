#include <Arduino.h>
#include <MIDI.h>

#ifdef BUILD_FOR_LINUX
    #include <RtMidiMIDI.h>
    #include <RtMidiTransport.h>
    MIDI_CREATE_RTMIDI_INSTANCE(RtMidiMIDI, rtMIDI,  MIDI);
#else
    MIDI_CREATE_DEFAULT_INSTANCE();
#endif

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

#ifdef BUILD_FOR_LINUX
int main() {
    setup();
    while(true) {        
        loop();
        delayMicroseconds(100);
    }
}
#endif