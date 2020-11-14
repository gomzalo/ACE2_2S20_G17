#include <Servo.h> //para motores
#include "HX711.h" //para balanza

/*MOTORES PARA ACTIVAR LOS SPRAYS*/
Servo motorDispensadorGel;
const int espera = 500;
Servo motorBuzon;
Servo motorPuerta;

/*SENSOR ULTRASONICO PARA ACTIVAR EL GEL*/
int TRIG = 11;
int ECO = 12;
int DURACION;
int DISTANCIA;

/*SENSOR DE PESO*/
const int DOUT = A1;
const int CLK = A0;
HX711 balanza;
float peso = 0.0;

/*SENSOR DE TEMPERATURA*/
float tempC; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
int pinLM35 = A2; // Variable del pin de entrada del sensor (A2)

/*SENSORES INFRARROJO*/
int SEN1 = 6;
int valorSensor = 0;

/*BUZZER*/
const int buzzer = 7; //por si la temperatura esta alta
const int buzzer2 = 5; //por si olvidó un objeto en el buzon



void setup() {
  Serial.begin(9600);
  Serial2.begin(115200); //para el modulo wifi
  //MOTORES
  motorDispensadorGel.attach(8);
  motorBuzon.attach(9);
  motorPuerta.attach(10);
  //ULTRASONICO
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  //BALANZA
  balanza.begin(DOUT, CLK);
  balanza.set_scale(346493.726); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.
  //TEMPERATURA
  analogReference(INTERNAL1V1);
  //INFRARROJO
  pinMode(SEN1, INPUT);
  //BUZZER
  pinMode(buzzer, OUTPUT); // Pin 7 declarado como salida
  pinMode(buzzer2, OUTPUT); // Pin 5 declarado como salida

}

void loop() {
  verificarGel();
  verificarBalanza();
  medirTemperatura();
  sensorPuerta();
}

void sensorPuerta()
{
  valorSensor = digitalRead(SEN1);

  if (valorSensor != 1)
  {
    if (peso > 0.02) //verifica si cuando abre la puerta hay algo en la cesta de cosas
    {
      tone(buzzer2, 50);
      delay(50);
      noTone(buzzer2);
      delay / 50);

      tone(buzzer2, 50);
      delay(50);
      noTone(buzzer2);
      delay / 50);

      tone(buzzer2, 50);
      delay(50);
      noTone(buzzer2);
      delay / 50);
    }
    else {
      delay(5000); //espera 5 segundos despues de abierta la puerta para rociar desinfectante
      motorPuerta.write(0);
      delay(espera);
      motorPuerta.write(180);
      delay(espera);

      motorPuerta.write(0);
      delay(espera);
      motorPuerta.write(180);
      delay(espera);

      motorPuerta.write(0);
      delay(espera);
      motorPuerta.write(180);
      delay(espera);

      motorPuerta.write(0);
      delay(espera);
      motorPuerta.write(180);
      delay(espera);
      motorPuerta.write(0);
    }
  }

}

void medirTemperatura()
{
  tempC = analogRead(pinLM35);
  tempC = (1.1 * tempC * 100.0) / 1024.0;
  //Serial.print(tempC)
  String cadena = "";
  cadena += tempC;
  cadena += ",";
  Serial2.print(cadena);
  delay(100);

  if (tempC > 38)
  {
    tone(buzzer, 50);
    delay(50);
    noTone(buzzer);
    delay / 50);

    tone(buzzer, 50);
    delay(50);
    noTone(buzzer);
    delay / 50);

    tone(buzzer, 50);
    delay(50);
    noTone(buzzer);
    delay / 50);
  }
}

void verificarBalanza()
{
  //Serial.print("Valor de lectura: ");
  Serial.print(balanza.get_units(20));
  //Serial.println(" kilogramos");
  peso = balanza.get_units(20);
  if (peso > 0.02) {
    //Serial.println("Hay un objeto en la pesa");
    motorBuzon.write(0);
    delay(espera);
    motorBuzon.write(180);
    delay(espera);

    motorBuzon.write(0);
    delay(espera);
    motorBuzon.write(180);
    delay(espera);

    motorBuzon.write(0);
    delay(espera);
    motorBuzon.write(180);
    delay(espera);

    motorBuzon.write(0);
    delay(espera);
    motorBuzon.write(180);
    delay(espera);
    motorBuzon.write(0);
  }
  else {
    //Serial.println("No hay un objeto en la pesa");
  }
  delay(500);
}

void verificarGel() {
  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  DURACION = pulseIn(ECO, HIGH);
  DISTANCIA = DURACION / 58.2;
  //Serial.println(DISTANCIA);
  if (DISTANCIA <= 5)
  {
    motorDispensadorGel.write(0);
    delay(espera);
    motorDispensadorGel.write(180);
    delay(espera);

    motorDispensadorGel.write(0);
    delay(espera);
    motorDispensadorGel.write(180);
    delay(espera);

    motorDispensadorGel.write(0);
    delay(espera);
    motorDispensadorGel.write(180);
    delay(espera);

    motorDispensadorGel.write(0);
    delay(espera);
    motorDispensadorGel.write(180);
    delay(espera);
    motorDispensadorGel.write(0);
  }
  delay(200);
}
