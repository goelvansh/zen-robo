#include <NewPing.h>
#include <DHT.h>
// #include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
int signalStrength = 0;
float temperature = 0.0;
float humidity = 0.0;
float distance = 0.0;

int targetSignal = -50;
float targetTemp = 22.0;
float targetHumidity = 50.0;

float weightSignal = 0.4;
float weightTemp = 0.4;
float weightHumidity = 0.2;

float bestScore = -999.0;
int bestSignal = 0;
float bestTemp = 0.0;
float bestHumidity = 0.0;
int bestDistance = 0;

// Set the RX and TX pins for the software serial connection
SoftwareSerial esp8266(5, 6); // RX, TX

#define DHTPIN 4 // Pin where the DHT11 sensor is connected
#define DHTTYPE DHT11 // Type of sensor used

NewPing sonar(2, 3, 200);
DHT dht(DHTPIN, DHTTYPE); // Create an instance of the DHT sensor

#define IR_SENSOR_RIGHT 12
#define IR_SENSOR_LEFT 11
#define MOTOR_SPEED 180

//Right motor
// int enableRightMotor=6;
int rightMotorPin1=7;
int rightMotorPin2=8;

//Left motor
// int enableLeftMotor=5;
int leftMotorPin1=9;
int leftMotorPin2=10;
// byte TCCR0B;

void setup()
{
  Serial.begin(9600);    // Initialize serial communication
  esp8266.begin(9600);  // Set the baud rate for the software serial connection
  Serial.println("Starting...");
  dht.begin();           // Initialize DHT11 sensor
  //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
  //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
  //For that we need to increase the frequency of analogWrite.
  //Below line is important to change the frequency of PWM signal on pin D5 and D6
  //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
  //This sets frequecny as 7812.5 hz.
  // TCCR0B = TCCR0B & B11111000 | B00000010 ;
  
  // put your setup code here, to run once:
  // pinMode(enableRightMotor, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  // pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  rotateMotor(0,0);   
}

unsigned long lostLineTime = 0;
bool lineLost = false;
const unsigned long lineLostDuration = 3000; // Time in milliseconds

void loop()
{
  // Send AT command to the ESP8266 to get the signal strength
  // esp8266.println("AT+CWJAP?");

  // // Wait for the response from the ESP8266
  // delay(1000);
  // while (esp8266.available() > 0) {
  //   // Read the response from the ESP8266
  //   String response = esp8266.readString();
  //   Serial.println(response);
    
  //   // Extract the signal strength from the response
  //   int start = response.indexOf("RSSI:");
  //   if (start != -1) {
  //     int end = response.indexOf("dBm", start);
  //     if (end != -1) {
  //       String rssi_str = response.substring(start + 5, end);
  //       int rssi = rssi_str.toInt();
  //       Serial.print("Signal Strength: ");
  //       Serial.print(rssi);
  //       Serial.println(" dBm");
  //     }
  //   }
  // }
  delay(2000);
  if (esp8266.available() >= 0) {
    String response = esp8266.readStringUntil('\n');
    if (response.startsWith("Wi-Fi signal strength: ")) {
      signalStrength = response.substring(23).toInt();
      Serial.print("Signal strength: ");
      Serial.print(signalStrength);
    }
  }
  
  float temperature = dht.readTemperature(); // Read temperature from the DHT sensor
  float humidity = dht.readHumidity(); // Read humidity from the DHT sensor
  int distance = sonar.ping_cm();
  // int signalStrength = WiFi.RSSI();
  Serial.println(" Temperature: " + String(temperature) + "°C "+"Humidity: " + String(humidity)+"% "+"Distance: " + String(distance)+"cm"+"\n");
  //" Wifi Strength "+ String(signalStrength)+
  // Serial.println("Humidity: " + String(humidity));
  // Serial.println("Distance: " + String(distance));

  // Calculate the scores
  float scoreSignal = (targetSignal - signalStrength) / 100.0;
  float scoreTemp = 1.0 - abs(targetTemp - temperature) / targetTemp;
  // Subtracting the normalized value from 1.0 is a way to convert it into a score where
  // a higher value indicates better compliance with the target value. 
  // For example, if the target temperature is 22 degrees Celsius and the current temperature is 20 degrees Celsius, 
  //then the absolute difference is 2 degrees Celsius. 
  //Dividing this by the target temperature of 22 gives 0.0909. 
  //Subtracting this from 1.0 gives a score of 0.9091, 
  //which indicates that the current temperature is close to the target temperature and thus receives a high score.

  float scoreHumidity = 1.0 - abs(targetHumidity - humidity) / targetHumidity;
  float score = (weightSignal*scoreSignal + weightTemp*scoreTemp + weightHumidity*scoreHumidity) ;

  // Update the best score and tuple if the current score is better
  if (score > bestScore) {
    bestScore = score;
    bestSignal = signalStrength;
    bestTemp = temperature;
    bestHumidity = humidity;
    bestDistance = distance;
  }


  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  // Stop moving if the robot is not on the line
  if (rightIRSensorValue == HIGH || leftIRSensorValue == HIGH) {
    // Print the best tuple and score
    Serial.print("Best tuple: ");
    Serial.print(bestSignal);
    Serial.print(", ");
    Serial.print(bestTemp);
    Serial.print(", ");
    Serial.print(bestHumidity);
    Serial.print(" - Score: ");
    Serial.print(bestScore);
    Serial.print(" Best Distance: ");
    Serial.println(bestDistance);
    // Wait for the robot to be placed back on the line
    // while ((rightIRSensorValue || leftIRSensorValue)==LOW) {
    //   delay(2000);
    // }
    // Reset the best score and tuple
    bestScore = -999.0;
    bestSignal = 0;
    bestTemp = 0.0;
    bestHumidity = 0.0;
    // return;
  }

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  }
  //If right sensor detects black line, then turn right
  // else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  // {
  //     rotateMotor(-MOTOR_SPEED, MOTOR_SPEED); 
  // }
  // //If left sensor detects black line, then turn left  
  // else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  // {
  //     rotateMotor(MOTOR_SPEED, -MOTOR_SPEED); 
  // } 
  //If both the sensors detect black line, then stop 
  else 
  {
    rotateMotor(0, 0);
  }
}


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }

  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else 
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  // analogWrite(enableRightMotor, abs(rightMotorSpeed));
  // analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}