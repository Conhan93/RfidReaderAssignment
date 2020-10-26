
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


int power = 7;
int led = 8; 

// stores read tag - 5 bytes
unsigned char serNum[5];

int nr_cards = 0;

char buffer[40];
char input;
int nr_bytes_read = 0;

typedef struct
{
  unsigned char card_nr[5];
} Card;

Card card_list[5];

void setup(){

    
    Serial.begin(9600);
    SPI.begin();
    rfid.init();

    
/*
* define VMA100 (UNO) pins 7 & 8 as outputs and put them LOW
*/
    pinMode(led, OUTPUT);
    pinMode (power,OUTPUT);
    digitalWrite(led, LOW);
    digitalWrite (power,LOW);
   
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
      if(nr_bytes_read >40)
      {
        nr_bytes_read = 0;
      }
        
  }
      //Serial.println(buffer);
      //Serial.println("Hello?");
    
    if(rfid.isCard()){
    
        if(rfid.readCardSerial()){
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");

       
            /*
            Serial.print(cards[1][0]);
            Serial.print(" ");
            Serial.print(cards[1][1]);
            Serial.print(" ");
            Serial.print(cards[1][2]);
            Serial.print(" ");
            Serial.print(cards[1][3]);
            Serial.print(" ");
            Serial.print(cards[1][4]);
            Serial.println("");
            */
            
            
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
          Serial.println(buffer);
          Serial.println(nr_cards);
          
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
  delay(1500);
}
void red_light()
{
  Serial.println(F("Not allowed!"));
  delay(1500); 
}
