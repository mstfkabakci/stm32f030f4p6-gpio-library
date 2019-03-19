#include <stdint.h>
#include "gpio_driver.h"
#include "main.h"

/**
	* @brief  Initialize the LED 
	* @param  None
	* @retval None
	*/
void led_init(void)
{
	
	gpio_pin_conf_t led_pin_conf;
	/* enable the clock for the GPIOC port */
	_HAL_RCC_GPIOC_CLK_ENABLE();

	led_pin_conf.pin = LED;
	led_pin_conf.mode = GPIO_PIN_MODE_OUTPUT_10MHZ;
	led_pin_conf.otype = GPIO_PIN_OUTPUT_PUSHPULL;

	hal_gpio_init(GPIOC, &led_pin_conf);
}

/**
	* @brief  Turns ON the led which is connected on the given pin  
	* @param  *GPIOx : Base address of the GPIO Port
	* @param  Pin : pin number of the LED
	* @retval None
	*/
void led_turn_on(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	hal_gpio_write_to_pin(GPIOx,pin, 1);
	
}

/**
	* @brief  Turns OFF the led which is connected on the given pin  
	* @param  *GPIOx : Base address of the GPIO Port
	* @param  Pin : pin number of the LED
	* @retval None
	*/
void led_turn_off(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	hal_gpio_write_to_pin(GPIOx,pin, 0);
	
}

/**
	* @brief  Toggels the led which is connected on the given pin  
	* @param  *GPIOx : Base address of the GPIO Port
	* @param  Pin : pin number of the LED
	* @retval None
	*/
void led_toggle(GPIO_TypeDef *GPIOx, uint16_t pin)
{
	if(hal_gpio_read_from_pin(GPIOx,pin))
	{
		 hal_gpio_write_to_pin(GPIOx,pin, 0);
	}else
	{
		 hal_gpio_write_to_pin(GPIOx,pin, 1);
		
	}
	
#if 0
	//Logic 2
	hal_gpio_write_to_pin(GPIOx,pin, ~(hal_gpio_read_from_pin(GPIOx,pin)));
#endif 
}

int main(void)
{

	/* Initializes the LEDs */
	led_init();
	
	//set the mode as input
	GPIOB->CRL &= ~(0x11 << 0);
	
	// enable rcc for exticr
	_HAL_RCC_AFIO_CLK_ENABLE();
	
	// enable interrupt port/pin in exticr
	afio_interrupt_source_select(AFIO, PORTB, BUTTON);
	
	/*Configure the button interrupt as falling edge */
	hal_gpio_configure_interrupt(BUTTON, INT_RISING_FALLING_EDGE);
	
	/*Enable the interrupt on EXTI0 line */
	hal_gpio_enable_interrupt(BUTTON, EXTI0_IRQn);
	
	while(1);
}



/**
	* @brief  ISR for the configured EXTI0 interrupt  
	* @retval None
	*/
void EXTI0_IRQHandler(void)
{
  hal_gpio_clear_interrupt(BUTTON);
	/* Do youR TASK here */
	led_toggle(GPIOC,LED);
	
}
