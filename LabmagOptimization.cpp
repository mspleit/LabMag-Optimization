// LabMagOptimization.cpp : Defines the entry point for the console application.
// This program performs a CPLEX optimization of an SIP formulation for the open-pit mine scheduling problem
// 1. Input from a set stochastic models is read in from a file
// 2. A SIP model is formulated for CPLEX to solve
// 3. CPLEX is called to solve the optimization
// 4. Details (average grades, total tonnages etc.) of the optimal schedule are stored in a Schedule entity and output to a file
// 5. A MineSight .csv is generated for visualizing the schedule (each period represented by an integer 1,2,3, etc.)
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>

#include <ilcplex/ilocplex.h>
#include "BlockModel.h"
#include "Schedule.h"
#include "CplexModel.h"
//#include "Caccetta.h"
 
int dest[2];

int main(int argc, char **argv)
{
	BlockModel myModel;
	//string filename = "kmy1-5.csv";
	//string filename = "kmms.csv"; 
	//string filename = "kmd0.csv"; 
	//string filename = "kmbr6.csv"; 
	//string filename = "kmtl.csv"; 
	string filename = "km290216.csv"; 
	//string filename = "km10y.csv"; 
	//string filename = "km10-25.csv"; 
	//string filename = "labmagv3.csv"; 
	//string filename = "y1-10.csv"; 
	//string filename = "lm-9mtpy-25y.csv"; 
	//string filename = "p2-3inp.csv"; 
	//string filename = "p4-5inp.csv"; 
	
	//string filename = "det-lm.csv"; 
	//string filename = "lmv3-1sim.csv"; 
	//string filename = "1p10yv3.csv"; 
	//string filename = "1p5yv3.csv"; 
	//string filename = "y1-3.csv"; 
	//string filename = "y4-5.csv";
	//string filename = "1py6-10.csv";
	//string filename = "1py6-8.csv";
	//string filename = "1py9-10.csv";

	//string filename = "1py6-10lm.csv";

	//string filename = "bm5y.csv"; 
	//string filename = "bm10y-5y.csv"; 
	//string filename = "bm10y.csv"; 
	//string filename = "zone1.csv"; 
	myModel.ReadBlocksFromFile(filename); //input is file to read with block model values
	
	IloEnv env;
	int periods = 2; //If set to less than 2, change reserve constraints to <=1 instead of ==1
	periods++;
	dest[0] = -1;
	dest[1]=periods-2;//used to index into (PERIODS-1)*DESTINATIONS.  If PERIODS=5+1, DESTINATIONS=2: dest[d]+p size 10. p=1 d=0 dest=-1 array=0, p=1 d=1 dest=4 array=5
	CplexModel myCplex(env, myModel, periods);
	//myCplex.OutputTruckHours(myModel,"s:/labmagsim/th.csv"); 

	float dr = 0.08; //discount rate
	float penExcessConc=80*1000000;  // 10,000,000 Penalty on Excess Concentrate Mtonnes
	//float penExcessConc=800000000*1000;  // 10,000,000 Penalty on Excess Concentrate Mtonnes
	float penShortConc =100*1000000;   // 10,000,000 Penalty on Shortage of Concentrate Mtonnes
	float penExcessSic =10*1000000;  // 100,000,000 Penalty on Excess Silica (weighted)
	float penShortSic  =10*1000000;  // 100,000,000 Penalty on Shortage of Silica (weighted)
	float penExcessDtwr=100*1000000;  // 100,000,000 Penalty on Excess DTWR (weighted)
	float penShortDtwr =100*1000000;  // 100,000,000 Penalty on Shortage of DTWR (weighted)
	float penInnerSmooth=100000; //Inner Smoothing disabled. Just outer smoothing is effective.
	float penOuterSmooth=100000; //1000
	myCplex.Objective(myModel, dr, penExcessConc, penShortConc, penExcessSic, penShortSic, penExcessDtwr, penShortDtwr, penOuterSmooth);

	myCplex.ReserveConstraints(myModel);

	//double targetConcentrate[15] = {(0.6+0.85+3)*22.0001,0.85*22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//double targetConcentrate[15] = {(0.6+0.85+8)*22.0001,0.85*22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//BF DR, BF DR, etc. 
	// Period of 4.72 Mt DR @ <2.5 SiO2, 17.28 Mt BF @ <3.5: 5 years x (1 BF period + 1 DR period) = 10 periods
	/*
	double targetConcentrate[10] = {0.6*4.72, 0.6*17.2801, 0.85*4.72, 0.85*17.2801, 4.72, 17.2801, 4.72, 17.2801, 4.72, 17.2801};// Value in Millions
	double maxSiO2[10] = {2.5,3.5,  2.5,3.5,  2.5,3.5,  2.5,3.5,  2.5,3.5};
	double minSiO2[10] = {1.8,1.8,  1.8,1.8,  1.8,1.8,  1.8,1.8,  1.8,1.8};
	double maxDTWR[10] = {29,29,  29,29,  29,29,  29,29,  29,29};
	double minDTWR[10] = {23,23,  23,23,  23,23,  23,23,  23,23};
	*/
	int years_per_period[15] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	//double targetConcentrate[15] = {(0.6+0.85+1)*22.0001,2*22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//double targetConcentrate[15] = {(0.6)*22.0001,(0.85)*22.0001,3*22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//double targetConcentrate[15] = {0.6*22.0001,0.85*22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//double targetConcentrate[15] = {22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//double targetConcentrate[15] = {(0.6+0.85+23)*9.0001,0.85*9.0001,9.0001,9.0001,9.0001,9.0001,9.0001,9.0001,9.0001,9.0001,3*9.0001,3*9.0001,3*9.0001,3*9.0001,3*9.0001};// Value in Millions
	//conc tonnage = 0.97*pellet tonnage 0.97*9 = 8.73
	//double targetConcentrate[15] = {(0.6+0.85+23)*8.73+0.0001,0.85*8.73+0.0001,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	
//	double targetConcentrate[15] = {(0.6*8.73)+0.0001,(0.85+23)*8.73+0.0001,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	//double targetConcentrate[15] = {(0.6+0.85+3)*8.73,5*8.73,5*8.73,5*8.73,5*8.73,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	//double targetConcentrate[15] = {(0.6+0.85+8)*8.73,15*8.73,10*8.73,5*8.73,5*8.73,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	
	//double targetConcentrate[15] = {(0.6+0.85+1)*8.73,2*8.73,8.73,8.73,8.73,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	double targetConcentrate[15] = {500,289,8.73,8.73,8.73,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	//double targetConcentrate[15] = {(15)*8.73,3*8.73,8.73,8.73,8.73,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	//double targetConcentrate[15] = {15*8.73,8.73,8.73,8.73,8.73,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions

	//double targetConcentrate[15] = {(0.85+1)*8.73+0.0001,22*8.73+0.0001,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	//double targetConcentrate[15] = {(2)*8.73+0.0001,20*8.73+0.0001,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions

	//double targetConcentrate[15] = {82.4991,130.9501,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301,3*8.7301};// Value in Millions
	//double targetConcentrate[15] = {(0.6+0.85+3)*22.0001,5*22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001,3*22.0001};// Value in Millions
	//double maxSiO2[15] = {2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5};
	//double minSiO2[15] = {1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8,1.8};
	//double maxSiO2[15] = {2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4};
	//double minSiO2[15] = {2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4,2.4};
	double maxSiO2[15] = {4,4,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2};
	double minSiO2[15] = {2.0,4,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2,2.2};
	

	//double maxDTWR[15] = {29,29,29,29,29,29,29,29,29,29,29,29,29,29,29};
	//double minDTWR[15] = {23,23,23,23,23,23,23,23,23,23,23,23,23,23,23};
	
	double maxDTWR[15] = {30,28,29,29,29,29,29,29,29,29,29,29,29,29,29};
	double minDTWR[15] = {26,26,27,27,27,27,27,27,27,27,27,27,27,27,27};
	
	myCplex.GradeTonnageConstraints(myModel, targetConcentrate, minSiO2, maxSiO2, minDTWR, maxDTWR);

	//myCplex.TruckConstraints(myModel, years_per_period);

	myCplex.SlopeConstraints(myModel, filename, targetConcentrate);
	
	myCplex.SmoothingConstraints(myModel);
	cout << "Done smoothing." << endl;

	myCplex.Optimize(myModel);
	
	Schedule mySchedule(periods); // input is # of periods
	myCplex.GetSchedule(myModel, mySchedule);
	//myCplex.MakeMineSightFile(myModel, "s:/labmagsim/ms.csv");
	myCplex.MakeMineSightFile(myModel, "ms.csv");
	
	mySchedule.OutputResults("schedule.txt", 2); // 0 for file only, 1 for screen only, 2 for both file/screen output
	
	std::cin.get(); //wait for keypress
	env.end();

	return 0;
}

