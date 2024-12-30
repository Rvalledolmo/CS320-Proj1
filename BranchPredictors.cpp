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

    // Read and process each line in the input file
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
  cout << "im running";
        ifstream infile(traceFile);
        ofstream outfile(outputFile, ios::app);
	int
        unsigned long long addr;
        string behavior;
        int correctPredictions = 0;
        int totalBranches = 0;
        string line;

        while (infile >> hex >> addr >> behavior) {
            totalBranches++;

          
            int index = addr % tableSize;  // Get index from lower bits of PC

            // Prediction based on the table entry
            bool predictedTaken = predictorTable[index];

            // Convert actual behavior to a boolean for comparison
            bool actualTaken = (behavior == "T");

            // Check if the prediction was correct
            if (predictedTaken == actualTaken) {
                correctPredictions++;
            }

            // Update the predictor table based on the actual outcome
            predictorTable[index] = actualTaken;  // 1 for "Taken", 0 for "Non-Taken"
        }

        // Write results to output file
        outfile << correctPredictions << "," <<  totalBranches << ";" << endl;

        // Close the files
        infile.close();
        outfile.close();
    }

BimodalTwoBit::BimodalTwoBit(int size) : tableSize(size) {
        predictorTable.resize(tableSize, 00); // Initialize to Strongly Non-Taken (NN)
}

void  BimodalTwoBit::updateCounter(int& counter, bool taken) {
        if (taken) {
            // If the branch is taken, increment the counter (up to 3)
	  if (counter < 3) {
	      
	      counter++;
	  }
        } else {
            // If the branch is not taken, decrement the counter (down to 0)
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

        // Make a prediction based on the current 2-bit counter value
        bool prediction = makePrediction(predictorTable[index]);

        // Check if the prediction was correct
        if (prediction == actualTaken) {
            correctPredictions++;
        }

        // Update the 2-bit counter based on the actual outcome
        updateCounter(predictorTable[index], actualTaken);

        totalBranches++;
    }

            // Write results to output file
        outfile << correctPredictions << "," <<  totalBranches << ";" << endl; // /n in main
}

     BimodalThreeBit::BimodalThreeBit(int size) : tableSize(size) {
        predictorTable.resize(tableSize, 1); // Initialize to state 1
    }

void BimodalThreeBit::updateState(int& state, bool actualOutcome, bool predictedOutcome) {
    if (predictedOutcome == actualOutcome) {
       if (actualOutcome) {
	 if (state > 0){
	      state--;
	 }
        } else {
            // Move towards "Strongly Not Taken" (State 6)
            if (state < 5) state++;
        }
    } else {
      if (actualOutcome) {
	if (state > 0){
	  state--;
	}
        } else {
	if (state < 5) {
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
        bool predictedOutcome = (state < 3);
        bool actualOutcome = (behavior == "T");

        if (predictedOutcome == actualOutcome) {
            correctPredictions++;
        }
        totalBranches++;

        updateState(state, actualOutcome, predictedOutcome);
        predictorTable[index] = state;
    }

    outfile << correctPredictions << "," <<  totalBranches << ";" << endl; // /n in main
}

/*
Gshare::Gshare(int size) : tableSize(size), globalHistory("1") {
        predictorTable.resize(tableSize, 0); // Initialize to Strongly Non-Taken (NN)
    }
int Gshare::getIndex(unsigned long long addr) {
    // Use the lower bits of the address and XOR with the global history
    int pcIndex = addr % tableSize;
    int ghrMask = globalHistory & ((1 << historyBits) - 1); // Mask to match the history bits length
    return (pcIndex ^ ghrMask) % tableSize; // XOR the PC index with the GHR and modulo table size
}

void Gshare::updateState(int& state, bool actualOutcome, bool predictedOutcome) {
    // Update the 2-bit saturating counter
    if (predictedOutcome == actualOutcome) {
        // Correct prediction
        if (actualOutcome) { // If actual outcome is Taken
            if (state < 3) state++; // Move towards Strongly Taken
        } else { // If actual outcome is Not Taken
            if (state > 0) state--; // Move towards Strongly Not Taken
        }
    } else {
        // Incorrect prediction
        if (actualOutcome) { // If actual outcome is Taken
            if (state < 3) state++; // Move towards Weakly or Strongly Taken
        } else { // If actual outcome is Not Taken
            if (state > 0) state--; // Move towards Weakly or Strongly Not Taken
        }
    }
}

void Gshare::updateGlobalHistory(bool actualOutcome) {
    // Update global history: shift left and set LSB to the actual outcome (0 = NT, 1 = T)
    globalHistory = ((globalHistory << 1) | actualOutcome) & ((1 << historyBits) - 1);
}

void Gshare::runGshare(const tring& traceFile) {
    ifstream infile(traceFile);
    unsigned long long addr;
    string behavior;
    int correctPredictions = 0;
    int totalBranches = 0;

    while (infile >> hex >> addr >> behavior) {
        int index = getIndex(addr);
        int state = predictorTable[index]; // Get the current state

        bool predictedOutcome = (state >= 2); // Predict "Taken" if in states 2 or 3
        bool actualOutcome = (behavior == "T"); // Actual outcome from the trace file

        // Update the correct prediction count
        if (predictedOutcome == actualOutcome) {
            correctPredictions++;
        }
        totalBranches++;

        // Update the state in the predictor table
        updateState(state, actualOutcome, predictedOutcome);
        predictorTable[index] = state;

        // Update the global history register
        updateGlobalHistory(actualOutcome);
    }

    // Output results
    cout << "Gshare Results for " << historyBits << "-bit Global History:" << endl;
    cout << "Correct Predictions: " << correctPredictions << endl;
    cout << "Total Branches: " << totalBranches << endl;
}
*/
