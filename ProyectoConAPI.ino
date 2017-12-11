#include <SPI.h> 
#include <Ethernet2.h>
#include <ArduinoJson.h>
#include <RestClient.h>
#define IP "127.0.0.1"  // Server IP
#define PORT 5000         // Server Port

RestClient client = RestClient(IP, PORT);
//-------------------------------------------------------
//const int sensorPin= A0;    //sensor 1 conectado a A0
//----------------------------------------------
#include <OneWire.h>              //sensor 2 conectado a pin2   
#include <DallasTemperature.h>
#define Pin 2 //Se declara el pin donde se conectará la DATA
OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature
//----------------------------------------------------
#include <Wire.h>
#include <Adafruit_BMP085.h>    //sensor 3 conectado a A4 Y A5
Adafruit_BMP085 bmp;           //Iniciamos una instancia de la librería BMP085 
//--------------------------------
const int gas_PIN = 8;       //sensor 4 conectado a pin 8
const int pinBuzzer = 9;     //relevador 1 conectado a pin 9
 
void setup(){
  delay(5000);
  Serial.begin(9600);
  sensors.begin(); 
  Serial.println("connect to network");
  client.dhcp();
  Serial.println("Setup!");
 
}
String response;
void loop(){
  
    TemperaturaDS18B20();
    delay(2000);
    Varometrobmp180();
    delay(2000);
    sensorgas();
    delay(6000);
}

void TemperaturaDS18B20()  {
 
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
   /*
  Serial.print("<fe02f10f-6919-4fcd-a5c2-603a2d1de5f5,");
  Serial.print(sensors.getTempCByIndex(0)); //Se lee e imprime la temperatura en grados Celsius
  Serial.print(">\n");*/
  response = "";
  client.setHeader("Authorization: Basic Z3VzdGF2bzoxMjM0==");
  client.setHeader("Content-Type: application/json");
  StaticJsonBuffer<200> jsonBuffer;
  float t=sensors.getTempCByIndex(0);
  char json[256];
  JsonObject& root = jsonBuffer.createObject();
  root["TemperaturaDS18B20"] = t;
  root.printTo(json, sizeof(json));
  Serial.println(json);
  int statusCode = client.post("/api/v1.0/temperature", json, &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  
 }

void Varometrobmp180(){
    /*
    Serial.print("<086ec220-c82d-434d-bef4-707c9ea16e94,");
    Serial.print(bmp.readPressure());
    Serial.print(">\n");
    */
  response = "";
  client.setHeader("Authorization: Basic Z3VzdGF2bzoxMjM0==");
  client.setHeader("Content-Type: application/json");
  StaticJsonBuffer<200> jsonBuffer2;
  float p=bmp.readPressure();
  char json2[256];
  JsonObject& root2 = jsonBuffer2.createObject();
  root2["Varometrobmp180"] = p;
  root2.printTo(json2, sizeof(json2));
  Serial.println(json2);
  int  statusCode = client.post("/api/v1.0/temperature", json2, &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
}

void sensorgas(){
  response = "";
  client.setHeader("Authorization: Basic Z3VzdGF2bzoxMjM0==");
  client.setHeader("Content-Type: application/json");
  StaticJsonBuffer<200> jsonBuffer4;
  bool state= digitalRead(gas_PIN);
  char json4[256];
  JsonObject& root4 = jsonBuffer4.createObject();
  root4["Varometrobmp180"] = state;
  root4.printTo(json4, sizeof(json4));
  if (!state){
    Serial.println(json4);
    int  statusCode = client.post("/api/v1.0/temperature", json4, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
    tone(pinBuzzer, 523, 1000);
  }
  else{
    Serial.println(json4);
    int  statusCode = client.post("/api/v1.0/temperature", json4, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
  }
 
} 
