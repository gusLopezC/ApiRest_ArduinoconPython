#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <RestClient.h>

#define delayTime 300     // Time in seconds beetwen sendings
#define IP "192.168.1.6"  // Server IP
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
  Serial.println("connect to network");
  client.dhcp();
  Serial.begin(9600);
  sensors.begin(); 
}
String response;
void loop(){
  response = "";
  client.setHeader("Authorization: Basic cmljdmVhbDoxMjM0==");
  client.setHeader("Content-Type: application/json");
    
    TemperaturaDS18B20();
    delay(2000);
    Varometrobmp180();
    delay(2000);
    sensorgas();
    delay(600000);
}

void TemperaturaDS18B20()  {
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
  Serial.print("<fe02f10f-6919-4fcd-a5c2-603a2d1de5f5,");
  Serial.print(sensors.getTempCByIndex(0)); //Se lee e imprime la temperatura en grados Celsius
  Serial.print(">\n");

  char json[256];
  JsonObject& root = jsonBuffer.createObject();
  root["TemperaturaDS18B20"] = sensors.getTempCByIndex(0));
  root.printTo(json, sizeof(json));
 
}

void Varometrobmp180(){
    
    Serial.print("<086ec220-c82d-434d-bef4-707c9ea16e94,");
    Serial.print(bmp.readPressure());
    Serial.print(">\n");

}

void sensorgas(){
  bool state= digitalRead(gas_PIN);
  if (!state){
    
    Serial.print("<7d9353d1-5b0a-4ffc-8acc-4374d7e3720d,");
    Serial.print(state);
    Serial.print(">\n");
    tone(pinBuzzer, 523, 1000);
  }
  else{
    Serial.print("<7d9353d1-5b0a-4ffc-8acc-4374d7e3720d,");
    Serial.print(state);
    Serial.print(">\n");
  }
  
}
