//Practica de Arduino - Modulo 1 - Motor Arraca/Paro
//Declaracion de variables necesarias
const int pinBoton = 2 ;
const int pinMotor = 3;
const int pinLed = 4;

int velocidad;
boolean encendido = false;
//Inicio de "mode" para los pines
void setup() {
  // put your setup code here, to run once:
  pinMode(pinBoton, INPUT);
  pinMode(pinMotor, OUTPUT);
  pinMode(pinLed, OUTPUT);

  Serial.begin(9600);

}

void loop() {
  //impresion de mensaje
  Serial.println("\n\nPresione el boton");

  while(digitalRead(pinBoton)==LOW);
  //Cambio entre arranque y paro del motor
  if(encendido ==false){
    Serial.println("Arrancando Motor:");
    digitalWrite(pinLed, HIGH);

    for(velocidad = 0; velocidad <= 255; velocidad++){
      analogWrite(pinMotor, velocidad);
      delay(30);
      Serial.println(porcentaje(velocidad));
      Serial.print("%");
    }

    Serial.println("Motor arrancando al 100%");

    for(int i=1; i <=3; i++){
      estadoLed();
    }

    digitalWrite(pinLed, HIGH);
    encendido = true;

  }else{
    Serial.println("Parando motor:");
    for(velocidad = 255; velocidad >= 0; velocidad--){
      analogWrite(pinMotor, velocidad);
      delay(30);
      Serial.println(porcentaje(velocidad));
      Serial.print("%");
    }
    digitalWrite(pinLed, LOW);
    encendido = false;
  }

}
//Metodo que calcula el porcentaje
int porcentaje (int velocidad){
  int operacion = velocidad * 100;
  int resultado = operacion / 255;

  return resultado;
}

//Metodo que cambia el estado del led
void estadoLed(){
  digitalWrite(pinLed, HIGH);
  delay(500);
  digitalWrite(pinLed, LOW);
  delay(500);
}
