#include "stm32f4xx.h"
#include "motorcontroller.h"




/* Simple Torque vectoring routine which only decreases torque on inner wheel and increases torque on outer wheel */
void torqueVector(uint16_t velocity, uint16_t steering, uint16_t torque) 
{
	if (velocity > 5)
	{
		if (steering > 0x7FFF>>1)	//Steering right
		{
			torqueR = simple_ackerman[velocity][(int)((steering/885)+0.5)]; 
			if (torqueR > torque) tractionControl(torque, frontR, rearR);

		}
		else										//steering left
		{
			torqueL = simple_ackerman[velocity][(int)((steering/442)+0.5)];	 
			if (torqueL > torque) tractionControl(torqueL, frontL, rearL);
			else
			{
				torqueR = (torque*2)-torqueL;
				torqueR = tractionControl();
				tractionControl(innerTorque, front, rear);
			}

		}
		
	}
}

/* Function only works when accelerating. Not when deccelerating */
uint16_t tractionControl(uint16_t torque, uint16_t front, uint16_t rear)
{	
	forhold = (double)rear/(double)front;
		
	if(forhold > MAX_SPIN && forhold > 0) /* Hvis bakhjul går  mer enn 15% raskere enn forhul */
	{
		torque = torque*forhold*(1+GRADIENT_AS);
	}
	return torque;
	
}


