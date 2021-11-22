//This Programm is based on the adafruit strannd test. 
//Most of the comment explaining functions is their work.
//Go jeck out their work on https://www.adafruit.com/
//For the multithreading check out the GitHub https://github.com/ivanseidel/ArduinoThread
//


// A basic everyday NeoPixel strip test program.
// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h> //add Adafruit NeoPixel used version 1.7.0
#include <Thread.h>           //and ArduinoThread by IvanSeidel used version 2.1.1
#include <StaticThreadController.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PINSTRIP    4 //4
#define LED_COUNTSTRIP 18

#define LED_PINSWING    6 //6
#define LED_COUNTSWING 15

#define LED_PINstar    5 //5
#define LED_COUNTstar 4

#define LED_PINwingis    8
#define LED_COUNTwingis 6

//Here you declare the Threads as pointers and tell them which function to use. Note there are no () on  the function. 

Thread* starThread = new Thread(stars);
Thread* bigWingThread = new Thread(bigWing);
Thread* smalWingThread = new Thread(sWing);
Thread* wingsThread = new Thread(wings);

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNTSTRIP, LED_PINSTRIP, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel smalwing(LED_COUNTSWING, LED_PINSWING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel star(LED_COUNTstar, LED_PINstar, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel wingis(LED_COUNTwingis, LED_PINwingis, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)xx
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//Here the Controller gets to know wich threads he has to manage 

StaticThreadController<4> controll (bigWingThread, starThread, smalWingThread, wingsThread);

// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  
  smalwing.begin();         
  smalwing.show();            
  smalwing.setBrightness(50);
  
  star.begin();         
  star.show();            
  star.setBrightness(50);
  
  wingis.begin();         
  wingis.show();            
  wingis.setBrightness(50); 
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
controll.run(); //Start the Controller25

// stars(); for testing the funnctionns
// sWing();
// wings();
// bigWing();
}

void bigWing(){
    colorWipe(strip.Color(255, 255, 255),250);
    rainbow(10);             // Flowing rainbow cycle along the whole strip
    colorWipe(strip.Color(255, 255, 255),250);
   
  }
void wings(){
    colorWipewingis(wingis.Color(255, 255, 255),250);
    rainbowwingis(10); 
    colorWipewingis(wingis.Color(255, 255, 255),250); 
          
  }
void stars(){
    colorWipestar(star.Color(255,   215,   0), 50);            
    theaterChaseRainbowstar(50); 
    colorWipestar(star.Color(255,   215,   0), 50);           

  }
void sWing(){
    colorWipesmalwing(smalwing.Color(255, 255, 255),250);
    rainbowsmalwing(10);      
    colorWipesmalwing(smalwing.Color(255, 255, 255),250);        
  }

// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void colorWipesmalwing(uint32_t color, int wait) {
  for(int i=0; i<smalwing.numPixels(); i++) { 
    smalwing.setPixelColor(i, color);         
    smalwing.show();                        
    delay(wait);                     
  }
}

void rainbowsmalwing(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<smalwing.numPixels(); i++) { 
    
    int pixelHue = firstPixelHue + (i * 65536L / smalwing.numPixels());
     smalwing.setPixelColor(i, smalwing.gamma32(smalwing.ColorHSV(pixelHue)));
    }
    smalwing.show(); 
    delay(wait);  
  }
}

void theaterChaseRainbowsmalwing(int wait) {
  int firstPixelHue = 0;     
  for(int a=0; a<30; a++) { 
    for(int b=0; b<3; b++) {
      smalwing.clear();        
      for(int c=b; c<smalwing.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / smalwing.numPixels();
        uint32_t color = smalwing.gamma32(smalwing.ColorHSV(hue)); 
        smalwing.setPixelColor(c, color); 
      }
      smalwing.show();               
      delay(wait);                 
      firstPixelHue += 65536 / 90; 
    }
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void colorWipestar(uint32_t color, int wait) {
  for(int i=0; i<star.numPixels(); i++) { 
    star.setPixelColor(i, color);         
    star.show();                        
    delay(wait);                     
  }
}

void rainbowstar(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<star.numPixels(); i++) { 
    
    int pixelHue = firstPixelHue + (i * 65536L / star.numPixels());
     star.setPixelColor(i, star.gamma32(star.ColorHSV(pixelHue)));
    }
    star.show(); 
    delay(wait);  
  }
}

void theaterChaseRainbowstar(int wait) {
  int firstPixelHue = 0;     
  for(int a=0; a<30; a++) { 
    for(int b=0; b<3; b++) {
      star.clear();        
      for(int c=b; c<star.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / star.numPixels();
        uint32_t color = star.gamma32(star.ColorHSV(hue)); 
        star.setPixelColor(c, color); 
      }
      star.show();               
      delay(wait);                 
      firstPixelHue += 65536 / 90; 
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void colorWipewingis(uint32_t color, int wait) {
  for(int i=0; i<wingis.numPixels(); i++) { 
    wingis.setPixelColor(i, color);         
    wingis.show();                        
    delay(wait);                     
  }
}

void rainbowwingis(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<wingis.numPixels(); i++) { 
    
    int pixelHue = firstPixelHue + (i * 65536L / wingis.numPixels());
     wingis.setPixelColor(i, wingis.gamma32(wingis.ColorHSV(pixelHue)));
    }
    wingis.show(); 
    delay(wait);  
  }
}

void theaterChaseRainbowwingis(int wait) {
  int firstPixelHue = 0;     
  for(int a=0; a<30; a++) { 
    for(int b=0; b<3; b++) {
      wingis.clear();        
      for(int c=b; c<wingis.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / wingis.numPixels();
        uint32_t color = wingis.gamma32(wingis.ColorHSV(hue)); 
        wingis.setPixelColor(c, color); 
      }
      wingis.show();               
      delay(wait);                 
      firstPixelHue += 65536 / 90; 
    }
  }
}
