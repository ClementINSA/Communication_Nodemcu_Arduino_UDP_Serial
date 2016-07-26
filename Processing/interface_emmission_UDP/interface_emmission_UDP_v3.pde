//*** UDP ***\\
import hypermedia.net.*;
int port_dest=8888;
String ip_dest="192.168.0.12";
UDP udp;

import controlP5.*;

ControlP5 cp5;

boolean Send = false;
boolean Load = false;
boolean GetNext = false;
boolean CopyTo = false;

String mon_message = "";
String mon_texte = "";

String[] mon_fichier; 


int parseur = 0; 

void setup() 
{
  //*** Configuration de Udp ***\\
  udp=new UDP(this);
  udp.log(true);
  noLoop();
  
  size(350,400);
  smooth();
  cp5 = new ControlP5(this);
  
  //*** Création entrée texte ***\\
  cp5.addTextfield("message")
   .setPosition(20,80)
   .setSize(200,40)
   .setFont(createFont("arial",20))
   .setAutoClear(false)
   ;
  
  //*** Création bouton envoyer ***\\
  cp5.addButton("Send")
     .setPosition(250,80)
     .setSize(60,40)
     ;
     
  //*** Création bouton charger ***\\
  cp5.addButton("Load")
     .setPosition(250,230)
     .setSize(60,40)
     ;
     
  //*** Création bouton modifier ***\\
  cp5.addButton("GetNext")
     .setPosition(250,280)
     .setSize(60,40)
     ;
     
  //*** Création bouton modifier ***\\
  cp5.addButton("CopyTo")
     .setPosition(250,330)
     .setSize(60,40)
     ;
    
  
  //*** Chargement fichier ***\\
  mon_fichier = loadStrings("commandes.txt");
  println("le fichier contient " + mon_fichier.length + " lignes");
}

void draw() 
{
    
    if (Load == true)
    {
      background(200);
      println(mon_fichier[0]);
      mon_texte = mon_fichier[0];
      fill(0);
      textSize(20);
      text(mon_texte, 20, 250);
      parseur = 1;
      Load = false;
    }
    
    
    if (GetNext == true)
    {
      background(200);
      println(mon_fichier[parseur]);
      mon_texte = mon_fichier[parseur];
      fill(0);
      textSize(20);
      text(mon_texte, 20, 250);
      parseur = parseur + 1 ;
      if (parseur == mon_fichier.length) 
      {
        parseur = 0;
      }
      GetNext = false;
    }
    
    
    if (CopyTo == true)
    {
      cp5.get(Textfield.class, "message").setText(mon_texte);
      CopyTo = false;
    }
    
    
    if (Send == true)
    {
      mon_message = cp5.get(Textfield.class,"message").getText();
      println(mon_message);
      if (mon_message != "")
      {
        udp.send(mon_message,ip_dest,port_dest);
      }
      clear();
      Send = false;
    }
    delay(1);
    loop();
}

void clear() 
{
  cp5.get(Textfield.class,"message").clear();
}