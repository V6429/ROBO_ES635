#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <math.h> //included to support power function
#include "lcd.c"

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned int BATT_Voltage;

//Function to configure LCD port
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7;    //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80;  // all the LCD pins are set to logic 0 except PORTC 7
}

//ADC pin configuration
void adc_pin_config (void)
{
 DDRA = 0x00;  //set PORTA direction as input
 PORTA = 0x00; //set PORTA pins floating
}

//Function to Initialize PORTs
void ir_port_init()
{
 lcd_port_config();
 adc_pin_config();	
}

//Function to Initialize ADC
void adc_init()
{
 ADCSRA = 0x00;
 ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
 ACSR = 0x80;
 ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//This Function accepts the Channel Number and returns the corresponding Analog Value 
unsigned char ADC_Conversion(unsigned char Ch)
{
 unsigned char a;
 Ch = Ch & 0x07;  			
 ADMUX= 0x20| Ch;	   		
 ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
 while((ADCSRA&0x10)==0);	    //Wait for ADC conversion to complete
 a=ADCH;
 ADCSRA = ADCSRA|0x10;          //clear ADIF (ADC Interrupt Flag) by writing 1 to it
 return a;
}

// This Function prints the Analog Value Of Corresponding Channel No. at required Row
// and Coloumn Location. 
void print_sensor(char row, char coloumn,unsigned char channel)
{
 ADC_Value = ADC_Conversion(channel);
 lcd_print(row, coloumn, ADC_Value, 3);
}

void init_devices_ir (void)
{
 cli();           //Clears the global interrupts
 ir_port_init();
 adc_init();
 sei();           //Enables the global interrupts
}


//Motion Code

void motion_pin_config (void)
{
 DDRB = DDRB | 0x0F;   //set direction of the PORTB3 to PORTB0 pins as output
 PORTB = PORTB & 0xF0; // set initial value of the PORTB3 to PORTB0 pins to logic 0
 DDRD = DDRD | 0x30;   //Setting PD4 and PD5 pins as output for PWM generation
 PORTD = PORTD | 0x30; //PD4 and PD5 pins are for velocity control using PWM
}

//Function to initialize ports
void motion_port_init()
{
 motion_pin_config();
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortBRestore = 0;

 Direction &= 0x0F; 			// removing upper nibbel as it is not needed
 PortBRestore = PORTB; 			// reading the PORTB's original status
 PortBRestore &= 0xF0; 			// setting lower direction nibbel to 0
 PortBRestore |= Direction; 	// adding lower nibbel for direction command and restoring the PORTB status
 PORTB = PortBRestore; 			// setting the command to the port
}

void forward (void)         //both wheels forward
{
  motion_set(0x06);
}

void back (void)            //both wheels backward
{
  motion_set(0x09);
}

void left (void)            //Left wheel backward, Right wheel forward
{
  motion_set(0x05);
}

void right (void)           //Left wheel forward, Right wheel backward
{   
  motion_set(0x0A);
}

void soft_left (void)       //Left wheel stationary, Right wheel forward
{
 motion_set(0x04);
}

void soft_right (void)      //Left wheel forward, Right wheel is stationary
{ 
 motion_set(0x02);
}

void soft_left_2 (void)     //Left wheel backward, right wheel stationary
{
 motion_set(0x01);
}

void soft_right_2 (void)    //Left wheel stationary, Right wheel backward
{
 motion_set(0x08);
}

void hard_stop (void)       //hard stop(stop suddenly)
{
  motion_set(0x00);
}

void soft_stop (void)       //soft stop(stops solowly)
{
  motion_set(0x0F);
}


void init_devices (void)
{
 cli(); //Clears the global interrupts
 motion_port_init();
 sei(); //Enables the global interrupts
}



//Main Function
int main(void)
{
 init_devices_ir();
 init_devices();

 lcd_set_4bit();
 lcd_init();

 unsigned char vl,vc,vr;
 
 while(1)
 {            
	print_sensor(1,1,0);		//Prints IR Proximity Sensor Left
	print_sensor(1,5,1);		//Prints IR Proximity Sensor Center
	print_sensor(1,9,2);		//Prints IR Proximity Sensor Right

    //BATT_Voltage = (ADC_Conversion(6) * 0.03921) + 0.7;	//Prints Battery Voltage Status
    //lcd_print(2,13,BATT_Voltage,4);
    print_sensor(2,13,6);		//Prints Battery Voltage Status
	

	vl=ADC_Conversion(0);
	vc=ADC_Conversion(1);
	vr=ADC_Conversion(2);

		if(vc>0xC8)

			{
			  forward(); 
			  _delay_ms(300);
			}

			else if(vl>0xC8)

			{
				soft_left_2(); 
				_delay_ms(300);

			}
				
				else if(vr>0xC8)

				{
					soft_right_2();
					_delay_ms(300);

				}
					else

					{
						hard_stop();
						_delay_ms(1500);
					
					}

 }
}
