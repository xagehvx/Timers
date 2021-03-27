/*
 * main.c
 *
 * author: Mehmet Akif GÜMÜŞ
 *
 * description: I work with general purpose timers. Set up a timer with lowest priority that will be used to
 * toggle on-board LED at 1 second intervals. Change the blinking speed using an external button. Each button press
 * should increase the blinking speed by 1 second up to a maximum of 10 seconds. Next button press after 10 should
 * revert it back to 1 second.
 *
 */

#include "stm32g0xx.h"

volatile int k=1;		//k yı başlangıç için volatile 1 tanımladık.

void EXTI0_1_IRQHandler(void)
{

	if (EXTI->RPR1 & (1U << 1))			//pinimiz exti1 üzerinde bazı hatalar yol açmamak için pending ii kontrol ediyoruz
	{
		k++;
		EXTI->RPR1 |= (1U << 1);
		if(k>10)		//k 10a ulaşınca 10dan sonra tekrar k ya 1 atadık.
		{
			k=1;
		}
	}
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	TIM1->SR &= ~(1U<<0); //clear update status register

	static int t = 1;
	if(t==k)				//buton interruptı ile harekete geçen k sayısını kullanarak kurulan algoritma ile ledin yanma sıklığını ayarlama
	{
		  /* blinky LED */
		GPIOC->ODR ^= (1U << 6);
		t = 1;
	}
	else if (t<k)
	{
		t++;
	}
	else t=1;
}

void init_timer1(){

	RCC->APBENR2 |= (1U<< 11);// enable time1 module clock

	TIM1->CR1=0;// zero out the control register just in case
	TIM1->CR1 |= (1<<7);	// ARPE
	TIM1->CNT=0;// zero out counter

	/*1 second interrupt	 */

	TIM1->PSC=999;
	TIM1->ARR=16000;

	TIM1->DIER |= (1 << 0);// update interrupt enable
	TIM1->CR1 |= (1 << 0);// 	tım1 enable

	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,3); // TIM1_BRK_UP_TRG_COM_IRQn fonksiyonuna butondan daha düşük olması için priority si 3 atandı
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //timer interrupt ı nvic içerisinde enable edildi

}

int main(void) {

    /* Enable GPIOC and GPIOA clock */
    RCC->IOPENR |= (1U << 2);
    RCC->IOPENR |= (1U << 0);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Setup PA1 as input */
    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->PUPDR |= (2U << 2*1); // Pull-down mode

    /*setup interrrupts for inputs*/
    EXTI->EXTICR[0] |=(0U << 8*1);//PA1	 EXTI1 mux ta PA1 için EXTICR0'ın 9.biti 0 yapıldı

    /* MASK*/
    EXTI->IMR1 |= (1U << 1);

    /*rising edge*/
    EXTI->RTSR1 |= (1U << 1);

    /*NVIC*/
    NVIC_SetPriority(EXTI0_1_IRQn,0);	// buton interruptı PA1 için EXTI1 in içerisinde olduğundan EXTI0_1_IRQn kullanıldı.
    NVIC_EnableIRQ(EXTI0_1_IRQn);	//buton interrupt ı nvic içerisinde enable edildi

    init_timer1();

    while(1) {

    }

    return 0;
}
