#include "nrf.h"
#include "nrf_gpio.h"
#include "led.h"

void LED_Init(void)
{
  // Configure LED-pins as outputs
	int i;
	for(i=17;i<=20;i++)
	{
  nrf_gpio_cfg_output(i);
	}
}

void LED_Open(int LED_X)
{
	nrf_gpio_pin_set(LED_X);
}

void LED_Close(int LED_X)
{
	   nrf_gpio_pin_clear(LED_X);
	
}
void LED_Toggle(int LED_X)
{
  nrf_gpio_pin_toggle(LED_X);
}

