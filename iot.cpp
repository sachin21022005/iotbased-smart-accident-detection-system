#include <SoftwareSerial.h>

// GSM Module (TX, RX)
SoftwareSerial gsm(9, 10);

// GPS Module (TX, RX)
SoftwareSerial gps(4, 3);

int vibrationSensor = 2;   // Vibration sensor pin
int threshold = 1;         // Digital HIGH when vibration detected

String latitude = "";
String longitude = "";

void setup() {
  Serial.begin(9600);
  gsm.begin(9600);
  gps.begin(9600);

  pinMode(vibrationSensor, INPUT);

  Serial.println("Accident Detection System Started...");
}

void loop() {
  int vibration = digitalRead(vibrationSensor);

  // If vibration detected (accident)
  if (vibration == HIGH) {
    Serial.println("Accident Detected!");

    delay(2000);

    getGPS();

    sendSMS();

    delay(10000); // prevent multiple messages
  }
}

// Function to get GPS location
void getGPS() {
  Serial.println("Fetching GPS data...");

  while (gps.available()) {
    char c = gps.read();
    Serial.write(c);
  }

  // NOTE: For simplicity, static values used
  // Replace with GPS parsing for real use
  latitude = "13.0827";    // Example: Chennai latitude
  longitude = "80.2707";   // Example: Chennai longitude
}

// Function to send SMS alert
void sendSMS() {
  Serial.println("Sending SMS...");

  gsm.println("AT+CMGF=1"); // Set SMS mode
  delay(1000);

  gsm.println("AT+CMGS=\"+919XXXXXXXXX\""); // Replace with your number
  delay(1000);

  gsm.print("Accident Detected!\nLocation: ");
  gsm.print("https://maps.google.com/?q=");
  gsm.print(latitude);
  gsm.print(",");
  gsm.print(longitude);

  delay(1000);
  gsm.write(26); // Ctrl+Z to send SMS

  Serial.println("SMS Sent!");
}
