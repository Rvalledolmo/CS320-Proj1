#include <fstream>  // For file writing
#include <sstream>  // For stringstream
#include <iostream> // For console output
#include <string>
#include "BranchPredictors.h"

using namespace std;

void AlwaysTaken::runAlwaysTaken(const string& traceFile, const string& outputFile) {
    unsigned long long addr;
    string behavior;
    ifstream infile(traceFile);
     ofstream outfile(outputFile, ios::app);

    int totalBranches = 0;
    int correctPredictions = 0;

    while (infile >> hex >> addr >> behavior) {

        bool actualOutcome = (behavior == "T");
        bool prediction = true;  // Always predict taken

        if (prediction == actualOutcome) {
            correctPredictions++;
        }
        totalBranches++;
    }

    infile.close();

    outfile << correctPredictions << "," <<  totalBranches << ";";
    outfile << endl;

    outfile.close();
}

void AlwaysNonTaken::runAlwaysNonTaken(const string& traceFile, const string& outputFile) {
    unsigned long long addr;
    string behavior;
    ifstream infile(traceFile);
    ofstream outfile(outputFile, ios::app);

        int totalBranches = 0;
    int correctPredictions = 0;

     while (infile >> hex >> addr >> behavior) {

        bool actualOutcome = (behavior == "T");
        bool prediction = false;  // Always predict non-taken

        if (prediction == actualOutcome) {
            correctPredictions++;
        }
        totalBranches++;
    }

    infile.close();
    outfile << correctPredictions << "," <<  totalBranches << ";";
    outfile << endl;

    outfile.close();
}

BimodalSingleBit::BimodalSingleBit(int size){
  tableSize = size;
        predictorTable.resize(tableSize, 0); // Initialize to Non-Taken (N)
    }

void BimodalSingleBit::runBimodalSingleBit(const string& traceFile, const string& outputFile) {
        ifstream infile(traceFile);
        ofstream outfile(outputFile, ios::app);
	int
        unsigned long long addr;
        string behavior;
        int correctPredictions = 0;
        int totalBranches = 0;
        string line;

        // Read through the trace file line by line
        while (infile >> hex >> addr >> behavior) {
            totalBranches++;

            int index = addr % tableSize; 
            bool predictedTaken = predictorTable[index];
            bool actualTaken = (behavior == "T");

            if (predictedTaken == actualTaken) {
                correctPredictions++;
            }

            predictorTable[index] = actualTaken;  // 1 for "Taken", 0 for "Non-Taken"
        }
        double accuracy = (static_cast<double>(correctPredictions) / totalBranches) * 100;
	(void)accuracy;
        outfile << correctPredictions << "," <<  totalBranches << "; ";
	if (tableSize == 4096) {
		outfile << endl;
}
        infile.close();
        outfile.close();
    }

BimodalTwoBit::BimodalTwoBit(int size) : tableSize(size) {
        predictorTable.resize(tableSize, 00); // Initialize to Strongly Non-Taken (NN)
}

void  BimodalTwoBit::updateCounter(int& counter, bool taken) {
        if (taken) {
	  if (counter < 3) {
	      
	      counter++;
	  }
        } else {
	  if (counter > 0){
	      counter--;
	  }
        }
	return;
    }

bool BimodalTwoBit::makePrediction(int counter) {
        return (counter >= 2); // Predict taken if in state 10 or 11
    }

void BimodalTwoBit::runBimodalTwoBit(const string& traceFile, const string& outputFile) {
    unsigned long long addr;
    string behavior;
    int correctPredictions = 0;
    int totalBranches = 0;

     ifstream infile(traceFile);
     ofstream outfile(outputFile, ios::app);

    while (infile >> hex >> addr >> behavior) {
        int index = addr % tableSize;
        bool actualTaken = (behavior == "T");

        bool prediction = makePrediction(predictorTable[index]);


        if (prediction == actualTaken) {
            correctPredictions++;
        }

        updateCounter(predictorTable[index], actualTaken);

        totalBranches++;
    }
        double accuracy = (static_cast<double>(correctPredictions) / totalBranches) * 100;
	
		(void)accuracy;
        outfile << correctPredictions << "," <<  totalBranches << "; ";
	if (tableSize == 4096) {
		outfile << endl;
}
}

     BimodalThreeBit::BimodalThreeBit(int size) : tableSize(size) {
        predictorTable.resize(tableSize, 1); // Initialize to state 1
    }

void BimodalThreeBit::updateState(int& state, bool actualOutcome, bool predictedOutcome) {
    if (predictedOutcome == actualOutcome) {
       if (actualOutcome) {
	 if (state > 1){
	      state--;
	 }
        } else {
	 if (state < 6) {
	   state++;
	 }
        }
    } else {
      if (actualOutcome) {
	if (state > 1){
	  state--;
	}
        } else {
	if (state < 6) {
	  state++;
	}
        }
    }
}

void BimodalThreeBit::runBimodalThreeBit(const string& traceFile, const string& outputFile) {
    ifstream infile(traceFile);
    ofstream outfile(outputFile, ios::app);
    unsigned long long addr;
    string behavior;
    int correctPredictions = 0;
    int totalBranches = 0;

    while (infile >> hex >> addr >> behavior) {
        int index = addr % tableSize;
        int state = predictorTable[index];
        bool predictedOutcome = (state < 4);
        bool actualOutcome = (behavior == "T");

        if (predictedOutcome == actualOutcome) {
            correctPredictions++;
        }
        totalBranches++;
    if (actualOutcome) {
	 if (predictorTable[index] > 1){
	      predictorTable[index]--;
	 }
        } else {
	 if (predictorTable[index] < 6) {
	   predictorTable[index]++;
	 }
    }

	// updateState(state, actualOutcome, predictedOutcome);
	// predictorTable[index] = state;
    }
        double accuracy = (static_cast<double>(correctPredictions) / totalBranches) * 100;
	(void)accuracy;

    outfile << correctPredictions << "," <<  totalBranches << "; ";
	if (tableSize == 4096) {
		outfile << endl;
}
}
