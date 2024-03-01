/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mateusz Salamon
 * @brief          : STM32 na Rejestrach
 ******************************************************************************
 ******************************************************************************

 	 	 	 	 https://msalamon.pl
 	 	 	 	 https://sklep.msalamon.pl
 	 	 	 	 https://kursstm32.pl
 	 	 	 	 https://stm32narejestrach.pl

 */
#include "main.h"

// LED LD4 macros
#define LD4_ON GPIOA->BSRR = GPIO_BSRR_BS5
#define LD4_OFF GPIOA->BSRR = GPIO_BSRR_BR5;

#define PC13 (1<<13)

// Tick for System Time
__IO uint32_t Tick;

// LED LD4 configuration
void LD4_Configure(void);

// Configure B1 button as input
void ConfigureButton(void);
void ConfigureButtonEXTI(void);


// Get current System Time
uint32_t GetSystemTick(void);


int main(void)
{
// 1s = 12 000 000
	// 0,001 = X
	SysTick_Config(12000000 / 1000);

// Configure LD4
	LD4_Configure();
	ConfigureButton();
	ConfigureButtonEXTI();

    /* Loop forever */
	while(1)
	{

	}
}


void LD4_Configure(void)
{
	// Enable Clock for PORTD
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Configure GPIO Mode - Output
	GPIOA->MODER |= GPIO_MODER_MODE5_0; // It's default reset state. Not necessary.
	GPIOA->MODER &= ~(GPIO_MODER_MODE5_1);

	// Configure Output Mode - Push-pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5); // It's default reset state. Not necessary.

	// Configure GPIO Speed - Low
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5); // Two bits together. It's default reset state. Not necessary.

	// Configure Pull-up/Pull-down - no PU/PD
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5); // It's default reset state. Not necessary.
}

// PC13 - Button
// 0 - Pushed
// 1 - Released / Idle
void ConfigureButton(void)
{
	// Enable Clock for PORTC
	RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

	// Configure GPIO Mode - Input
	GPIOC->MODER &= ~(GPIO_MODER_MODE13);

	// Configure Pull-up/Pull-down - no PU/PD
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD13);
}

void ConfigureButtonEXTI(void)
{
	// EXTI13 because B1 is on PC13

	// Enable SYSCFG RCC clock, there are "clonned" interrupt statuses for EXTI
	// We can use EXTI on C0 without it
	//RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN;

	// Configure EXTI13 line
	EXTI->EXTICR[3] |= EXTI_EXTICR4_EXTI13_1;

	// Disable EXTI IMR mask
	EXTI->IMR1 |= EXTI_IMR1_IM13;

	// Enable EXTI on falling edge
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;

	// Set NVIC Priority for EXTI4_15
	NVIC_SetPriority(EXTI4_15_IRQn, 0);

	// Enable Interrupt
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

// Interrupt handler function from VTOR - startup file
void EXTI4_5_IRQHandler(void) // ERROR IN CMSIS - should be EXTI4_15_IRQHandler. It's on on ST's GitHub
{
	if(EXTI->FPR1 & EXTI_FPR1_FPIF13)
	{
		EXTI->FPR1 |= EXTI_FPR1_FPIF13;

		// Interrupt Action
	}

}

void SysTick_Handler(void)
{
	Tick++; // Increase system timer
}

uint32_t GetSystemTick(void)
{
	return Tick; // Return current System Time
}

