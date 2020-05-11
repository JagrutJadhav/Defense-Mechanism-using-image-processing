/*
 *
  * Project Name:   Defence Mechanism using Image Processing
  * Author List:    Rahul Chandak, Jagrut Jadhav
  * Filename:       NRF_Transmitter.ino
  * Functions:      setup(),loop()
  * Global Variables: VRxser,VRyser,VRxm,VRym,mode,push,xser,yser,modeled,count,buttonState,address
  *
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8);
const byte address[6] = "00001";
const int VRxser = A0; 
const int VRyser = A1; 
const int VRxm = A2; 
const int VRym = A3; 
const int mode=2;
const int push=5; //push button for laser
int xser;
int yser;
int xm;
int ym;
int modeled=9;
int count=0;
int buttonState;

/*
  * Function Name:  setup()
  * Input:          none
  * Output:         none
  * Logic:          Setting the address at which we will transmit the data of NRF same as that defined in receiver.
  *                 
  */
void setup() {
  radio.begin();                  
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);  
  radio.stopListening();         
  pinMode(push, INPUT);
  pinMode(modeled, OUTPUT);
  pinMode (mode,INPUT);
  Serial.begin(9600);
}

/*
  * Function Name:  loop()
  * Input:          none
  * Output:         characters sent from nrf module  
  * Logic:          setting threshold values of analogue joystick and sending appropiate character through NRF module.
  *                 Setting count to switch between auto mode and manual mode
  */
void loop() {

  xser = (analogRead(VRxser));
  yser = (analogRead(VRyser));
  xm = (analogRead(VRxm));
  ym = (analogRead(VRym));
  buttonState = digitalRead(push);

  if (xser < 490 && (yser < 540 || yser > 490))             //if-else ladder for servo motion
  {
    
    const char text = 'W';
    radio.write(&text, sizeof(text));
    Serial.println(text);
  }
  if (xser > 540 && (yser < 540 || yser > 490))
  {
    const char text = 'S';
    radio.write(&text, sizeof(text));
  }
  if (yser> 540 && (xser < 540 || xser > 490))
  {
    const char text = 'A';
    radio.write(&text, sizeof(text));
  }
  if (yser < 490 && (xser < 540 || xser > 490))
  {
    const char text = 'D';
    radio.write(&text, sizeof(text));
  }
  if ((xser < 540 && xser > 490) && (yser < 540 && yser > 490))
  {
   const char text = 'O';
    radio.write(&text, sizeof(text));
  }
 

  if (xm < 490 && (ym < 540 || ym > 490))             //if-else ladder for bot motion  
  {
    
    const char text = 'F';
    radio.write(&text, sizeof(text));
    Serial.println(text);
  }
  if (xm > 540 && (ym < 540 || ym > 490))
  {
    const char text = 'B';
    radio.write(&text, sizeof(text));
  }
  else if (ym> 540 && (xm < 540 || xm > 490))
  {
    const char text = 'L';
    radio.write(&text, sizeof(text));
  }
  if (ym < 490 && (xm < 540 || xm > 490))
  {
    const char text = 'R';
    radio.write(&text, sizeof(text));
  }
  if ((xm < 540 && xm > 490) && (ym < 540 && ym > 490))
  {
   const char text = 'M';
    radio.write(&text, sizeof(text));
  }

  if ( digitalRead(mode) == 1)
  {
   const char text = 'G';
   count = count+1;
   //Serial.println(count);
   if(count%2==0)
   {
   digitalWrite(modeled,HIGH);
   //Serial.println(HIGH);
   }
   else
   {
    digitalWrite(modeled,LOW);
    //Serial.println(LOW);
   }
  // Serial.println("mode");
   radio.write(&text, sizeof(text));
   delay(500);
  }
  
  if ( buttonState == 1)
  {
   const char text = 'P';
   Serial.println("laser");
    radio.write(&text, sizeof(text));
  }
 if ( buttonState == 0)
  {
   const char text = 'O';
   Serial.println("offffffff");
    radio.write(&text, sizeof(text));
  }  
}
