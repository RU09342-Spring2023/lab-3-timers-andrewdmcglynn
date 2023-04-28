/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: FILL THIS IN
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */
#include <msp430.h>
int buttonPress = 0; //store button press
void gpioInit();
void timerInit();

void main(){
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    gpioInit();
    timerInit();

    PM5CTL0 &= ~LOCKLPM5;  // low power mode
    __bis_SR_register(LPM3_bits | GIE);
}

void gpioInit() {
    // initialize pins
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P2DIR &= ~BIT3;
    P2REN |= BIT3;
    P2OUT |= BIT3;
    P2IES |= BIT3;
    P2IFG &= ~BIT3;
    P2IE |= BIT3;
    P4DIR &= ~BIT1;
    P4REN |= BIT1;
    P4OUT |= BIT1;
    P4IES |= BIT1;
    P4IFG &= ~BIT1;
    P4IE |= BIT1;
}

void timerInit() {
    //set up timer
    TB1CCR0 = 800;
    TB1CTL |= TBSSEL__ACLK + MC_0 + TBCLR;
    TB1CCTL0 |= CAP + CM_3;

    TB0CTL |= TBCLR;
    TB0CTL |= TBSSEL__ACLK;
    TB0CTL |= MC__UP;
    TB0CCR0 = 8192;

    TB0CCTL0 |= CCIE;
    TB0CCTL0 &= ~CCIFG;
}
#pragma vector=PORT2_VECTOR
__interrupt void ISR_Port2_S2(void){
    if(P2IES & BIT3){
        TB1CTL |= MC_2;
        P2IES&= ~BIT3;
    }
    else if (P2IES != BIT3) {
        TB1CTL &= ~(MC0 + MC1);

        buttonPress = TB1R;
        TB0CCR0 = buttonPress;

        TB1CTL |= TBCLR;
        P2IES |= BIT3;
    }
    P2IFG &= ~BIT3;
}

#pragma vector=PORT4_VECTOR
__interrupt void ISR_Port4_S1(void)
{

    TB1CTL &= ~(MC0 + MC1);
    TB1CTL |= TBCLR;
    TB0CCR0 = 8192;
    P4IFG &= ~BIT1;
}

