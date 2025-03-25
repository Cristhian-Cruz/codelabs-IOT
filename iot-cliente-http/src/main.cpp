#include <Arduino.h>
#define LED 2
#include "libwiFi.h"
#include <WiFi.h>
#include "Adafruit_SHT31.h"

const char* ssid = "MIRED"; //nombre de la red wifi
const char* password = "a1b2c3d4"; //contraseña de la red wifi
const char* host = "codelab.denkitronik.com"; //host al que se va a conectar
const int port = 8080; //puerto al que se va a conectar
int temperatura;
int humedad;

Adafruit_SHT31 sht31 = Adafruit_SHT31();//creamos un objeto de la clase Adafruit_SHT31

void setup() {
  pinMode(LED, OUTPUT);
  //conectar a la red wifi
  Serial.begin(115200);
  Serial.println("Iniciando conexion a la red wifi");
  conectarWifi(ssid, password);
  if (!sht31.begin(0x44)) {  // Dirección I2C del sensor (0x44 o 0x45)
    Serial.println("¡Error! No se encontró el SHT31");
    while (1); // Detener ejecución
}

}

void loop() {
  temperatura = sht31.readTemperature();
  humedad = sht31.readHumidity();
  Serial.println("Temperatura: " + String(temperatura) + " Humedad: " + String(humedad));
  WiFiClient client;

  if (!client.connect(host, port)) {
    delay(2000);
    Serial.println("Error al conectar al servidor");
    return;
  }

  String url = "/thingname/thian?humedad=" + String(humedad) + "&temperatura=" + String(temperatura);
  Serial.println("Enviando a: " + url);

  // ENVIAMOS LA SOLICITUD GET
  client.print("GET " + url + " HTTP/1.1\r\n");
  client.print("Host: " + String(host) + ":" + String(port) + "\r\n");
  client.print("Connection: close\r\n\r\n");

  // Esperamos la respuesta del servidor
  unsigned long tiempo = millis();
  while (client.available() == 0) {
    if (millis() - tiempo > 5000) {
      Serial.println("Tiempo de espera agotado");
      client.stop();
      return;
    }
  }

  // Imprimir respuesta del servidor
  while (client.available()) {
    String respuesta = client.readStringUntil('\r');
    Serial.println(respuesta);
  }

  client.stop();
  delay(5000);  // Esperar antes del próximo envío
}
