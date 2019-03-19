#include <stdint.h>
#include "gpio_driver.h"

/**
	* @brief  Configures the mode of a pin : input, output, alt or analog mode 
	* @param  *GPIOx : GPIO Port Base address
	* @param  pin_no : GPIO pin number 
	* @param  mode   : mode to be configured
	* @retval None
	*/
void hal_gpio_configure_pin_mode_type(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode)
{
	if(pin_no < 8)
	{
		GPIOx->CRL &= ~(0x03 << ( 4 * pin_no));							// clear mode bits
		GPIOx->CRL |= (mode << ( 4 * pin_no));							// select mode
	}
	else if(pin_no > 7)
	{
		GPIOx->CRH &= ~(0x03 << ( 4 * (pin_no % 8)));				// clear mode bits
		GPIOx->CRH |= (mode << ( 4 * (pin_no % 8)));				// select mode
	}
	if(mode == 0)
	{
		if(pin_no < 8)
		{
			GPIOx->CRL &= ~(0x03 << ( 4 * pin_no + 2)); 			// clear cnf bits
			GPIOx->CRL |= (GPIO_PIN_INPUT_FLOATING << ( 4 * pin_no + 2)); 			// default
		}
		else if(pin_no > 7)
		{
			GPIOx->CRH &= ~(0x03 << ( 4 * (pin_no % 8) + 2)); // clear cnf bits
			GPIOx->CRH |= (GPIO_PIN_INPUT_FLOATING << ( 4 * (pin_no % 8) + 2)); // default
		}
	}
}

/**
	* Before configure output type, you must configure pin mode
	* @brief  Configures the output type of a pin  
	* @param  *GPIOx : GPIO Port Base address
	* @param  pin_no : GPIO pin number 
	* @param  op_type   : output type to be configured with 
	* @retval None
	*/
void hal_gpio_configure_pin_otype(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint32_t mode, uint32_t otype)
{
	if(mode != 0)
	{
		GPIOx->BSRR &= ~(0x1 << pin_no);										// clear bit
		GPIOx->BSRR |= (0x1 << pin_no);											// as an output

		if(pin_no < 8)
		{
			GPIOx->CRL &= ~(0x03 << ( 4 * pin_no + 2)); 			// clear cnf bits
			GPIOx->CRL |= (otype << ( 4 * pin_no + 2)); 			// select output type
		}
		else if(pin_no > 7)
		{
			GPIOx->CRH &= ~(0x03 << ( 4 * (pin_no % 8) + 2)); // clear cnf bits
			GPIOx->CRH |= (otype << ( 4 * (pin_no % 8) + 2)); // select output type
		}
	} else if(mode == 0)
	{
		GPIOx->BSRR &= ~(0x1 << (pin_no + 16));							// clear bit
		GPIOx->BSRR |= (0x1 << (pin_no + 16));							// as an input
		
		if(pin_no < 8)
		{
			GPIOx->CRH &= ~(0x03 << ( 4 * (pin_no % 8) + 2)); // clear cnf bits
			GPIOx->CRH |= (otype << ( 4 * (pin_no % 8) + 2)); // select output type
		}
		else if(pin_no > 7)
		{
			GPIOx->CRH &= ~(0x03 << ( 4 * (pin_no % 8) + 2)); // clear cnf bits
			GPIOx->CRH |= (otype << ( 4 * (pin_no % 8) + 2)); // select output type
		}
	}
}


/**
	* @brief  Initializes the gpio pin 
	* @param  *GPIOx : GPIO Port Base address
	* @param  *gpio_pin_conf :Pointer to the pin conf structure sent by application 
	* @retval None
	*/
void hal_gpio_init(GPIO_TypeDef *GPIOx, gpio_pin_conf_t *gpio_pin_conf)
{
	hal_gpio_configure_pin_mode_type(GPIOx,gpio_pin_conf->pin, gpio_pin_conf->mode);
	hal_gpio_configure_pin_otype(GPIOx,gpio_pin_conf->pin, gpio_pin_conf->mode, gpio_pin_conf->otype);
}


/**
	* @brief  Write a value to given pin number 
	* @param  *GPIOx : GPIO Port Base address
	* @param  pin_no : GPIO pin number 
	* @param  value   : value to be written 
	* @retval None
	*/
void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx,uint16_t pin_no, uint8_t val)
{
	if(val)
		GPIOx->ODR |=  (1 << pin_no);
	else
		GPIOx->ODR &=  ~(1 << pin_no);
	
}


/**
	* @brief  Read a value from a  given pin number 
	* @param  *GPIOx : GPIO Port Base address
	* @param  pin_no : GPIO pin number 
	* @retval uint8_t: Value read 
	*/
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx,uint16_t pin_no)
{
	
	uint8_t value ; 
	
	value = ((GPIOx->IDR >> pin_no ) & 0x00000001 );
	
	return value ;
}

/**
	* @brief  Configure the interrupt for a given pin number   
	* @param  pin_no : GPIO pin number 
	* @param  edge_sel   :  Triggering edge slection value of type "int_edge_sel_t"
	* @retval None
	*/
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel)
{
	
	if (edge_sel == INT_RISING_EDGE )
	{
		EXTI->RTSR |= 1 << pin_no;
	}	
	else if (edge_sel == INT_FALLING_EDGE)
	{
		EXTI->FTSR |= 1 << pin_no;
	}else if (edge_sel == INT_RISING_FALLING_EDGE )
	{
		EXTI->FTSR |= 1 << pin_no;
		EXTI->RTSR |= 1 << pin_no;
	}
	else
	{
		 ;//TODO
	}
}

/**
	* @brief  Enable the interrupt for a give pin number and irq number  
	* @param  pin_no : GPIO pin number 
	* @param  irq_no   :  irq_number to be enabled in NVIC 
	* @retval None
	*/
void hal_gpio_enable_interrupt(uint16_t pin_no, IRQn_Type irq_no)
{
	EXTI->IMR |= 1 << pin_no;
	NVIC_EnableIRQ(irq_no);
}

/**
	* @brief  Clear the sticky interrupt pending bit if set 
	* @param  pin_no : GPIO pin number 
	* @retval None
	*/
void 	hal_gpio_clear_interrupt(uint16_t pin)
{
	if(EXTI->PR & (1 << pin ))
	{
		EXTI->PR |= (1 << pin);
	}

}

void afio_interrupt_source_select(AFIO_TypeDef *pAFIO, int_port_sel_t port_sel, uint8_t pin_no)
{
	uint8_t i = pin_no / 4;
	if(port_sel == PORTA)
		pAFIO->EXTICR[i] |= 0x0;
	else if(port_sel == PORTB)
		pAFIO->EXTICR[i] |= 0x1;
	else if(port_sel == PORTC)
		pAFIO->EXTICR[i] |= 0x10;
	else if(port_sel == PORTD)
		pAFIO->EXTICR[i] |= 0x11;
	else if(port_sel == PORTE)
		pAFIO->EXTICR[i] |= 0x100;
	else
		;
}
