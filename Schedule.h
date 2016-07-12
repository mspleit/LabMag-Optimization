#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>
#include "Block.h"
using namespace std;

class Schedule {

public:
	int numPeriods;
	vector<gradeItems> periodGrades;
	vector<gradeItems> minperiodGrades;
	vector<gradeItems> maxperiodGrades;
	vector<double> romTonnes, concentrateTonnes, obTonnes, msTonnes;
	vector<double> minromTonnes, minconcentrateTonnes, minobTonnes, minmsTonnes;
	vector<double> maxromTonnes, maxconcentrateTonnes, maxobTonnes, maxmsTonnes;
	vector<double> truckHours;
	Schedule(int periods);
	void OutputResults(string filename, int outputType);
	void Schedule::PrintVector(ofstream &file, int outputType, vector<double>& myvec);
	void Schedule::PrintGradeVector(ofstream &file, int outputType, vector<gradeItems>& myvec);

};

#endif
