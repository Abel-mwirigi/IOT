// C++ code
//
#include <SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);

SoftwareSerial Bluetooth(0, 1);  // TX, RX (Connect to HC-05/HC-06)

#define TRIG_PIN 9
#define ECHO_PIN 10
#define LED_PIN 11
#define DANGER_LED 12
#define TMP A1
//#define MOTOR_PIN 6
#define BUZZER_PIN 8

void setup(){
  pinMode(A0,INPUT);
  pinMode(DANGER_LED, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); // for ultrasonic
  pinMode(ECHO_PIN, INPUT); // for ultrasonic
  pinMode(LED_PIN, OUTPUT); // led for motion
  //pinMode(MOTOR_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  Serial.begin(9600);
  Bluetooth.begin(9600);
  lcd.begin(16, 2);  // LCD 16x2 Setup
  
  lcd.print("System Ready...");
  delay(2000);
}
void loop ()
{
  // Ultrasonic logic for presence sensing
  int duration;
  int distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Bluetooth.print("Distance: ");
  Bluetooth.print(distance);
  Bluetooth.println(" cm");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");

  if (distance > 0 && distance < 20){
    digitalWrite(LED_PIN, HIGH);
    Bluetooth.println("Motion detected! Lights ON.");
    lcd.setCursor(0, 1);
    lcd.print("Motion: Yes");
  } else {
    digitalWrite(LED_PIN, LOW);
    Bluetooth.println("No motion. Lights OFF.");
    lcd.setCursor(0, 1);
    lcd.print("Motion: No ");
  }
  delay(500); // delay for stability
  
  // Photoresistor logic for security lights
  int A = analogRead(A0);
  Serial.print ("Light level=");
  Serial.println(A);
  Bluetooth.print("Light level: ");
  Bluetooth.println(A);

  if (A<=60){
    digitalWrite(DANGER_LED, HIGH);
    Bluetooth.println("Warning: Low light! Danger LED ON.");
  }
  else{
    digitalWrite(DANGER_LED, LOW);
    Bluetooth.println("Normal light levels. Danger LED OFF.");
  }

  // TEMP LOGIC
  int sensorValue = analogRead(TMP);
  

  Serial.print("Temperature: ");
  Serial.print(sensorValue);
  Serial.print("Celsius");

  Bluetooth.print("Temperature: ");
  Bluetooth.print(sensorValue);
  Bluetooth.println(" °C");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(sensorValue);
  lcd.print(" C");

  if (sensorValue > 120){
    digitalWrite(BUZZER_PIN, HIGH);
    //digitalWrite(DANGER_LED, HIGH);
    Bluetooth.println("🔥 FIRE ALERT! 🔥");
    lcd.setCursor(0, 1);
    lcd.print("FIRE ALERT!");
  } else{
    digitalWrite(BUZZER_PIN, LOW);
    Bluetooth.println("Temperature Normal.");
  }
  delay(2000);
}
