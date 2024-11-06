#include <Arduino_JSON.h>
#include <JSON.h>
#include <JSONVar.h>
#include <cjson\cJSON.h>
#include <Wire.h>
#include <SPI.h>
#include <BMP280.h>
#include <LoRaNode.h>


BMP280   bmp280;

#define PIN_POWER_SENSOR 4

//variables globales
double presion;
double temperatura;
double sensor_humedad;
double humedad_calculada;

unsigned long previousMillis = 0;
const long interval = 2000;

void setup() {
  pinMode(PIN_POWER_SENSOR, OUTPUT);
  Serial.begin(9600);

  if (!LoRaNode.begin(433E6)) {
    Serial.println("No se pudo iniciar LoRa");
    while (1);
  }
  Serial.println("LoRa iniciado");

  Wire.begin(); //Join I2C bus
 
  bmp280.begin();

}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      digitalWrite(PIN_POWER_SENSOR, HIGH);           // Enciende el sensor
      delay(10);                              // Espera breve para estabilizar el sensor


      // Toma de mediciones
      temperatura = bmp280.getTemperature();
      presion = bmp280.getPressure();

      sensor_humedad = analogRead(A1);
      humedad_calculada = (1 - (sensor_humedad / 1023.0)) * 100;


      
      JSONVar doc;
      doc["TEMPERATURA"] = temperatura;
      doc["presion"] = presion;
      doc["HUMEDAD"] = humedad_calculada;

      digitalWrite(PIN_POWER_SENSOR, LOW);            // Apaga el sensor después de la medición

      LoRaNode.beginPacket();
      LoRaNode.print("HOLA");  // Envía el JSON como texto
      LoRaNode.endPacket(); 
      LoRaNode.showStatus();

      Serial.println(doc);



  }
}
