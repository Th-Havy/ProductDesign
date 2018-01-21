/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
//#include <avr/sfr_defs.h>// for _BV

#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H



typedef uint8_t  neoPixelType;

typedef struct {
      bool       begun;         // true if it has begun
      uint16_t   numLEDs; 		// Number of RGB LEDs in strip
      uint16_t   numBytes;		// Size of 'pixels' buffer below (3 or 4 bytes/pixel)
      int8_t     pin; 		    //Output pin Number (-1 if not set).
      uint8_t    brightness;
      uint8_t   *pixels;        // Holds LED color values (3 or 4 bytes each)
	  uint8_t    rOffset;       // Index of red byte within each 3- or 4-byte pixel
	  uint8_t    gOffset;       // Index of green byte
	  uint8_t    bOffset;       // Index of blue byte
  } Adafruit_NeoPixel;

 void begin(Adafruit_NeoPixel);
 void setPin(Adafruit_NeoPixel,uint8_t);
 void setPixelColor(Adafruit_NeoPixel,uint16_t, uint8_t, uint8_t, uint8_t);
 void setallPixelColor(Adafruit_NeoPixel ,uint8_t, uint8_t, uint8_t); //RGB
 void setBrightness(Adafruit_NeoPixel,uint8_t);
 void espShow(uint8_t, uint8_t*, uint32_t);

#endif // ADAFRUIT_NEOPIXEL_H
