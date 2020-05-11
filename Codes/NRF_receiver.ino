
   
/*
 *
  * Project Name:   Defence Mechanism using Image Processing
  * Author List:    Jagrut Jadhav,Rahul Chandak
  * Filename:       NRF_Receiver.ino
  * Functions:      setup(),loop(),forward(),backward(),right(),left(),stop(),cam_up(),cam_down(),cam_left,cam_right()
  * Global Variables: m11,m12,m21,m22,count,pos1,pos2,laser,address
  *
*/
    #include <SPI.h>
    #include <nRF24L01.h>
    #include <RF24.h>
    #include <Servo.h>
    
    const byte address[6] = "00001";
    int m11=2,m12=3,m21=4,m22=5;
    int count =0;
    int pos1;
    int pos2;
    int laser=6;
    
    RF24 radio(7, 8);
    Servo myservo1;
    Servo myservo2;

    /*
  * Function Name:  setup()
  * Input:          none
  * Output:         setting output pins on arduino for servos and motors
  * Logic:          Setting the address at which we will receive the data of NRF same as that defined in transmitter.
  *                 
  * 
  *
  */
    
    void setup() {
    Serial.begin(9600);
    pinMode(laser,OUTPUT);
    pinMode(m11,OUTPUT);
    pinMode(m12,OUTPUT);
    pinMode(m21,OUTPUT);
    pinMode(m22,OUTPUT);
    radio.begin();
    radio.openReadingPipe(0, address);    //Setting the address at which we will receive the data
    radio.setPALevel(RF24_PA_HIGH);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
    radio.startListening();              //This sets the module as receiver
    
    myservo1.attach(9);
    myservo2.attach(10);
    }

   /*
  * Function Name:  loop()
  * Input:          none
  * Output:         characters received from nrf module  
  * Logic:          calling specific function according to data recieved by trasnsmitter using switch case
  */
    void loop()
    {
    pos1=myservo1.read();
    pos2=myservo2.read();  
    if (radio.available())              //Looking for the data.
    {
    char text;                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    //radio.read(&button_state, sizeof(button_state));    //Reading the data
      switch(text)
      {
        case 'F': forward();
                  break;
        case 'B': backward();
                  break;
        case 'L': right();
                  break;
        case 'R': left();
                  break;
        case 'W': cam_up();
                  break;
        case 'D': cam_right();
                  break;
        case 'A': cam_left();
                  break;
        case 'S': cam_down();
                  break;
        case 'M': stopp();
                  break;
        case 'O': Serial.println('O');
                  digitalWrite(laser,LOW);
                  break;
        case 'P': digitalWrite(laser,HIGH);
                  Serial.print("shoot");
                  break;
       }
    }
    delay(5);
    }

  /*
  * Function Name:  forward()
  * Input:          none
  * Output:         HIGH or LOW such that bot moves in forward direction       
  */

    void forward(){
      Serial.println("forward");
      digitalWrite(m11,HIGH);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,HIGH);
    }
    
   /*
  * Function Name:  backward()
  * Input:          none
  * Output:         HIGH or LOW such that bot move in backward direction       
  */
    void backward(){
      Serial.println("back");
      digitalWrite(m11,LOW);
      digitalWrite(m12,HIGH);
      digitalWrite(m21,HIGH);
      digitalWrite(m22,LOW);
    }
    /*
  * Function Name:  left()
  * Input:          none
  * Output:         HIGH or LOW such that bot move in left direction       
  */

    void left(){
      Serial.println("left");
      digitalWrite(m11,HIGH);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,LOW);
    }

    /*
  * Function Name:  right()
  * Input:          none
  * Output:         HIGH or LOW such that bot move in right direction       
  */
    void right(){
      Serial.println("right");
      digitalWrite(m11,LOW);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,HIGH);
    }

    /*
  * Function Name:  stopp()
  * Input:          none
  * Output:         HIGH or LOW such that bot move in stopp direction       
  */
    void stopp(){
      Serial.println("stop");
      digitalWrite(m11,LOW);
      digitalWrite(m12,LOW);
      digitalWrite(m21,LOW);
      digitalWrite(m22,LOW);
    }

/*
  * Function Name:  cam_up()
  * Input:          none
  * Output:         Moving servo in up direction with increase in 1 degree per receieved character      
  */
    void cam_up()
    {
   Serial.println("cam up");
        pos1=pos1+1;
        Serial.println(pos1);
         myservo1.write(pos1);             
         delay(15);
    }

/*
  * Function Name:  cam_down()
  * Input:          none
  * Output:         Moving servo in down direction with decrease in 1 degree per receieved character      
  */    
    void cam_down()
    {
   Serial.println("cam down"); 
        Serial.println(pos1);
        pos1=pos1-1;
        myservo1.write(pos1);            
        delay(15);
    }

/*
  * Function Name:  cam_left()
  * Input:          none
  * Output:         Moving servo in left direction with increase in 1 degree per receieved character      
  */
    void cam_left()
    {
    Serial.println("cam left"); 
        { 
          pos2=pos2+1;
           myservo2.write(pos2);             
           delay(15);
        }
    }

/*
  * Function Name:  cam_right()
  * Input:          none
  * Output:         Moving servo in right direction with decrease in 1 degree per receieved character      
  */
    void cam_right()
    {
    Serial.println("cam right");
        { 
          pos2=pos2-1;
           myservo2.write(pos2);             
           delay(15);
        }
    }    
