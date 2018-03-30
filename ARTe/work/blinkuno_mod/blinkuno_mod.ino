/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

#include "arte.h"

int led = 13;
int setup_completed = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
    Serial.begin(57600);
    setup_completed = 1;
}

// the loop routine runs over and over again forever:
void loop() {
// digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
 // delay(1000);               // wait for a second
//  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
 // delay(1000);               // wait for a second
 while(1);
}


int variable = 0;
int led1status = 1;


/*
void loop1(1000)
{
  return;
}
*/

void loop1(1000)
{
/*  arteLock();
  variable++;
  arteUnlock();
*/
  if (!setup_completed) return;

  if(led1status == 0)
  {
    digitalWrite(led, HIGH); 
    led1status = 1;
  }
  else
  {
    digitalWrite(led, LOW); 
    led1status = 0;
  }
  
  //for(int i=0; i<100000; i++) if(i%2) {i++; i=i-1;}
  //Serial.println("a"); 

  

}

int counter = 0;


void loop2(500)
{
    if (!setup_completed) return;
    
    
   arteLock();
   Serial.println(counter++); 
   arteUnlock();
}


void loop3(5000)
{
    if (!setup_completed) return;
    
    arteLock(); 
   Serial.println("a"); 
   if(arteEnabled())
       Serial.println("arteEnabled()"); 
    arteUnlock();
}

void loop4(40)
{
    if (!setup_completed) return;
       arteLock();
       Serial.println("44444444444444444444444444444444444444"); 
       arteUnlock();
}



void loop5(10)
{
    if (!setup_completed) return;
       arteLock();
       Serial.println("555555555555555555555555555555555555555555"); 
       arteUnlock();
}




