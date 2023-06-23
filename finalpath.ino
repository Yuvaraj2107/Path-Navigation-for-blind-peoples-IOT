#include <NewPing.h>

#include <SPI.h>
#include <MFRC522.h>

#define LEFT_SENSOR 4  // Define the digital pin connected to the left IR sensor
#define RIGHT_SENSOR 5  // Define the digital pin connected to the right IR sensor
#define TRIGGER_PIN 6  // Define the digital pin connected to the ultrasonic sensor's trigger pin
#define ECHO_PIN 7  // Define the digital pin connected to the ultrasonic sensor's echo pin
#define buzzerPin 2  // Define the digital pin connected to the buzzer
#define induction_PIN 8 // Defines the induction pin

#define SS_PIN 10
#define RST_PIN 9

#define MAX_DISTANCE 200 //Object detection
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance


void setup() {
  Serial.begin(9600);    // Initialize serial communication at 9600 baud rate
  pinMode(LEFT_SENSOR, INPUT);  // Set the left sensor pin as an input
  pinMode(RIGHT_SENSOR, INPUT);  // Set the right sensor pin as an input
  //pinMode(TRIGGER_PIN, OUTPUT);  // Set the trigger pin as an output
  //pinMode(ECHO_PIN, INPUT);  // Set the echo pin as an input
  pinMode(buzzerPin, OUTPUT);  // Set the buzzer pin as an output
  pinMode(induction_PIN, INPUT);
  SPI.begin(); // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize MFRC522
  Serial.println("Scan your RFID tag...");

}

void loop() {
  // put your main code here, to run repeatedly:
  int leftSensor = digitalRead(LEFT_SENSOR);  // Read the digital value from the left sensor pin
  int rightSensor = digitalRead(RIGHT_SENSOR); // Read the digital value from the right sensor pin
  int inductionSensor = digitalRead(induction_PIN); // Read the digital value from the induction sensor pin

  delay(100);
  unsigned int distance = sonar.ping_cm(); // distance calculation

   if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {    // RFID is used to find out desired room or destination using tags.
    // Get the UID of the card
    Serial.println("Scan your RFID tag...");
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    // Display the UID on the serial monitor
    Serial.println("UID: " + uid);
    if(uid=="9dd0e836")  // For example tag id : 9dd0e836 is our destination.
    {
      digitalWrite(buzzerPin, HIGH);   // when your destination is reached buzzer is set to high
      delay(50);
      digitalWrite(buzzerPin, LOW);  // To turn off the buffer when not in use  
    }
    if(uid=="73013b1c")  // Other destination tag id.
    {
      delay(2000);
    }
  
    mfrc522.PCD_StopCrypto1();
  }

  else 
  {
  if(inductionSensor==LOW)   // To identify the originality of black metal line which the person is following 
  {
    // Check for obstacles
  if (distance!=0 && distance < 10) { 

    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(100);
    digitalWrite(buzzerPin,LOW);
    Serial.println("Obstacle detected");  // when ultrasonic sensor detects objects
  }

 else if (leftSensor == HIGH && rightSensor == LOW) {
    // Turn left
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(200); 
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    Serial.println("Turn left");

  } else if (leftSensor == LOW && rightSensor == HIGH) {
    // Turn right
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(300);
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer 
    Serial.println("Turn right");

  } else if (leftSensor == LOW && rightSensor == LOW) {
    // Move forward
    digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
    delay(500); 
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    Serial.println("Move forward");
  } 
  
  }
  }

  delay(200);  
}
