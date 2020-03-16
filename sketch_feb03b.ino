//code Nodemcu
#include <SPI_VFD.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
SoftwareSerial s(D6,D5);
BlynkTimer timer;
const byte numChars = 4;
float ReceivedValues[numChars];
char auth[] = "7JSMzN7i_XYhttl6avkS1H7jY_K3O46Y"; 
char ssid[]= "Livebox-85A0"; // Wifi name
char pass[] = "0493613157"; // Wifi password

void ReceivingData() {
  int n = 0;
  float data;
  while (s.available()) {
    data=s.read();
    Serial.println(data);
    ReceivedValues[n]=data;
    n++; }
}
void SendingData() {
  Blynk.virtualWrite(V5, ReceivedValues[0]);  //V5 is for Temperature
  Blynk.virtualWrite(V6, ReceivedValues[1]);  //V6 is for Humidity
  //if (ReceivedValues[1]<40) {
    //Blynk.notify("Humidité basse, attention!");}
  //if (ReceivedValues[1]>50) {
    //Blynk.notify("Humidité haute, attention!");}
  Blynk.virtualWrite(V7, ReceivedValues[2]);  //V7 is for Pollution
  if (ReceivedValues[2]>300) {
    Blynk.notify("Air légèrement pollué");}
  if (ReceivedValues[2]>700) {
    Blynk.notify("Air très pollué, attention danger possible!");}
  Blynk.virtualWrite(V8, ReceivedValues[3]);  //V8 is for Dust
  //if (ReceivedValues[3]>1500) {
    //Blynk.notify("Concentration importante: risque potentiel d'allergie!");}
}

void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  s.begin(115200);
  Blynk.begin(auth, ssid, pass);
  //timer.setInterval(5000L, SendingData);
}
 
void loop() {
  Blynk.run(); //Initiates Blynk
  timer.run();
  bool co = Blynk.connected();
  Serial.println(co);
  ReceivingData();
  
  //Print the data in the serial monitor
  Serial.println("");
  Serial.print("Temperature ");
  Serial.println(ReceivedValues[0]);
  Serial.print("Humidity    ");
  Serial.println(ReceivedValues[1]);
  Serial.print("Indice de pollution   ");
  Serial.println(ReceivedValues[2]);
  Serial.print("Concentration de l'air (en particles per liter)   ");
  Serial.println(ReceivedValues[3]);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  Serial.println("");
} 
