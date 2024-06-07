#include <avr/io.h>

#define TRANSMIT (PORTD |= (1 << 4))
#define RECEIVE (PORTD &= ~(1 << 4))


//byte CTRL = PORT_D4;
String cc, idc, command, cs;
char id[] = "02";

void setup(){
  Serial.begin(115200);
  while(!Serial);
}

void loop(){
  if(Serial.findUntil("#","/")){
    cc = String(Serial.readStringUntil("/"));
    cc.remove(cc.length()-2);
    //cc[cc.length()-2] = '\0';
    //Serial.println(msg(cc,cs));
    Serial.println(cc);
    idc = String(cc[0]) + String(cc[1]);
    if(idc == id){
      command = "";
      for(int x=2; x<cc.length()-2;x++){
         command = command + cc[x];        
      }
      cs = String(cc[cc.length()-2]) + String(cc[cc.length()-1]);
      Serial.println(command);        
      Serial.println(cs);
      int checksum = idc.toInt()/10*16+idc.toInt()%10 + command.toInt()/10*16+command.toInt()%10 + (cs[0]-55)*16+(cs[1]-55);
      if (checksum%100 == 0){
        Serial.println(checksum, HEX);         
      }
    } 
  }
  delay(100);
}

/*String msg(String cc){ 
  String S = "*" + String(id) + cc + cs "/";
  return S;
}*/
