/*
Programa de prueba para control del camioncito mediante conexión WiFi
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *NOMBRE_RED = "Camioncito",
           *CLAVE_RED = "00000000"; // Son 8 ceros

const int PUERTO = 80;

// Tener un estado del LED
bool estadoDelLED = false;

IPAddress ip(192, 168, 1, 1);
IPAddress puertaDeEnlace(192, 168, 1, 1);
IPAddress mascaraDeRed(255, 255, 255, 0);

ESP8266WebServer servidor(PUERTO);

const int AIA = D1;
const int AIB = D2;
const int BIA = D3;
const int BIB = D4;
int speedA = 1023;
int speedB = 1023;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
  
  // Configurar como un access point
  WiFi.softAP(NOMBRE_RED, CLAVE_RED);
  WiFi.softAPConfig(ip, puertaDeEnlace, mascaraDeRed);
  delay(100);

  // Configuramos la ruta y la función que responderá a la solicitud de dicha ruta
  servidor.on("/", rutaRaiz);
  servidor.on("/encender", rutaEncenderLed);
  servidor.on("/apagar", rutaApagarLed);
  servidor.onNotFound(rutaNoEncontrada);
  // Empezar a escuchar
  servidor.begin();
}

// the loop function runs over and over again forever
// En el loop manejamos al cliente conectado y eso es todo ;)
void loop() {
  servidor.handleClient();
//   digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
//                                     // but actually the LED is on; this is because 
//                                     // it is active low on the ESP-01)
//   delay(1000);                      // Wait for a second
//   digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
//   delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
// ///

  // forward();
  // delay(2000);
  // backward();
  // delay(2000);
}

void quieto()
{
  analogWrite(AIA, LOW);
  analogWrite(AIB, LOW);
  analogWrite(BIA, LOW);
  analogWrite(BIB, LOW);
}

void atras()
{
  analogWrite(AIA, LOW);
  analogWrite(AIB, speed);
  analogWrite(BIA, LOW);
  analogWrite(BIB, speed);
}
void adelante()
{
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  analogWrite(BIA, speed);
  analogWrite(BIB, 0);
}

String obtenerInterfaz()
{
  String HtmlRespuesta = "<!DOCTYPE html>"
                         "<html lang='en'>"
                         "<head>"
                         "<meta charset='UTF-8'>"
                         "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                         "<title>LEDS by parzibyte</title>"
                         "</head>"
                         "<body>";
  HtmlRespuesta += "<h1>Encender y apagar LED</h1>";
  HtmlRespuesta += "<p>El LED est&aacute; actualmente ";
  if (estadoDelLED)
  {
    HtmlRespuesta += "encendido</p>";
    HtmlRespuesta += "<a href='/apagar'>Apagar</a>";
  }
  else
  {
    HtmlRespuesta += "apagado</p>";
    HtmlRespuesta += "<a href='/encender'>Encender</a>";
  }
  
  HtmlRespuesta += "</body>"
                   "</html>";
  return HtmlRespuesta;
}

void mostrarInterfazHTML()
{
  servidor.send(200, "text/html", obtenerInterfaz());
}

void rutaEncenderLed() //adelante
{
  estadoDelLED = true;
  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(AIA, speed);
  analogWrite(AIB, 0);
  analogWrite(BIA, speed);
  analogWrite(BIB, 0);
  mostrarInterfazHTML();
}

void rutaApagarLed() //quieto
{
  estadoDelLED = false;
  digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(AIA, LOW);
  analogWrite(AIB, LOW);
  analogWrite(BIA, LOW);
  analogWrite(BIB, LOW);
  mostrarInterfazHTML();
}

void rutaRaiz()
{
  mostrarInterfazHTML();
}

void rutaNoEncontrada()
{
  servidor.send(404, "text/plain", "404");
}
