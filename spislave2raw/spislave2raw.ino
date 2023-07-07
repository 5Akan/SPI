// Slave
#include <LiquidCrystal.h>
#include <SPI.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int SensorPin = A4;

void setup (void)
{

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
   pinMode(A4, INPUT);

    lcd.begin(16, 2);
  // turn on SPI in slave mode
  SPCR |= bit(SPE);

  // turn on interrupts
  SPI.attachInterrupt();
}  // end of setup

volatile char buf [20] = "Hello, worls!";
volatile int pos;
volatile bool active;

// SPI interrupt routine
ISR (SPI_STC_vect)
{
  byte c = SPDR;

  if (c == 1)  // starting new sequence?
    {
    active = true;
    pos = 0;
    SPDR = buf [pos++];   // send first byte
    return;
    }

  if (!active)
    {
    SPDR = 0;
    return;
    }

  SPDR = buf [pos];
  if (buf [pos] == 0 || ++pos >= sizeof (buf))
    active = false;
}  // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
 int SensorValue = analogRead(SensorPin);   
  float SensorVolts = SensorValue *0.0048828125;

 
  
  
  lcd.setCursor(0, 0);  
  lcd.print(SensorValue);
  lcd.setCursor(6, 0);  
  lcd.print(SensorVolts);     
  lcd.print(" V");
  delay(500);

if(SensorVolts >= 0 && SensorVolts <= 1.6 ){
    lcd.setCursor(0, 1);  
    lcd.print("Moisture Low ");
     delay(500);
     //digitalWrite(8, HIGH);
     
  }
  if(SensorVolts >= 1.7 && SensorVolts <= 3.3){
    lcd.setCursor(0, 1);  
    lcd.print("Moisture OK ");
     delay(500);
     //digitalWrite(8, LOW);
  }
   if(SensorVolts >= 3.4 && SensorVolts <= 5){
    lcd.setCursor(0, 1);  
    lcd.print(" Moisture High ");
     delay(500);
     //digitalWrite(8, LOW);
   }
}  // end of loop
