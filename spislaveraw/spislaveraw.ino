// Slave
#include <LiquidCrystal.h>
#define TEMPERATURE A0 

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i;

void setup (void)
{
 lcd.begin(16, 2);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // turn on SPI in slave mode
  SPCR |= bit(SPE);

  // turn on interrupts
  SPCR |= bit(SPIE);
}  // end of setup

volatile char buf [20] = "Hello, world!";
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

  if (c == 2)
{
  SPDR = 1;
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
i=analogRead(TEMPERATURE);
float tempv = i*(5.0/1023.0);
int tempC = tempv * 100;
lcd.setCursor(0, 1);
lcd.print(tempC);
 delay(1000);
}  // end of loop
