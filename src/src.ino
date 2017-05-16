#include <DigitalToggle.h>

//Pin definition
static int trigger = 9; // Pin HC-SR04
static int echo = 8; // Pin HC-SR04
static int led_HC = 10; // Pin HC-SR04
static int led_pulsed = 13; // Timed pulsed led
static int led_temperature = 12; // Led to temperature overflow
static int LM35_analogPin = 0; // analog pin for temperature sensor


// Variables for calculating the distance with sensor HC-SR04
long duration; // Flight time of signal
long distance;  // Calculated distance 
float temp_C; // temperature variable (Celsius degrees)

void setup() {
  //Serial communication
  Serial.begin(9600);

  //Pin setup
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led_HC, OUTPUT);
  pinMode(led_pulsed, OUTPUT);
  pinMode(led_temperature, OUTPUT);

  digitalWrite(echo, LOW);
  digitalWrite(trigger, LOW);
  digitalWrite(led_HC, LOW);
  digitalWrite(led_pulsed, LOW);
  digitalWrite(led_temperature, LOW);

  //Analog input
  analogReference(INTERNAL); // prendo la misura analogica di riferimento di Arduino a 1.1V

  //Variable initialization of the distance
  duration = 0;
  distance = 0;
}

void loop() {
}

void loop1(3000) {
  //Send a HIGH pulse to the trigger pin
  digitalWrite(trigger, HIGH);
  //I leave it to the HIGH value for 10 microseconds
  delayMicroseconds(10);
  //I carry it back to the LOW state
  digitalWrite(trigger, LOW);

  //I get the number of microseconds for which the echo PIN is left in HIGH state
  duration = pulseIn(echo, HIGH);

  /*Sound velocity is 340 meters per second, or 29 microseconds per cent.
    Our impulse travels back and forth, so to calculate the distance
   Between the sensor and our obstacle we need to do:*/
  distance = duration / 29 / 2;

  // Turn on the led if there is an obstacle at a distance of less than 10 cm
  if (distance < 10) {
    digitalWrite(led_HC, HIGH);
  }
  else {
    digitalWrite(led_HC, LOW);
  }
  // Print on the serial buffer
  Serial.print("duration : ");
  Serial.print(duration);
  Serial.print(" - distance : ");
  Serial.println(distance);
}

void loop2(7000) {
  int i;
  Serial.println("Pulsed LED");
  // Code for the flashing of the led
  for (i = 0; i < 5; i++)
  {
    digitalWrite(led_pulsed, HIGH);
    delay(500);
    digitalWrite(led_pulsed, LOW);
    delay(500);
  }
}

void loop3(11000) {
  float sum_temp = 0;
  float average_temp = 0;
  /* Acquire 3 values from the sensor every 500 milliseconds.
     Through an appropriate conversion I get the measure in degrees Celsius.
     Calculate the average of subsequent measurements.*/
  for (int i = 0; i < 3; i++){  
    delay(500);
    temp_C = (1.1 * analogRead(LM35_analogPin) * 100.0) / 1024;
    sum_temp += temp_C;
  }
  average_temp = sum_temp / 3;
  // If the temperature is greater than 23 degrees, turn on the led
  if (average_temp > 23) {
    digitalWrite(led_temperature, HIGH);
  }
  else {
    digitalWrite(led_temperature, LOW);
  }
  Serial.print("Temperature : ");
  Serial.println(average_temp);
}



