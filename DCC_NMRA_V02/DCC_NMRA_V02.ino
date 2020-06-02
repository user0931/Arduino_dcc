// Accessory Decoder - www.dccinterface.com

#include <NmraDcc.h>
#include <Wire.h>

typedef struct
{
  int address;
  uint8_t arduinoPin;
}
DCCAccessoryAddress;

// this is designed for 2 addresses, if you want more decoder functions, increase this to 3 for 3, 5 for 5, etc etc etc
DCCAccessoryAddress gAddresses[2];

NmraDcc  Dcc;
uint16_t lastAddr = 0xFFFF;
uint8_t lastDirection = 0xFF;

//
// Decoder Init
//
void ConfigureDecoder()
{
  // this is address number 1
  gAddresses[0].address = 241;
  gAddresses[0].arduinoPin = 3;

  // this is address number 2
  gAddresses[1].address = 242;
  gAddresses[1].arduinoPin = 4;

  // if you wanted more, make sure the array on line 13 is bigger, and start adding lines as follows
  //gAddresses[2].address = 202;
  //gAddresses[2].arduinoPin = A8;

  //gAddresses[3].address = 203;
  //gAddresses[3].arduinoPin = A9;

  // the above are commented out, and just for example only

  // set the pin for output
  for (int i = 0; i<(int)(sizeof(gAddresses) / sizeof(gAddresses[0])); i++)
  {
    pinMode(gAddresses[i].arduinoPin, OUTPUT);
  }
}

// This function is called whenever a normal DCC Turnout Packet is received
void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower)
{
  Serial.print("notifyDccAccTurnoutOutput: ");
  Serial.print(Addr, DEC);
  Serial.print(',');
  Serial.print(Direction, DEC);
  Serial.print(',');
  Serial.println(OutputPower, HEX);

  for (int i = 0; i < (sizeof(gAddresses) / sizeof(DCCAccessoryAddress)); i++)
  {
    if ((Addr == gAddresses[i].address) && ((Addr != lastAddr) || (Direction != lastDirection)) && OutputPower)
    {
      lastAddr = Addr;
      lastDirection = Direction;

      Serial.print(F("Activating Decoder Address "));
      Serial.println(i, DEC);

      if (Direction)
      {
        Serial.print(F("Turning Accessory On : "));
        Serial.println(gAddresses[i].arduinoPin, DEC);
        digitalWrite(gAddresses[i].arduinoPin, HIGH);
        break;
      }
      else
      {
        Serial.print(F("Turning Accessory Off : "));
        Serial.println(gAddresses[i].arduinoPin, DEC);
        digitalWrite(gAddresses[i].arduinoPin, LOW);
        break;
      }
    }
  }
}

void setupDCCDecoder()
{
  Serial.println(F("Setting up DCC Decorder..."));

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

  // Configure the Decoder
  ConfigureDecoder();
}

void setup()
{
  Serial.begin(19200);
  Serial.println(F("Accessory Decoder - www.dccinterface.com"));
  Serial.println(F("Initializing...."));
  setupDCCDecoder();
}

void loop()
{
  // You MUST call the NmraDcc.process() method frequently from the Arduino loop() function for correct library operation
  Dcc.process();
}
