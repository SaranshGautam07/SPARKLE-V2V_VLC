//This program receives input through the photodiode and displays the received signal on the Serial Monitor and LCD Display
//At the end of 1 message signal, it clears the LCD Display and adds a newline to the Serial Monitor

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#define LDR_PIN A2
#define THRESHOLD 50
#define PERIOD 20////MAKE SURE THIS IS SAME AS TRANSMITTER

LiquidCrystal_I2C lcd(0x27,16,2);

bool previous_state = false;//initialize the previous and current states of received LED input as 0
bool current_state = false;
long int t1;//variables used to store time values
long int t2;
long int elapsedTime;//t2 - t1; Measures time between a rising edge and a falling edge
char c1;

void setup() 
{
    Serial.begin(9600);//initialize serial monitor
    lcd.init();//initialize LCD Display
    lcd.backlight();//set Backlight of LCD as ON
    lcd.setCursor(0,0);//set Cursor to starting position on LCD
    pinMode(5, OUTPUT);
}

void loop() 
{
  //digitalWrite(5, HIGH);
  current_state = get_ldr();
  
  /*if(current_state && !previous_state)//if previous state = 0 and current state = 1, then we start timer
  {
    t1 = millis();//measure the time at a rising edge
  }*/
  
  if(!current_state && previous_state)//if previous state = 1 and current state = 0, then we start detecting character
  {
    //t2 = millis();//measure the time at a falling edge
    
    //elapsedTime = t2 - t1;//measures time difference between a rising edge and a falling edge
    c1 = get_byte();
    print_byte(c1);
    /*if(elapsedTime >= 900)//if time diff greater than 900ms, it means we have now started to receive a fresh message signal
    {
      clearfunc();//Clear the LCD screen and add a newline to serial monitor for the new Message signal
    }
    
    else
    {
      print_byte(c1);//else print the character to LCD screen and on the serial monitor
    }*/
  }
  previous_state = current_state;
}

/*
void clearfunc()
{
  lcd.clear();
  lcd.home();
  Serial.print("\n");
}*/

bool get_ldr()//returns detected signal from PhotoDiode as 0 or 1 according to the set threshold
{
  int voltage = analogRead(LDR_PIN);
  return voltage > THRESHOLD ? true : false;
}

char get_byte()//returns the character
{
  char ret = 0;
  delay(PERIOD*1.5);//** wait for 1.5 * Period after detecting falling edge
  for(int i = 0; i < 8; i++)
  {
   ret = ret | (get_ldr() << i); 
   delay(PERIOD);
  }
  return ret;
}

void print_byte(char my_byte)//printing character to serial monitor; takes a character as input
{
  char buff[2];
  sprintf(buff, "%c", my_byte);
  Serial.print(buff);
  lcd.print(my_byte);
}
