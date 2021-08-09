#include <Arduino.h>
#include <MIDI.h>
#include <ST7735_t3.h>
#include <SD.h>
#include <smfwriter.h>
#include <string>

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

const double TEMPO = 120.0;
bool ledOn = false;
SmfWriter smf;
unsigned long lastNoteMicros = 0;
unsigned long microseconds_per_tick;
bool isRecording = false;

unsigned long getDeltaTicks() {
    unsigned long ms = micros();
    unsigned long deltaTicks = (ms - lastNoteMicros) / microseconds_per_tick;
    lastNoteMicros = ms;
    return deltaTicks;
}

void toggleScreen() {
    // just a visual confirmation that we've received input from the midi controller
    ledOn = !ledOn;
    if (ledOn) {
        tft.fillScreen(ST7735_GREEN);
    } else 
        tft.fillScreen(ST7735_BLACK);
}

void startRecording() {
    if (!isRecording) {
        smf.setFilename("test");
        smf.writeHeader();

        Serial.printf("writing to file: %s\n", smf.getFilename() );

        // start the midi recording timing from now
        lastNoteMicros = micros();
        isRecording = true; 
    }
}

void stopRecording() {
    if (isRecording) {
        isRecording = false;
        smf.addEndofTrack(0, 0);
        smf.flush();
        Serial.printf("recoring completed: %s\n", smf.getFilename() );
    }
}

#pragma region handle midi input
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    toggleScreen();
    if (isRecording) {
        smf.addNoteOnEvent(getDeltaTicks(), pitch, velocity, channel);
        smf.flush();
    }
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    toggleScreen();
    if (isRecording) {
        smf.addNoteOffEvent(getDeltaTicks(), pitch, channel);
        smf.flush();
    }
}

void handleControlChange(byte channel, byte control, byte value) {
    if (isRecording) {
        smf.addControlChange(getDeltaTicks(), control, value, channel);
        smf.flush();
    }
}

void handleProgramChange(byte channel, byte program) {
    if (isRecording) {
        smf.addProgramChange(getDeltaTicks(), program, channel);
        smf.flush();
    }
}

void handleMIDIStartMessage() {
    startRecording();
}

void handleMIDIStopMessage() {
    stopRecording();
}

#pragma endregion

void setup()
{
    
    microseconds_per_tick = SmfWriter::get_microseconds_per_tick(TEMPO);
    Serial.begin(9600);
    const std::string outputPath = "output"; 
    SD.setSDCardFolderPath(outputPath);

    tft.initR(INITR_GREENTAB);
    tft.fillScreen(ST7735_BLACK);

    // Connect the handleNoteOn function to the library,
    // so it is called upon reception of a NoteOn.
    MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
    // Do the same for NoteOffs
    MIDI.setHandleNoteOff(handleNoteOff);
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleStart(handleMIDIStartMessage);
    MIDI.setHandleStop(handleMIDIStopMessage);

    // Initiate MIDI communications, listen to all channels
    MIDI.begin(MIDI_CHANNEL_OMNI);
}


void loop()
{
    MIDI.read();
    if (Serial.available()) {
        int ch = Serial.read();
        switch (ch) {
            case 'R' :
            case 'r' : startRecording(); break;
            case 'S' :
            case 's' : stopRecording(); break;
        }
    }
    delayMicroseconds(100);
}