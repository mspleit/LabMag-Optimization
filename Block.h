#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

using std::vector;

struct gradeItems{
	float feh, dtwr, fec, sio2c;
};

struct properties{
	gradeItems grades;
	float density;
	float ob, ms, tper;
	double romTonnes, concentrateTonnes, obTonnes, msTonnes;
	vector<double> bnet; // bnet[0] is bnet for destination 0
};

class Block {

public:
	float x,y,z;
	int numSimulations;
	float pitPercentage;
	vector<properties> simProperties;
	double truckHours;

	vector<int> precedents;
	double precedent_conc_tonnage;

	Block() {numSimulations=1;	};
	void Calculate(int dx, int dy, int dz, float rev, float ocost, float wcost, float pcost);
};


#endif
