#include <stm32f4xx.h>
#include "antispinn.h"


//struct paadrag_as {
//	double paadrag_as_h;
//	double paadrag_as_v;
//};


double forhold;
uint16_t newTorque;
#define GRADIENT_AS 0.0526	// Stigningstallet til antispinnfunksjonen.
#define MAX_SPIN 115 			// Will allow 15 percent spin
#define SPIN 1

/* BASIS FOR CALCULATING GRADIENT_AS */
/*
const double ANTISPINN[] = {0, 0.010526,	0.021052,	0.031578,	0.042104,	0.05263,	0.063156,
		0.073682,	0.084208,	0.094734,	0.10526,	0.115786	,0.126312	,0.136838	,0.147364	,0.157890,
		0.168416	,0.178942	,0.189468	,0.199994	,0.210520,	0.221046	,0.231572	,0.242098	,0.252624,
		0.263150,	0.273676	,0.284202	,0.294728	,0.305254	,0.315780,	0.326306	,0.336832	,0.347358	,
		0.357884	,0.368410,	0.378936	,0.389462	,0.399988	,0.410514	,0.421040,	0.431566	,0.442092	,
		0.452618	,0.463144	,0.473670,	0.484196	,0.494722	,0.505248	,0.515774	,0.526300,	0.536826	,
		0.547352	,0.557878	,0.568404	,0.578930,	0.589456	,0.599982	,0.610508	,0.621034	,0.631560,	
		0.642086	,0.652612	,0.663138	,0.673664	,0.684190,	0.694716	,0.705242	,0.715768	,0.726294	,
		0.736820,	0.747346	,0.757872	,0.768398	,0.778924	,0.789450,	0.799976	,0.810502	,0.821028	,
		0.831554	,0.842080,	0.852606	,0.863132	,0.873658	,0.884184	,0.894710,	0.905236	,0.915762	,
		0.926288	,0.936814	,0.947340,	0.957866	,0.968392	,0.978918	,0.989444	,1,	1,	1,	1,	1,	1};
*/
void antispinnsjekk(uint16_t torque, uint16_t front, uint16_t rear)
{	
	forhold = (double)rear/(double)front;
		
	if(forhold > MAX_SPIN && forhold > 0) // Hvis høyre bakhjul spinner mer enn 15% raskere enn forhulet,
	{
		newTorque = torque*forhold*(1+GRADIENT_AS);
	}
	
}
