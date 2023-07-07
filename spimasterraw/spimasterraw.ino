// master
#include <SoftwareSerial.h>
#include <SPI.h>
#define SS2 9
#define SS 10
SoftwareSerial SIM900A(2,3);

byte Mastersend;
char bufcmp [20] = "Hello, world!";
char buf2cmp [20] = "Hello, worls!";
int progcount = 0;
void setup (void)
{
   SIM900A.begin(9600);
  Serial.begin (9600);
  Serial.println ("Starting");

  pinMode(SS,OUTPUT);
  pinMode(SS2,OUTPUT);
  digitalWrite(SS, HIGH);  // ensure SS stays high for now
  digitalWrite(SS2, HIGH);
  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);

}  // end of setup

void loop (void)
{

  
  char buf [20];
  char buf2 [20];
  // enable Slave Select
  digitalWrite(SS, LOW);    
  SPI.transfer (1);   // initiate transmission
  
  for (int pos = 0; pos < sizeof (buf) - 1; pos++)
    {
    delayMicroseconds (15);
    buf [pos] = SPI.transfer (0);
    if (buf [pos] == 0)
      {
      break;
      }
    }

  buf [sizeof (buf) - 1] = 0;  // ensure terminating null

  // disable Slave Select
  digitalWrite(SS, HIGH);
  progcount++;

if (progcount >1){

  Serial.print ("We received2: ");
  Serial.println (buf);
  if(strcmp(buf,bufcmp) == 0)
  {
    Serial.println("Slave1 on network");
    }
  else{Serial.println ("Slave1 off network");
   SendMessage("Slave1 off network");
  }
}

  
  delay (1000);  // 1 second delay

   // enable Slave Select
  digitalWrite(SS2, LOW);    
  SPI.transfer (1);   // initiate transmission
  for (int pos = 0; pos < sizeof (buf2) - 1; pos++)
    {
    delayMicroseconds (15);
    buf2 [pos] = SPI.transfer (0);
    if (buf2 [pos] == 0)
      {
      break;
      }
    }

  buf2 [sizeof (buf2) - 1] = 0;  // ensure terminating null

  // disable Slave Select
  digitalWrite(SS2, HIGH);
  if (progcount >1){
  Serial.print ("We received: ");
  Serial.println (buf2);





  if(strcmp(buf2,buf2cmp) == 0)
  {
    Serial.println("Slave2 on network");
    }
  else{Serial.println ("Slave2 off network");
   SendMessage("Slave2 off network");
  }
}


  delay(1000);
}  // end of loop 

void SendMessage (char message[20])
{
  Serial.println ("Sending Message please wait....");
  SIM900A.println("AT+CMGF=1");    //Text Mode initialisation 
  delay(1000);
  Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+91XXXXXXXXXX\"\r"); // Receiver's Mobile Number
  delay(1000);
  Serial.println (message);
  SIM900A.println(message);// Messsage content
  delay(100);
  Serial.println ("Done");
  SIM900A.println((char)26);//ASCII code for CTRL + Z   delay(1000);
  Serial.println ("Message sent succesfully");
}
