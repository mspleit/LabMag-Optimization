#include "stdafx.h"
#include "Schedule.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

Schedule::Schedule(int periods){
	numPeriods = periods;
	for (int i=0; i<periods; i++){
		float x=0.0;
		truckHours.push_back(x);
//		romTonnes
	}
}

void Schedule::PrintVector(ofstream &file, int outputType, vector<double>& myvec) {

	for (int p=1; p<numPeriods; p++) {
		if (outputType>0) {
			cout  << std::fixed << myvec[p-1] << " ";;
		}
		if (outputType!=1)
		{
			file  << std::fixed << myvec[p-1] << " ";;
		}
	}
	if (outputType > 0)	cout << endl;
	if (outputType !=1) file << endl;
}

void Schedule::PrintGradeVector(ofstream &file, int outputType, vector<gradeItems>& myvec) {
	
	for (int p=1; p<numPeriods; p++) {
		if (outputType>0) {
			cout << setprecision(2) << std::fixed << myvec[p-1].feh << " ";
		}
		if (outputType!=1)
		{
			file << setprecision(2) << std::fixed << myvec[p-1].feh << " ";
		}
	}
	if (outputType > 0)	cout << endl;
	if (outputType !=1) file << endl;

	for (int p=1; p<numPeriods; p++) {
		if (outputType>0) {
			cout << setprecision(2) << std::fixed << myvec[p-1].dtwr << " ";
		}
		if (outputType!=1)
		{
			file << setprecision(2) << std::fixed << myvec[p-1].dtwr << " ";
		}
	}
	if (outputType > 0)	cout << endl;
	if (outputType !=1) file << endl;

	for (int p=1; p<numPeriods; p++) {
		if (outputType>0) {
			cout << setprecision(2) << std::fixed << myvec[p-1].fec << " ";
		}
		if (outputType!=1)
		{
			file << setprecision(2) << std::fixed << myvec[p-1].fec << " ";
		}
	}
	if (outputType > 0)	cout << endl;
	if (outputType !=1) file << endl;

	for (int p=1; p<numPeriods; p++) {
		if (outputType>0) {
			cout << setprecision(2) << std::fixed << myvec[p-1].sio2c << " ";
		}
		if (outputType!=1)
		{
			file << setprecision(2) << std::fixed << myvec[p-1].sio2c << " ";
		}
	}
	if (outputType > 0)	cout << endl;
	if (outputType !=1) file << endl;
}
void Schedule::OutputResults(string filename, int outputType) {  // 0 for file only, 1 for screen only, 2 for both file/screen output

	ofstream myfile;
	
	if (outputType!=1) {
		myfile.open (filename);
	}

	PrintVector(myfile, outputType, romTonnes);
	PrintVector(myfile, outputType, minromTonnes);
	PrintVector(myfile, outputType, maxromTonnes);
	cout << endl;
	PrintVector(myfile, outputType, concentrateTonnes);
	PrintVector(myfile, outputType, minconcentrateTonnes);
	PrintVector(myfile, outputType, maxconcentrateTonnes);
	cout << endl;
	PrintVector(myfile, outputType, obTonnes);
	PrintVector(myfile, outputType, minobTonnes);
	PrintVector(myfile, outputType, maxobTonnes);
	cout << endl;
	PrintVector(myfile, outputType, msTonnes);
	PrintVector(myfile, outputType, minmsTonnes);
	PrintVector(myfile, outputType, maxmsTonnes);
	cout << endl;
	PrintGradeVector(myfile, outputType, periodGrades);
	cout << endl;
	PrintGradeVector(myfile, outputType, minperiodGrades);
	cout << endl;
	PrintGradeVector(myfile, outputType, maxperiodGrades);
}

