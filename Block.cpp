#include "stdafx.h"
#include "Block.h"

void Block::Calculate(int dx, int dy, int dz, float rev, float ocost, float wcost, float pcost) {
	double magFe,BF_WR, DR_WR, effectiveWR;
	double bnet_waste, bnet_ore;
	double pellets_to_concentrate = 0.941176476;
	struct point{
		double x; 
		double y;
		double z;
		double i;
		double j;
	}myPlant, p1, p2, edgepoint;
	//myPlant.x = 611337.74;
	//myPlant.y = 6083358.10;
	//myPlant.z = 611.30;
	myPlant.x = 611337.74;
	myPlant.y = 6083358.10;
	myPlant.z = 611.30;
	double x0 = 611803.38;
	double y0 = 6085808;
	double angle = -46*3.14159/180;
	myPlant.i = (myPlant.x - x0)*cos(angle) - (myPlant.y-y0)*sin(angle);//-2615.5;
	myPlant.j = (myPlant.x - x0)*sin(angle) + (myPlant.y-y0)*cos(angle);//-7025;
	double xydist, zdist, xyzdist, productivity, cycletime, edgedist;

	for (int i=0; i<numSimulations; i++){
		simProperties[i].romTonnes = dx*dy*dz*simProperties[i].density*pitPercentage/100 * (simProperties[i].tper - simProperties[i].ob - simProperties[i].ms)/100;
		simProperties[i].obTonnes = dx*dy*dz*(2.0)*(simProperties[i].ob/100)*pitPercentage/100;
		simProperties[i].msTonnes = dx*dy*dz*(2.94)*(simProperties[i].ms/100)*pitPercentage/100;
		magFe = simProperties[i].grades.fec*simProperties[i].grades.dtwr/100;
		BF_WR = magFe*(0.95)*(0.975)/69.5*100;
		DR_WR = magFe*(0.95)*(0.95)/70.0*100;
		effectiveWR = (BF_WR*5 + DR_WR*4)/9;
		simProperties[i].concentrateTonnes = simProperties[i].romTonnes*effectiveWR/100;
		// ***************** Binder? Effective here is weighted average of BF and DR WRs, but where is consideration for additives?
		// Because for M.Eng. degree, you were targeting concentrate, not pellets. Now you have a pellet target!
		//Or you can convert the pellet targets to concentrate targets

		/* Calculate distance to edge of the pit, use 20 km/h for that distance***********************
		// Calculate X and Y distance from the edge point to the crusher, use 40 km/h for that distance
		For edge of pit line, p1 = 609351, 6088311
							  p2 = 615092, 6080115
		*/
		
		/*
		p1.x = 609351; p1.y = 6088311;
		p2.x = 615092; p2.y = 6080115;
		p1.i = (p1.x - x0)*cos(angle) - (p1.y-y0)*sin(angle);
		p1.j = (p1.x - x0)*sin(angle) + (p1.y-y0)*cos(angle);
		p2.i = (p2.x - x0)*cos(angle) - (p2.y-y0)*sin(angle);
		p2.j = (p2.x - x0)*sin(angle) + (p2.y-y0)*cos(angle);

		double m = (p2.j - p1.j) / (p2.i - p1.i);
		double b = p2.j - m*p2.i;
		// Equation is mx - y + b = 0, a = m, b = -1, c=b
		// Distance to line is |ax + by + c| / sqrt(a^2+b^2)  =  |m*x - y + b| / sqrt(m^2 + (-1)^2)
		edgedist = abs(m*x - y + b) / sqrt(m*m + 1);
		edgepoint.i = (x + m*y - m*b) / (m*m + 1);
		edgepoint.j = m * (x + m*y - m*b) / (m*m + 1) + b;
		double xdist = abs(myPlant.i - edgepoint.i);
		double ydist = abs(myPlant.j - edgepoint.j);
		xydist = xdist + ydist;

		zdist = myPlant.z-z;
		if (zdist<0) zdist=0;
		zdist /= sin(6*3.14159/180);
		double remainder = edgedist-zdist;
		if (remainder<0) remainder = 0;

		cycletime = zdist/1000/15*60; // 20 km/h coming out of the pit at 6 degrees
		cycletime += (2*xydist+2*remainder+zdist)/1000/35*60; //40 km/h the rest of the trip to the crusher and all the way back
		cycletime += 0.75+3.0+1.0 ; // Cycle time in minutes, Times 2 for full cycle (twice one-way), 20 km/h, 0.75 spot @ shovel, 0.5 spot@dump, 0.5 dump time
		// End of distance routine *********************************************************************

		/*
		xydist = sqrt( (x - myPlant.x)*(x - myPlant.x) + (y - myPlant.y)*(y - myPlant.y) ); // 2D distance of the block to the crusher, in meters
		xyzdist = sqrt( (x - myPlant.i)*(x - myPlant.i) + (y - myPlant.j)*(y - myPlant.j) + (z - myPlant.z)*(z - myPlant.z) ); // 3D distance of the block to the crusher, in meters
		zdist = myPlant.z - z;
		cycletime = (2*xyzdist/1000/20*60)+0.75+0.5+0.5 ; // Cycle time in minutes, Times 2 for full cycle (twice one-way), 20 km/h, 0.75 spot @ shovel, 0.5 spot@dump, 0.5 dump time
		*/
		
		/*cycletime is minutes / cycle
			cycletime/60 is hours / cycle
			1/340 cycles/tonne -> cycletime/60/340 is hours/tonne
			ie. 340*60/cycletime is tonnes per hour */
		/*
		productivity = 60/cycletime*340; // 340 t payload for CAT 797, tph = 340 tonnes / (cycletime in mins / 60 mins/hour) = 340*60/cycletime
		truckHours = (simProperties[0].romTonnes + simProperties[0].obTonnes + simProperties[0].msTonnes) / productivity;
		//truckHours = cycletime;
		*/
		//bnet_waste = - (simProperties[i].romTonnes + simProperties[i].obTonnes + simProperties[i].msTonnes)*(wcost) - truckHours * 531.24;
	    
	//bnet_waste = - (simProperties[i].romTonnes + simProperties[i].obTonnes + simProperties[i].msTonnes)*(wcost);
		//simProperties[i].bnet.push_back(bnet_waste);
		
		//bnet_ore = simProperties[i].concentrateTonnes/pellets_to_concentrate * rev - simProperties[i].romTonnes*(ocost) -  (simProperties[i].obTonnes + simProperties[i].msTonnes)*(wcost) - simProperties[i].concentrateTonnes*pcost - truckHours * 531.24;
		bnet_ore = simProperties[i].concentrateTonnes/pellets_to_concentrate * rev - simProperties[i].romTonnes*(ocost) -  (simProperties[i].obTonnes + simProperties[i].msTonnes)*(wcost) - simProperties[i].concentrateTonnes*pcost;
		simProperties[i].bnet.push_back(bnet_ore);
		//(17.32280343 - 0.00062434*xydist + 0.055520177*zdist)/60 * simProperties[0].romTonnes/360; // hours to haul the romTonnes (assume sim0 is typical)
	}
}
