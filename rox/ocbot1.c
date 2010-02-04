/****************************************************************************
*
*  	Copyright (c) 2007 Osiloke Harold Emoekpere
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*	
*	with help from societyofrobots.com and other sources
*
****************************************************************************/


#include "helper.h" //helper file
int LSENS_VAL=0;//left photoresistor
int RSENS_VAL=0;//right photoresistor
int MSENS_VAL=0;//Middle photoresistor 
int i,j, k, x,w, n =1, firstcal, SENS_THRESH = 0;
int cal_count = 50;
int sens_arr[3];
int sens_l[50];
int sens_h[50];
int sens_highest = 0, sens_lowest = 0;
int average_low = 0;
int average_high=0;
int f = 1;


void scan_2sensors(void);
void scan_3sensors(void);	
void read_left(void);
void read_right(void);
void read_middle(void);
void oc_caliberate(void);

int main(void)
{
	//blink_n_times (1);
  	initialize_ports();
	
	initUART();
	//blink_n_times (1);
	// initialize our libraries
	timerInit();
	// initialize the timer system
	servoInit();
	// initialize the servo system
	//blink_n_times (1);
	initA2D();
	//blink_n_times (1);
    // Repeat indefinitely
    rprintf("Initialization Complete\r\n");
	//blink_n_times (1);
	turn_on_led();
	
	//blink_n_times (3);
	//delay_inms(50);	
	oc_caliberate();
	while(1)
    {
		
		rprintf("count: %d\r\n", n);
		f++;
		
		//turn_on_led();		
		//scan_2sensors();
		
		// Test port
       //if((COMMAND_PIN&(1<<COMMAND_BIT)) == 1)
		//{
            // Enable LED
            //turn_on_led();
			//blink_n_times (2);
			scan_3sensors();
        //}
        //else
        //{
            // Disable LED
            //turn_off_led();
		//	scan_2sensors();
        //}
		
		

    }
}
void scan_3sensors(void)
{
	
	//blink_n_times (n);
		rprintf("Left Sensor Value: %d\r\n", LSENS_VAL);
		//read_input8(LSENS_VAL,LSENS_BIT);//PORTC4
		LSENS_VAL=a2dConvert8bit(LSENS_BIT);
		
		rprintf("Right Sensor Value: %d\r\n", RSENS_VAL);
		//read_input8(RSENS_VAL,RSENS_BIT);//PORTC6
		RSENS_VAL=a2dConvert8bit(RSENS_BIT);		
		
		
		rprintf("Middle Sensor Value: %d\r\n", MSENS_VAL);
		MSENS_VAL=a2dConvert8bit(MSENS_BIT);//PORTC5
		
		//delay_inms(100);

	//detect black line in the middle sensor so robot stays aligned with the line
	if (((MSENS_VAL  - sens_lowest) < (SENS_THRESH)) && ((sens_highest - MSENS_VAL) > SENS_THRESH) && (MSENS_VAL < LSENS_VAL) && (MSENS_VAL < RSENS_VAL) && ((RSENS_VAL - MSENS_VAL) > SENS_THRESH ) && ((LSENS_VAL - MSENS_VAL) > SENS_THRESH) )
	{
	//move straight
    rprintf("Moving Straight \r\n");
	move_straight();
	}
	else
	{
		if ((LSENS_VAL < MSENS_VAL) || ((LSENS_VAL - MSENS_VAL) < SENS_THRESH))
		{
			rprintf("Moving right \r\n");
			move_left();
	

		}
		else if(RSENS_VAL < MSENS_VAL ||( (RSENS_VAL - MSENS_VAL) < SENS_THRESH))
		{
			rprintf("Moving Left \r\n");
			move_right();
	
		}
		else
		{ //cout <<"HELP"<<endl;
		hold_position();
		}
		
	}


}
void scan_2sensors(void)
{
	//blink_n_times (n);
		rprintf("Left Sensor Value: %d\r\n", LSENS_VAL);
		//read_input8(LSENS_VAL,LSENS_BIT);//PORTC4
		LSENS_VAL=a2dConvert8bit(LSENS_BIT);
		
		rprintf("Right Sensor Value: %d\r\n", RSENS_VAL);
		//read_input8(RSENS_VAL,RSENS_BIT);//PORTC6
		RSENS_VAL=a2dConvert8bit(RSENS_BIT);		
		//delay_inms(100);
		//rprintf("Middle Sensor Value: %d\r\n", MSENS_VAL);
		//read_input8(MSENS_VAL,MSENS_BIT);//PORTC5

	//detects more light on left side of robot
		if(LSENS_VAL > RSENS_VAL && (LSENS_VAL - RSENS_VAL) > SENS_THRESH)
			{//go left
			//blink_n_times (2);
			move_left();
			}

		//detects more light on right side of robot
		else if(RSENS_VAL > LSENS_VAL && (RSENS_VAL - RSENS_VAL) > SENS_THRESH)
			{//go right
			//blink_n_times (4);
			move_right();
			}

		//light is about equal on both sides
		else
			{//go straight
			//blink_n_times (5);
			
			move_straight();
			}
}
void oc_caliberate(void)

{


firstcal=1;
for (j = 0; j < cal_count; j++)
    {
       sens_l[j] = 0;
       sens_h[j] = 0;
    }
for (i = 0; i < cal_count; i++)
{
//blink_n_times (1);
   // cout << n << endl;
    n++;
    read_left();
    read_right();
    read_middle();
    if (firstcal == 1)
       {
          sens_lowest =  sens_arr[0];
          sens_highest =  sens_arr[0];
       }
    for (x = 0; x < 3; x++)
    {
       // cout <<  "sensor: ("<< x << ") " << "Value: " << sens_arr[x] << endl;
       
       
            if (sens_arr[x] <  sens_lowest)
            {
               sens_lowest = sens_arr[x];
            }
            if (sens_arr[x] >  sens_highest)
            {
               sens_highest = sens_arr[x];
            }
        
    }
    sens_l[i]=sens_arr[0];
    sens_h[i]=sens_arr[0];
    
    for (w = 0; w < 3; w++)
    {
        
            if (sens_arr[w] <  sens_l[i])
            {
               sens_l[i] = sens_arr[w];
            }
            if (sens_arr[w] >  sens_l[i])
            {
               sens_l[i] = sens_arr[w];
            }
        
    }
    
    //cout <<  "Highest Value: " << sens_highest << endl;
    //cout <<  "Lowest Value: " << sens_lowest << endl;
    firstcal = 0;
    
}

AverageLowHigh();

}
void AverageLowHigh(void){
     int K;
    for (K = 0; K < cal_count; K++)
    {
        average_low = average_low+sens_l[K];
        average_high = average_high+sens_h[K];
                 
    }
    average_low = (average_low/cal_count);
    average_high = (average_high/cal_count);
    SENS_THRESH = ((average_high-average_low))/2;
    
    //cout <<  "Average Lowest Value: " << average_low << endl;
    //cout <<  "Average highest Value: " << average_high << endl;
    if (SENS_THRESH < 0 ) {SENS_THRESH = 0;}
    //cout <<  "sensor threshhold: " << SENS_THRESH << endl;
    //cout <<  "Thresh from highes  lowest: " << ((sens_highest-sens_lowest)/2) << endl;
    
}

void read_left(void)
{
 sens_arr[0]=a2dConvert8bit(LSENS_BIT);
}

void read_right(void)
{
 sens_arr[1]= a2dConvert8bit(RSENS_BIT);
}
void read_middle(void)
{
 sens_arr[2]= a2dConvert8bit(MSENS_BIT);
}

