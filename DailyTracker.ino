#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoPixel.h>

#define LED_PIN    5
#define LED_COUNT 4
#define BUTTON1_PIN 2 // Most right (next the USB power input)
#define BUTTON2_PIN 4
#define BUTTON3_PIN 7
#define BUTTON4_PIN 8
#define DEBOUNCE_DELAY 200
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
void updateButtonsState();
void increaseCounter(uint8_t& counter,const uint8_t& max_count);
void simpleButtonHandler(const uint8_t pixel_index, uint8_t button_counter);
void smoothTransition(const uint8_t pixel_index, uint8_t button_counter);
int64_t lastRefreshTime=0;
struct ButtonHandler{
  int8_t button_pin;
  uint8_t pixel_index;
  uint8_t button_counter;
  uint8_t max_count;
  uint64_t lastDebounceTime ;
  void (*button_handler)(const uint8_t pixel_index, uint8_t button_counter);
};

typedef enum {
  Button1,
  Button2,
  Button3,
  Button4,
  ButtonCount,
}Buttons;

struct ButtonHandler ButtonHandlerCalbacks[] ={
  [Button1] = {BUTTON1_PIN,0,0,1,0, simpleButtonHandler},
  [Button2] = {BUTTON2_PIN,1,0,1,0, simpleButtonHandler},
  [Button3] = {BUTTON3_PIN,2,0,1,0, simpleButtonHandler},
  [Button4] = {BUTTON4_PIN,3,0,8,0, smoothTransition},  
};

void EmptyButtonHandler(){}
void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.clear();
  strip.setBrightness(10); // Set BRIGHTNESS to about 1/5 (max = 255)
  strip.show();            // Turn OFF all pixels ASAP

pinMode(BUTTON1_PIN, INPUT_PULLUP);
pinMode(BUTTON2_PIN, INPUT_PULLUP);
pinMode(BUTTON3_PIN, INPUT_PULLUP);
pinMode(BUTTON4_PIN, INPUT_PULLUP);
Serial.begin(9600);
}


void loop() {
  updateButtonsState();
  
  for(int8_t i =0 ; i<ButtonCount; ++i){
    ButtonHandlerCalbacks[i].button_handler(ButtonHandlerCalbacks[i].pixel_index,ButtonHandlerCalbacks[i].button_counter);
  }
}


void updateButtonsState(){
  for(int8_t i =0 ; i<ButtonCount; ++i){
    if(digitalRead(ButtonHandlerCalbacks[i].button_pin)==LOW){
      if(millis() - ButtonHandlerCalbacks[i].lastDebounceTime > DEBOUNCE_DELAY){
        increaseCounter(ButtonHandlerCalbacks[i].button_counter,ButtonHandlerCalbacks[i].max_count);
        ButtonHandlerCalbacks[i].lastDebounceTime =millis();  
      }
      
    }
  }
}

void increaseCounter(uint8_t& counter,const uint8_t& max_count){
  counter++;
  if(counter >max_count){
    counter = 0;
   
  }
}

void simpleButtonHandler(const uint8_t pixel_index, uint8_t button_counter){
  uint32_t color = strip.getPixelColor((uint16_t)pixel_index);
  // Serial.print(Color: );
  // Serial.println(color);
  if(button_counter==0){
    strip.setPixelColor(pixel_index, 0,255,0,0);
  }
  else{
    strip.setPixelColor(pixel_index, 255,0,0,0);
  }

  if(color != strip.getPixelColor(pixel_index)){
  Serial.println("Update");
   strip.show();                         
  }
    
}

void smoothTransition(const uint8_t pixel_index, uint8_t button_counter){
  uint32_t color = strip.getPixelColor(pixel_index);
  switch(button_counter){
  case 7:
    strip.setPixelColor(pixel_index, 255,0,0,0);
    break;
  case 6:
    strip.setPixelColor(pixel_index, 255,69,0,0);
    break;
  case 5:
    strip.setPixelColor(pixel_index, 255,140,0,0);
    break;
  case 4:
    strip.setPixelColor(pixel_index, 255,215,0,0);
    break;
  case 3:
    strip.setPixelColor(pixel_index, 255,255,0,0);
    break;
  case 2:
    strip.setPixelColor(pixel_index, 154,205,50,0);
    break;
  case 1:
    strip.setPixelColor(pixel_index, 124,255,0,0);
    break;
  case 0:
    strip.setPixelColor(pixel_index, 0,255,0,0);
    break;
  }
  
  if(color != strip.getPixelColor(pixel_index)){
      Serial.println("Update");
    strip.show();                          
  }
}