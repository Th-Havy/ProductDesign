/*
 * i2s_dac.c
 *
 *  Created on: 28 nov. 2017
 *      Author: matthieu
 *      Modified version of i2s_dac_adc example.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "driver/i2s.h"
#include "driver/adc.h"
#include "trash_bin_header.h"


/*---------------------------------------------------------------
                            EXAMPLE CONFIG
---------------------------------------------------------------*/

//i2s sample rate
#define I2S_SAMPLE_RATE   (16000) // audio file sample rate in Hz.
//I2S play buffer length
#define I2S_PLAY_LEN      (4*1024) // in bytes, the size of the buffer can be changed


/**
 * @brief I2S ADC/DAC mode init.
 * I2S Bits per sample : Have to take 16 bits for the bits per sample, whereas the audio and the DAC can only
 * take 8 bits. If you choose 8 bits, the I2S communication will crash.
 * I2S channel number : only I2S0 possible here for built in DAC functions
 */
//have to take g.
void i2s_init()
{
	 i2s_config_t i2s_config = {
        .mode= I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
	    .sample_rate =  I2S_SAMPLE_RATE,
		.bits_per_sample=I2S_BITS_PER_SAMPLE_16BIT,
	    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
	    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,//I2S data format, a eclaircir
	    .intr_alloc_flags = 0,
	    .dma_buf_count = 2,
	    .dma_buf_len = 1024 //in bytes
	 };
	 //install and start i2s driver
	 i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
	 //init DAC pad
	 i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
}


/**
 * @brief debug buffer data
 */
void disp_buf(uint8_t* buf, int length)
{
    printf("======\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("======\n");
}


/**
 * @brief Set i2s clock for example audio file
 */
void set_file_play_mode()
{
	//Only Mono possible for the DAC (TBC, for the moment mono).
    i2s_set_clk(I2S_NUM_0, I2S_SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

/**
 * @brief Scale the audio data in 8 bits (source_buff, len) to 16bit for I2S DMA output (to_dac_buff)
 *        DAC can only output 8bit data value.
 *        I2S DMA will still send 16 bit data (to_dac_buff), the highest 8bit contains DAC data.
 *        returns 2*(length of to_dac_buff)
 */
int i2s_dac_data_scale(uint8_t* to_dac_buff, uint8_t* source_buff, uint32_t len)
{
	//scaling from 8 bits to 16 bits
    uint32_t j = 0;
    for (int i = 0; i < len; i++) {
        to_dac_buff[j++] = 0; //this 0 is an uint8, which means it is 8 bit long.
        to_dac_buff[j++] = source_buff[i];
    }
    return (len * 2);
}


//Call this function only when the header of the wav file has already been read.
void i2s_play_data_wave(FILE* wav_file)
{
   //Initialization I2S
    i2s_init();
    set_file_play_mode();

    int i2s_play_len = I2S_PLAY_LEN;
    uint8_t* i2s_read_buff = (uint8_t*) calloc(i2s_play_len, sizeof(char));//8bits
    uint8_t* to_dac_buff = (uint8_t*) calloc(2*i2s_play_len, sizeof(char));//16 bits
    printf("Playing sd Wav file: \n");
    size_t nread;
    while ((nread = fread(i2s_read_buff, sizeof(char), i2s_play_len, wav_file)) > 0) {
    	i2s_dac_data_scale(to_dac_buff, i2s_read_buff, nread);
    	//disp_buf(i2s_read_buff, nread);
    	printf("%d",nread);
    	printf("\n");
    	i2s_write_bytes(I2S_NUM_0, (const char*) to_dac_buff, 2*nread, portMAX_DELAY);
    }
    printf("J ai fini de lire le wav\n");
    free(i2s_read_buff);
    free(to_dac_buff);
    printf("j ai free les buffer.\n");
    return;
}


void read_header_file(FILE* wav_file)
{
    uint8_t* buff = (uint8_t*) calloc(12, sizeof(char));//8bits
    fread(buff, sizeof(char), 12, wav_file);
    disp_buf(buff, 12);
    free(buff);
    printf("j ai lu les 12 premiers octets du buffer\n");
}

void read_header_audio(FILE* wav_file)
{
    uint8_t* buff = (uint8_t*) calloc(32, sizeof(char));//8bits
    fread(buff, sizeof(char), 32, wav_file);
    disp_buf(buff, 32);
    free(buff);
    printf("j ai lu les 32 premiers octets du buffer\n");
    return;
}

void i2s_play_sound_wav(FILE* wav_file)
{
	read_header_file(wav_file);
	read_header_audio(wav_file);
	i2s_play_data_wave(wav_file);
    printf("je rends la main au main\n");
}


//
esp_err_t play_thank_you(void)
{
    esp_log_level_set("I2S", ESP_LOG_INFO);
   //Initialization
    i2s_init();
    int i2s_play_len = I2S_PLAY_LEN;
    uint8_t* i2s_write_buff = (uint8_t*) calloc(2*i2s_play_len, sizeof(char));
    set_file_play_mode();

        printf("Playing thank you: \n");
        int offset = 0;
        int tot_size = thank_you_len;
        while (offset < tot_size) {
            int play_len = ((tot_size - offset) > (I2S_PLAY_LEN)) ? (I2S_PLAY_LEN) : (tot_size - offset);
            disp_buf((uint8_t*)(thank_you + offset), 32);
            int i2s_wr_len = i2s_dac_data_scale(i2s_write_buff, (uint8_t*)(thank_you + offset), play_len);
            i2s_write_bytes(I2S_NUM_0, (const char*) i2s_write_buff, i2s_wr_len, portMAX_DELAY);
            offset += play_len;
            disp_buf((uint8_t*) i2s_write_buff, 32);
            printf("En cours ...\n");
        }
        //vTaskDelay(100 / portTICK_PERIOD_MS);
    printf("I have finished playing thank you !\n");
    free(i2s_write_buff);
    //vTaskDelete(NULL);
    return ESP_OK;
}

uint8_t* audio_glob_init(void)
{
    esp_log_level_set("I2S", ESP_LOG_INFO);
   //Initialization
    i2s_init();
    uint8_t* i2s_write_buff = (uint8_t*) calloc(I2S_PLAY_LEN, sizeof(char));
    set_file_play_mode();
	return i2s_write_buff;
}

void audio_glob_init_2(void)
{
    esp_log_level_set("I2S", ESP_LOG_INFO);
   //Initialization
    i2s_init();
    set_file_play_mode();
}


esp_err_t play_thank_you_opt(uint8_t* i2s_write_buff)
{
	printf("Playing thank you: \n");
	int offset = 0;
	int tot_size = thank_you_len;
	while (offset < tot_size) {
		int play_len = ((tot_size - offset) > (I2S_PLAY_LEN)) ? (I2S_PLAY_LEN) : (tot_size - offset);
		int i2s_wr_len = i2s_dac_data_scale(i2s_write_buff, (uint8_t*)(thank_you + offset), play_len);
		i2s_write_bytes(I2S_NUM_0, (const char*) i2s_write_buff, i2s_wr_len, portMAX_DELAY);
		offset += play_len;
		//disp_buf((uint8_t*) i2s_write_buff, 32);
		printf("En cours ...\n");
	}
	printf("I have finished playing thank you !\n");
	return ESP_OK;
}

esp_err_t play_thank_you_opt_2()
{
    uint8_t* i2s_write_buff = (uint8_t*) calloc(I2S_PLAY_LEN, sizeof(char));
	printf("Playing thank you: \n");
	int offset = 0;
	int tot_size = thank_you_len;
	while (offset < tot_size) {
		int play_len = ((tot_size - offset) > (I2S_PLAY_LEN)) ? (I2S_PLAY_LEN) : (tot_size - offset);
		int i2s_wr_len = i2s_dac_data_scale(i2s_write_buff, (uint8_t*)(thank_you + offset), play_len);
		i2s_write_bytes(I2S_NUM_0, (const char*) i2s_write_buff, i2s_wr_len, portMAX_DELAY);
		offset += play_len;
		//disp_buf((uint8_t*) i2s_write_buff, 32);
		printf("En cours ...\n");
	}
	free(i2s_write_buff);
	printf("I have finished playing thank you !\n");
	return ESP_OK;
}



esp_err_t play_sound(uint8_t* i2s_write_buff, const uint8_t* sound_name_h,
		const int sound_len, char* name)
{
	printf("Playing sound: %s\n", name);
	int offset = 0;
	int tot_size = sound_len;
	while (offset < tot_size) {
		int play_len = ((tot_size - offset) > (I2S_PLAY_LEN)) ? (I2S_PLAY_LEN) : (tot_size - offset);
		int i2s_wr_len = i2s_dac_data_scale(i2s_write_buff, (uint8_t*)(sound_name_h + offset), play_len);
		i2s_write_bytes(I2S_NUM_0, (const char*) i2s_write_buff, i2s_wr_len, portMAX_DELAY);
		offset += play_len;
		//disp_buf((uint8_t*) i2s_write_buff, 32);
	}
	printf("I have finished playing sound : %s\n", name);
	return ESP_OK;
}

esp_err_t play_sound_2(const uint8_t* sound_name_h,
		const uint32_t sound_len, char* name)
{
    uint8_t* i2s_write_buff = (uint8_t*) calloc(2*I2S_PLAY_LEN, sizeof(char));
	printf("Playing sound: %s\n", name);
	uint32_t offset = 0;
	uint32_t tot_size = sound_len;
	while (offset < tot_size) {
		uint32_t play_len = ((tot_size - offset) > (I2S_PLAY_LEN)) ? (I2S_PLAY_LEN) : (tot_size - offset);
		int i2s_wr_len = i2s_dac_data_scale(i2s_write_buff, (uint8_t*)(sound_name_h + offset), play_len);
		i2s_write_bytes(I2S_NUM_0, (const char*) i2s_write_buff, i2s_wr_len, portMAX_DELAY);
		offset += play_len;
	}
	printf("I have finished playing sound : %s\n", name);
	i2s_zero_dma_buffer(I2S_NUM_0);
	free(i2s_write_buff);
	return ESP_OK;

}
