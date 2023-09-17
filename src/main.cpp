#include <Arduino.h>
#include <WiFi.h>


const char *ssid = "CHOE Y JOSE";
const char *pass = "1116232122peter9156@";
const char *host = "dweet.io";
const int port = 80; // Puerto por defecto del servicio web

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass); // Inicializar modulo WIFI y que se vincule a la red indicada en el ssid
  Serial.print("Intentando Conexion");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Conectado");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  pinMode(2, OUTPUT);    // Configurar pin 2 como salida
  digitalWrite(2, HIGH); // Encender Led
}

void loop()
{
  WiFiClient cliente; // Objeto que realiza peticiones a un socket servidor

  if (!cliente.connect(host, port))
  {
    Serial.println("Conexion Fallida al servidor");
    delay(1000);
    return;
  }

  // Si llega a este linea es porque si hubo conexion con el servidor
  // Ahora tenemos un flujo de transmision y otro de recepcion
  int randomInRange = random(100); 
  int randomInRange2 = random(100);
  
  String datos = "TEMPERATURA=" + String(randomInRange) + "&" +
                      "HUMEDAD=" + String(randomInRange2); 

  String URL = "/dweet/for/isabellafajardo?" + datos;
  String request = String("GET ") + URL + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n";

  // Enviamos la peticion en  protocolo http al servidor
  cliente.print(request);
  unsigned long inicio = millis();
  while (cliente.available() == 0)
  {
    if (millis() - inicio > 5000)
    {
      Serial.println("Tiempo de espera del servidor agotado");
      cliente.stop();
      return;
    }
  }

  // Si se llega a este punto es porque se recibieron datos del servidor (cliente.available() !=0)
  while (cliente.available())
  {
    String linea = cliente.readStringUntil('\r'); // Lea un string hasta que encuentre el caracter
    Serial.println(linea);
  }

  Serial.println("Fin de conexion al servidor");
  cliente.stop();
  delay(5000); //Para evitar de que se hagan muchas peticiones al servidor y nos meta en un black list

  /* delay(500); //Retardo de 500ms
    digitalWrite(2, LOW); //Apagar el LED
    delay(500);
    digitalWrite(2, HIGH); //Encender el LED*/
}