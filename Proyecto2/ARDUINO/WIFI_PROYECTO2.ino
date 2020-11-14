// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::: PROYECTO 2 :::::::::::::::::::::::::::::::::
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Arqui2";
const char* password = "12345678";

float temperatura;

void setup() {
  Serial.begin(115200);              //Serial connection
  WiFi.begin(ssid, password);  //WiFi connection

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Esperando conexion WiFi.");
  }
  Serial.println("Conectado a red WiFi.");

}

void loop() {
  // ###############################  WIFI   ###############################
  if (WiFi.status() == WL_CONNECTED)
  { //Check WiFi connection status
    if (Serial.available())
    {
      String datos = Serial.readString();
      //Serial.print("Los datos recibidos son");
      Serial.println(datos);
      int cont = 0;
      String val = "";
      for (int i = 0; i < datos.length(); i++)
      {
        if (datos[i] == ',')
        {
          switch (cont)
          {
            case 0: {
                temperatura = stof(val);
                val = "";
                break;
              }
          }
          cont++;
        }
        else
        {
          val += datos[i];
        }
      }
      enviar();
      delay(1000);
    }
  }
  else
  {
    Serial.println("Error en conexion WiFi.");
  }
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
// ********************************      C O D E      ********************************
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

// ###############################  ENVIAR   ###############################
void enviar() {
  StaticJsonBuffer<200> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject();
  // |||||||||||||  Estructura de JSON  |||||||||||||
  JSONencoder["temperatura"] = temperatura;
  char JSONmessageBuffer[200];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  HTTPClient http;    //Declare object of class HTTPClient

  http.begin("http://api-p1-ace2.herokuapp.com/temperatura");      //Specify request destination
  http.addHeader("Content-Type", "application/json");  //Specify content-type header
  int httpCode = http.POST(JSONmessageBuffer);   //Send the request
  String payload = http.getString();            //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
}

// Funcion para convertir a float un String
float stof(String Texto) {
  char cadena[Texto.length() + 1];
  Texto.toCharArray(cadena, Texto.length() + 1);
  float myFloat = atof(cadena);

  return myFloat;
}
