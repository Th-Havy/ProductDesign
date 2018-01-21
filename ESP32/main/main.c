/*
 * Main smart trash bin project.
 *
 *  Created on: 2 déc. 2017
 *      Author: matthieu
 */

/*
 * Unsolved questions.
 *  Begin with nvs_flash_init ?
 *  Task system ?
 *  Initalize I2S each time we play a file ?
 *  A new thing is thrown into the bin, do we detect this event using the fillness variation
 *  (once the thing has reached the bottom), or do we detect before, the hand does not get inside
 *  with the geometry we have chosen.
 */

#include "freertos/FreeRTOS.h" // for vTaskDelay
#include "trash_bin_header.h"
#include "esp_system.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include "math.h" //for fabs(double) : absolute value of a double

void testsound_SD()
{
	FILE* wav_file = fopen("/sdcard/pinkfloyd.WAV", "rb");
	if (wav_file == NULL) {
		ESP_LOGE("sd_opening", "Failed to open wav file for reading");
	}
	i2s_play_sound_wav(wav_file);
	printf("done testing playing a wav file from the SD card.\n");

}

int adaptLEDlevel (Adafruit_NeoPixel stick, double measured_dist, double max_dist, double min_dist)
{
	int lednumbers=7;//only 7 LEDs for the level
	if (measured_dist<min_dist){
	    setallPixelColor(stick,15,0,0);
	    setPixelColor(stick,0,0,0,15);//the first pixel is always blue
		espShow(stick.pin,stick.pixels,stick.numBytes); // This sends the updated pixel color to the hardware.
		return 7;
	}
	double step = (max_dist-min_dist)/lednumbers;
	int nb_red = (max_dist - measured_dist)/step;
	printf("I am adapting LED level \n");
    printf("Number RED LED is %d \n", nb_red);

    setPixelColor(stick,0,0,0,15);//the first pixel is always blue
	espShow(stick.pin,stick.pixels,stick.numBytes); // This sends the updated pixel color to the hardware.

	for (int i=1; i<nb_red+1; i=i+1){
	   setPixelColor(stick,8-i, 15, 0, 0);
	   espShow(stick.pin,stick.pixels,stick.numBytes); // This sends the updated pixel color to the hardware.
	}
	for (int i=nb_red+1;i<stick.numLEDs;i=i+1){
		   setPixelColor(stick,8-i, 0, 15, 0);
		   espShow(stick.pin,stick.pixels,stick.numBytes); // This sends the updated pixel color to the hardware.
		}
	return nb_red;
}

void crash_test (Adafruit_NeoPixel ledstick,int nb_red_LED, double max_dist, double min_dist)
{
	while(1)
	{
		double new_dist=getmeandistance(100);
	    printf("Distance is %f cm\n", new_dist);
	    printf("Number LED red %d \n", nb_red_LED);
		play_thank_you();
		nb_red_LED=adaptLEDlevel(ledstick, new_dist, max_dist, min_dist);
	}
}

double ultraDistance (int,int);

void random_init()
{
	time_t t;
	srand((unsigned) time(&t));
	printf("Random init done \n");
}

int random_picker()
{
	int i=rand()%5;
	//printf("random number is %d \n",i);
	return i;
}

void random_sound_player()
{
	int i=random_picker();
	switch (i){
	case 0 :
		play_sound_2(ta_da,ta_da_len,"ta da");//ok
		break;
	case 1 :
		play_sound_2(applause2,applause2_len,"applause2");
		break;
	case 2:
		play_sound_2(hallelujah,hallelujah_len,"hallelujah");//ok
		break;
	case 4:
		play_sound_2(thank_you,thank_you_len,"thank you");
		break;
	case 3 :
		play_sound_2(thank_you,thank_you_len,"thank you");
		break;
	default :
		play_sound_2(thank_you,thank_you_len,"thank you");
	}
}

double ultraDistance_filtered(int,int,int);
int countingzeros(double,double,double,double);

void app_main(void)
{
//	int delay =100;
//	double d1, d2, d3, d4;
//	while(1) {
//		d1=ultraDistance(PIN_TRIG1, PIN_ECHO1);
//		printf("Dist 1 %f \n", d1);
//		vTaskDelay(delay/portTICK_PERIOD_MS);
//	//	double d2=ultraDistance_filtered(PIN_TRIG2, PIN_ECHO2,max_dist);
//	//	vTaskDelay(delay/portTICK_PERIOD_MS);
//		d3=ultraDistance(PIN_TRIG3, PIN_ECHO3);
//		vTaskDelay(delay/portTICK_PERIOD_MS);
//		printf("Dist 3 %f \n", d3);
//		d4=ultraDistance(PIN_TRIG4, PIN_ECHO4);
//		vTaskDelay(delay/portTICK_PERIOD_MS);
//		printf("Dist 4 %f \n", d4);
//		d2=ultraDistance(PIN_TRIG3, PIN_ECHO3);
//		printf("Dist 3 v2 %f \n", d2);
//		vTaskDelay(delay/portTICK_PERIOD_MS);
//		int nbgoodmeasures= 4 - countingzeros(d1,d2,d3,d4);
//		printf("Nb good measures %d \n", nbgoodmeasures);
//		double mean_dist=(d1+d2+d3+d4)/nbgoodmeasures;
//		printf("Mean dist %f \n", mean_dist);
//	}
	//nvs_flash_init(); // Not sure if necessary.

	double max_dist = 75; //in cms, when bin empty (en réalité 90).
	double min_dist = 18; //in cms, when bin full
	double threshold = 8; // variation in cm, above which we consider that an object has been thrown
	random_init();// to pick the sound

	//Sound initializing
	audio_glob_init_2();
//	play_sound_2(thank_you,thank_you_len,"thank you");//ok
//	play_sound_2(ta_da,ta_da_len,"ta da");//ok
//	play_sound_2(hallelujah,hallelujah_len,"hallelujah");//ok
//	play_sound_2(combobreaker,combobreaker_len,"combobreaker");
//	play_sound_2(trex_roar,trex_roar_len,"trex roar");
//	play_sound_2(kids_cheering,kids_cheering_len,"kids cheering");
//	play_sound_2(monster_growl,monster_growl_len,"monster growl");
//	play_sound_2(cheering3,cheering3_len,"cheering3");
//	play_sound_2(applause2,applause2_len,"applause2");

	//Wifi Initializing

	//optimize plutot que change
	//Initializing LEDs
	Adafruit_NeoPixel ledstick = initLEDstick();
	setallPixelColor(ledstick,0,15,0);// all LEDs in green
	espShow(ledstick.pin,ledstick.pixels,ledstick.numBytes); // This sends the updated pixel color to the hardware.

	//Getting first measure;
	int delay_between_measures=120;
	double old_dist = getmeandistance(delay_between_measures);
	printf("Distance is %f cm\n", old_dist);
	int nb_red_LED=	adaptLEDlevel(ledstick, old_dist, max_dist, min_dist);
	int count=0;// The counter is used to readapt the LED level when it has not been adjusted for a long time

	//crash_test(ledstick, nb_red_LED, max_dist, min_dist);

	double new_dist;
	while(1)
	{
		count=count+1;
		new_dist=getmeandistance(delay_between_measures);
	    printf("New Distance is %f cm\n", new_dist);
	    printf("Number LED red %d \n", nb_red_LED);
		if (fabs(old_dist-new_dist)>threshold)
		{
			random_sound_player();
			printf("Playing Sound");
			old_dist=getmeandistance(delay_between_measures);
			nb_red_LED=adaptLEDlevel(ledstick, old_dist, max_dist, min_dist);
            printf("Stored Distance is %f cm\n", old_dist);
    	    printf("Number LED red %d \n", nb_red_LED);
			//we should also send the variation to the Wifi App
			count=0;
		}
		if (count==5)
		{
           printf("Compteur à 5\n");
           old_dist=new_dist;
		   nb_red_LED=adaptLEDlevel(ledstick, old_dist, max_dist, min_dist);
		   count=0;
		}
	}

}
