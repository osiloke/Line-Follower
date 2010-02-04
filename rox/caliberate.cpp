#include <iostream>

using namespace std;

int i,j, k, x,w, firstcal, SENS_THRESH = 37;
const int cal_count = 4;
int sens_arr[3];
int sens_l[cal_count];
int sens_h[cal_count];
int sens_highest = 255, sens_lowest = 10;
int average_low = 0;
int average_high=0;

int LSENS_VAL=0;//left photoresistor
int RSENS_VAL=0;//right photoresistor
int MSENS_VAL=0;

void read_left(void);
void read_right(void);
void read_middle(void);
void oc_caliberate(void);

void AverageLowHigh(void);

void move_right(void);
void move_left(void);
void move_straight(void);

int main()
{
    ///oc_caliberate();
    //detect black line in the middle sensor so robot stays aligned with the line
    cout << "Finished Caliberation going to detection: "<< endl;
    
    int v=0;
	while(1)
    {
              
         cout << "Enter Left sensor: "<< endl;
         cin >> LSENS_VAL;
        
         cout << "Enter Right sensor: "<< endl;
         cin >> RSENS_VAL;
        
         cout << "Enter Middle sensor: "<< endl;
         cin >> MSENS_VAL;
         cout << "Sensor Thresh: " << SENS_THRESH/2 << endl;
     cout << "Sensor Thresh: " << SENS_THRESH << endl;
     cout << "sens highest - msesns"<<sens_highest - MSENS_VAL<< endl;
    cout << "msens-sens_lowest"<<MSENS_VAL  - sens_lowest<< endl;
    cout << "RSENS_VAL - MSENS_VAL"<<RSENS_VAL - MSENS_VAL<< endl;
    cout << "LSENS_VAL - MSENS_VAL"<<LSENS_VAL - MSENS_VAL<< endl;
             
    if (((MSENS_VAL  - sens_lowest) < (SENS_THRESH/2)) && ((sens_highest - MSENS_VAL) > SENS_THRESH) && (MSENS_VAL < LSENS_VAL) && (MSENS_VAL < RSENS_VAL) && ((RSENS_VAL - MSENS_VAL) > SENS_THRESH ) && ((LSENS_VAL - MSENS_VAL) > SENS_THRESH) )
	{
	//move straight
    //	rprintf("Moving Straight \r\n");
	move_straight();
	}
	else
	{
		if ((LSENS_VAL < MSENS_VAL) || ((LSENS_VAL - MSENS_VAL) < SENS_THRESH))
		{
			//rprintf("Moving right \r\n");
			move_left();
	

		}
		else if(RSENS_VAL < MSENS_VAL ||( (RSENS_VAL - MSENS_VAL) < SENS_THRESH))
		{
			//rprintf("Moving Left \r\n");
			move_right();
	
		}
        //else if ((RSENS_VAL < MSENS_VAL) && ((LSENS_VAL - MSENS_VAL) < SENS_THRESH))
        //{    
             //move_right();
        //}
        else
        { 
            cout <<"HELP"<<endl;
        }
		
	}
}
}
void oc_caliberate(void)

{
int n=1;

firstcal=1;
for (j = 0; j < cal_count; j++)
    {
       sens_l[j] = 0;
       sens_h[j] = 0;
    }
for (i = 0; i < cal_count; i++)
{
    cout << n << endl;
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
        cout <<  "sensor: ("<< x << ") " << "Value: " << sens_arr[x] << endl;
       
       
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
    
    cout <<  "Highest Value: " << sens_highest << endl;
    cout <<  "Lowest Value: " << sens_lowest << endl;
    firstcal = 0;
    
}
int s;
for (k = 0; k < cal_count; k++)
{
    cout <<  "Highest Value: " << sens_h[k] << endl;
    cout <<  "Lowest Value: " << sens_l[k] << endl;
     
}
AverageLowHigh();

cin >> s ;
}
void AverageLowHigh(void){
     int K;
    for (K = 0; K < cal_count; K++)
    {
        average_low += sens_l[K];
        average_high +=sens_h[K];
                 
    }
    average_low = (average_low/cal_count);
    average_high = (average_high/cal_count);
    SENS_THRESH = (average_high-average_low)/2;
    
    cout <<  "Average Lowest Value: " << average_low << endl;
    cout <<  "Average highest Value: " << average_high << endl;
    if (SENS_THRESH < 0 ) SENS_THRESH = 0;
    cout <<  "sensor threshhold: " << SENS_THRESH << endl;
    //cout <<  "Thresh from highes  lowest: " << ((sens_highest-sens_lowest)/2) << endl;
    
}

void read_left(void)
{
 cout << "Enter Left sensor: "<< endl;
 cin >> sens_arr[0];
}

void read_right(void)
{
 cout << "Enter Right sensor: "<< endl;
 cin >> sens_arr[1];
}
void read_middle(void)
{
 cout << "Enter Middle sensor: "<< endl;
 cin >> sens_arr[2];
}
void move_right(void){
     cout << "moving right "<< endl;
}
void move_left(void){
     cout << "Moving left "<< endl;
}
void move_straight(void){
     cout << "Moving Straight "<< endl;
}
