/*
 * main.c
 *
 * author: Mehmet Akif GÜMÜŞ
 *
 * description: I will work with watchdog timers. Setup either window or indepdentent watchdog timer and
 * observe its behavior in the simple blinky example from the repo. Calculate the appropriate reset time and implement
 * it.
 */

#include "stm32g0xx.h"

#define LEDDELAY    160000
#define delayled	16000000
int main(void);
void turn_off(void);
void init_wd(void);

void delay(volatile uint32_t);

void EXTI0_1_IRQHandler(void){
	if (EXTI->RPR1 & (1U << 1)){
		 while(1) {
		   	   delay(LEDDELAY);
		         /* Toggle LED */
		         GPIOC->ODR ^= (1U << 6);
		      }
		EXTI->RPR1 |= (1U << 1);
		}
}
 void NonMaskableInt_IRQHandler(void){
	turn_off();
}


int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);
    RCC->IOPENR |= (1U << 0);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Setup PA1 as input */
    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->PUPDR |= (2U << 2*1); // Pull-down mode

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

	 /*setup interrrupts for inputs*/
	    EXTI->EXTICR[0] |=(0U << 8*1);//PA1

	    /* MASK*/
	    EXTI->IMR1 |= (1U << 1);

	    /*rising edge*/
	    EXTI->RTSR1 |= (1U << 1);



	    /*NVIC*/
	    NVIC_SetPriority(EXTI0_1_IRQn, 0);
	    NVIC_EnableIRQ(EXTI0_1_IRQn);

	    init_wd();

    while(1) {

    }

    return 0;
}


void init_wd(void){
	RCC->CSR |= (3U << 0);
    IWDG->KR = 0xAAAA;

    // Wait until the IWDG is ready to accept the new parameters
    while (IWDG->SR != 0) { }

	IWDG->KR = 0x5555; // enable access to the e IWDG_PR, IWDG_RLR

	IWDG->PR = 6;

	//IWDG->SR = (1U << 2*0); // enable WVU and RVU
	IWDG->RLR = 0x0AA; //watchdog counter each time the value

	IWDG->WINR= 0x0AA; // access protected
	IWDG->KR = 0xAAAA;
	IWDG->KR = 0xCCCC; //Starts if wasn't running yet
    /*NVIC*/
    NVIC_SetPriority(NonMaskableInt_IRQn, 3);
    NVIC_EnableIRQ(NonMaskableInt_IRQn);
}



void turn_off(void){

    /* Turn off LED */
    GPIOC->ODR |= (0U << 6);
    delay(delayled);
    main();
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}


