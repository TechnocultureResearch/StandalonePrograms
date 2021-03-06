#include"REQLIBS.h"

const int thermocouple = A2;
int adc_val;
float y;
int temperature;
volatile byte flag = LOW;


void setup() 
{
pinMode(SERVOPIN1, OUTPUT);
pinMode(SERVOPIN2, OUTPUT);
pinMode(8, INPUT);
myservo[0].attach(SERVOPIN1);
myservo[1].attach(SERVOPIN2);
myservo[0].write(HOME);
myservo[1].write(HOME);
Serial.begin(BAUDRATE);
attachInterrupt(digitalPinToInterrupt(8), InterruptFunc, FALLING);

}

void loop() 
{
  if(flag == 1)
  {
   Serial.println("flag =1");     //////////////////////////////////////////////
           myservo[0].write(CHAMBER1POS);         //enter in chamber 1
           myservo[1].write(CHAMBER1POS);         //enter in chamber 1
  
           adc_val = analogRead(thermocouple);
           y = map(adc_val, 0,1023 ,0 ,100 );
          //  y = ( ((adc_val * 4.88) - 0.0027 ) / 10.0 );
            temperature = (y);
            // temperature = round(y);
           if(temperature < 95)         //compare chamber temp to reference
             { 
               myservo[0].write(CHAMBER1POS);          // if true stay in chamber 1
               myservo[1].write(CHAMBER1POS);
               Serial.println("waiting in chamber 1");
               Serial.println();
             }

           else                               
            {
               Serial.println("Chamber 1 temp");      //else print///////////////////////
               Serial.print(temperature);                //////////////////////////////

               myservo[0].write(CHAMBER2POS);             //move to chamber two
               myservo[1].write(CHAMBER2POS);
               Serial.println("In chamber 2");        ///////////////////////////////
               goto in;
               in:
               if(temperature > 53 && flag == 1)                     //wait until temperature is less than 55
               {
                 Serial.println("Waiting in ch2");         ////////////////////////
                 myservo[0].write(CHAMBER2POS);           // if true stay in chamber 2
                 myservo[1].write(CHAMBER2POS);
                 adc_val = analogRead(thermocouple);
                 y = map(adc_val, 0,1023 ,0 ,100 );
                //  y = ( ((adc_val * 4.88) - 0.0027 ) / 10.0 ); 
                 temperature = (y);
                 Serial.print("ch2 temp");                /////////////////////////
                 Serial.println(temperature);             ////////////////////////////
                 if(temperature < 56 && flag == 1)
                 {
                   Serial.println(temperature);         ////////////////////////////////
                   goto out;
                 }
                 else
                 {
                   goto in;
                 }         
                }
               out:
               Serial.print("Chamber 2 successfull");   
            }
         
  }
  else
 {
   Serial.println("Interrupt not occured");              ///////////////////////////
   myservo[0].write(HOME);
   myservo[1].write(HOME); 
  }

}

void InterruptFunc()
{
  flag = !flag;
 }