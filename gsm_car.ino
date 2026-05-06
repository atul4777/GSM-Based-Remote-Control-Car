#include "SoftwareSerial.h"

# define SIM800L_Tx 11
# define SIM800L_Rx 12

int LMotor1 = 5;
int LMotor2 = 6;
int RMotor1 = 10;
int RMotor2 = 9;

SoftwareSerial SIM800L(SIM800L_Tx, SIM800L_Rx);

char dtmf_cmd;
bool call_flag = false;

void init_gsm();
void update_robot();

void setup()
{
  SIM800L.begin(9600);
  Serial.begin(9600);


  pinMode(LMotor1, OUTPUT);
  pinMode(LMotor2, OUTPUT);
  pinMode(RMotor1, OUTPUT);
  pinMode(RMotor2, OUTPUT);
  init_gsm();
}

void loop()
{
  String gsm_data;
  int x = -1;

  // Store serial data from SIM800L
  while (SIM800L.available())
  {
    char c = SIM800L.read();
    gsm_data += c;
    delay(10);
  }

  // Check if DTMF is receive from SIM800L
  if (call_flag)
  {

    x = gsm_data.indexOf("DTMF");
    if (x > -1)
    {
      dtmf_cmd = gsm_data[x + 6];
      Serial.println(dtmf_cmd);
      update_robot();
    }

    x = gsm_data.indexOf("NO CARRIER");
    if (x > -1)
    {
      // Terminate ongoing call when call is disconnected
      SIM800L.println("ATH");
      call_flag = false;
    }
  } else {
    x = gsm_data.indexOf("RING");
    if (x > -1)
    {
      delay(1000);
      SIM800L.println("ATA");
      call_flag = true;
    }
  }

}


void init_gsm()
{

  boolean gsm_Ready = 1;
  Serial.println("initializing GSM module");
  while (gsm_Ready > 0)
  {
    SIM800L.println("AT");
    Serial.println("AT");
    while (SIM800L.available())
    {
      if (SIM800L.find("OK") > 0)
        gsm_Ready = 0;
    }
    delay(15000);
  }
  Serial.println("AT READY");

  boolean ntw_Ready = 1;
  Serial.println("finding network");
  while (ntw_Ready > 0)
  {
    SIM800L.println("AT+CPIN?");
    Serial.println("AT+CPIN?");
    while (SIM800L.available())
    {
      if (SIM800L.find("+CPIN: READY") > 0)
        ntw_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("NTW READY");

  boolean DTMF_Ready = 1;
  Serial.println("turning DTMF ON");
  while (DTMF_Ready > 0)
  {
    SIM800L.println("AT+DDET=1");
    Serial.println("AT+DDET=1");
    while (SIM800L.available())
    {
      if (SIM800L.find("OK") > 0)
        DTMF_Ready = 0;
    }
    delay(2000);
  }
  Serial.println("DTMF READY");

}


void update_robot()
{
  if (dtmf_cmd == '1') {
    //forward
    analogWrite(LMotor1, 80);
    analogWrite(LMotor2, 0);
    analogWrite(RMotor1, 80);
    analogWrite(RMotor2, 0);

  }

  if (dtmf_cmd == '2') {
    //reverse
    analogWrite(LMotor1, 0);
    analogWrite(LMotor2, 80);
    analogWrite(RMotor1, 0);
    analogWrite(RMotor2, 80);

  }

  if (dtmf_cmd == '3') {
    //Right
    analogWrite(LMotor1, 0);
    analogWrite(LMotor2, 0);
    analogWrite(RMotor1, 100);
    analogWrite(RMotor2, 0);

  }
  if (dtmf_cmd == '4') {
    //Forward
    analogWrite(LMotor1, 100);
    analogWrite(LMotor2, 0);
    analogWrite(RMotor1, 0);
    analogWrite(RMotor2, 0);
  }
  if (dtmf_cmd == '5') {
    //Stop
    analogWrite(LMotor1, 0);
    analogWrite(LMotor2, 0);
    analogWrite(RMotor1, 0);
    analogWrite(RMotor2, 0);
  }


}
