#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 6
#define LED_PIN 3

CRGB leds[NUM_LEDS];

uint8_t paletteIndex = 0;
byte messageArray[6];

void receiveEvent(int howMany) {
  Serial.println(howMany);
  for(int i = 0; i < howMany; i++){
    messageArray[i] = Wire.read();
    Serial.print(messageArray[i]);
  }
  Serial.println(); 
}

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Serial.begin(9600);           /* start serial for debug */
}


void loop() {
  // put your main code here, to run repeatedly:
  if (messageArray[3] == 2) {
    fill_palette(leds, NUM_LEDS, paletteIndex, 1, RainbowColors_p, 255, LINEARBLEND);

    EVERY_N_MILLISECONDS(50) {
      paletteIndex++;
    }
    FastLED.show();
  } else {
    FastLED.clear();
    FastLED.show();
  }
}
