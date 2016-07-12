#ifndef MINING_H
#define MINING_H

#include <vector>
#include <fstream>
#include <iostream>
#include "Block.h"
using namespace std;

class BlockModel {
	int numBlocks, numSimulations;
	
public:
	int dx, dy, dz;
	float ore_cost, waste_cost, proc_cost, revenue;
	vector<Block> blocks;

	BlockModel();
	void ReadBlocksFromFile(string filename);
	int getNumBlocks(){return numBlocks;}
	int getNumSimulations(){return numSimulations;}
};

#endif
