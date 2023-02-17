#include "MIDIUSB.h"

#define SOL_1_PIN 2
#define SOL_2_PIN 3
#define SOL_3_PIN 4
#define SOL_4_PIN 5
#define SOL_5_PIN 6
#define SOL_6_PIN 7
#define SOL_7_PIN 8
#define SOL_8_PIN 9
#define DEBUG 0

int lastNote = -1;
int c = -1;

int divToPin[] = {
    SOL_1_PIN,
    -1,
    SOL_2_PIN,
    -1,
    SOL_3_PIN,
    SOL_4_PIN,
    -1,
    SOL_5_PIN,
    -1,
    SOL_6_PIN,
    -1,
    SOL_7_PIN,
    SOL_8_PIN,
};

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity)
{
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup()
{
  Serial.begin(115200);
  pinMode(SOL_1_PIN, OUTPUT);
  pinMode(SOL_2_PIN, OUTPUT);
  pinMode(SOL_3_PIN, OUTPUT);
  pinMode(SOL_4_PIN, OUTPUT);
  pinMode(SOL_5_PIN, OUTPUT);
  pinMode(SOL_6_PIN, OUTPUT);
  pinMode(SOL_7_PIN, OUTPUT);
  pinMode(SOL_8_PIN, OUTPUT);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value)
{
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop()
{
  midiEventPacket_t rx;
  do
  {
    rx = MidiUSB.read();
    if (rx.header != 0)
    {
      if (DEBUG)
      {
        Serial.print("Received: ");
        Serial.print(rx.header, DEC);
        Serial.print("-");
        Serial.print(rx.byte1, DEC);
        Serial.print("-");
        Serial.print(rx.byte2, DEC);
        Serial.print("-");
        Serial.println(rx.byte3, DEC);
      }

      int header = rx.header;
      int velocity = rx.byte3;

      if (header == 11)
      {

        int value = rx.byte3;
        int number = rx.byte2;
        if (DEBUG)
        {
          Serial.print("CTL IN");
          Serial.print(", number: ");
          Serial.print(number);
          Serial.print(", value: ");
          Serial.print(value);
          Serial.println();
        }
      }

      if (header == 9 && velocity > 0)
      {

        int note = rx.byte2;
        int div = (note % 12);
        int solPin = divToPin[div];

        if (div == 0 && lastNote > 0)
        {
          if (lastNote != note)
          {
            if (lastNote > note)
            {
              c = divToPin[0];
            }
            if (lastNote < note)
            {
              c = divToPin[12];
            }
            solPin = c;
          }
          else
          {
            solPin = c;
          }
        }

        int sol_wait = map(velocity, 0, 127, 4800, 6000);

        if (DEBUG)
        {
          Serial.print("sol wait: ");
          Serial.print(sol_wait);
          Serial.print(", note: ");
          Serial.print(note);
          Serial.print(", div: ");
          Serial.print(div);
          Serial.print(", velocity: ");
          Serial.print(velocity);
          Serial.print(", solPin=");
          Serial.println(solPin);
        }

        if (solPin > 0 && velocity > 0)
        {
          digitalWrite(solPin, HIGH);
          delayMicroseconds(sol_wait);
          digitalWrite(solPin, LOW);
        }

        lastNote = note;
      }
    }
  } while (rx.header != 0);
}
