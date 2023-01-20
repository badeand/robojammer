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

      if (header == 9)
      {

        int note = rx.byte2;
        int velocity = rx.byte3;

        if (velocity > 100)
        {
          sol_wait = 8;
        }
        else if (velocity > 80)
        {
          sol_wait = 7;
        }
        else if (velocity > 50)
        {
          sol_wait = 5;
        }

        if (DEBUG)
        {
          Serial.print("sol wait");
          Serial.println(sol_wait, DEC);

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
          Serial.println("sol 3");
          if (velocity > 0)
          {
            digitalWrite(SOL_3_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_3_PIN, LOW);
          }
        }

        if (note == 65)
        {
          Serial.println("sol 4");
          if (velocity > 0)
          {
            digitalWrite(SOL_4_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_4_PIN, LOW);
          }
        }

        if (note == 67)
        {
          Serial.println("sol 5");
          if (velocity > 0)
          {
            digitalWrite(SOL_5_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_5_PIN, LOW);
          }
        }

        if (note == 69)
        {
          Serial.println("sol 6");
          if (velocity > 0)
          {
            digitalWrite(SOL_6_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_6_PIN, LOW);
          }
        }

        if (note == 71)
        {
          Serial.println("sol 7");
          if (velocity > 0)
          {
            digitalWrite(SOL_7_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_7_PIN, LOW);
          }
        }
        if (note == 72)
        {
          Serial.println("sol 8");
          if (velocity > 0)
          {
            digitalWrite(SOL_8_PIN, HIGH);
            delay(sol_wait);
            digitalWrite(SOL_8_PIN, LOW);
          }
        }
      }
    }
  } while (rx.header != 0);
}
