/*
 * tpa2016_amp.c
 *
 *  Created on: 5 déc. 2017
 *      Author: matthieu
 *      Using i2c example https://github.com/espressif/esp-idf/blob/master/examples/peripherals/i2c/main/i2c_example_main.c
 *      and the Adafruit TPA2016 library.
 *
 */

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "trash_bin_header.h" // for pixel stick


/*
 * This is to set the gain of the amplifier using I2C communication.
 * The ESP32 is the Master.
 *
 */


#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_0      /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000           /*!< I2C master clock frequency */

#define ACK_EN                             1    // acknowledgement enabled in I2C with TOA 2016.

#define TPA2016_SETUP 0x1
#define TPA2016_SETUP_R_EN 0x80
#define TPA2016_SETUP_L_EN 0x40
#define TPA2016_SETUP_SWS 0x20
#define TPA2016_SETUP_R_FAULT 0x10
#define TPA2016_SETUP_L_FAULT 0x08
#define TPA2016_SETUP_THERMAL 0x04
#define TPA2016_SETUP_NOISEGATE 0x01

#define TPA2016_ATK 0x2
#define TPA2016_REL 0x3
#define TPA2016_HOLD 0x4
#define TPA2016_GAIN 0x5
#define TPA2016_AGCLIMIT 0x6
#define TPA2016_AGC 0x7
#define TPA2016_AGC_OFF 0x00
#define TPA2016_AGC_2 0x01
#define TPA2016_AGC_4 0x02
#define TPA2016_AGC_8 0x03

#define TPA2016_I2CADDR 0x58
//#define TPA2016_I2CADDR 0xB0


#ifdef MAPPING_PCB
void i2c_example_master_init()
{
    esp_log_level_set("I2C init", ESP_LOG_INFO);
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    esp_err_t ret = i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
    if (ret !=ESP_OK)
    	printf("Problem with init \n");
    else printf("I2C Init OK \n");
}
#endif



esp_err_t setGain(int g) {
    esp_log_level_set("Set Gain", ESP_LOG_INFO);
	  if (g > 30) g = 30;
	  if (g < -28) g = -28;
	esp_err_t ret;
    //i2c_example_master_init();
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TPA2016_I2CADDR << 1 | I2C_MASTER_WRITE, ACK_EN);
    //i2c_master_write_byte(cmd, TPA2016_I2CADDR | I2C_MASTER_WRITE, ACK_EN);
    i2c_master_write_byte(cmd, TPA2016_GAIN, ACK_EN);
    uint8_t g_temp=g;
    printf("Valeur gain %d \n",g_temp);
    i2c_master_write_byte(cmd, g_temp, ACK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
	if (ret !=ESP_OK){
		printf("Problem with setGain \n");
		printf("Valeur 0x%03x\n", ret);}
    return ret;
}

esp_err_t turnoffAGCCompression() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TPA2016_I2CADDR << 1 | I2C_MASTER_WRITE, ACK_EN);
    //i2c_master_write_byte(cmd, TPA2016_I2CADDR | I2C_MASTER_WRITE, ACK_EN);
    i2c_master_write_byte(cmd, TPA2016_AGC, ACK_EN);
    i2c_master_write_byte(cmd, TPA2016_AGC_OFF, ACK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
	if (ret !=ESP_OK){
		printf("Problem with turnoffAGC \n");
		printf("Valeur de ret %d\n", ret);}
    return ret;
}
esp_err_t turnoffReleaseControl() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, TPA2016_I2CADDR << 1 | I2C_MASTER_WRITE, ACK_EN);
    //i2c_master_write_byte(cmd, TPA2016_I2CADDR | I2C_MASTER_WRITE, ACK_EN);
    i2c_master_write_byte(cmd, TPA2016_REL, ACK_EN);
    i2c_master_write_byte(cmd, 0x00, ACK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
	if (ret !=ESP_OK){
		printf("Problem with turnoffReleaseControl \n");
		printf("%d\n", ret);}
    return ret;

}

//esp_err_t i2c_master_write_byte(i2c_cmd_handle_t cmd_handle, uint8_t data, boolean ack_en);
