#line 1 "Blink.ino"
  
       
                                                                         

                                            
   

                                                      
                  
#include "Arduino.h"
void setup();
void loop();
#line 10
int led = 13;

                                                    
void setup() {
                                             
  pinMode(led, OUTPUT);
}

                                                     
void loop() {
  digitalWrite(led, HIGH);                                                 
  delay(1000);                                   
  digitalWrite(led, LOW);                                                 
  delay(1000);                                   
}

