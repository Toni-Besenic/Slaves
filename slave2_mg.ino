/*
*
* Slave program
*
* Written by: Marin Gudelj, 24/06/2024
*/

#include <avr/io.h>
#include <DS18B20.h>

#define TRANSMIT (PORTD |= (1 << 4))
#define RECEIVE (PORTD &= ~(1 << 4))

DS18B20 ds(PORTC |= (1<<4));


//byte CTRL = PORT_D4;
String stringinfo;
char info [] = "", id[] = "02", idinf[3], ccinf[3], csinf[3], hexString[4], csout[3], ccout[3];
int checksum = 0, idnum, ccnum, csnum, temp, t, cstop = 256;
bool csflag = 0;

void setup(){
  DDRB = 0x00;
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  Serial1.begin(115200);
  Serial.begin(115200);
  while(!Serial1);
}

void loop(){
  RECEIVE;
  while(!Serial1.available()){
    Serial.println("Nema");
  }
  if(Serial1.findUntil("#","/")){
    Serial.println("Dobivam");
    stringinfo = (Serial1.readStringUntil("/"));
    Serial.println(stringinfo);
    //stringinfo.remove(stringinfo.length()-2);
    //Serial.println(stringinfo);
    for (int x=0; x<2; x++){
      idinf[x] = stringinfo[x];
      ccinf[x] = stringinfo[x+2];
      csinf[x] = stringinfo[x+4];
    }
    idinf[2]='\0';
    ccinf[2]='\0';
    csinf[2]='\0';
    idnum = strtoul(idinf, NULL, 16);
    ccnum = strtoul(ccinf, NULL, 16);
    csnum = strtoul(csinf, NULL, 16);
    checksum = idnum + ccnum + csnum;
    snprintf(hexString, sizeof(hexString), "%X", checksum);
    if ((atoi(hexString))%10 == 0){
      csflag = 1;
      Serial.println("Skibidi");
      
    }
    }
    if (csflag){
      if (atoi(ccinf)/10 >=3 and atoi(ccinf)/10 <=6){
          digitalWrite(((atoi(ccinf)/10)+3), !bool((atoi(ccinf)%10)));
          snprintf(csout, sizeof(csout), "%X", 256-2-1);
          TRANSMIT;
          Serial1.print("*");
          Serial1.print(id);
          Serial1.print("01");
          Serial1.print(csout);
          Serial1.println("/");
          Serial1.flush();
      }
      if (atoi(ccinf) == 20){
        Serial.println("20");
        byte portBstate = PINB;
        if (int(portBstate)>254){
          cstop = 512;
        }else{
          cstop = 256;
        }
        snprintf(ccout, sizeof(ccout), "%X", int(portBstate));
        snprintf(csout, sizeof(csout), "%X", cstop-2-int(portBstate));
          TRANSMIT;
          Serial1.print("*");
          Serial1.print(id);
          Serial1.print(ccout);
          Serial1.print(csout);
          Serial1.println("/");
          Serial1.flush();
          Serial.println(portBstate);
          Serial.println(ccout);
          Serial.println(csout);
      }
      if (atoi(ccinf) == 25){
        temp = ds.getTempC();
        snprintf(ccout, sizeof(ccout), "%X", temp);
        snprintf(csout, sizeof(csout), "%X", 256-2-temp);
          TRANSMIT;
          Serial1.print("*");
          Serial1.print(id);
          Serial1.print(ccout);
          Serial1.print(csout);
          Serial1.println("/");
          Serial1.flush();     
      }
      csflag = 0;
    }   
}
