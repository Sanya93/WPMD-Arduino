#include <EEPROM.h>

String config_string =
"APName=WPMD|"\
"APPass=pass0000|"\
"APAuth=3|"\
"APHiden=1|"\
"STAName=Xperia E3_|"\
"STAPass=1234qwer|"\
"STAIP=192.168.43.2|"\
"STAMask=255.255.255.0|"\
"STAGate=192.168.43.1|"\
"PageLogin=admin|"\
"PagePass=12345#";

int Adresses[]={0,  //APName
                20, //APPass
                40, //APAuth
                42, //APHiden
                44, //STAName
                64, //STAPass
                84, //STAIP
                104,//STAMask
                124,//STAGate
                144,//PageLogin
                164 //PagePass
};
String Config_names[] = {"APName","APPass","APAuth","APHiden","STAName","STAPass","STAIP","STAMask","STAGate","PageLogin","PagePass"};

uint32_t baud = 115200;
String s="";

void setup() {
  Serial.begin(baud);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(5,INPUT);
  digitalWrite (2, HIGH);
}

void serialEvent(){
  char a;
  while( Serial.available()>0) {
    a=Serial.read();
    if (a=='#')analyze();
    else s+=a;
    if (s.length()>255) s="";
    if (Serial.available()==0) delayMicroseconds(70);
  }
}

void analyze(){
  if (s.indexOf("control")==0) {
    power_control();
  }
  if (s.indexOf("ap")==0) {
    save_ap();
  }
  if (s.indexOf("sta")==0) {
    save_sta();
  }
  if (s.indexOf("auth")==0) {
    save_auth();
  }
  if (s.indexOf("config")>0)
  {
    get_config();
  }
  //set_pin(13,500);
  s="";
}

void power_control()
{
  String res_ans="";
  int state = digitalRead(5);
  if (s.indexOf("hpwr")>-1) {
    set_pin(3,10000);
    res_ans="off#";
  }
  else if (s.indexOf("pwr")>-1) {
    set_pin(3,500);
    delay(500);
    state = digitalRead(5);
  }
  else if (s.indexOf("rst")>-1) {
    set_pin(4,500);
  }
  else if (s.indexOf("upd")>-1) {
  }
  state==false?res_ans="off#":res_ans="on#";
  Serial.print(res_ans);
}

void save_ap()
{
  char* param = strtok(s.c_str(), "|");
  param = strtok(0, "|");
  char s_param[20];
  for (int i = 0; i<=1; i++) {
    strcpy(s_param,param);
    EEPROM.put(Adresses[i], s_param);
    param = strtok(0, "|");
  }
  int i_param;
  for (int i = 2; i<=3; i++) {
    i_param = atoi(param);
    EEPROM.put(Adresses[i], i_param);
    param = strtok(0, "|");
  }
  Serial.print("Acess Point cofiguration saved#");
}

void save_sta()
{
  char* param = strtok(s.c_str(), "|");
  param = strtok(0, "|");
  char s_param[20];
  for (int i = 4; i<=8; i++) {
    strcpy(s_param,param);
    EEPROM.put(Adresses[i], s_param);
    param = strtok(0, "|");
  }
  Serial.print("Station cofiguration saved#");
}

void save_auth()
{
  char* param = strtok(s.c_str(), "|");
  param = strtok(0, "|");
  char s_param[20];
  for (int i = 9; i<=10; i++) {
    strcpy(s_param,param);
    EEPROM.put(Adresses[i], s_param);
    param = strtok(0, "|");
  }
  Serial.print("Authorization cofiguration saved#");
}

void set_pin(int pin, int pause)
{
  digitalWrite(pin,HIGH);
  delay(pause);
  digitalWrite(pin,LOW);
}

void get_config()
{
  String conf="";
  char s_param[20]; 
  for (int i = 0; i<=1; i++) {
    EEPROM.get(Adresses[i],s_param);
    conf = conf+Config_names[i]+"="+s_param+"|";
  }
  int i_param;
  for (int i = 2; i<=3; i++) {
    EEPROM.get(Adresses[i],i_param);
    conf = conf+Config_names[i]+"="+String(i_param)+"|";
  }
  for (int i = 4; i<=10; i++) {
    EEPROM.get(Adresses[i],s_param);
    conf = conf+Config_names[i]+"="+s_param+"|";
  }
  conf+="#";
  Serial.print(conf);  
}

void loop() 
{
  /*if (s.length()>3) {
      if (s.indexOf("E:M")>0) {
      digitalWrite (2, LOW);
      delay(500);
      digitalWrite (2, HIGH);
      s="";
    }
  }*/
}

