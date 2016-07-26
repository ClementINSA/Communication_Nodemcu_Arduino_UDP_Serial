#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 8); //RX, TX

const int ledRouge=9; 
const int ledVert=10; 
const int ledBleu=11; 

String message;

int no_ack = 0;
String acquitement[8] = {"0A0B0C",
                         "0A0B1023C",
                         "0A1023B0C",
                         "0A1023B1023C",
                         "1023A0B0C",
                         "1023A0B1023C",
                         "1023A1023B0C",
                         "1023A1023B1023C"};
                         

void setup() 
{
  // Mise en place Comm' Serie
  Serial.begin(9600); // Avec PC
  mySerial.begin(9600); // Avec Nodemcu
  
  // Mise en place LED RGB \\
  pinMode (ledRouge,OUTPUT); 
  pinMode (ledVert,OUTPUT);
  pinMode (ledBleu,OUTPUT); 
  pinMode (13,OUTPUT); 

  digitalWrite(ledVert,HIGH); // Extinction de la LED
  digitalWrite(ledRouge,HIGH); // Extinction de la LED
  digitalWrite(ledBleu,HIGH); // Extinction de la LED
  digitalWrite(13,HIGH);
}

void loop() 
{
  // Attente d'un message sur serial
  while (! mySerial.available()) 
  {
    Serial.print(".");
    delay(10);
  }
  message = mySerial.readString();
  Serial.println(message);
  
  // Décodage du message et allumage des LEDs
  traiter_message(message);

  //Envoi d'acquitement
  envoi_acquittement();
}

void traiter_message (String message)
{
  int taille = message.length();
  int nombreEnCours = 0;
  int R = 0;
  int G = 0;
  int B = 0;
  
  for (int i = 0; i < taille; i++)
  {
    char c = message[i];
    if ( ( c >= '0' ) && ( c <= '9' ) ) 
    {
      nombreEnCours = ( ( nombreEnCours * 10 ) + ( c - '0' ) );
    }

    if ( ( c >= 'A' ) && ( c <= 'Z' ) )
    {
      if ( c == 'R' ) 
      {
        R = nombreEnCours;
      }
      if ( c == 'G' ) 
      {
        G = nombreEnCours;
      }
      if ( c == 'B' ) 
      {
        B = nombreEnCours;
      }
      nombreEnCours = 0;
    }
  }
  ledRVBpwm (R/4, G/4, B/4);
  
}

void ledRVBpwm(int pwmRouge, int pwmVert, int pwmBleu) 
{ 
 analogWrite(ledRouge, 255-pwmRouge); 
 analogWrite(ledVert, 255-pwmVert); 
 analogWrite(ledBleu, 255-pwmBleu);
}


void envoi_acquittement()
{
  mySerial.println(acquitement[no_ack]);
  no_ack ++; 
  if (no_ack == 8) 
  {
    no_ack = 0;
  }
}

