/*
* edited by Velleman / Patrick De Coninck
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno - Velleman VMA100):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA (MSS on Velleman VMA405) : Pin 10
* RST: Pin 9
* VCC: 3,3V (DO NOT USE 5V, VMA405 WILL BE DAMAGED IF YOU DO SO)
* GND: GND on Arduino UNO / Velleman VMA100
* IRQ: not used
*/

#include <SPI.h>
#include <RFID.h>
#include <stdlib.h>
#include <string.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN,RST_PIN);


// led light for door
int led = 8; 

// stores read tag - 5 bytes
unsigned char serNum[5];

// number of cards stored on device - max set to 5 but can take more.
int nr_cards = 0;

// stores input
char buffer[40];
// recieves input
char input;
int nr_bytes_read = 0;

typedef struct
{
  unsigned char card_nr[5];
} Card;

// list of cards
Card card_list[5];

void setup(){

    
    Serial.begin(9600);
    SPI.begin();
    rfid.init();

    // set up for led light
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
   
}
void add_card(char* buffer);
bool check_card();
void clear_cards();
void green_light();
void red_light();

void loop(){
    if(Serial.available() > 0)
    {
      input = Serial.read();
      buffer[nr_bytes_read] = input;
      nr_bytes_read++;
      if(input == '\0' || input == '\n')
      {
        Serial.println(buffer);
        if(strstr(buffer,"ADDCARD") && nr_cards < 5)
        {
          add_card(buffer);
        }
        if(strstr(buffer,"CLEARALLCARDS"))
        {
          clear_cards();
        }
        if(strstr(buffer,"OPENDOOR"))
        {
          green_light();
        }
        nr_bytes_read = 0;
      }
      if(nr_bytes_read == 40)
      {
        nr_bytes_read = 0;
      }
        
  }   
  if(rfid.isCard())
  {
    if(rfid.readCardSerial())
    {
      if(check_card())
      {
        green_light();
      }
      else
      {
        red_light();
      }
    }
  }
}
void add_card(char* buffer)
{ // adds new card to card list
          
          char delim[] = ".";

          char* digit = strtok(buffer,delim);
          int iteration = 0;
          // splits input string
          while(digit)
          {
            if(iteration > 0)
            {
              // adds card ID numbers to card ID
 
              card_list[nr_cards].card_nr[iteration-1] = (unsigned char)atoi(digit);
            
              
            }
  
            // moves onto next word/split string
            digit = strtok(NULL,delim);
            iteration++;
          }
          nr_cards++;
          Serial.println("Card added!");
}
bool check_card()
{
  // checks if read card is stored and has access
  bool access = false;
  for(int x = 0; x < nr_cards; x++)
  {
    for(int i = 0; i < sizeof(rfid.serNum); i++ )
    {
      if(rfid.serNum[i] != card_list[x].card_nr[i])
      {
        access = false;
        break;
      } 
      else 
      {
          access = true;
      }
    }
    if(access)
    {
      rfid.halt();
      return true;
    }
  }
  
   rfid.halt();
   return false;
}
void clear_cards()
{ // sets nr cards to zero.
  nr_cards = 0;
}
void green_light()
{ // opens door
  Serial.println(F("Welcome Velleman "));
  digitalWrite(led, HIGH);
  delay(3000);
  digitalWrite(led, LOW);
}
void red_light()
{
  // flashes light five times
  for(int index = 0 ; index < 5 ; index++)
  {
    digitalWrite(led,HIGH);
    delay(100);
    digitalWrite(led,LOW);
    delay(100);
  }
}
