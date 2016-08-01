#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1



#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
float temp = 10;
char str_temp[100];
char bufferUdp[UDP_TX_PACKET_MAX_SIZE]; // Buffer qui contiendra le paquet Udp reçu

int taille_udp = 0;


#include "WifiAccess.h"
#ifndef PASS
#error You have to define SSID and PASS in WifiAccess.h file, e.g. look in WifiAccessTemplate.h
#endif


unsigned int localPort = 8888; // port d'écoute

// Création d'une instance UDP pour recevoir et envoyer des datagrammes UDP
WiFiUDP Udp;

String message;

// Destinataire des paquets UDP (PC)
#define DEST_IP "192.168.0.32"
#define DEST_PORT 8888


void setup() 
{
  // Mise en place de la communication série
  Serial.begin(9600); 


    // Connection à un réseau WiFi
  Serial.print("Connexion à : ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);
  int tries = 1;
  while (WiFi.status() != WL_CONNECTED && tries < 100) 
  {
    delay(100);
    Serial.print(".");
    tries ++;
  }
  
  if (tries > 99)
  {
    Serial.println("");
    Serial.println("[ERREUR] IMPOSSIBLE DE SE CONNECTER AU RESEAU WIFI !"); 
  }
  else
  {
      Serial.println("");
      Serial.println("Connexion reussie ! ");  
      Serial.print("Addresse IP : ");
      Serial.println(WiFi.localIP());

      // Passage en mode écoute sur le port UDP
      if (Udp.begin(localPort))
      {
        Serial.println("");
        Serial.print("Passage en mode ecoute sur le port ");
        Serial.println(localPort);
      }
      else
      {
        Serial.println("[ERREUR] AUCUN SOCKET N'EST DISPONIBLE !");
      }
  }


  delay(100);
  // Passage à la communication série avec l'Arduino
  Serial.swap();

  // Définition des IO utilisées
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
}

void loop() 
{
  // attente datagramme UDP 
  while (taille_udp == 0)
  {
    taille_udp = Udp.parsePacket();
  }

  // réemmission du paquet UDP sur port comm
  Udp.read(bufferUdp, taille_udp);
  for (int i=0; i<taille_udp; i++)
  {
    Serial.print(bufferUdp[i]);
  }
  Serial.println();
  taille_udp = 0;

  // attente acquittement
  while (! Serial.available()) 
  {
  }
  message = Serial.readStringUntil('\n');
  
  // réémission du message en UDP
  Udp.beginPacket(DEST_IP, DEST_PORT);
  Udp.write(message.c_str()); // conversion d'un string en char*
  Udp.endPacket();

  // traitement du paquet
  traiter_paquet(message, message.length());
}

void traiter_paquet (String message, int taille)
{
  int nombreEnCours = 0;
  for (int i = 0; i < taille; i++)
  {
    char c = message[i];
    if ( ( c >= '0' ) && ( c <= '9' ) ) 
    {
      nombreEnCours = ( ( nombreEnCours * 10 ) + ( c - '0' ) );
    }

    if ( ( c >= 'A' ) && ( c <= 'Z' ) )
    {
      if ( c == 'A' ) 
      {
        analogWrite(D1, nombreEnCours);
      }
      if ( c == 'B' ) 
      {
        analogWrite(D2, nombreEnCours);
      }
      if ( c == 'C' ) 
      {
        analogWrite(D3, nombreEnCours);
      }
      nombreEnCours = 0;
    }
  }
  
}

