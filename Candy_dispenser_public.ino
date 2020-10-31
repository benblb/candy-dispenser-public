#include "CTBot.h"
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// LED setup
#define NUM_LEDS 6
#define DATA_PIN D8
int whichLED = 0; 

// Define the array of leds
CRGB leds[NUM_LEDS];

//Telegram bot setup
CTBot myBot;
TBMessage msg;
int candyCount = 20;  // Reboot device to reset candy counter (takes ~5 secs), change this value based on number of candies you load

//WiFi auth, Telegram bot token
String ssid = "WIFI_SSID_GOES_HERE";
String pass = "WIFI_PASSWORD_GOES_HERE";
String token = "TELEGRAM_BOT_TOKEN_GOES_HERE";  

//Stepper motor, digital pins 1-4
int Pin1 = D1; 
int Pin2 = D2; 
int Pin3 = D3; 
int Pin4 = D4; 
int _step = 0; 
bool dir = true;// false=clockwise, true=counter clockwise
int count=0;

//Sonar, digital pins 5 and 6
int trigPin = D5;
int echoPin = D6;
float duration, distance; 
int detected = 0;
int handDist = 15;

void setup() {
   
 Serial.begin(115200);

 FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
    
 pinMode(Pin1, OUTPUT);  
 pinMode(Pin2, OUTPUT);  
 pinMode(Pin3, OUTPUT);  
 pinMode(Pin4, OUTPUT);  

 pinMode(trigPin, OUTPUT); 
 pinMode(echoPin, INPUT); 

 //Connect the ESP8266 to the desired access point
 myBot.wifiConnect(ssid, pass);

  //Set the telegram bot token
 myBot.setTelegramToken(token);
} 

void loop(){

 // LED waiting flash
  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(8, 255, 100);
  }
  FastLED.show();
  delay(500);

  for(int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(8, 255, 50);
  }
  FastLED.show();

 // Sonar pulse
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW); 

 duration = pulseIn(echoPin, HIGH); 
 distance = (duration*.0343)/2; 


 // Turn motor to dispense candy
 // Full motor rotation is 4096
 if(distance < handDist && detected == 0)
 {
    // Optional anti-covering check, prevents continuous candy dispensing, slows user experience
    // detected = 1;
    whichLED = 0;

    // LEDs off
    for(int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CHSV(0, 0, 0);
    }
    FastLED.show();
      
    dir = true;
    for(int i = 0; i < 2048; i++) 
    {
      turn();
      if(i % 200 == 0)
      {
        circle();
      }
    }
    dir = false;
    for(int i = 0; i < 2048; i++)
    {
      turn();
      if(i % 200 == 0)
      {
        circle();
      }
    }

     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 

     candyCount = candyCount - 1;
     // Notify when 3 pieces are left
     if(candyCount == 3)
     {
        for(int i = 0; i < NUM_LEDS; i++)
        {
          leds[i] = CHSV(0, 255, 100);
        }
        FastLED.show();

        // Replace "1234567890" with your Telegram user number. This can be found using the get IDs bot
        myBot.sendMessage(1234567890, String(candyCount));
     }
  }

// Optional anti-covering check
/*
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin, LOW); 

 duration = pulseIn(echoPin, HIGH); 
 distance = (duration*.0343)/2; 

  if(distance > handDist)
  {
    detected = 0;
  }
*/

  // Delay needed for FastLED
  delay(500);
}


void turn() 
{ 
 switch(_step){ 
   case 0: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH); 
   break;  
   case 1: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, HIGH); 
   break;  
   case 2: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 3: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, HIGH); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 4: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 5: 
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, HIGH); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
     case 6: 
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
   case 7: 
     digitalWrite(Pin1, HIGH);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, HIGH); 
   break;  
   default: 
     digitalWrite(Pin1, LOW);  
     digitalWrite(Pin2, LOW); 
     digitalWrite(Pin3, LOW); 
     digitalWrite(Pin4, LOW); 
   break;  
 } 
 if(dir){ 
   _step++; 
 }else{ 
   _step--; 
 } 
 if(_step>7){ 
   _step=0; 
 } 
 if(_step<0){ 
   _step=7; 
 } 
 delay(1); 

}

void circle()
{
  leds[whichLED] = CHSV(8, 255, 100);

  FastLED.show();
  
  leds[whichLED] = CHSV(8, 255, 50);

  whichLED = whichLED + 1;
  
  if(whichLED == 6)
  {
    whichLED = 0;
  }
}
