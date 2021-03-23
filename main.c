/*
 * Project4.c
 *
 * Created: 3/4/2021 2:33:52 PM
 * 
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"

// s is a number between 0 - 1023
void displayBasic(int sample, int maximum, int minimum, long sum, int num_samples){
	lcd_clr();
	
	int instantf, maxf, minf, avgf;
	instantf = sample;
	maxf = maximum;
	char buf[17];
	sprintf(buf, "%d  %d", instantf, maxf); // occupy 4 spaces with decimal spaces ex: 2.71
	lcd_pos(0,0);
	lcd_puts(buf);
	
	minf = minimum;
	avgf = sum/num_samples;
	
	char buf2[17];
	sprintf(buf2, "%d  %d", minf, avgf);
	lcd_pos(1,0);
	lcd_puts(buf2);
	
	
}

void displayInit(){
	lcd_clr();
	
	char buf[17];
	sprintf(buf, "IN:---- MAX:----"); // occupy 4 spaces with decimal spaces ex: 2.71
	lcd_pos(0,0);
	lcd_puts(buf);
	
	
	char buf2[17];
	sprintf(buf2, "MN:---- AVG:----");
	lcd_pos(1,0);
	lcd_puts(buf2);
	
	
}

void displayRestart(int sample){
	lcd_clr();
	
	float instantf;
	instantf = (sample/1023.0) * 5.0;
	
	char buf[17];
	sprintf(buf, "IN:%3.2f MAX:----", instantf); // occupy 4 spaces with decimal spaces ex: 2.71
	lcd_pos(0,0);
	lcd_puts(buf);
	
	
	char buf2[17];
	sprintf(buf2, "MN:---- AVG:----");
	lcd_pos(1,0);
	lcd_puts(buf2);
	
	
}


// s is a number between 0 - 1023
void displayConversion(int sample, int maximum, int minimum, long sum, int num_samples){
	lcd_clr();
	
	float instantf, maxf, minf, avgf;
	instantf = (sample/1023.0) * 5.0;
	maxf = (maximum/1023.0) * 5.0;
	char buf[17];
	sprintf(buf, "IN:%3.2f MAX:%3.2f", instantf, maxf); // occupy 4 spaces with decimal spaces ex: 2.71
	lcd_pos(0,0);
	lcd_puts(buf);
	
	minf = (minimum/1023.0) * 5.0;
	avgf = (sum/(float)num_samples)/1023.0 *5.0;
	
	char buf2[17];
	sprintf(buf2, "MN:%3.2f AVG:%3.2f", minf, avgf);
	lcd_pos(1,0);
	lcd_puts(buf2);
	
	
}

int is_pressed(int r, int c)
{
	// Set DDRC to 0 - input mode
	DDRC = 0;
	// Set all pins to N/C
	PORTC = 0;
	CLR_BIT(DDRC, r);
	SET_BIT(PORTC, r);
	SET_BIT(DDRC, c + 4);
	CLR_BIT(PORTC, c + 4);
	
	// Check if PINC is 1 or not at c
	if (!GET_BIT(PINC, r))
	{
		return 1;
	}
	
	return 0;
}
int get_key(){
	int r,c;
	for(r = 0; r < 4; r++){
		for(c = 0; c < 4; c++){
			if (is_pressed(r, c))
			{
				return 1 +  (r*4 + c);
			}
		}
	}
	return 0;
}

// returns a number from 0 - 1023
int get_sample(){
	// Configure mux - set to 0, already done?
	
	// enable converter
	SET_BIT(ADCSRA, 7);
	
	// start conversion
	SET_BIT(ADCSRA, 6);
	while (GET_BIT(ADCSRA, 6)) // when status bit equals 0 we exit out of loop else do nothing
	{
		
	}
	return ADC;
}

void voltage_init(){
	// set ADMUX bit 7 to 0 and bit 6 to 1 for voltage reference: AVCC with ext capacitor at AREF pin
	CLR_BIT(ADMUX, 7); // bit 7 to 0
	SET_BIT(ADMUX, 6); // bit 6 to 1
}
int main(void)
{
	lcd_init();
	voltage_init();
	displayInit();
    int instant, min, max;
	long sum; // we track sum of samples as a long number
	int sample_number;
	int calculate = 1;
    while(1)
	{
		int a = get_key();
		if (a == 4)
		{
			while(1)			{
				
				avr_wait(500);
				instant = get_sample();
				if (calculate)
				{
					
					min = instant;
					max = instant;
					sum = 0 + instant;
					sample_number = 1;
					displayConversion(instant, max, min, sum, sample_number);
					while (1)
					{
						int b = get_key();
						if (b == 4)
						{
							calculate = 0;
							break;
						}
						avr_wait(500);
						instant = get_sample();
						if (instant < min){
							min = instant;
						}
						if (instant > max)
						{
							max = instant;
						}
						sum = sum + instant;
						sample_number++;
						
						displayConversion(instant, max, min, sum, sample_number);
						
					}
				}
				else{
					displayRestart(instant);
					avr_wait(500);
					int c = get_key();
					if (c == 4)
					{
						calculate = 1;
						avr_wait(500);
					}
					
				}
				
			}
		}
	    
    }
}

