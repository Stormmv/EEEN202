// EEEN202 Staircase ADC
// Nick Thompson, Robin Dykstra 2018

#include "lcd.h"
#include <stdio.h>

//Macros
#define Sfr(x, y)       sfr x = y
#define Sbit(x, y, z)   sbit x = y^z

// Pin and Port definitions 
Sfr(PORT_DAC,0x80);  //DAC output port sfr(NAME,ADDR) //Port 0
Sbit(PIN_CMP,0xB0,7); //Comparator input pin sbit(NAME,ADDR,BIT_NUM) // Port 3 pin 7

// Successive Approximation Converter code
unsigned char sarConverter(){
    unsigned char result = 0;
    unsigned char test_val;
    for (test_val = 0x80; test_val != 0; test_val >>= 1) {
        result |= test_val;      // Set the test bit
        PORT_DAC = result;       // Output test value to DAC
        delay(100);              // Wait for the comparator to stabilize
        if (!PIN_CMP) {          // Check if the comparator is low
            result &= ~test_val; // Clear the test bit if comparator is low
        }
    }
    return result;
}

void main(){
    int adc_value;
    char output_text[16];
    initLCD();
    while(1){
        adc_value = sarConverter();
        sprintf(output_text,"V: %d", adc_value);
        writeLineLCD(output_text);
        delay(10000);
        clearLCD();
    }
}