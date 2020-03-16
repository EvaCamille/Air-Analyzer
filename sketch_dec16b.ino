//Code Arduino
//Temp&Humi
//AirQuality
//Dust
#include <SoftwareSerial.h>
#include <DHT.h>
SoftwareSerial s(5,6); //communication serie
#define DHTPIN 2 //connected to A0
#define DHTTYPE DHT11
#define AIR_PIN A1 //connected to A1
DHT dht(DHTPIN, DHTTYPE);
int DUST_PIN = 8;//connecté à I/O 8
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;// 30s
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float SensorsData[]={0, 0, 0, 0}; //set an array for the values of sensors

//  fonction qui récupère et affiche les données du capteur Temp&Humi
void sendSensorTH() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("La température est: ");
  Serial.print(t);
  Serial.println("°C");
  Serial.print("L’humidité est: ");
  Serial.print(h);
  Serial.println("%");
  if (isnan(h) || isnan(t)){ //on check si on a bien des nombres {
    Serial.println("ERROR");
    return;}
  if (h<40) {
    Serial.println("Humidité basse, attention!");}
  if (h>50) {
    Serial.println("Humidité haute, attention!");}
  SensorsData[0] = t;
  SensorsData[1] = h;
}


//  fonction qui récupère et affiche les données du capteur AirQuality
void sendSensorAQ() {
  float value = analogRead(AIR_PIN);
  Serial.print("L'indice de pollution est: ");
  Serial.println(value);
  if (isnan(value)){ //on check si on a bien des nombres {
    Serial.println("ERROR");
    return;}
  if (value>300) {
    Serial.println("Air légèrement pollué");}
  if (value>700) {
    Serial.println("Air très pollué, attention danger possible!");}
  else {Serial.println("Air frais");}
  SensorsData[2] = value;
    }

// fonction qui récupère et affiche les données du capteur Dust
void sendSensorD() {
  duration = pulseIn(DUST_PIN, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) > sampletime_ms)//temps d'échantillonnage == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0); //pourcentage
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("La concentration de l'air (en particles per cubic foot) est ");
    Serial.print(concentration);
    Serial.println("pcs/0.01 cf");
    SensorsData[3] = concentration;
    lowpulseoccupancy = 0;
    starttime = millis(); //restarting
    if (concentration>1500) {Serial.println("Concentration importante: risque potentiel d'allergie!");}
    }
   }

void setup(){
  Serial.begin(115200);
  s.begin(115200);
  dht.begin();
  pinMode(AIR_PIN, INPUT);
  pinMode(DUST_PIN,INPUT);
  starttime = millis(); //curent time
}


void loop(){
sendSensorTH();
sendSensorAQ();
sendSensorD();
for (int i=0; i<4; i++) {
s.write(SensorsData[i]);}
delay(5000);
}
