#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!

#define RF95_FREQ 434.0

// Singleton instance of the radio driver

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int x=4; 
//MUC Port


void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  pinMode(3, OUTPUT);
  
  
  Serial.begin(9600);
  delay(100);
  Serial.println("Arduino LoRa TX Test!");
  
  
// manual reset

  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
  
  
// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  
//Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  
  
// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  
  
// The default transmitter power is 13dBm, using PA_BOOST.

  
// If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then

  
// you can set transmitter powers from 5 to 23 dBm:

  rf95.setTxPower(23, false);
}

int16_t packetnum = 0; 
// packet counter, we increment per xmission


void loop()
{
  
//transmitter code

  char radiopacket[20] = "Hello World # ";
  itoa(packetnum++, radiopacket+13, 10);
  
  radiopacket[19] = 0;
  rf95.send((uint8_t *)radiopacket, 20);
  
  delay(10);
  rf95.waitPacketSent();
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  
  
//Receiver code

  if (rf95.recv(buf, &len))
    {
      Serial.print("COM");   
      Serial.print(x);   
//Port x receiver

      Serial.print(" Receiver: ");      
      Serial.println(rf95.lastRssi(), DEC);
      
      led1();
      
      
// Send a reply

      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
    }
    else
    {
      
//Serial.println("Receive failed");

    }
      delay(500);
  
