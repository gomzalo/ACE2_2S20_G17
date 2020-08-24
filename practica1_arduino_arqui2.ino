#include "HX711.h"   //<----- libreria para balanza
#include <Servo.h>


//**********************************************
//************SENSOR DE PESO********************
const int DOUT=A1;
const int CLK=A0;
int led_peso=7;
HX711 balanza;
//**********************************************
//**********************************************


//**********************************************
//************NIVEL DE DESINFECTANTE************
int TRIG=10;
int ECO=9;
int DURACION;
int DISTANCIA;
int led_maximo=4;
int led_medio=5;
int led_bajo=6;
//**********************************************
//**********************************************

//**********************************************
//************PUERTA ABIERTA/CERRADA************
int entrada_infraroja= 2;
//**********************************************
//**********************************************


//**********************************************
//************MOTOR DE SPRAY *******************
/*
 * GRIS=GND
 * ROJO=Vcc
 * NARANJA=OUT
*/
Servo motor1;
const int espera=1000;

//*********************************************
//********VARIABLES PARA LA LOGICA*************
//*********************************************
bool bandera=false;
float peso;
int puerta;
int nivel;

void setup() {
  Serial.begin(9600);
  //peso  
  balanza.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:  ");
  Serial.println(balanza.read());
  Serial.println("No ponga ningún objeto sobre la balanza");
  Serial.println("Destarando...");  
  Serial.println("...");  
  balanza.set_scale(346493.726); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado Tara.
  Serial.println("Coloque un peso conocido:");
  pinMode(led_peso,OUTPUT);

  //nivel de desinfectante
  pinMode(TRIG,OUTPUT);
  pinMode(ECO,INPUT);
  pinMode(led_maximo,OUTPUT);
  pinMode(led_medio,OUTPUT);
  pinMode(led_bajo,OUTPUT);
  

  //puerta abierta o cerrada
  pinMode(entrada_infraroja, INPUT);

  //motor para el spray
  motor1.attach(8);//pin para el servo 

  sensorNivelLiquido();

}

void loop() {

  if(bandera==true)
  {
    sensorPeso();
    if(peso<0.03)
    {
      bandera=false;
    }
    
  }
  else //bandera==false
  {
    sensorPeso();
    if(peso>0.03)
    {
      sensorTapadera();
      if(puerta==0) //esta cerrada
      {
        activarSpray();
        bandera=true;
        sensorNivelLiquido();
        Serial.println("los valores actuales son: ");
        Serial.print("Peso: ");
        Serial.println(peso);
        Serial.println("Objeto: 1");
        Serial.print("Nivel de liquido: ");
        Serial.println(nivel);      
        Serial.write(peso);
        Serial.write(nivel);
        delay(1000);
      }
    }   
  }
  delay(1000);

  /*
   * verifica si bandera esta activda, entonces hay un paquete dentro q ya fue rociado pero no a sido recogido
   *    si peso es mayor a cero, entonces seguir avanzando porque el paquete sigue alli    
   *    si peso es menor a cero, poner bandera en desactivada
   *    
   * si la bandera esta desactivada entonces verificar:
   *     peso menor a cero
   *        entonces no hay nada en la caja, avanzar y seguir verificando cada 5 segundo
   *        
   *      sino si peso es mayor a cero entonces
   *        verificar si tapa esta cerrada
   *          si si entonces rociar liquido y poner bandera en activada
   *           - obtener peso
   *           -obtener nivel de loquido
   *           -objeto = si
   *           -hacer un arreglo y mandar los datos a wifi
   *        si no esta cerrada 
   *          avanzar al principio
   *        
   *        
   *     
   * 
   * 4)
   */

}


void sensorPeso(){
  Serial.print("Valor de lectura: ");
  Serial.print(balanza.get_units(20));
  Serial.println(" kilogramos");
  peso=balanza.get_units(20);
  delay(500);

  if(peso>0.03)
  {
    digitalWrite(led_peso,HIGH);
  }
  else
  {
    digitalWrite(led_peso,LOW);
  }
}

void sensorNivelLiquido(){
  digitalWrite(TRIG,HIGH);
  delay(1);
  digitalWrite(TRIG,LOW);
  DURACION=pulseIn(ECO,HIGH);
  DISTANCIA=DURACION/58.2;
  Serial.print("Nivel de desinfectante: ");
  Serial.println(DISTANCIA);
  
  if(DISTANCIA>=0 && DISTANCIA<=4)
  {
    Serial.println("NIVEL DE DESINFECTANTE LLENO");    
    nivel=1;
    digitalWrite(led_maximo,HIGH);
    digitalWrite(led_medio,LOW);
    digitalWrite(led_bajo,LOW);
    
  }
  else if(DISTANCIA>=5 && DISTANCIA<=8)
  {
    Serial.println("NIVEL DE DESINFECTANTE MEDIO");    
    nivel=2;
    digitalWrite(led_maximo,LOW);
    digitalWrite(led_medio,HIGH);
    digitalWrite(led_bajo,LOW);
  }
  else
  {
    Serial.println("NIVEL DE DESINFECTANTE MUY BAJO");      
    nivel=3;
    digitalWrite(led_maximo,LOW);
    digitalWrite(led_medio,LOW);
    digitalWrite(led_bajo,HIGH);
  }
  delay(1000);
  
}

void sensorTapadera(){
  int value_entrada = digitalRead(entrada_infraroja);
  if(value_entrada==1)
  {    
    Serial.println("PUERTA ABIERTA");
    puerta=1;
  }
  else
  {    
    Serial.println("PUERTA CERRADA");
    puerta=0;
  }
  delay(1000);
}

void activarSpray(){
  motor1.write(0);
  delay(espera);
  motor1.write(40);
  delay(espera);

  motor1.write(0);
  delay(espera);
  motor1.write(40);
  delay(espera);

  motor1.write(0);
  delay(espera);
  motor1.write(40);
  delay(espera);

  motor1.write(0);
  delay(espera);
  motor1.write(40);
  delay(espera);
  motor1.write(0);  
}
