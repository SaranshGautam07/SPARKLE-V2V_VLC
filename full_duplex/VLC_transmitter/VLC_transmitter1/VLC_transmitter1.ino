//This program takes input string and sends each character through the LED (OOK - ASCII encoding)
//Comment out digitalWrite(LED_PIN, LOW) in loop() function to run.

#define LED_PIN 11
#define PERIOD 20//MAKE SURE THIS IS SAME AS RECEIVER


char* string = " Channel 1\n";
int string_length;

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  string_length = strlen(string);
}

void loop() 
{
  loopfunction();
  //digitalWrite(LED_PIN, LOW);
}

void loopfunction()
{
  for(int i = 0; i < string_length; i ++)
  {
    send_byte(string[i]);
  }
  delay(1000);
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
