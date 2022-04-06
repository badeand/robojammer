#include "MIDIUSB.h"

#define SOL_1_PIN 2
#define SOL_2_PIN 3
#define SOL_3_PIN 4
#define DEBUG 0

int sol_wait = 6;

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

      if (header == 11 && rx.byte2 == 1)
      {
        sol_wait = rx.byte3;
      }

      if (header == 9)
      {

        int note = rx.byte2;
        int velocity = rx.byte3;

        if (DEBUG)
        {
          Serial.print("note: ");
          Serial.println(note);
          Serial.print("velocity: ");
          Serial.println(velocity);
        }

        if (note == 60)
        {
          Serial.println("sol 1");
          if (velocity > 0)
          {
            digitalWrite(SOL_1_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_1_PIN, LOW);
          }
        }

        if (note == 62)
        {
          Serial.println("sol 2");
          if (velocity > 0)
          {
            digitalWrite(SOL_2_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_2_PIN, LOW);
          }
        }

        if (note == 64)
        {
          Serial.println("sol 2");
          if (velocity > 0)
          {
            digitalWrite(SOL_3_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_3_PIN, LOW);
          }
        }
      }
    }
  } while (rx.header != 0);
}
