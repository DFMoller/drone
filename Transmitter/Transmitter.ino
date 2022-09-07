#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; // This code should be the same for the receiver

RF24 radio(9, 10);  //Set CE and CSN pins

// The sizeof this struct should not exceed 32 bytes
struct Data_to_be_sent {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
};

//Create a variable with the structure above and name it sent_data
Data_to_be_sent sent_data;

unsigned long lastTransTime = 0;

unsigned long freq = 0;

int ch1_val = 0;
int ch2_val = 0;
int ch3_val = 0;
int ch4_val = 0;

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);

  //Reset each channel value
  sent_data.ch1 = 127;
  sent_data.ch2 = 127;
  sent_data.ch3 = 127;
  sent_data.ch4 = 127;
}

void loop()
{
  // If channel is reversed, just swap 0 to 255 by 255 to 0 below
  
  ch1_val = analogRead(A0);
  ch2_val = analogRead(A1);
  ch3_val = analogRead(A2);
  ch4_val = analogRead(A3);

  if(ch1_val > 700) ch1_val = 700;
  if(ch2_val > 700) ch2_val = 700;
  if(ch3_val > 700) ch3_val = 700;
  if(ch4_val > 700) ch4_val = 700;
  
  sent_data.ch1 = map( ch1_val, 0, 700, 255, 0);
  sent_data.ch2 = map( ch2_val, 0, 700, 255, 0);
  sent_data.ch3 = map( ch3_val, 0, 700, 255, 0);
  sent_data.ch4 = map( ch4_val, 0, 700, 255, 0);


  Serial.print("Left: ");
  Serial.print(sent_data.ch1);
  Serial.print("  ");
  Serial.print(sent_data.ch2);
  Serial.print("   Right: ");
  Serial.print(sent_data.ch3);
  Serial.print("  ");
  Serial.print(sent_data.ch4);
  Serial.println("   Freq: " + (String)freq + " us");

  lastTransTime = micros();

  radio.write(&sent_data, sizeof(Data_to_be_sent));


  freq = (micros() - lastTransTime); // in us
}
