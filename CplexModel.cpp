#include "stdafx.h"
#include "CplexModel.h"
#include "BlockModel.h"

void CplexModel::Optimize(BlockModel &myModel){
	IloEnv env = model.getEnv();
	try{
		cplex.extract(model);
	}
	catch (IloException& e) {
      cerr << "Concert exception caught: " << e << endl;
   }
   catch (...) {
      cerr << "Unknown exception caught" << endl;
   }
    cplex.setParam( IloCplex::DataCheck, true );
    //cplex.setParam(IloCplex::SiftItLim, 1000);
    //cplex.setParam(IloCplex::TiLim, 600); //Max optimizaton time limit, in seconds (600 = 10 minutes)
	cplex.setParam(IloCplex::MIPDisplay, 3); //0-5, 0=no display until optimal sol. found, 5=lots of display
	cplex.setParam(IloCplex::MIPInterval, 1000); //Display a log line every n nodes
	cplex.setParam(IloCplex::MIRCuts, 2); //-1 do not generate cuts, 0, auto-default, 1 generate cuts moderately, 2 generate them aggressively
	cplex.setParam(IloCplex::MIPEmphasis, 1); //0 balance, 1 feas, 2 optim, 3 best bound, 4 hidden feas
	cplex.setParam(IloCplex::Probe, 3);
	cplex.setParam(IloCplex::ProbeTime, 60); //in seconds
	cplex.setParam(IloCplex::EpAGap, 100000); //Absolute Gap threshold
	cplex.setParam(IloCplex::EpGap, 0.0384);//to instruct CPLEX to stop as soon as it has found a feasible integer solution proved to be within (0.01 = one percent) of optimal
	//cplex.setParam(IloCplex::ObjULim, 524828000000);
	//set the relative mipgap tolerance (EpGap) to 0.05.  
	//cplex.setParam (IloCplex::WorkMem, 12000); //in Mbytes
	cplex.setParam(IloCplex::MemoryEmphasis, 1); // 1 uses harddisk in addition to central memory
	//cplex.setParam(IloCplex::PreInd, 0); // Turn off presolve
	//cplex.setParam(IloCplex::NodeAlg,  4); // 0 default, 1 primal simplex, 2 dual simplex, 3 network simplex, 4 barrier, 5 sifting
	cplex.setParam(IloCplex::PreDual, 1);   //Set to 1 = Presolve on primal, optimize on dual		
	cplex.setParam(IloCplex::VarSel, 3); //4 less expensive variable selection strategy by setting the parameter, pseudo reduced costs. (for reducing root solve time)
	//3 strong branching, 2 branch based on pseudo costs, 1 branch on variable with maximum infeasibility, 0 let cplex choose
    cplex.setParam(IloCplex::HeurFreq, 0); // -1 turns off the periodic heuristic (for reducing root solve time)
	
	Priorities(myModel);
	Assumptions(myModel);

	cplex.setParam(IloCplex::MIPOrdInd, true); //default is true

	// Optimize the problem and obtain solution.
    if ( !cplex.solve() ) {
		env.error() << "Failed to optimize LP" << endl;
        throw(-1);
    }

    env.out() << "Solution status = " << cplex.getStatus() << endl;
    env.out() << "Solution value  = " << cplex.getObjValue() << endl;
	
	cout << "Optimization complete." << endl;

}

//****************************************************************************************************************************************************************
void CplexModel::Assumptions(BlockModel &myModel) {
	int i, d, p, s;
	double dt;
	int bench;
	int N = myModel.getNumBlocks();
	
	for (i=0;i<N;i++) {
	/*
		cout << i << endl;
		//for (d=0;d<DESTINATIONS;d++){
			dt = myModel.blocks[i].simProperties[0].grades.dtwr;
			if (dt>=18) { // If DTWR>18, this block will not be sent to the Waste destination dest[0]
				for(p=1;p<periods;p++) {
					model.add(q[i][dest[0]+p]==0);
				}
			}
			if (dt<18) { // If DTWR<=2, this block will not be sent to the Ore destination dest[1]
				for(p=1;p<periods;p++) {
					model.add(q[i][dest[1]+p]==0);
				}
			}*/
			
			//if the block is lower than Z=460m elevation, it will not be mined in the first period
			/*if (myModel.blocks[i].z < 437	) {
					//model.add(q[i][dest[1]+1]==0);
					model.add(q[i][dest[0]+1]==0);
			}*/

		//}
		
		/*
		bench = 14 - (int)(myModel.blocks[i].z - 417.5)/15; //lowest bench is 417.5, there are 13 benches total, so "14-etc." is to make top bench 1
		
		double x0 = 611803.38;
		double y0 = 6085808;
		double angle = -46*3.14159/180;
		double northendx = (611065 - x0)*cos(angle) - (6087000-y0)*sin(angle);// 6087958
		double northendy = (611065 - x0)*sin(angle) + (6087000-y0)*cos(angle);// 6087958

		//double southmidx=615720;
		//double southmidy=6080621;
		double southmidx = (615720 - x0)*cos(angle) - (6080621-y0)*sin(angle);// 6087958
		double southmidy = (615720 - x0)*sin(angle) + (6080621-y0)*cos(angle);// 6087958
		//double northmidx=610407;
		//double northmidy=6089213;
		double northmidx = (610407 - x0)*cos(angle) - (6089213-y0)*sin(angle);// 6087958
		double northmidy = (610407 - x0)*sin(angle) + (6089213-y0)*cos(angle);// 6087958
		//double m = (northendy-southendy)/(northendx-southendx);
		//double b = northendy-northendx*m;
		if ( ((northmidx - southmidx)*(myModel.blocks[i].y - southmidy) - (northmidy - southmidy)*(myModel.blocks[i].x - southmidx)) < 0 && bench>9	) {
		//if (bench > 6 ) { // set benches greater than x to be NOT mined in the first 5 periods
		//if (myModel.blocks[i].z <= 462.5 || myModel.blocks[i].y >= northendy) {
			for (p=1; p<min(5+1, periods); p++) {
				model.add(q[i][dest[0]+p]==0);
				model.add(q[i][dest[1]+p]==0);
			}
		}*/
	}
}

//****************************************************************************************************************************************************************
void CplexModel::Priorities(BlockModel &myModel) {
	int i, d, p, s;
	double dt;
	int bneto, bnetw;
	int N = myModel.getNumBlocks();
	for (i=0;i<N;i++) {
		for (p=1;p<periods;p++) {
			//dt = myModel.blocks[i].simProperties[0].grades.dtwr;
			//bneto = int(myModel.blocks[i].simProperties[0].bnet[1]);
			//bnetw = int(myModel.blocks[i].simProperties[0].bnet[0]);
			//if (bneto<0) bneto=0;
			//cplex.setPriority(q[i][dest[1]+p], bneto); //Prioritize decision variables with higher Block Net Value
			//cplex.setPriority(q[i][dest[0]+p], bneto);

			//cplex.setPriority(q[i][dest[1]+p], periods - p); //Prioritize decision variables with earlier period
			cplex.setPriority(q[i][dest[0]+p], periods - p);

				/*
				//priority to DTWR. within each DTWR %, prioritize higher benches
				cplex.setPriority(q[i][dest[d]+p], int(dt)*13+int((myModel.blocks[i].z - 417.5)/15));
				if (dt>18.0) {
					cplex.setDirection(q[i][dest[1]+p], IloCplex::BranchUp);
					cplex.setDirection(q[i][dest[0]+p], IloCplex::BranchDown);
				}else {
					cplex.setDirection(q[i][dest[1]+p], IloCplex::BranchDown);
					cplex.setDirection(q[i][dest[0]+p], IloCplex::BranchDown);
				}*/

		}
	}
}

//****************************************************************************************************************************************************************
void CplexModel::Objective(BlockModel &myModel, float dr, float pExConc, float pShortConc, float pExSic, float pShortSic, 
	float pExDtwr, float pShortDtwr, float penOuterSmooth) {
	
	IloEnv env = model.getEnv();
	IloExpr objective_value(env);
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();
	int i, d, p, s;
	float discount_factor;

	cout << "Inializing Objective Function... ";

	for (i=0;i<N;i++) {
		for (p=1;p<(periods);p++) {
			discount_factor = pow((1.0+dr), p);
			for (d=0;d<DESTINATIONS;d++){
				for(s=0;s<Nsims;s++) {
					objective_value += 100;
					objective_value += q[i][dest[d]+p];
					objective_value += ( q[i][dest[d]+p] * myModel.blocks[i].simProperties[s].bnet[d]) / discount_factor / Nsims;
				}
				// 23/07/2015 taken out for IPCC
				//objective_value -= q[i][dest[d]+p] * myModel.blocks[i].truckHours / discount_factor * 531.24; // cost $/hour to operate CAT 797 (from FS)
			}
			if (penOuterSmooth>0) {
				objective_value -= penOuterSmooth * smoothingDeviation1[i][p] / discount_factor;
			}
			//objective_value -= INNER_SMOOTHING_FACTOR * smoothing_deviation1[i][p];
			/*if (penOuterSmooth>0) {
				objective_value -= penOuterSmooth * smoothingDeviation2[i][p] / discount_factor;
			}*/
		}
	}
	//period-1 because don't penalize last period, since all material is being mined (see reserve constraints)
	for (p=1;p<(periods-1);p++) {
		discount_factor = pow((1.0+dr), p);
		for(s=0;s<Nsims;s++) {
			objective_value -= (pExConc*dev_conc_upper[s][p] + pShortConc*dev_conc_lower[s][p] ) / discount_factor; //tonnes deviation per model per period
			//objective_value -= (pExSic*dev_sio2_upper[s][p] + pShortSic*dev_sio2_lower[s][p] ) / discount_factor; // Penalize excess and shortage
			//objective_value -= (pExDtwr*dev_dtwr_upper[s][p] + pShortDtwr*dev_dtwr_lower[s][p] ) / discount_factor; // Penalize excess and shortage
			objective_value -= (pShortDtwr*dev_dtwr_lower[s][p] ) / discount_factor; // Penalize excess and shortage
		}
	}
	
	model.add(IloMaximize(env, objective_value));    
	objective_value.end();
	cout << "Complete." << endl;
}
//****************************************************************************************************************************************************************
void CplexModel::TruckConstraints(BlockModel &myModel, int *years_per_period) {
	IloEnv env = model.getEnv();
	int i,p,d; //Block, period, destination indices
	int N = myModel.getNumBlocks();
	double halftruck_working_hours = 5258.0/2;
	cout << "Initializing trucking constraints... ";

	for(p=2; p<periods; p+=1) {
		IloExpr truckhours_total(env);
		IloExpr prevperiod_truckhours_total(env);
		for (i=0; i<N; i++)	{
			for (d=0;d<DESTINATIONS;d++)
			{
				truckhours_total += q[i][dest[d]+p]*myModel.blocks[i].truckHours;
				//truckhours_total += q[i][dest[d]+p+1]*myModel.blocks[i].truckHours;
				prevperiod_truckhours_total += q[i][dest[d]+p-1]*myModel.blocks[i].truckHours;
				//prevperiod_truckhours_total += q[i][dest[d]+p-2]*myModel.blocks[i].truckHours;
			}
		}
		truckhours_total /= years_per_period[p-1];
		prevperiod_truckhours_total /= years_per_period[p-2];
		model.add(truckhours_total - prevperiod_truckhours_total >= -halftruck_working_hours);
		// >= Total working hours avaialable for one truck in one period / 2
		truckhours_total.end();
		prevperiod_truckhours_total.end();
	}
	cout << "Complete." << endl;
}
//****************************************************************************************************************************************************************
void CplexModel::ReserveConstraints(BlockModel &myModel){
	IloEnv env = model.getEnv();
	int i,p,d; //Block, period, destination indices
	int N = myModel.getNumBlocks();

	cout << "Initializing reserve constraints... ";

	for (i=0; i<N; i++)
	{
		IloExpr reserve_total(env);
		for (p=1;p<periods;p++)	{
			for (d=0;d<DESTINATIONS;d++) {
				reserve_total += q[i][dest[d]+p];
			}
		}
		//model.add(reserve_total <= 1); //block can be scheduled in only one period for only one destination
		model.add(reserve_total == 1); //==1 requires all blocks be scheduled, but only one period and one destination, <=1 allows non-scheduled blocks
		reserve_total.end();
	}
	cout << "Complete." << endl;

}
//****************************************************************************************************************************************************************

void CplexModel::SlopeConstraints(BlockModel &myModel, string filename, double *targetConcentrate) {
	IloEnv env = model.getEnv();
	
	int i, j, j_init, p,p2, d; //Block, period, destination indices
	int count;
	int current, one, two;
	int pcurrent, pone, ptwo;
	float percentage;
	
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();
	cout << "Initializing slope constraints... \n\r";
	
	ofstream myfile;
	string slope_filename = filename.substr(0,filename.length()-4) + ".slp";
    
	myfile.open (slope_filename);
		
	for (i=0; i<N; i++)	{
		count=0;
		current=9999; one=9999; two=9999;
		pcurrent=0;	  pone=0;	ptwo=0;
	
		if (i%(int)(N/100)==0) {
			percentage = (float)i/(float)N*100.0;
			if ((int)percentage % 10 == 0) {
				cout << setprecision(1) << std::fixed << percentage << "% ";
			}
		}
		
		//i is the current block. we will consider blocks above it.
		//since blocks are in decreasing Z sorted order, we need only keep track of the index of the 1st occurence of i.z-2*dz
		if (current != myModel.blocks[i].z) {
			two=one;
			ptwo = pone;
			one = current;
			pone = pcurrent;
			current = myModel.blocks[i].z;			
			pcurrent = i;
		}
		int onebenchlower = i+2454; 
		if (onebenchlower>N) onebenchlower=N;
		for (j=ptwo; j<onebenchlower; j++) {
		//for (j=1; j<N; j++) {
			if( ((myModel.blocks[j].x==myModel.blocks[i].x) && (myModel.blocks[j].y==myModel.blocks[i].y) && (myModel.blocks[j].z-myModel.blocks[i].z==myModel.dz))
            ||  ((myModel.blocks[j].x==myModel.blocks[i].x+myModel.dx) && (myModel.blocks[j].y==myModel.blocks[i].y) && (myModel.blocks[j].z-myModel.blocks[i].z==myModel.dz))
	        ||  ((myModel.blocks[j].x==myModel.blocks[i].x-myModel.dx) && (myModel.blocks[j].y==myModel.blocks[i].y) && (myModel.blocks[j].z-myModel.blocks[i].z==myModel.dz))
	        ||  ((myModel.blocks[j].x==myModel.blocks[i].x) && (myModel.blocks[j].y==myModel.blocks[i].y+myModel.dy) && (myModel.blocks[j].z-myModel.blocks[i].z==myModel.dz))	
	        ||  ((myModel.blocks[j].x==myModel.blocks[i].x) && (myModel.blocks[j].y==myModel.blocks[i].y-myModel.dy) && (myModel.blocks[j].z-myModel.blocks[i].z==myModel.dz))

			//2 blocks to the right, same Y, up one bench (ptwo refers to not the current bench, but the one before, ie. one bench up)
			//||  ((myModel.blocks[j].x==myModel.blocks[i].x+2*myModel.dx) && (myModel.blocks[j].y==myModel.blocks[i].y) && (myModel.blocks[j].z-myModel.blocks[i].z==myModel.dz))//Redundandt due to 45 deg slope constraint,no?

			//block to the left and block to the left and down must be mined, used for L->R mining progression 
			//|| ((myModel.blocks[j].x==myModel.blocks[i].x-myModel.dx) && (myModel.blocks[j].y==myModel.blocks[i].y) && (myModel.blocks[j].z-myModel.blocks[i].z==0)) //Redundant due to slope of lower block

			//Actually, this enforces mining blocks 45deg to each other MANDATORY in the same period.
			//|| ((myModel.blocks[j].x==myModel.blocks[i].x-5*myModel.dx) && (myModel.blocks[j].y==myModel.blocks[i].y) && (myModel.blocks[j].z-myModel.blocks[i].z==-myModel.dz))
			)
			{	
				myfile << i << " " << j << endl;
				
				for (p=1;p<periods;p++) {	
					IloExpr mined(env);
					IloExpr upper(env);
					mined = q[i][dest[0]+p];// + q[i][dest[1]+p];// mined[p]=1 if BLOCKi was mined in the current period 
					for (p2=1;p2<p+1;p2++) {	
						upper += q[j][dest[0]+p2];// + q[j][dest[1]+p2]; //Running total of predecessor blocks to BLOCKi mined in the current or any previous period
					}
					model.add(mined <= upper);
					upper.end();
					mined.end();
					count+=1;
				}//end for p
			}//endif
		}//end for j
		
		/*
		double tonnes = GetConcTonnes(myModel, i);
		if (i>2000) 
			i=i;
		int first_period = GetFirstPeriod(myModel, tonnes, targetConcentrate);
		if (first_period>4)
			i=i;
		for (p=1; p<first_period; p++) {
			model.add(q[i][dest[0]+p]==0);
			model.add(q[i][dest[1]+p]==0);
		}*/

	}//end for i
	cout << "Complete." << endl;
	myfile.close();
}

const double CplexModel::GetConcTonnes(BlockModel &myModel, int i) {
	//Returns the maximum tonnes of concentrate across all sims of a given block

	int j,s;
	double tonnes=0;
	int Nsims = myModel.getNumSimulations();
	if (myModel.blocks[i].precedent_conc_tonnage == -1) {
		for (s=0; s<Nsims; s++) {
			if (myModel.blocks[i].simProperties[s].concentrateTonnes > tonnes) { //get the maximum tonnes of concentrate of the block
				tonnes = myModel.blocks[i].simProperties[s].concentrateTonnes;
			}
		}
		for (j=0; j<myModel.blocks[i].precedents.size(); j++) {
			tonnes += GetConcTonnes(myModel, myModel.blocks[i].precedents[j]);
		}
		myModel.blocks[i].precedent_conc_tonnage=tonnes;
	}
	else {
		tonnes = myModel.blocks[i].precedent_conc_tonnage;
	}
	return tonnes;
}

const int CplexModel::GetFirstPeriod(BlockModel &myModel, double tonnes, double *targetConcentrate) {
	//Returns the period where the cumulative target annual tonnage has not yet surpassed "tonnes"
	int p;
	double cumTargets[15];
	for (p=1; p<periods; p++) {
		if (p==1) cumTargets[p-1] = targetConcentrate[p-1];
		else cumTargets[p-1] = targetConcentrate[p-1]+cumTargets[p-2];
	}

	p=periods-1;
	double blah = targetConcentrate[p-1]*1000000;
	while ( (cumTargets[p-1]*1000000 > tonnes) && (p>0) ) {
		p--;
	}
	//p++;
	return p;
}

//****************************************************************************************************************************************************************
void CplexModel::SmoothingConstraints(BlockModel &myModel) {
	IloEnv env = model.getEnv();
	float percentage;
	int i, j, j_init, j_end, p, p2, d; //Block, period, destination indices
	int xi,xj,yi,yj,zi,zj,dx,dy,dz;
	int nb_inner;
	int nb_outer;
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();
	p2=0;
	int current=9999, one=9999, two=9999;
	int pcurrent=0, pone=0, ptwo=0;
	
	cout << "Initializing smoothing constraints... \n\r";

	for (i=0; i<N; i++) //every 3rd?
	{
		IloExprArray smoothing1(env,periods); // array of IloExpressions that hold the 
		IloExprArray smoothing2(env,periods);
		
		if (i%(int)(N/10)==0)
		{	percentage = (float)i/(float)N*100.0;
			cout << setprecision(1) << std::fixed << percentage << "% ";
		}
		
		for (p=1;p<periods;p++)
		{	smoothing1[p] = IloExpr(env);
			smoothing2[p] = IloExpr(env);
		}
		nb_inner=0;
		nb_outer=0;
		
		int size=250;    //250 found by examining max number of entries with same elevation in full model 8311 (POSSIBLE BUG!!!)
		if (i>=size)	//We need only check at most one slab of blocks before the current BLOCKi under consideration if the blocks are in descending Z order!
			j_init = i-size;
		else
			j_init = 0;
		if (i<=N-size)
			j_end = i + size;
		else
			j_end = N;

		for (j=j_init; j<j_end; j++)
		{
			xi = myModel.blocks[i].x;
			xj = myModel.blocks[j].x;
			yi = myModel.blocks[i].y;
			yj = myModel.blocks[j].y;
			zi = myModel.blocks[i].z;
			zj = myModel.blocks[j].z;
			dx = myModel.dx;
			dy = myModel.dy;
			dz = myModel.dz;
			
			if( ((xj==xi-dx) && (yj==yi+dy) && (zj==zi))
			||  ((xj==xi) 	 && (yj==yi+dy) && (zj==zi))
            ||  ((xj==xi+dx) && (yj==yi+dy) && (zj==zi))
	        ||  ((xj==xi-dx) && (yj==yi)	&& (zj==zi))
			||  ((xj==xi+dx) && (yj==yi)	&& (zj==zi))
	        ||  ((xj==xi-dx) && (yj==yi-dy) && (zj==zi))
			||  ((xj==xi)	 && (yj==yi-dy) && (zj==zi))
            ||  ((xj==xi+dx) && (yj==yi-dy) && (zj==zi)) )
			{	
				nb_inner+=1; //keeps track of how many surrounding blocks j there are for the block i
				for (p=1;p<periods;p++) {	
					// if we find a surrounding block, 
					// within each period, we see if that surrounding block is mined
					// if it is, we reduce the smoothing factor by 1
					smoothing1[p] -= (q[j][dest[0]+p]);// + q[j][dest[1]+p]); 
				}//end for p
			}//endif

			if( ((xj==xi-2*dx) && (yj==yi+2*dy) && (zj==zi))
			||	((xj==xi-dx)   && (yj==yi+2*dy) && (zj==zi))
			||	((xj==xi)	   && (yj==yi+2*dy) && (zj==zi))
			||	((xj==xi+dx)   && (yj==yi+2*dy) && (zj==zi))
			||	((xj==xi+2*dx) && (yj==yi+2*dy) && (zj==zi))

			||	((xj==xi-2*dx) && (yj==yi+dy)   && (zj==zi))
			||	((xj==xi+2*dx) && (yj==yi+dy)   && (zj==zi))

			||	((xj==xi-2*dx) && (yj==yi)		&& (zj==zi))
			||	((xj==xi+2*dx) && (yj==yi)		&& (zj==zi))
			
			||	((xj==xi-2*dx) && (yj==yi-dy)   && (zj==zi))
			||	((xj==xi+2*dx) && (yj==yi-dy)   && (zj==zi))

			||  ((xj==xi-2*dx) && (yj==yi-2*dy) && (zj==zi))
			||	((xj==xi-dx)   && (yj==yi-2*dy) && (zj==zi))
			||	((xj==xi)	   && (yj==yi-2*dy) && (zj==zi))
			||	((xj==xi+dx)   && (yj==yi-2*dy) && (zj==zi))
			||	((xj==xi+2*dx) && (yj==yi-2*dy) && (zj==zi)) 
			
			||	((xj==xi-dx)   && (yj==yi+dy)	  && (zj==zi))
			||  ((xj==xi)	   && (yj==yi+dy)	  && (zj==zi))
            ||  ((xj==xi+dx)   && (yj==yi+dy)	  && (zj==zi))
	        ||  ((xj==xi-dx)   && (yj==yi)		  && (zj==zi))
			||  ((xj==xi+dx)   && (yj==yi)		  && (zj==zi))
	        ||  ((xj==xi-dx)   && (yj==yi-dy)	  && (zj==zi))
			||  ((xj==xi)	   && (yj==yi-dy)	  && (zj==zi))
            ||  ((xj==xi+dx)   && (yj==yi-dy)	  && (zj==zi)) 	)
			{	
				nb_outer+=1;
				for (p=1;p<periods;p++) {	
					smoothing2[p] -= (q[j][dest[0]+p]);// + q[j][dest[1]+p]);
				}//end for p
			}//endif
		}//end for j
		
		for (p=1; p<periods; p++) {	
			smoothing1[p] += nb_inner*(q[i][dest[0]+p]);// + q[i][dest[1]+p]);
			smoothing1[p] -= smoothingDeviation1[i][p];
			model.add(smoothing1[p] <= 0); //purposely ommitted! just using outer smoothing, just as effective.
			smoothing1[p].end();

			smoothing2[p] += nb_outer*(q[i][dest[0]+p]);// + q[i][dest[1]+p]);
			smoothing2[p] -= smoothingDeviation2[i][p];
			//model.add(smoothing2[p] <= 0);
			smoothing2[p].end();
			p2+=2;
		}
		smoothing1.end();	
		smoothing2.end();	
	}//end for i
	cout << endl << "Complete. Added " << p2 << " constraints." << endl;
}
//****************************************************************************************************************************************************************
void CplexModel::GradeTonnageConstraints(BlockModel &myModel, double *concTarget, double *minSiO2C, double *maxSiO2C, double *minDTWR, double *maxDTWR) {
	IloEnv env = model.getEnv();
	
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();

	IloExprArray per_dtwr(env,Nsims);
	IloExprArray per_sio2c(env,Nsims);
	IloExprArray per_tonnes(env,Nsims);
	IloExprArray per_conc(env,Nsims);

	int i, d, p, s;
	cout << "Initializing grade and tonnage constraints... ";	
	
	for (p=1;p<periods;p++) {	
		for (s=0;s<Nsims;s++) {
			per_dtwr[s] = IloExpr(env);
			per_sio2c[s] = IloExpr(env);
			per_tonnes[s] = IloExpr(env);
			per_conc[s] = IloExpr(env);
			
			d=0; //Ore blocks only

			for (i=0;i<N;i++) {
				per_dtwr[s] += q[i][dest[d]+p] * myModel.blocks[i].simProperties[s].romTonnes * myModel.blocks[i].simProperties[s].grades.dtwr/1000000/100;
				per_sio2c[s] += q[i][dest[d]+p] * myModel.blocks[i].simProperties[s].concentrateTonnes * myModel.blocks[i].simProperties[s].grades.sio2c/1000000/100;
				per_tonnes[s] += q[i][dest[d]+p] * myModel.blocks[i].simProperties[s].romTonnes/1000000;
				per_conc[s] += q[i][dest[d]+p] * myModel.blocks[i].simProperties[s].concentrateTonnes/1000000;
			}
			
			/*	
			if (maxDTWR>=0) {
				model.add(per_dtwr <= per_tonnes * maxDTWR/100);
			}
			if (minDTWR>=0) {
				model.add(per_dtwr >= per_tonnes * minDTWR/100);
			}
				
			//model.add(per_sio2c <= per_conc * MAX_SIO2C/100);
			//model.add(per_sio2c >= per_conc * MIN_SIO2C/100);
	
			//model.add(per_tonnes <= MAX_ROM_TONNES[p-1]); //-1 is because the array is initialized starting at p=0 for P1
			//model.add(per_tonnes >= MIN_ROM_TONNES[p-1]);*/
		
			//model.add(per_conc <= MAX_CONC_TONNES[p-1]);
			//model.add(per_conc >= MIN_CONC_TONNES[p-1]);
			
			/*if (concTarget!=NULL) {
				model.add(per_conc[s] - dev_conc_upper[s][p] <= concTarget[p-1]); //deviation is in millions
				model.add(per_conc[s] + dev_conc_lower[s][p] >= concTarget[p-1]); 
			}

			if (sio2Target!=NULL) {
				model.add(per_sio2c[s] - dev_sio2_upper[s][p] <= sio2Target[p-1]); //deviation is in % //added 23/10/2013
				model.add(per_sio2c[s] + dev_sio2_lower[s][p] >= sio2Target[p-1]); 
			}

			if (dtwrTarget!=NULL) {
				model.add(per_dtwr[s] - dev_dtwr_upper[s][p] <= dtwrTarget[p-1]); //deviation is in % //added 23/10/2013
				model.add(per_dtwr[s] + dev_dtwr_lower[s][p] >= dtwrTarget[p-1]); 
			}*/
			
			if (concTarget!=NULL) {
				model.add(per_conc[s] - dev_conc_upper[s][p] <= concTarget[p-1]); //deviation is in millions
				model.add(per_conc[s] + dev_conc_lower[s][p] >= concTarget[p-1]); 
			}

			/* 27-07-2015 taken out for max DTWR and we don't care about SiO2
			if (maxSiO2C!=NULL) {
				model.add(per_sio2c[s] - dev_sio2_upper[s][p] <= maxSiO2C[p-1]/100*per_conc[s]); //deviation is in % //added 23/10/2013
			}

			if (minSiO2C!=NULL) {
				model.add(per_sio2c[s] + dev_sio2_lower[s][p] >= minSiO2C[p-1]/100*per_conc[s]); 
			}
			
			if (maxDTWR!=NULL) {
				model.add(per_dtwr[s] - dev_dtwr_upper[s][p] <= maxDTWR[p-1]/100*per_tonnes[s]); //deviation is in % //added 23/10/2013
			}*/
			/*if (minDTWR!=NULL) {
				model.add(per_dtwr[s] + dev_dtwr_lower[s][p] >= minDTWR[p-1]/100*per_tonnes[s]); 
			}*/

			per_tonnes[s].end();
			per_conc[s].end();
			per_dtwr[s].end();
			per_sio2c[s].end();
		}
	}

	cout << "Complete." << endl;
}
//****************************************************************************************************************************************************************
void CplexModel::MakeMineSightFile(BlockModel &myModel, string filename) {

	ofstream myfile;
	myfile.open (filename);
	if(!myfile)
	{   // file couldn't be opened
      cerr << "Error: MineSight file could not be opened" << endl;
      exit(1);
	}	
	int i,p,d,s, val, period, destination;
	float smooth1, smooth2;
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();
	int flag;

	for(i=0;i<N;i++) {
		myfile << myModel.blocks[i].x << "," << myModel.blocks[i].y << "," << myModel.blocks[i].z << ",";//<< myModel.blocks[i].simProperties[0].romTonnes<< ",";
		//myfile << myModel.blocks[i].pitPercentage << "," << myModel.blocks[i].simProperties[0].density <<",";
		flag=0;
		for (int p=1; p<periods; p++) {
			for(d=0; d<DESTINATIONS; d++) {
				val = cplex.getValue(q[i][dest[d]+p]);
				if (val>0) {
					myfile << p << "," << d;
					flag=1;
				}//endif
			}//end d
		}//end p
		if (flag==0) 		{
			myfile << "-1,-1";
		}
		myfile << endl;
	}//end i
	myfile.close();
	cout << "MineSight file created." << endl;
}
//****************************************************************************************************************************************************************
void CplexModel::GetSchedule(BlockModel &myModel, Schedule &mySchedule){
	
	int i,p,d,s, ore, waste, period, destination, ob, ms;
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();
	float smooth1, smooth2;
	
	double wROM, wCONC, wOB, wMS, wFeH, wDTWR, wFeC, wSiO2C;

	int flag;
	
	gradeItems grades;
	gradeItems mingrades;
	gradeItems maxgrades;
	
	
	for (p=1; p<mySchedule.numPeriods; p++) {
		double minROM=99999999999, minCONC=99999999999, minOB=999999999999, minMS=999999999999;
		double maxROM=0, maxCONC=0, maxOB=0, maxMS=0;
		mingrades.feh = mingrades.dtwr = mingrades.fec = mingrades.sio2c = 999;
		maxgrades.feh = maxgrades.dtwr = maxgrades.fec = maxgrades.sio2c = 0;

		double pROM[10], pCONC[10], pOB[10], pMS[10], pFeH[10], pDTWR[10], pFeC[10], pSiO2C[10], rom, conc;
		for (s=0; s<Nsims; s++) {
			pROM[s]=0;
			pCONC[s]=0;
			pOB[s]=0;
			pMS[s]=0;
			pFeH[s]=0;
			pDTWR[s]=0;
			pFeC[s]=0;
			pSiO2C[s]=0;
		}
		for(i=0;i<N;i++) {
			for(s=0;s<Nsims;s++) {
				//cout << "pis: " << p << " " << i << " " << s << endl;
				ore = cplex.getValue(q[i][dest[0]+p]);
				//ore = cplex.getValue(q[i][dest[1]+p]);
				//waste = cplex.getValue(q[i][dest[0]+p]);

				rom = myModel.blocks[i].simProperties[s].romTonnes;
				conc = myModel.blocks[i].simProperties[s].concentrateTonnes;
				ob = myModel.blocks[i].simProperties[s].obTonnes;
				ms = myModel.blocks[i].simProperties[s].msTonnes;
				pROM[s] += rom * ore;
				pCONC[s] += conc * ore;
				
				pOB[s] += ob*ore; //+ rom * waste;
				pMS[s] += ms*ore;
				pFeH[s] += rom * myModel.blocks[i].simProperties[s].grades.feh * ore;
				pDTWR[s] += rom * myModel.blocks[i].simProperties[s].grades.dtwr * ore;
				pFeC[s] += conc * myModel.blocks[i].simProperties[s].grades.fec * ore;
				pSiO2C[s] += conc * myModel.blocks[i].simProperties[s].grades.sio2c * ore;
			}
		}
		//cout << "Loop done." <<endl;
		wROM = wCONC = wOB = wMS = wFeH = wDTWR = wFeC = wSiO2C = 0;
		for(s=0; s<Nsims; s++) { // finish the weighted averages for grades
			
			// calculate sums of rom and conc, and weighted sums of each grade
			wROM += pROM[s]/Nsims;
			wCONC += pCONC[s]/Nsims;
			wOB += pOB[s]/Nsims;
			wMS += pMS[s]/Nsims;
			wFeH += pFeH[s]/Nsims;
			wDTWR += pDTWR[s]/Nsims;
			wFeC += pFeC[s]/Nsims;
			wSiO2C += pSiO2C[s]/Nsims;

			// de-weight the grades for each simulation in the current period
			
			if (pROM[s] > 0) {
				pFeH[s] /= pROM[s];
				pDTWR[s] /= pROM[s];
			}
			if (pCONC[s] > 0) {
				pFeC[s] /= pCONC[s];
				pSiO2C[s] /= pCONC[s];
			}
			
			//for the current simulation, keep track of the min and max stuff
			if (pROM[s] < minROM) minROM = pROM[s];
			if (pROM[s] > maxROM) maxROM = pROM[s];
			
			if (pCONC[s] < minCONC) minCONC = pCONC[s];
			if (pCONC[s] > maxCONC) maxCONC = pCONC[s];
			
			if (pOB[s] < minOB) minOB = pOB[s];
			if (pOB[s] > maxOB) maxOB = pOB[s];
			
			if (pMS[s] < minMS) minMS = pMS[s];
			if (pMS[s] > maxMS) maxMS = pMS[s];
		
			if (pFeH[s] < mingrades.feh) mingrades.feh = pFeH[s];
			if (pFeH[s] > maxgrades.feh) maxgrades.feh = pFeH[s];

			if (pDTWR[s] < mingrades.dtwr) mingrades.dtwr = pDTWR[s];
			if (pDTWR[s] > maxgrades.dtwr) maxgrades.dtwr = pDTWR[s];
			
			if (pFeC[s] < mingrades.fec) mingrades.fec = pFeC[s];
			if (pFeC[s] > maxgrades.fec) maxgrades.fec = pFeC[s];

			if (pSiO2C[s] < mingrades.sio2c) mingrades.sio2c = pSiO2C[s];
			if (pSiO2C[s] > maxgrades.sio2c) maxgrades.sio2c = pSiO2C[s];

		}
		// de-weight the average grades across each simulation (in the current period)
		if (wROM > 0) {
			wFeH /= wROM;
			wDTWR /= wROM;
		}
		if (wCONC > 0) {
			wFeC /= wCONC;
			wSiO2C /= wCONC;
		}

		mySchedule.romTonnes.push_back(wROM);
		mySchedule.minromTonnes.push_back(minROM);
		mySchedule.maxromTonnes.push_back(maxROM);
		mySchedule.concentrateTonnes.push_back(wCONC);
		mySchedule.minconcentrateTonnes.push_back(minCONC);
		mySchedule.maxconcentrateTonnes.push_back(maxCONC);
		mySchedule.obTonnes.push_back(wOB);
		mySchedule.msTonnes.push_back(wMS);
		mySchedule.minobTonnes.push_back(minOB);
		mySchedule.maxobTonnes.push_back(maxOB);
		mySchedule.minmsTonnes.push_back(minMS);
		mySchedule.maxmsTonnes.push_back(maxMS);

		//cout << "Trying grades...";
		grades.feh = wFeH;
		grades.dtwr = wDTWR;
		grades.fec = wFeC;
		grades.sio2c = wSiO2C;

		mySchedule.periodGrades.push_back(grades);
		mySchedule.minperiodGrades.push_back(mingrades);
		mySchedule.maxperiodGrades.push_back(maxgrades);

		double devconcuppersum=0;
		double devconclowersum=0;
		double devsuppersum=0;
		double devslowersum=0;
		for(s=0;s<Nsims;s++){
			for(int p2=1;p2<periods;p2++){
				double val = cplex.getValue(dev_conc_upper[s][p2]);
				double val4 = cplex.getValue(dev_conc_lower[s][p2]);
//a				double val2 = cplex.getValue(dev_sio2_upper[s][p2]);
//a				double val3 = cplex.getValue(dev_sio2_lower[s][p2]);
				devconcuppersum += val;
				devconclowersum += val4;
//a				devsuppersum+=val2;
//a				devslowersum+=val3;
			}
		}
		double devSDsum=0;
		for(i=0;i<N;i++){
			for(int p2=1;p2<periods;p2++){
				double val = cplex.getValue(smoothingDeviation1[i][p2]);
				devSDsum += val;
			}
		}
		/*
		//a		  cout << "Smoothing deviation, Surplus conc., shortage conc., surplus SiO2*t, shortage SiO2*t: " << devSDsum << " " << devconcuppersum << " " << devconclowersum << " " << devsuppersum << " " << devslowersum << endl;
		*/
		cout << "Smoothing deviation, Surplus conc., shortage conc., surplus SiO2*t, shortage SiO2*t: " << devSDsum << " " << devconcuppersum << " " << devconclowersum << " " << endl;
	}
}

//****************************************************************************************************************************************************************
void CplexModel::OutputTruckHours(BlockModel &myModel, string filename) {

	ofstream myfile;
	myfile.open (filename);
	if(!myfile)
	{   // file couldn't be opened
      cerr << "Error: MineSight file could not be opened" << endl;
      exit(1);
	}	
	int i,p,d,s, val, period, destination;
	float smooth1, smooth2;
	int N = myModel.getNumBlocks();
	int Nsims = myModel.getNumSimulations();
	int flag;

	for(i=0;i<N;i++) {
		myfile << myModel.blocks[i].x << "," << myModel.blocks[i].y << "," << myModel.blocks[i].z << "," << myModel.blocks[i].truckHours;//<< myModel.blocks[i].simProperties[0].romTonnes<< ",";
		myfile << endl;
	}//end i
	myfile.close();
	cout << "MineSight file created." << endl;
}
//****************************************************************************************************************************************************************
