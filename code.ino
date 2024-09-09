#include <TinyGPS++.h>
#include <SoftwareSerial.h>
// Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;
int GPSBaud = 9600;
// Create a TinyGPS++ object
TinyGPSPlus gps;
// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);
//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8
/* Change these values based on your observations */
#define wetSoil 277 // Define max value we consider soil 'wet'
#define drySoil 380 // Define min value we consider soil 'dry'
// Define analog input
#define sensorPin A0
// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 11;
const int ledPin = 13;
// defines variables
long duration;
int distance;
int safetyDistance;
void setup() {
 //ultrasonic sensor
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzzer, OUTPUT);
pinMode(ledPin, OUTPUT);
Serial.begin(9600); // Starts the serial communication
// for flame sensor
pinMode(2, INPUT); //initialize Flame sensor output pin connected pin as input.
 pinMode(LED_BUILTIN, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
 Serial.begin(9600);// initialize serial communication @ 9600 baud:
 //soil moisture sensor
 Serial.begin(9600);
 //gps and gsm
 // Start the Arduino hardware serial port at 9600 baud
 Serial.begin(9600);
 // Start the software serial port at the GPS's default baud
 gpsSerial.begin(GPSBaud);
 //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
 Serial.begin(9600);
 
 //Begin serial communication with Arduino and SIM900
 mySerial.begin(9600);
 Serial.println("Initializing..."); 
 delay(1000);
 mySerial.println("AT"); //Handshaking with SIM900
 updateSerial();
 mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
 updateSerial();
mySerial.println("AT+CMGS=\"+917858023441\"");// phone number to sms
 updateSerial();
 mySerial.print("I need help"); //text content
 mySerial.print("https://www.google.com/maps/?q=");
 delay(1000);
 updateSerial();
 mySerial.write(26);
}
void loop() {
 //ultrasonic sensor
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
safetyDistance = distance;
if (safetyDistance <= 5){
 digitalWrite(buzzer, HIGH);
 digitalWrite(ledPin, HIGH);
}
else{
 digitalWrite(buzzer, LOW);
 digitalWrite(ledPin, LOW);
}
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
//for flame sensor
if (digitalRead(2) == 0 )
 {
 digitalWrite(LED_BUILTIN, HIGH); // Led ON
 Serial.println("** Fire detected!!! **");
 }
 else
 {
 digitalWrite(LED_BUILTIN, LOW); // Led OFF
 Serial.println("No Fire detected");
 }
 delay(100);
 //soil moisture sensor
 // Read the Analog Input and print it
 int moisture = analogRead(sensorPin);
 Serial.print("Analog output: ");
 Serial.println(moisture);
 
 // Determine status of our soil
 if (moisture < wetSoil) {
 Serial.println("Status: Soil is too wet");
 } else if (moisture >= wetSoil && moisture < drySoil) {
 Serial.println("Status: Soil moisture is perfect");
 } else {
 Serial.println("Status: Soil is too dry - time to water!");
 }
 Serial.println();
 
 // Take a reading every second
 delay(1000);
 //gps and gsm
 // This sketch displays information every time a new sentence is correctly encoded.
 while (gpsSerial.available() > 0)
 if (gps.encode(gpsSerial.read()))
 displayInfo();
 // If 5000 milliseconds pass and there are no characters coming in
 // over the software serial port, show a "No GPS detected" error
 if (millis() > 5000 && gps.charsProcessed() < 10)
 {
 Serial.println("No GPS detected");
 while(true);
 }
}
//gps and gsm
void displayInfo()
{
 if (gps.location.isValid())
 {
 Serial.print("Latitude: ");
 Serial.println(gps.location.lat(), 6);
 Serial.print("Longitude: ");
 Serial.println(gps.location.lng(), 6);
 Serial.print("Altitude: ");
 Serial.println(gps.altitude.meters());
 }
 else
 {
 Serial.println("Location: Not Available");
 }
 
 Serial.print("Date: ");
 if (gps.date.isValid())
 {
 Serial.print(gps.date.month());
 Serial.print("/");
 Serial.print(gps.date.day());
 Serial.print("/");
 Serial.println(gps.date.year());
 }
 else
 {
 Serial.println("Not Available");
 }
 Serial.print("Time: ");
 if (gps.time.isValid())
 {
 if (gps.time.hour() < 10) Serial.print(F("0"));
 Serial.print(gps.time.hour());
 Serial.print(":")
if (gps.time.minute() < 10) Serial.print(F("0"));
 Serial.print(gps.time.minute());
 Serial.print(":");
 if (gps.time.second() < 10) Serial.print(F("0"));
 Serial.print(gps.time.second());
 Serial.print(".");
 if (gps.time.centisecond() < 10) Serial.print(F("0"));
 Serial.println(gps.time.centisecond());
 }
 else
 {
 Serial.println("Not Available");
 }
 Serial.println();
 Serial.println();
 delay(1000);
}
void updateSerial()
{
 delay(500);
 while (Serial.available()) 
 {
 mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
 }
 while(mySerial.available()) 
 {
 Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
 }
}
