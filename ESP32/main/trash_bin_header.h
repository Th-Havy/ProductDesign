/*
 * trash_bin_header.h
 *
 *  Created on: 2 déc. 2017
 *      Author: matthieu
 *      For the trash bin
 */

#ifndef MAIN_TRASH_BIN_HEADER_H_
#define MAIN_TRASH_BIN_HEADER_H_

#include <stdio.h>
#include "esp_err.h"
#include "esp_log.h"
#include "Adafruit_NeoPixel.h" // for pixel stick
#include "driver/i2c.h" //for audio amp

//sounds to include
extern unsigned char thank_you[];
extern unsigned int thank_you_len;

extern unsigned char applause2[];
extern unsigned int applause2_len;
extern unsigned char cheering3[];
extern unsigned int cheering3_len;
extern unsigned char combobreaker[];
extern unsigned int combobreaker_len;
extern unsigned char hallelujah[];
extern unsigned int hallelujah_len;
extern unsigned char kids_cheering[];
extern uint32_t kids_cheering_len;
extern unsigned char monster_growl[];
extern unsigned int monster_growl_len;
extern unsigned char ta_da[];
extern unsigned int ta_da_len;
extern unsigned char trex_roar[];
extern unsigned int trex_roar_len;


#define MAPPING_BREADBOARD

#ifdef MAPPING_PCB
// Pin Mapping PCB
#define PIN_ECHO1 17
#define PIN_ECHO2 4
#define PIN_ECHO3 15
#define PIN_ECHO4 19
#define PIN_TRIG1 23
#define PIN_TRIG2 22
#define PIN_TRIG3 3
#define PIN_TRIG4 21
#define PIN_PIXELSTICK         7 // A changer car cramé.
#define I2C_EXAMPLE_MASTER_SCL_IO          9//32             /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO          10//33             /*!< gpio number for I2C master data  */
#endif

#ifdef MAPPING_BREADBOARD
//Pin Mapping BreadBoard
#define PIN_ECHO1 36
#define PIN_ECHO2 39
#define PIN_ECHO3 34
#define PIN_ECHO4 35
#define PIN_TRIG1 32
#define PIN_TRIG2 33
#define PIN_TRIG3 9
#define PIN_TRIG4 10
#define PIN_PIXELSTICK   13
//Pas de réglage dynamique  du gain
#endif


//SD card
esp_err_t initialize_sd(void);
esp_err_t testing_SD();
void read_txt_file (FILE* txt_file);

//I2S for playing audio
esp_err_t play_thank_you(void);
void i2s_play_sound_wav(FILE* wav_file);
uint8_t* audio_glob_init(void);
esp_err_t play_thank_you_opt(uint8_t* i2s_write_buff);
esp_err_t play_thank_you_opt_2();
void audio_glob_init_2(void);


esp_err_t play_sound(uint8_t* i2s_write_buff, const uint8_t* sound_name_h,
		const int sound_len, char* name);

esp_err_t play_sound_2(const uint8_t* sound_name_h,
		const uint32_t sound_len, char* name);


//Measuring distance with the ultrasound sensor
double measure_distance_opt(int trig_pin, int echo_pin);
double getmeandistance(int delay);

//Audio Amplification
void i2c_example_master_init();
esp_err_t turnoffReleaseControl();
esp_err_t turnoffAGCCompression();
esp_err_t setGain(int g);

//LED stick
Adafruit_NeoPixel initLEDstick();
void testLEDstick(Adafruit_NeoPixel neopixels_stick);

#endif /* MAIN_TRASH_BIN_HEADER_H_ */
