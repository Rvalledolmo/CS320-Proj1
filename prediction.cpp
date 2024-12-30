#include <iostream>
#include <stdlib.h>
#include "BranchPredictors.h"


int main(int argc, char *argv[]) {
  AlwaysTaken alwaysTaken;
  alwaysTaken.runAlwaysTaken(argv[1], argv[2]);

  AlwaysNonTaken alwaysNonTaken;
  alwaysNonTaken.runAlwaysNonTaken(argv[1], argv[2]);
}

  
