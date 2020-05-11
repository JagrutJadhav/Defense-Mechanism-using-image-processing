
/*
 *
  * Project Name:   Defence Mechanism using Image Processing
  * Author List:    Rahul Chandak, Jagrut Jadhav
  * Filename:       Auto_mode_Arduino_file.ino
  * Functions:      setup(),loop()
  * Global Variables: cntx, cnty, count, num;
  *
*/

#include <Servo.h>

Servo myservox;
Servo myservoy;
int cntx=90;
int cnty=90;
int count;
int num=0;

/*
  * Function Name:  setup()
  * Input:          none
  * Output:         setting output pins on arduino for servos 
  *
  */

void setup() {
  
  Serial.begin(9600);
  myservoy.attach(9);
  myservox.attach(10);
  myservox.write(90);
  myservoy.write(90);
}

/*
  * Function Name:  setup()
  * Input:          none
  * Output:         setting output pins on arduino for servos and motors
  * Logic:          The servo will move 'cntx + (count - 255)' in horizontal direction and 
                                        '(cnty +(count - 255)' in vertical direction 
  *                 where, cntx and cnty is 90 degrees that is the initial position of servo motors 
  */
void loop() 
{
  while (Serial.available()>0)
  {
        cntx = myservox.read();
        cnty = myservoy.read();
        count = Serial.read();
  
       if(num%2==0)
        {
            if(count>=220 && count<=255)
            {
               Serial.println(cntx+(count - 255) );
               myservox.write(cntx+(count - 255) ); 
            }

            else
            {
               Serial.println(cntx+count);  
               myservox.write(cntx+count);
            } 
        }
        
        else
        {
          if(count>=220 && count<=255)
            { 
               Serial.println(cnty+(count - 255));
               myservoy.write(cnty+(count - 255)); 
            }

            else
            {
               Serial.println(cnty+count);  
               myservoy.write(cnty+count);
            } 
        } 
        num = num+1;
  }

}
