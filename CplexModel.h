#ifndef CPLEX_MODEL_H
#define CPLEX_MODEL_H

#include <ilcplex/ilocplex.h>
#include "Schedule.h"
#include "BlockModel.h"

ILOSTLBEGIN

const int DESTINATIONS = 1;
extern int dest[2];

typedef IloArray<IloIntVarArray> IloIntVarArray2;
typedef IloArray<IloNumVarArray> IloNumVarArray2;
typedef IloArray<IloExprArray> IloExprArray2;

typedef IloArray<IloIntVarArray2> IloIntVarArray3;
typedef IloArray<IloNumVarArray2> IloNumVarArray3;
typedef IloArray<IloExprArray2> IloExprArray3;

class CplexModel {
	IloModel model;
	IloCplex cplex;
	int periods;
	IloIntVarArray2 q;
	IloNumVarArray2 dev_sio2_upper, dev_sio2_lower;
	IloNumVarArray2 dev_dtwr_upper, dev_dtwr_lower;
	IloNumVarArray2 dev_conc_upper, dev_conc_lower;
	IloNumVarArray2 smoothingDeviation1, smoothingDeviation2;

public:
	
	/*CplexModel(IloEnv &env, int p) : model(env) {periods = p;}*/
	CplexModel(IloEnv &env, BlockModel &myModel, int p) : model(env), cplex(env), q(env, myModel.getNumBlocks()), smoothingDeviation1(env, myModel.getNumBlocks()),
		smoothingDeviation2(env, myModel.getNumBlocks()), dev_conc_upper(env, myModel.getNumSimulations()), dev_conc_lower(env, myModel.getNumSimulations()),
		dev_sio2_upper(env, myModel.getNumSimulations()), dev_sio2_lower(env, myModel.getNumSimulations()),
		dev_dtwr_upper(env, myModel.getNumSimulations()), dev_dtwr_lower(env, myModel.getNumSimulations()){
		periods = p;
		int Nblocks = myModel.getNumBlocks();
		int Nsims = myModel.getNumSimulations();
			  
		for (int s=0;s<Nsims;s++){
			dev_conc_upper[s] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
			dev_conc_lower[s] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
			dev_sio2_upper[s] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
			dev_sio2_lower[s] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
			dev_dtwr_upper[s] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
			dev_dtwr_lower[s] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
		}
	  
		for (int i=0; i<Nblocks; i++){
			q[i] = IloIntVarArray(env, (periods-1)*DESTINATIONS, 0, 1);
			smoothingDeviation1[i] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
			smoothingDeviation2[i] = IloNumVarArray(env, periods, 0, IloInfinity, ILOFLOAT);
		}
	};

	void GetSchedule(BlockModel &myModel, Schedule &mySchedule); 
	void GradeTonnageConstraints(BlockModel &myModel, double *concTarget, double *minSiO2C, double *maxSiO2C, double *minDTWR, double *maxDTWR);
	void MakeMineSightFile(BlockModel &myModel, string filename);
	void OutputTruckHours(BlockModel &myModel, string filename);
	void Objective(BlockModel &myModel, float dr, float pExConc, float pShortConc, float pExSic, float pShortSic, float pExDtwr, float pShortDtwr, float penOuterSmooth);
	void Optimize(BlockModel &myModel);
	void TruckConstraints(BlockModel &myModel, int *years_per_period);
	void ReserveConstraints(BlockModel &myModel);
	void SlopeConstraints(BlockModel &myModel, string filename, double *targetConcentrate);
	void SmoothingConstraints(BlockModel &myModel);
	void Priorities(BlockModel &myModel);
	void Assumptions(BlockModel &myModel);
	const double CplexModel::GetConcTonnes(BlockModel &myModel, int i);
	const int CplexModel::GetFirstPeriod(BlockModel &myModel, double tonnes, double *targetConcentrate);
};

#endif
