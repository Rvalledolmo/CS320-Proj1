CXX = g++
CXXFLAGS = -Wall -g 

all: predictors

predictors: BranchPredictors.o prediction.o
	$(CXX) -o $@ $^

BranchPredictors.o: BranchPredictors.cpp BranchPredictors.h
	$(CXX) $(CXXFLAGS) -c BranchPredictors.cpp

prediction.o: prediction.cpp BranchPredictors.h
	$(CXX) $(CXXFLAGS) -c prediction.cpp

clean:
	rm -f *.o predictors *~ *output.txt
