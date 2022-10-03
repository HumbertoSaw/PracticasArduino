// Practica de Arduino - Modulo 2 - Acceso con RFID
//Importancion de librerias
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9
#define SS_PIN 10
#define pinBuzzer 8
#define pinBoton 7

//Creacion de objetos
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x2F, 16, 2);

//Declaracion de variables
byte usuario[7] = {0x04, 0xE6, 0xAA, 0xAA, 0x9E, 0x33, 0x80};
byte lectorUID[7];
int estadoBoton = 0;
int estadoBotonAnt = 0;
int contador = 0;
bool menuBool = false;

// Inicio de metodos y de pines
void setup() {
  
  Serial.begin(9600);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinBoton, INPUT);

  SPI.begin();
  mfrc522.PCD_Init();
}

//Creacion de procesos
void loop() {
  estadoBotonRFID();
  //Calculo de proceso segun el pulsado del boton
  if(contador == 0){
    Serial.print("Presione boton");
    Serial.println(" Para una opcion");

  }else if(contador == 1){
    Serial.println("Escanear Tarjeta");

    if(!mfrc522.PICC_IsNewCardPresent()){
      return;
    }
    if(!mfrc522.PICC_ReadCardSerial()){
      return;
    }
    sonido(100,100);
    //LLamada a metodo de escaneo de usuario
    escanearUsuario();

    mfrc522.PICC_HaltA();
  }else if(contador == 2){
    Serial.print("Acerque la");
    Serial.println(" Tarjeta");
    if(!mfrc522.PICC_IsNewCardPresent()){
      return;
    }
    if(!mfrc522.PICC_ReadCardSerial()){
      return;
    }
    sonido(100,100);

    //Guardado de id
    for(byte i = 0; i < mfrc522.uid.size; i++){
      lectorUID[i] = mfrc522.uid.uidByte[i];
    }

    Serial.print("Comprobando");
    Serial.println(" Acceso");

    //Comprobacion
    if(compararUID(lectorUID,usuario)){
      Serial.print("Bienvenido ");
      Serial.println("Adrian Sansores");
      delay(3000);

      Serial.print("Iniciado");
      Serial.println(" Sesion");
      delay(1500);

      menuBool = true;

      mfrc522.PICC_HaltA();
      //Llamada de metodo menu
      menu();
    }else{
       Serial.print("Error, Usuario");
        Serial.println(" No permitido");

      sonido(1000, 500);

      mfrc522.PICC_HaltA();
    }
  }
  mfrc522.PICC_HaltA();
}
//Metodo que obtiene el estado del boton
void estadoBotonRFID(){
  estadoBoton = digitalRead(pinBoton);

  if(estadoBoton != estadoBotonAnt){
    if(estadoBoton == HIGH){
      contador++;
      if(contador == 3){
        contador = 0;
      }
    }
    delay(20);
  }
  estadoBotonAnt = estadoBoton;
}
//Metodo que activa buzzer
void sonido(int T1, int T2){
  digitalWrite(pinBuzzer,HIGH);
  delay(T1);
  digitalWrite(pinBuzzer,LOW);
  delay(T2);
}
//Metodo que escanea el usuario y genera UID en hexadecimal y decimal
void escanearUsuario(){
  Serial.print("UID: ");

  for(byte i = 0; i < mfrc522.uid.size; i++){
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);

  }
  Serial.print("  HEX\t\t");
 
  for(byte i = 0; i < mfrc522.uid.size; i++){
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  }

  Serial.print("  DEC");
}
//Metodo que compara la UID
bool compararUID(byte lector[], byte usuario[]){
  for(byte i = 0; i < mfrc522.uid.size; i++){
    if(lector[i] != usuario[i]){
      return false;
    }
  }
  return true;
}

void menu(){

  while(menuBool == true){
    inicio:
    Serial.println("Actividades");

    Serial.println("Entrar como otro usuario");
    //Reescaneo de usuario
    if(!mfrc522.PICC_IsNewCardPresent()){
      goto inicio;
    }
    if(!mfrc522.PICC_ReadCardSerial()){
      goto inicio;
    }

    for(byte i = 0; i < mfrc522.uid.size; i++){
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }

    for(byte i = 0; i < mfrc522.uid.size; i++){
      lectorUID[i] = mfrc522.uid.uidByte[i];
    }

    if(compararUID(lectorUID,usuario)){

      menuBool = false;
      Serial.println("");
      Serial.println("Cerrando sesion");
      delay(1500);

      sonido(300,300);
      sonido(300,300);
      contador = 0;
    }else{
      //Bloquear usuario no reconocido
      while(true){
        Serial.print("No eres usuario,");
  
        Serial.println(" Bloqueado !");
        sonido(10,10);
      }
    }
    mfrc522.PICC_HaltA();
  }
}