/*************************************************************
 IOT 3000 - Oppgave 2, Temperatur og Luftfuktighetsmåler på web 
 Av Alf Almén - 081747
 
 Dette er programkoden til en Temperatur og Luftfuktighetsmåler
 som  presenterer resultatet av målingene på en nettside

 Sensoren som er brukt er en DHT 11

 Devicen er en NodeMCU
  *************************************************************/
 
 
// Legger til bibliotek for Wi-Fi
#include <ESP8266WiFi.h>

// Legger til bibliotek for webserver
#include <ESP8266WebServer.h>

//Legger til bibliotek for Temperatur og luftfuktighetsmåler
#include "DHT.h"

// Setter DHT-type til DHT 11 (det finnes flere)
#define DHTTYPE DHT11

// Assigner verdier for Wi-Fi ssid og passord
const char* ssid = "**********";
const char* password = "**********";

// Konstruerer et webserverobjekt. Svarer på port 80 
ESP8266WebServer server(80);

// Setter DHT Sensor pinne
uint8_t DHTPin = D2; 
               
// Initialiserer DHT Sensor
DHT dht(DHTPin, DHTTYPE);                

// Deklarere variabler til å holde på temperatur og luftfuktighet
float Temperature;
float Humidity;
 
 
void setup() {
	
  // Starter seriell output		
  Serial.begin(115200);
  delay(100);
  
  // Aktiverer lytting på sensorpinnen
  pinMode(DHTPin, INPUT);

  // Starter DHT sensor 
  dht.begin();              

  // Skriver info om Wi-fi pålogging til seriell monitor 
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Kobler til Wi-fi nettverk
  WiFi.begin(ssid, password);

  // Sjekker om devicen er koblet til og skriver ett punktum
  // i sekundet til seriell monitor så lenge den ikke er det
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  // Skriver bekreftelse om pålogging til seriell monitor
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
		
  // Request-handler. Kalles når det kommer en web-forespørsel til
  // web-serveren. Returnerer websiden når klienten ber om rotkatalogen.
  // I vårt tilfelle en IP-adresse
  server.on("/", handle_OnConnect);
  
  // Request-handler. Kalles når det kommer en web-forespørsel til
  // web-serveren. Returnerer "Not Found" når klienten ber om en ressurs som 
  // ikke finnes på webserveren. feks. "IP-adresse/etellerannet"
  server.onNotFound(handle_NotFound);
  
  // Starter webserveren
  server.begin();
  Serial.println("HTTP server started");

}
void loop() {
  
  // Håndterer innkommende klient-forespørsler (HTTP-requests)
  server.handleClient();
  
}

// Kalles på i server.on(). Returnerer webside med temperatur og luftfuktighet
// til klienten
void handle_OnConnect() {
 
 // Leser temperatur fra sensoren og assigner den til variabelen
 // "Temperature"
 float Temperature = dht.readTemperature();
 
 // Leser luftfuktighet fra sensoren og assigner den til variabelen
 // "Humidity"
 float Humidity = dht.readHumidity(); // Gets the values of the humidity 
 
  // Sender returkode, mediatype og HTML-dokument til klienten
  server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
}

// Kalles på i server.onNotFound(). 
// Returnerer feilmelding til klienten
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

// Funksjon som genererer HTML-dokument når den blir kallet på i "handle_OnConnect"
// funksjonen mottar to argumenter (luftfuktighet og temperatur) og returnerer et 
// komplett HTML-dokument med intern CSS med responsivt design, SVG-grafikk og med 
// aktuelle verdier for Temperatur og Luftfuktighet
String SendHTML(float Temperature,float Humidity){
   String html = "<!DOCTYPE html> <html>\n";
  html +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html +="<meta http-equiv=\"refresh\" content=\"10\" >\n";
  html +="<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  html +="<title>IOT 3000 - 081747</title>\n";
  html +="<style>html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;}\n";
  html +="body{margin-top: 50px;}\n";
  html +="h1 {margin: 50px auto 30px;}\n";
  html +=".side-by-side{display: inline-block;vertical-align: middle;position: relative;}\n";
  html +=".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}\n";
  html +=".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  html +=".humidity{font-weight: 300;font-size: 60px;color: #3498db;}\n";
  html +=".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;}\n";
  html +=".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}\n";
  html +=".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}\n";
  html +=".superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}\n";
  html +=".data{padding: 10px;}\n";
  html +="</style>\n";
  html +="</head>\n";
  html +="<body>\n";
  
   html +="<div id=\"webpage\">\n";
   
   html +="<h1>IOT 3000 - 081747<br/>oppgave 3</h1>\n";
   html +="<div class=\"data\">\n";
   html +="<div class=\"side-by-side temperature-icon\">\n";
   html +="<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n";
   html +="width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">\n";
   html +="<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142\n";
   html +="c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491\n";
   html +="c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463\n";
   html +="c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394\n";
   html +="c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>\n";
   html +="</svg>\n";
   html +="</div>\n";
   html +="<div class=\"side-by-side temperature-text\">Temperatur</div>\n";
   html +="<div class=\"side-by-side temperature\">";
   html +=(int)Temperature;
   html +="<span>&deg\;C</span></div>\n";
   html +="</div>\n";
   html +="<div class=\"data\">\n";
   html +="<div class=\"side-by-side humidity-icon\">\n";
   html +="<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">\n";
   html +="<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057\n";
   html +="c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>\n";
   html +="</svg>\n";
   html +="</div>\n";
   html +="<div class=\"side-by-side humidity-text\">Luftfuktighet</div>\n";
   html +="<div class=\"side-by-side humidity\">";
   html +=(int)Humidity;
   html +="<span>%</span></div>\n";
   html +="</div>\n";

  html +="</div>\n";
  html +="</body>\n";
  html +="</html>\n";
  return html;
}
