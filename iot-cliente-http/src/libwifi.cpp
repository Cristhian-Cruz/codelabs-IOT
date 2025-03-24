#include <WiFi.h>
#include "libwifi.h"

void conectarWifi(const char* ssid, const char* password){
    WiFi.begin(ssid, password);
    Serial.print("Conectando a la red wifi");
    while(WiFi.status() != WL_CONNECTED){
        delay(1000);
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Conectado a la red wifi");
    Serial.print("Direccion IP: ");
    Serial.println(WiFi.localIP());//imprime la direccion IP asignada
}