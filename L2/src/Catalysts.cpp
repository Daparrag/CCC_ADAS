//============================================================================
// Name        : Catalysts.cpp
// Author      : diego
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>       /* sqrt */

using namespace std;

typedef double Number;

enum class CarStatus {STOP, MOVING_LESS_60KMH, MOVING_MORE_60KMH, BALANCE,NO_LIMIT};
enum class EngineStatus{ OFF,ON,ACCCELERATE, DESACCELERATE, KEEPSPEED  };

#define KMHtoMPS(x)	(x*1000/3600)
#define MAX_DISTANCE  500
#define COMPENSATE 50


int main(){

	Number a = -5.8; 		// Deceleration m/s2 ;
    Number speed = 0.00; 		// speed in Km/h;
    Number distance = 0.00;		// distance in meters;
    Number time = 0.00;			// Leaped time in seconds;
    std::string update;			// stop condition



    Number  target_speed = 0.00;  // previous speed limit;
    Number climit = 0.00;		// current speed limit;
    Number dnextlimit = 0.00;   // distance-to-next limit;
    Number nextlimit = 0.00;   	//  next limit;



    Number Pass_Speed= 0.00;   	//  last speed Measure;
    Number percent = 0.01;

    Number tstop=0.00;
    Number dstop=0.00;

    unsigned int throttle = 100;  // throttle power 0-100
    unsigned int brake = 0;		//  brake  0-100

    auto STcar = CarStatus::STOP;
    auto STengine = EngineStatus::OFF;
    auto New_Limit = false;
    auto maxspeed = false;
    auto minspeed = false;
    bool need_stop = false;



    std::string input_ref;



    while(1){


    	std::cin >> input_ref;

    	if(input_ref == "speed"){

    		std::cin >> speed;

    		//std::cout << "speed is: "<< speed << std::endl ;

    	}else if(input_ref == "distance"){

    		std::cin >> distance;
    		//std::cout << "speed is: "<< distance << std::endl ;

    	}else if (input_ref == "time"){
    		std::cin >> time;
    		//std::cout << "time: "<< time << std::endl ;

    	}else if (input_ref == "speedlimit"){

    		std::cin >> climit >> dnextlimit >> nextlimit;

    		if (climit != Pass_Speed){Pass_Speed =climit; New_Limit = true; }


    	}else if (input_ref == "update"){

    		/*computation*/
    		//std::cout << "update: "<< input_val << std::endl;

    		//check for stop condition.


    		if(New_Limit){



    			if( dnextlimit== 0.00 && nextlimit == 0.00 )
    			{
    				//there is no limit

    					STcar=CarStatus::NO_LIMIT;

    			}else if(nextlimit < climit){
						// in this case we have to decelerate at some point
						need_stop = true;
					}else{

						need_stop=false;

					}


    			if( STcar != CarStatus::STOP )
    			{
    				if(climit <= 60)
    				{
    					STcar = CarStatus::MOVING_LESS_60KMH;
    					if(target_speed < climit) {maxspeed = false; }

    				}
    				if(climit > 60)	{

    					STcar = CarStatus::MOVING_MORE_60KMH;
    					if(target_speed < climit) {maxspeed = false; }

    				}
    			}


					New_Limit = false;


    		}else{
    			// monitoring
    			switch (STcar){
    					case CarStatus::STOP:
    					{
    						if (distance < MAX_DISTANCE){

    							STengine=EngineStatus::ON;
    								if(climit < 60.00)
    									STcar=CarStatus::MOVING_LESS_60KMH;
    								else
    									STcar=CarStatus::MOVING_MORE_60KMH;
    						}//else emergency stop ??
    					}break;

    					case CarStatus::MOVING_LESS_60KMH:case CarStatus::MOVING_MORE_60KMH:
    					{
    						if(maxspeed==false){
    							target_speed = climit;
    							STengine=EngineStatus::ACCCELERATE;

    						}else{

    							//this means that we reach the max_speed
    							if(need_stop==true){
    								tstop = ((KMHtoMPS(nextlimit) - KMHtoMPS(climit)) /(2 * a));
    								dstop =  ( KMHtoMPS(climit) * tstop) + COMPENSATE;
    								minspeed=false;

    							}

    							STengine=EngineStatus::KEEPSPEED;
    							STcar=CarStatus::BALANCE;

    						}


    					}break;
    					case CarStatus::BALANCE:
    					{
    						if(need_stop == true )
    						{

    							if (dnextlimit <= dstop){
										//apply deceleration
    									target_speed = nextlimit;

										if(minspeed==false){

											STengine=EngineStatus::DESACCELERATE;

										}else{

											need_stop = false;
										}
    							}


    						}else{

    							STengine=EngineStatus::KEEPSPEED;
    						}


    					}break;
    					case CarStatus::NO_LIMIT:
    					{

    						if (STengine!=EngineStatus::KEEPSPEED){

    							STengine=EngineStatus::ACCCELERATE;
    						}

    					}break;
    			}


    			//engine FSM

    			switch (STengine){

    						//EngineStatus{ OFF,ON,ACCCELERATE, DESACCELERATE, KEEPSPEED  };

    						case EngineStatus::ON:case EngineStatus::OFF:break;
    						case EngineStatus::ACCCELERATE:
    						{
    							if(maxspeed == false ){  if(speed >= target_speed*(1-percent))maxspeed = true;  }

    							throttle =50; brake=0;

    						}break;
    						case EngineStatus::DESACCELERATE:
							{
								if(minspeed==false){ if(speed < target_speed*(1-percent)){minspeed=true;}  }
								throttle =0; brake=50;

							}break;
    						case EngineStatus::KEEPSPEED:
							{

    							if(speed < target_speed*(1-percent)){throttle =20;brake=0;}
    							if(speed > target_speed*(1-percent)){throttle =0;brake=25;}


							}break;

    			}


    		}

    		std::cout << "throttle "<< throttle << std::endl; std::cout << "brake "<< brake << std::endl;
    	}
    		//std::cout << "throttle "<< throttle << std::endl; std::cout << "brake "<< brake << std::endl;

    }

    return 0;
}
