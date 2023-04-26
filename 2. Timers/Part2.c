/*
 * Part2.c
 *
 *  Created on: Feb 11, 2023
 *      Author: Andrew McGlynn
 *
 *      This code will need to change the speed of an LED between 3 different speeds by pressing a button.
 */

#include <msp430.h>

void gpioInit();
void timerInit();
int counter = 3000;
void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM3_bits | GIE);

}


void gpioInit(){
    // @TODO Initialize the Red or Green LED
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    // @TODO Initialize Button 2.3
    P2OUT |= BIT3;
    P2REN |= BIT3;
    P2IES &=~BIT3;
    P2IE |= BIT3;

}

void timerInit(){
    // @TODO Initialize Timer B1 in Continuous Mode using ACLK as the source CLK with Interrupts turned on
    TB1CCTLO= CCIE;
    TB1CCR0 = 50000;
    TB1CTL = TBSSEL_1 |MC_2;
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    // @TODO Remember that when you service the GPIO Interrupt, you need to set the interrupt flag to 0.
    P2IFG &= ~BIT3;
    if(counter == 3000){
        counter = 20000;
    }
    else if(counter == 20000){
        counter = 50000;
    }
    else{
        counter = 30000;
    }
    // @TODO When the button is pressed, you can change what the CCR0 Register is for the Timer. You will need to track what speed you should be flashing at.
    P1OUT ^= BIT0;
    TB1CCR0 += counter;
}


// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void)
{
    // @TODO You can toggle the LED Pin in this routine and if adjust your count in CCR0.
}


