import hypermedia.net.*;
int PORT_RX=8888;
String HOST_IP = "192.168.0.32";//Addresse IP du PC
UDP udp;

import controlP5.*;
ControlP5 cp5;
Textarea Titre;
Textarea Affichage;

void setup()
{
  size(400,200);
  smooth();
  cp5 = new ControlP5(this);
  
  Affichage = cp5.addTextarea("txt")
                .setPosition(100,70)
                .setSize(200,45)
                .setFont(createFont("arial",20))
                .setColor(color(128))
                .setColorBackground(color(255,100))
                .setColorForeground(color(255,100));
                ;
   
  
  
  udp= new UDP(this, PORT_RX, HOST_IP);
  udp.log(true);
  udp.listen(true);
}

void draw()
{;} 

void receive(byte[] data, String ip, int port)
{
 String message = new String( data );
  
 // print the result
 println( "receive: \""+message+"\" from "+ip+" on port "+port );
 
 background(200);
 
 Affichage.setText(message);
}
