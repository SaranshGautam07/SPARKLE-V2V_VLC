//This program takes input string and sends each character through the LED (OOK - ASCII encoding)

#include <Wire.h>

#define LED_PIN 11
#define PERIOD 20//MAKE SURE THIS IS SAME AS RECEIVER
#define LDR_PIN A2
#define THRESHOLD 100


char* string = "HELLO\n";
int string_length;

bool previous_state = false;//initialize the previous and current states of received LED input as 0
bool current_state = false;
char c1;

void setup()
{
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  string_length = strlen(string);
}

void loop() 
{
  digitalWrite(LED_PIN, LOW);
  Serial.println("Set Mode for Board: ");
  Serial.println("Press 1 for Transmission: ");
  Serial.println("Press 2 to Receive: \n\n");
  while(Serial.available() == 0)
  {}
  int choice = Serial.parseInt();

  switch(choice){
    case 1:
    digitalWrite(LED_PIN, HIGH);
    delay(5*PERIOD);
    for(int i = 0; i < 5; i++)
    {
      transmitfunction();
    }
    Serial.println("\nTransmission Terminated.\n\n");
    digitalWrite(LED_PIN, LOW);
    break;

    case 2:
    Serial.println("\nReceiving. [Send any character to Terminate Reception]");
    
    if (Serial.available() > 0) {
    // If there is data available, read it and print it
    char incomingByte = Serial.read();
    Serial.print("Start of Reception: \n");
    }
    
    while(Serial.available() == 0)
    {
      receivefunction();
    }
    
    Serial.println("\nReception Terminated.\n\n");
    break;
  }
  
  //digitalWrite(LED_PIN, HIGH);
}

void transmitfunction()
{
  for(int i = 0; i < string_length; i ++)
  {
    send_byte(string[i]);
  }
  delay(1000);
}

void receivefunction()
{
  current_state = get_ldr();
  if(!current_state && previous_state)//if previous state = 1 and current state = 0, then we start detecting character
  {
    c1 = get_byte();
    print_byte(c1);
  }
  previous_state = current_state;
}

void send_byte(char my_byte)
{
  digitalWrite(LED_PIN, LOW);
  delay(PERIOD);

  //transmission of bits
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(LED_PIN, (my_byte&(0x01 << i))!=0 );
    delay(PERIOD);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(PERIOD);

}

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
}
