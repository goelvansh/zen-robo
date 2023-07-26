// #include <SoftwareSerial.h>
// #include <ESP8266WiFi.h>

// SoftwareSerial espSerial(4, 5); // RX, TX
// String ssid = "B-702_5G";
// String password = "VvNmLpJ@B702";

// void setup() {
//   Serial.begin(9600);
//   espSerial.begin(9600);
//   delay(1000);
//   connectWiFi();
// }

// void loop() {
//   int signalStrength = getSignalStrength();
//   Serial.print("Wi-Fi signal strength: ");
//   Serial.println(signalStrength);
//   espSerial.print("Wi-Fi signal strength: ");
//   espSerial.println(signalStrength);
//   delay(2000);
// }

// int getSignalStrength() {
//   String response = sendCommand("AT+CWJAP?");
//   int index = response.indexOf("RSSI");
//   if (index == -1) {
//     return 0;
//   }
//   String signalString = response.substring(index + 5);
//   signalString.trim();
//   return signalString.toInt();
// }

// void connectWiFi() {
//   sendCommand("AT+UART_CUR?");
//   delay(2000);
//   sendCommand("AT+RST");
//   delay(1000);
//   sendCommand("AT+CWMODE=1");
//   delay(1000);
//   String cmd = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"";
//   sendCommand(cmd);
//   delay(5000);
// }

// String sendCommand(String command) {
//   espSerial.println(command);
//   delay(1000);
//   String response = "";
//   while (espSerial.available()) {
//     char c = espSerial.read();
//     response += c;
//   }
//   Serial.println(response);
//   return response;
// }

// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>

// const char ssid[] = "Galaxy52BD";  // name of wifi
// const char password[] = "vgop12345"; // password of wifi
// void setup() {
// Serial.begin(115200);
// Serial.print("Trying to connect to Wifi");
// Serial.print(ssid);

// WiFi.begin(ssid,password);

// while(WiFi.status() != WL_CONNECTED)
// {
//   delay(500);
//   Serial.print("\n");
//   Serial.print("not Connected to Wifi Yet");
// }
// Serial.print("\n");
// Serial.println("Successfully connected with Wifi");

// Serial.print("Ip Address : ");
// Serial.println(WiFi.localIP());
// Serial.print("Mac Address of Board : ");
// Serial.println(WiFi.macAddress());
// Serial.print("Subnet Mask  : ");
// Serial.println(WiFi.subnetMask());

// Serial.print("Gateway IP  : ");
// Serial.println(WiFi.gatewayIP());
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// Serial.print("WiFi Signal Strength : ");
// Serial.println(WiFi.RSSI());
// delay(5000);
// }





// #include <SoftwareSerial.h>
// #include <ESP8266WiFi.h>

// SoftwareSerial espSerial(4, 5); // RX, TX
// String ssid = "Galaxy52BD";
// String password = "vgop12345";

// void setup() {
//   Serial.begin(115200);
//   espSerial.begin(9600);
//   delay(1000);
//   connectWiFi();
// }

// void loop() {
//   int signalStrength = getSignalStrength();
//   Serial.print("Wi-Fi signal strength: ");
//   Serial.println(signalStrength);
//   espSerial.write("Wi-Fi signal strength: ");
//   espSerial.write(signalStrength);
//   delay(2000);
// }

// int getSignalStrength() {
//   return WiFi.RSSI();
// }

// void connectWiFi() {
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }
//   Serial.println("Connected to WiFi");
// }

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial espSerial(4, 5); // RX, TX
String ssid = "Galaxy52BD";
String password = "vgop12345";

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  delay(1000);
  connectWiFi();
}

void loop() {
  int signalStrength = getSignalStrength();
  Serial.print("Wi-Fi signal strength: ");
  Serial.println(signalStrength);
  espSerial.print("Wi-Fi signal strength: ");
  espSerial.println(signalStrength);
  delay(2000);
}

int getSignalStrength() {
  return WiFi.RSSI();
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

