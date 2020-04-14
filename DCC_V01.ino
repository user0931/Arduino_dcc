#define NUMACCESSORIES 2 // Enter the number of accessories here

// GO TO setup() TO CONFIGURE DCC ADDRESSES AND PIN NUMBERS

#include <DCC_Decoder.h>

typedef struct DCCAccessoryData {
  int   address;   // User Configurable DCC address
  byte  outputpin; // User Configurable Arduino pin
  byte  dccstate;  // Internal use DCC state of accessory, 1=on, 0=off
};
DCCAccessoryData accessory[NUMACCESSORIES];

// The DCC library calls this function to set / reset accessories
void BasicAccDecoderPacket_Handler(int address, boolean activate, byte data) {
  address -= 1;
  address *= 4;
  address += 1;
  address += (data & 0x06) >> 1;
  // address = address - 4; // uncomment this line for Roco Maus or z21
  boolean enable = (data & 0x01) ? 1 : 0;
  for (byte i=0; i<NUMACCESSORIES; i++) {
    if (address == accessory[i].address) {
      if (enable) accessory[i].dccstate = 1;
      else accessory[i].dccstate = 0;
    }
  }
}

void setup() { 
// CONFIGURATION OF ACCESSORIES
// Copy & Paste as many times as you have accessories 
// The amount must be same as NUMACCESSORIES
// Don't forget to increment the array index
  accessory[0].address   = 77; // DCC address
  accessory[0].outputpin = 13; // Arduino pin

  accessory[1].address   = 78; // DCC address
  accessory[1].outputpin = 12; // Arduino pin
// END OF CONFIGURATION OF ACCESSORIES

  DCC.SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket_Handler, true);
  DCC.SetupDecoder( 0x00, 0x00, 0 );

  for(byte i=0; i<NUMACCESSORIES; i++) {
    pinMode     (accessory[i].outputpin, OUTPUT);
    digitalWrite(accessory[i].outputpin, LOW);
  }
}

void loop() {
  DCC.loop(); // Call to library function that reads the DCC data

  for(byte i=0; i<NUMACCESSORIES; i++) {
    if (accessory[i].dccstate) {
      digitalWrite(accessory[i].outputpin, HIGH);
      Serial.print("Activado el accessory[i] en high");
      Serial.print("el valor de i es **");
      Serial.print(i);
      Serial.println("**");
    }
    else   {
      digitalWrite(accessory[i].outputpin, LOW);
      Serial.print("Activado el accessory[i] en low");
      Serial.print("el valor de i es **");
      Serial.print(i);
      Serial.println("**");
    }
  }
}
