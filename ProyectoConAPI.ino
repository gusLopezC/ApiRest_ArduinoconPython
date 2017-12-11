#include <SPI.h> 
//#include <Ethernet2.h>
#include <ArduinoJson.h>
#include <RestClient.h>
#define IP "127.0.0.1"  // Server IP
#define PORT 5000         // Server Port
#include <SD.h>

File Archivo;

RestClient client = RestClient(IP, PORT);
//-------------------------------------------------------
const int sensorPin= A0;    //sensor 1 conectado a A0
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

   if(!SD.begin(4)){
    Serial.println("Se ha producido un fallo al iniciar la comunicacion");
    return;  
    }
    Serial.println("Se ha iniciado la comunicacion correctamente");
    
   
 
 
}
String response;
void loop(){
   
 
  sensors.requestTemperatures(); //Prepara el sensor para la lectura
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

  client.setHeader("Authorization: Basic Z3VzdGF2bzoxMjM0==");
  client.setHeader("Content-Type: application/json");
  StaticJsonBuffer<200> jsonBuffer2;
  float p=bmp.readPressure();
  char json2[256];
  JsonObject& root2 = jsonBuffer2.createObject();
  root2["Varometrobmp180"] = p;
  root2.printTo(json2, sizeof(json2));
  Serial.println(json2);
  int  statusCode2 = client.post("/api/v1.0/temperature", json2, &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode2);

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
    int  statusCode3= client.post("/api/v1.0/temperature", json4, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode3);
    tone(pinBuzzer, 523, 1000);
  }
  else{
    Serial.println(json4);
    int  statusCode = client.post("/api/v1.0/temperature", json4, &response);
    Serial.print("Status code from server: ");
    Serial.println(statusCode);
  }

  Archivo = SD.open("hola.txt", FILE_WRITE);     
 
  if (Archivo) {
    Archivo.print("Datos: ");
    // A continuacion escribimos el valor de la variable pot y saltamos a la linea siguiente.
    Archivo.println(json);
    Archivo.println(json2);
    Archivo.println(json4);
    Archivo.println("-----5s-----");
    // Cerramos el archivo.
    Archivo.close();
     Serial.println("todos los datos fueron almacenados");
    // Avisamos de que se ha podido escribir correctamente.
    Serial.println("impresion correcta");
  // Si no pudimos escribir en el fichero avisamos por el puerto serie.
  }else{
    Serial.println("Error al escribir en datosPIR.txt");
  }   
  // Esperamos 5s para tomar la siguiente medida.
  delay(8000);

   Archivo = SD.open("hola.txt");
        if(Archivo){
          Serial.println("informacion contenida en hola.txt: ");
          delay(5000);
          while (Archivo.available()){
            Serial.write(Archivo.read());
            }
            Archivo.close();
          }
        else{
          Serial.println("el archivo datos.txt no se abrio correctamente");
          }
      delay(10000);

}
