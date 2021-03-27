/*
 * main.c
 *
 * author: Mehmet Akif GÜMÜŞ
 *
 * 			description:I work on creating an accurate delay function using SysTick exception. I create a SysTick
 * 			exception with 1 millisecond interrupt intervals. Then create a delay_ms(..) function that will accurately wait for
 * 			(blocking) given number of milliseconds.
 *
 */

#include "stm32g0xx.h"

void delay_ms(volatile unsigned int);


int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);
    int Start = SysTick->VAL;		//Systic VAL ile o systick in o anki değeri alınır
    	delay_ms(100000);
    int Stop = SysTick->VAL;		//Systic VAL ile o systick in o anki değeri alınır
    unsigned int Delta = 0x00FFFFFF&(Start-Stop);	// Start eksi stop ile arada geçen zamanı bulup deltaya atadım

    SystemCoreClockUpdate();	//contains the system frequency

    while(1) {
        delay_ms(100000);		//delay fonksiyonu ile while döngüsü içerisinde led in toggle etmesi sağlandı
        /* LED Toggle */
        GPIOC->ODR ^= (1U << 6);
    }
    return 0;
}

void delay_ms(volatile unsigned int s){

	for(int i=s; i>0; i--){
	 SysTick_Config(SystemCoreClock / 1000); // 16 MHz / 1000 ile 1 ms elde edildi.
	 }
}

