#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f10x.h"


/*******************************************************/
/*        Macros used for GPIO pin initialization      */
/*-----------------------------------------------------*/

/* GPIO Control Settings Values */
#define GPIO_PIN_INPUT_ANALOG_MODE				( (uint32_t) 0x00)
#define GPIO_PIN_INPUT_FLOATING						( (uint32_t) 0x01)
#define GPIO_PIN_INPUT_PUPD								( (uint32_t) 0x10)

#define GPIO_PIN_OUTPUT_PUSHPULL					( (uint32_t) 0x00)
#define GPIO_PIN_OUTPUT_OPENDRAIN					( (uint32_t) 0x01)
#define GPIO_PIN_OUTPUT_ALT_PUSHPULL			( (uint32_t) 0x10)
#define GPIO_PIN_OUTPUT_ALT_OPENDRAIN			( (uint32_t) 0x11)

#define GPIO_PIN_MODE_INPUT								( (uint32_t) 0x00)
#define GPIO_PIN_MODE_OUTPUT_10MHZ				( (uint32_t) 0x01)
#define GPIO_PIN_MODE_OUTPUT_2MHZ					( (uint32_t) 0x10)
#define GPIO_PIN_MODE_OUTPUT_50MHZ				( (uint32_t) 0x11)


/* GPIO Ports Clock Configuration                       */
#define _HAL_RCC_AFIO_CLK_ENABLE()        (RCC->APB2ENR |=  (1 << 0) )
#define _HAL_RCC_GPIOA_CLK_ENABLE()       (RCC->APB2ENR |=  (1 << 2) )
#define _HAL_RCC_GPIOB_CLK_ENABLE()       (RCC->APB2ENR |=  (1 << 3) )
#define _HAL_RCC_GPIOC_CLK_ENABLE()       (RCC->APB2ENR |=  (1 << 4) )
#define _HAL_RCC_GPIOD_CLK_ENABLE()       (RCC->APB2ENR |=  (1 << 5) )
#define _HAL_RCC_GPIOE_CLK_ENABLE()       (RCC->APB2ENR |=  (1 << 6) )

#define _HAL_RCC_GPIOA_CLK_DISABLE()       (RCC->APB2ENR &=  ~(1 << 2) )
#define _HAL_RCC_GPIOB_CLK_DISABLE()       (RCC->APB2ENR &=  ~(1 << 3) )
#define _HAL_RCC_GPIOC_CLK_DISABLE()       (RCC->APB2ENR &=  ~(1 << 4) )
#define _HAL_RCC_GPIOD_CLK_DISABLE()       (RCC->APB2ENR &=  ~(1 << 5) )
#define _HAL_RCC_GPIOE_CLK_DISABLE()       (RCC->APB2ENR &=  ~(1 << 6) )

/*********************************************************/
/*     Data Structure for GPIO pin initialization        */
/*-------------------------------------------------------*/

/**
* @brief GPIO pin configuration structure
*				 This structure will be filled and passed to driver 
*				 by the applications to initialize the gpio pin
*/

typedef struct
{
	uint32_t pin;	
	uint32_t mode;	
	uint32_t otype;	
	uint32_t alternate;
}gpio_pin_conf_t;

typedef enum
{
	INT_RISING_EDGE,
	INT_FALLING_EDGE,
	INT_RISING_FALLING_EDGE
}int_edge_sel_t;

/***********************************************************/
/*               Driver exposed APIs                       */
/*---------------------------------------------------------*/

/**
			* @brief Initializes the gpio pin
			* @param *GPIOx : GPIO Base address
			* @param *gpio_pin_conf : Pointer to the pin conf structure sent by application
			* @retval None
			*/
void hal_gpio_init(GPIO_TypeDef *GPIOx, gpio_pin_conf_t *gpio_pin_conf);

/**
			* @brief Read a value from a given pin number
			* @param *GPIOx : GPIO Base address
			* @param pin_no : GPIO pin number
			* @retval uint8_t : Value read
			*/
uint8_t hal_gpio_read_from_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no);

/**
			* @brief Write a value to given pin number
			* @param *GPIOx : GPIO Base address
			* @param pin_no : GPIO pin number
			* @param value : Value to be written
			* @retval None
			*/
void hal_gpio_write_to_pin(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint8_t val);

/**
			* @brief Set the alternate functionality for a given pin
			* @param *GPIOx : GPIO Base address
			* @param pin_no : GPIO pin number
			* @param alt_fun_value : Alternate function to be configured with
			* @retval None
			*/
void hal_gpio_set_alt_function(GPIO_TypeDef *GPIOx, uint16_t pin_no, uint16_t alt_fun_value);

/*********************************************************/
/*          Data Structure for GPIO interrupt            */
/*-------------------------------------------------------*/
typedef enum
{
	PORTA,
	PORTB,
	PORTC,
	PORTD,
	PORTE
}int_port_sel_t;

/*********************************************************/
/*                GPIO interrupt APIs                    */
/*-------------------------------------------------------*/

/**
			* @brief  Select the source input for EXTIx external interrupt
			* @param  port_sel: GPIO port name
			* @param  pin_no	: GPIO pin number
			* @retval None
			*/
void afio_interrupt_source_select(AFIO_TypeDef *pAFIO, int_port_sel_t port_sel, uint8_t pin_no);

/**
			* @brief  Configure the interrupt for a given pin number   
			* @param  pin_no   : GPIO pin number 
			* @param  edge_sel : Triggering edge selection value of type "int_edge_sel_t"
			* @retval None
			*/
void hal_gpio_configure_interrupt(uint16_t pin_no, int_edge_sel_t edge_sel);

/**
			* @brief  Enable the interrupt for a give pin number and irq number  
			* @param  pin_no : GPIO pin number 
			* @param  irq_no : irq_number to be enabled in NVIC 
			* @retval None
			*/
void hal_gpio_enable_interrupt(uint16_t pin_no, IRQn_Type irq_no);

/**
			* @brief  Clear the sticky interrupt pending bit if set 
			* @param  pin_no : GPIO pin number 
			* @retval None
			*/
void 	hal_gpio_clear_interrupt(uint16_t pin);

#endif
