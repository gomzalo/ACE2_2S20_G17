#include "HX711.h"
//MOTORES
int IN1 = 4;
int IN2 = 5;
int ENA = 2;

int IN3 = 6;
int IN4 = 7;
int ENB = 3;

int val1 = 0;
int val2 = 0;

//SENSORES INFRARROJO
int SEN1 = 8;
int SEN2 = 9;


//SENSOR DE OBSTACULOS
int TRIG = 10;
int ECO = 9;
int DURACION;
int DISTANCIA;

//SENSOR DE PESO
const int DOUT = A1;
const int CLK = A0;
HX711 balanza;

//VARIABLES PARA LOGICA
float peso;
int obstaculos;
bool banderaobstaculo;

String estado;

String ubicacion;

int modo = 0;

int reversa = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(115200); //para el modulo wifi
  //MOTORES
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  //INFRARROJO
  pinMode(SEN1, INPUT);
  pinMode(SEN2, INPUT);

  //OBSTACULOS
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);

  //PESO
  balanza.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningún objeto sobre la balanza");
  Serial.println("Destarando...");
  Serial.println("...");
  balanza.set_scale(346493.726); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.
  Serial.println("Coloque un peso conocido:");
}

void loop() {

  sensorPeso();
  //sino hay peso en la balanza estar parado
  if (peso < 0.03)
  {
    detener();
  }
  else
  {
    estado = "en camino";
    ubicacion = "en recorrido";
    obstaculos();
    val1 = digitalRead(SEN1);
    val2 = digitalRead(SEN2);

    if (reversa == 0)
    {
      if (val1 == 1 && val2 == 1)
      {
        avanzar();
      }
      else if (val1 == 0 && val2 == 1)
      {
        derecha();
      }
      else if (val1 == 1 && val2 == 0)
      {
        izquierda();
      }
      else
      {
        detener();
      }

    }
    else
    {
      if (val1 == 1 && val2 == 1)
      {
        retroceso();
      }
      else if (val1 == 0 && val2 == 1)
      {
        derecha();
      }
      else if (val1 == 1 && val2 == 0)
      {
        izquierda();
      }
      else
      {
        detener();
      }

    }
    String cadena = "";
    cadena += obstaculos;
    cadena += ",";
    cadena += peso;
    cadena += ",";
    cadena += estado;
    cadena += ",";
    cadena += ubicacion;
    Serial2.print(cadena);
    delay(1000);
  }
}


void sensorPeso()
{
  /*Serial.print("Valor de lectura: ");
    Serial.print(balanza.get_units(20));
    Serial.println(" kilogramos");*/
  peso = balanza.get_units(20);
}

void obstaculos()
{
  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  DURACION = pulseIn(ECO, HIGH);
  DISTANCIA = DURACION / 58.2;
  if (DISTANCIA >= 0 && DISTANCIA <= 4)
  {
    Serial.println("OBJETOS DETECTADO");
    estado = "detenido por objeto";
    obstaculos += 1;
    detener();
  }

}

void avanzar()
{

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void detener()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void derecha()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void izquierda()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


void retroceso()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
