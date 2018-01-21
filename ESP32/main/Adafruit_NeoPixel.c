

#include <stdlib.h>
#include "Adafruit_NeoPixel.h"
#include "driver/gpio.h"
#include "trash_bin_header.h"


void begin(Adafruit_NeoPixel stick) {
  if(GPIO_IS_VALID_OUTPUT_GPIO(stick.pin)) {
	    gpio_pad_select_gpio(stick.pin);
	    gpio_set_direction(stick.pin, GPIO_MODE_OUTPUT);
        gpio_set_level(stick.pin, 0);
  }
  stick.begun = 1;
}

// Set pixel color from separate R,G,B components:
void setPixelColor(Adafruit_NeoPixel stick,
 uint16_t n, uint8_t r, uint8_t g, uint8_t b) {

  if(n < stick.numLEDs) {
    if(stick.brightness) { // See notes in setBrightness()
      r = (r * stick.brightness) >> 8;
      g = (g * stick.brightness) >> 8;
      b = (b * stick.brightness) >> 8;
    }
    uint8_t *p;
    p = &stick.pixels[n * 3];    // 3 bytes per pixel
    p[stick.rOffset] = r;          // R,G,B always stored
    p[stick.gOffset] = g;
    p[stick.bOffset] = b;
  }
}

void setallPixelColor(Adafruit_NeoPixel stick,uint8_t r, uint8_t g, uint8_t b)
{
	  for(int i=0;i<stick.numLEDs;i++){
		  setPixelColor(stick,i, r,  g,  b);
	  }
}

void showPixelColor (Adafruit_NeoPixel neopixels_stick)
{for(int i=0;i<neopixels_stick.numLEDs;i++){
	  printf("Pixel numéro: %d\n", i);
	  printf("Rouge %d\n", neopixels_stick.pixels[3*i]);
	  printf("Vert %d\n", neopixels_stick.pixels[3*i+1]);
	  printf("Bleu %d\n", neopixels_stick.pixels[3*i+2]);
}
}



// Adjust output brightness; 0=darkest (off), 255=brightest.  This does
// NOT immediately affect what's currently displayed on the LEDs.  The
// next call to show() will refresh the LEDs at this level.  However,
// this process is potentially "lossy," especially when increasing
// brightness.  The tight timing in the WS2811/WS2812 code means there
// aren't enough free cycles to perform this scaling on the fly as data
// is issued.  So we make a pass through the existing color data in RAM
// and scale it (subsequent graphics commands also work at this
// brightness level).  If there's a significant step up in brightness,
// the limited number of steps (quantization) in the old data will be
// quite visible in the re-scaled version.  For a non-destructive
// change, you'll need to re-render the full strip data.  C'est la vie.
void setBrightness(Adafruit_NeoPixel stick,uint8_t b) {
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  uint8_t newBrightness = b + 1;
  if(newBrightness != stick.brightness) { // Compare against prior value
    // Brightness has changed -- re-scale existing data in RAM
    uint8_t  c,
            *ptr           = stick.pixels,
             oldBrightness = stick.brightness - 1; // De-wrap old brightness value
    uint16_t scale;
    if(oldBrightness == 0) scale = 0; // Avoid /0
    else if(b == 255) scale = 65535 / oldBrightness;
    else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
    for(uint16_t i=0; i<stick.numBytes; i++) {
      c      = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
    stick.brightness = newBrightness;
  }
}





static uint32_t _getCycleCount(void) __attribute__((always_inline));
static inline uint32_t _getCycleCount(void) {
  uint32_t ccount;
  __asm__ __volatile__("rsr %0,ccount":"=a" (ccount));
  return ccount;
}


#define F_CPU 160*1000*1000 //default CPU frequency, 160 MHz, can be changed in make menuconfig, Component config, ESP32 specific

#define CYCLES_800_T0H  (F_CPU / 2500000) // 0.4us
#define CYCLES_800_T1H  (F_CPU / 1250000) // 0.8us
#define CYCLES_800      (F_CPU /  800000) // 1.25us per bit

void espShow(uint8_t pin, uint8_t *pixels, uint32_t numBytes) {

  uint8_t *p, *end, pix, mask;
  uint32_t t, time0, time1, period, c, startTime;

  p         =  pixels;
  end       =  p + numBytes;
  pix       = *p++;
  mask      = 0x80;
  startTime = 0;
  time0  = CYCLES_800_T0H;
  time1  = CYCLES_800_T1H;
  period = CYCLES_800;
  for(t = time0;; t = time0) {
    if(pix & mask) t = time1;                             // Bit high duration
    while(((c = _getCycleCount()) - startTime) < period); // Wait for bit start

    gpio_set_level(pin, 1);
    startTime = c;                                        // Save start time
    while(((c = _getCycleCount()) - startTime) < t);      // Wait high duration

    gpio_set_level(pin, 0);
    if(!(mask >>= 1)) {                                   // Next bit/byte
      if(p >= end) break;
      pix  = *p++;
      mask = 0x80;
    }
  }
  while((_getCycleCount() - startTime) < period); // Wait for last bit
}

Adafruit_NeoPixel initLEDstick() {
	uint numpixels =8;
	uint8_t* pixel_values = (uint8_t*) calloc(3*numpixels, sizeof(char)); // 3 bytes per pixels (RGB)
	Adafruit_NeoPixel neopixels_stick = {
			.begun= 0,
			.numLEDs=numpixels,
			.numBytes=3*numpixels,  //only 3 bytes per pixel (rgb)
			.pin = PIN_PIXELSTICK,
			.brightness =0, //à changer 0=darkest (off), 255=brightest
			.pixels = pixel_values,
			.rOffset=1,//depends on the stick, color orders in the bytes sent
			.gOffset=0,
			.bOffset=2,
	};
	begin(neopixels_stick);
	return neopixels_stick;
}

void testLEDstick(Adafruit_NeoPixel neopixels_stick)
{
	//Red
	setPixelColor(neopixels_stick,0,15,0,0);
	espShow(neopixels_stick.pin,neopixels_stick.pixels,neopixels_stick.numBytes); // This sends the updated pixel color to the hardware.
	vTaskDelay(1000 / portTICK_RATE_MS);
	//Green
	setPixelColor(neopixels_stick,0,0,15,0);
	espShow(neopixels_stick.pin,neopixels_stick.pixels,neopixels_stick.numBytes);
	vTaskDelay(1000 / portTICK_RATE_MS);
	//Blue
	setPixelColor(neopixels_stick,0,0,0,15);
	espShow(neopixels_stick.pin,neopixels_stick.pixels,neopixels_stick.numBytes);
	vTaskDelay(1000 / portTICK_RATE_MS);
	printf("done testing the LED stick.\n");
}
