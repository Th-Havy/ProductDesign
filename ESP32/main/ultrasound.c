/*
 * ultrasound.c
 *
 *  Created on: 2 déc. 2017
 *      Author: matthieu
 */

//
// Lots of this based on ncolbans work
// https://github.com/nkolban/esp32-snippets
/*
MIT License

Copyright (c) 2017 Olof Astrand (Ebiroll)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <sys/time.h>
#include "trash_bin_header.h"

//static const char *TAG = "ultrasound";

// Similar to uint32_t system_get_time(void) --> essayer avec cette fonction.
uint32_t get_usec() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return (tv.tv_sec*1000000 + tv.tv_usec);
}


bool valid_trigpin(int trig_pin)
{
	return GPIO_IS_VALID_OUTPUT_GPIO(trig_pin);
}

bool valid_echopin(int echo_pin)
{
	return GPIO_IS_VALID_GPIO(echo_pin);
}

//
// Toggle trig pin and wait for input on echo pin
//
double measure_distance(int trig_pin, int echo_pin) {

	double measured_distance=0;

	gpio_pad_select_gpio(trig_pin);
	gpio_pad_select_gpio(echo_pin);

	gpio_set_direction(trig_pin, GPIO_MODE_OUTPUT);
	gpio_set_direction(echo_pin, GPIO_MODE_INPUT);

	// HC-SR04
	gpio_set_level(trig_pin, 1);
	vTaskDelay(2 / portTICK_PERIOD_MS);
	gpio_set_level(trig_pin, 0);
	uint32_t startTime=get_usec();

	while (gpio_get_level(echo_pin)==0 && get_usec()-startTime < 500*1000)
	{
		// Wait until echo goes high
	}

	startTime=get_usec();

	while (gpio_get_level(echo_pin)==1 && get_usec()-startTime < 500*1000)
	{
		// Wait until echo goes low again
	}

	if (gpio_get_level(echo_pin) == 0)
	{
		uint32_t diff = get_usec() - startTime; // Diff time in uSecs
		// Distance is TimeEchoInSeconds * SpeeOfSound / 2
		measured_distance = 340.29 * diff / (1000 * 1000 * 2) * 100; // In centimeters
		//printf("Distance is %f cm\n", measured_distance);
	}
	else
	{
		// No value
		printf("Did not receive a response!\n");
		return 0;
	}
	return measured_distance;
}

double ultraDistance(int trig_pin, int echo_pin)
{
	if (valid_trigpin(trig_pin) && valid_echopin(echo_pin))
	{
		double dist;
		dist = measure_distance(trig_pin,echo_pin);
		return dist;
	}
	else {
		printf("Invalid pin number");
		return 0;
	}
}

//double measure_distance_opt(int trig_pin, int echo_pin) {
//
//	double measured_distance=0;
//	// HC-SR04	gpio_set_level(trig_pin, 1);
//	vTaskDelay(1 / portTICK_PERIOD_MS); // at least 10 µs
//	gpio_set_level(trig_pin, 0);
//	uint32_t startTime=get_usec();
//	while (gpio_get_level(echo_pin)==0 && get_usec()-startTime < 500*1000)
//	{
//		// Wait until echo goes high
//	}
//	startTime=get_usec();
//	while (gpio_get_level(echo_pin)==1 && get_usec()-startTime < 500*1000)
//	{
//		// Wait until echo goes low again
//	}
//	if (gpio_get_level(echo_pin) == 0)
//	{
//		uint32_t diff = get_usec() - startTime; // Diff time in uSecs
//		// Distance is TimeEchoInSeconds * SpeeOfSound / 2
//		measured_distance = 340.29 * diff / (1000 * 1000 * 2) * 100; // In centimeters
//		//printf("Distance is %f cm\n", measured_distance);
//		return measured_distance;
//	}
//	else
//	{
//		// No value
//		printf("Did not receive a response!\n");
//		return 0;
//	}
//}
//
//void init_gpio_pins(int trig_pin, int echo_pin) {
//	if (valid_trigpin(trig_pin) && valid_echopin(echo_pin))
//	{
//	gpio_pad_select_gpio(trig_pin);
//	gpio_pad_select_gpio(echo_pin);
//	gpio_set_direction(trig_pin, GPIO_MODE_OUTPUT);
//	gpio_set_direction(echo_pin, GPIO_MODE_INPUT);
//	}
//	else
//	{
//		printf("Invalid GPIO Pin numbers");
//	}
//}
//
//void init_gpio_pad(){
//	init_gpio_pins(PIN_TRIG1, PIN_ECHO1);
//	init_gpio_pins(PIN_TRIG2, PIN_ECHO2);
//	init_gpio_pins(PIN_TRIG3, PIN_ECHO3);
//	init_gpio_pins(PIN_TRIG4, PIN_ECHO4);
//}
//

//returns 0 if distance > max
double ultraDistance_filtered(int trig_pin, int echo_pin, double max_dist)
{
	double d=ultraDistance(trig_pin, echo_pin);
	if (d>(max_dist+20))
			d=0;
	return d;
}

int countingzeros(double d1, double d2, double d3, double d4){
	int n=0;
	if (d1==0)
		n=n+1;
	if (d2==0)
		n=n+1;
	if (d3==0)
		n=n+1;
	if (d4==0)
		n=n+1;
	return n;
}

// we could add a filtering systems to these values
// if value superior to max, we discard it.
double getmeandistance(int delay)
{
	//int nbmeasures_persensor=10; //nb of measures per sensor.

	//double distance[4];
	double d1=ultraDistance(PIN_TRIG1, PIN_ECHO1);
	printf("Distance 1 is %f cm \n",d1);
	vTaskDelay(delay/portTICK_PERIOD_MS);
//	double d2=ultraDistance_filtered(PIN_TRIG2, PIN_ECHO2,max_dist);
//	vTaskDelay(delay/portTICK_PERIOD_MS);
	double d3=ultraDistance(PIN_TRIG3, PIN_ECHO3);
	printf("Distance 3 is %f cm \n",d3);
	vTaskDelay(delay/portTICK_PERIOD_MS);
	double d4=ultraDistance(PIN_TRIG4, PIN_ECHO4);
	printf("Distance 4 is %f cm \n",d4);
	vTaskDelay(delay/portTICK_PERIOD_MS);
	double d2=ultraDistance(PIN_TRIG3, PIN_ECHO3);
	printf("Distance 3 V2 is %f cm \n",d2);
	vTaskDelay(delay/portTICK_PERIOD_MS);
	int nbgoodmeasures= 4 - countingzeros(d1,d2,d3,d4);
	if (nbgoodmeasures==0)
		return 0;
	double mean_dist=(d1+d2+d3+d4)/nbgoodmeasures;
	return mean_dist;
}


