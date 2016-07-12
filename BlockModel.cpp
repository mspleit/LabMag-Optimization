#include "stdafx.h"
#include "BlockModel.h"

BlockModel::BlockModel(){
	dx = 25;
	dy = 50;
	dz = 15;
	numBlocks = 0;
	numSimulations = 1;

	ore_cost = 2.10;   //2.10; mining cost of 1.40 has CAT 797 truck costs removed
	waste_cost = 2.10; //2.10;
	proc_cost = 51.00;
	revenue = 100;
}

void BlockModel::ReadBlocksFromFile(string filename){
	vector<Block>::iterator it;
	ifstream infile;
	infile.open(filename.c_str());
	if(!infile){   // file couldn't be opened
      cout << "Error: file could not be opened" << endl;
      exit(1);
	}

	cout << "Reading Block Model file: " << filename.c_str() << "..." << endl;
	int i=0;
	char line[200];
	while ( !infile.eof() ){  // keep reading until end-of-file
	//while (infile.getline(line))
		Block myBlock;
		blocks.push_back(myBlock);
		
		infile >> blocks[numBlocks].x;
		infile >> blocks[numBlocks].y;
		infile >> blocks[numBlocks].z;
		infile >> blocks[numBlocks].pitPercentage;
		blocks[numBlocks].pitPercentage=100; // Ignore pit percentage and treat block as 100%
		for (int s=0; s<numSimulations; s++){
			properties currentSim;
			blocks[numBlocks].simProperties.push_back(currentSim);
			infile >> blocks[numBlocks].simProperties[s].grades.feh;
			infile >> blocks[numBlocks].simProperties[s].grades.dtwr;
			infile >> blocks[numBlocks].simProperties[s].grades.fec;
			infile >> blocks[numBlocks].simProperties[s].grades.sio2c;

			infile >> blocks[numBlocks].simProperties[s].density;

			infile >> blocks[numBlocks].simProperties[s].ob;
			infile >> blocks[numBlocks].simProperties[s].ms;
			infile >> blocks[numBlocks].simProperties[s].tper;
			infile.ignore(256,'\n');
			infile.ignore(256,'\r');

			if (numBlocks % 1000 == 0) {
				cout << blocks[numBlocks].simProperties[s].tper << " ";
				cout << numBlocks << endl;
			}
	    }
		blocks[numBlocks].Calculate(dx,dy,dz, revenue, ore_cost, waste_cost, proc_cost);
		
		i=numBlocks;
		/* 
		for (int j=0; j<i; j++) {
			if ( ((blocks[j].x==blocks[i].x) && (blocks[j].y==blocks[i].y) && (blocks[j].z-blocks[i].z==dz))
            ||  ((blocks[j].x==blocks[i].x+dx) && (blocks[j].y==blocks[i].y) && (blocks[j].z-blocks[i].z==dz))
	        ||  ((blocks[j].x==blocks[i].x-dx) && (blocks[j].y==blocks[i].y) && (blocks[j].z-blocks[i].z==0))
	        ||  ((blocks[j].x==blocks[i].x) && (blocks[j].y==blocks[i].y+dy) && (blocks[j].z-blocks[i].z==0))	
	        ||  ((blocks[j].x==blocks[i].x) && (blocks[j].y==blocks[i].y-dy) && (blocks[j].z-blocks[i].z==dz))

			//2 blocks to the right, same Y, same Z
			||  ((blocks[j].x==blocks[i].x+2*dx) && (blocks[j].y==blocks[i].y) && (blocks[j].z-blocks[i].z==dz))

			//block to the left and block to the left and down must be mined, used for L->R mining progression
			|| ((blocks[j].x==blocks[i].x-dx) && (blocks[j].y==blocks[i].y) && (blocks[j].z-blocks[i].z==0))  //Redundant due to slope of lower block
			){
				blocks[i].precedents.push_back(j);
			}
		}
		blocks[i].precedent_conc_tonnage=-1;
		*/
		numBlocks++;
		//cout << blocks[numBlocks-1].x << " " << blocks[numBlocks-1].y << " " << blocks[numBlocks-1].z << endl;
	}
	   
	infile.close();
	cout << "Last line of input:" << endl;
	cout << blocks[numBlocks-1].x << " " << blocks[numBlocks-1].y << " " << blocks[numBlocks-1].z << endl;
	cout << "Input file read." << endl;
	cout << numBlocks << " blocks read." << endl;
}
