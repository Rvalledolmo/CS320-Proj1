#include <iostream>
#include <stdlib.h>
#include "BranchPredictors.h"


int main(int argc, char *argv[]) {
  AlwaysTaken alwaysTaken;
  alwaysTaken.runAlwaysTaken(argv[1], argv[2]);

  AlwaysNonTaken alwaysNonTaken;
  alwaysNonTaken.runAlwaysNonTaken(argv[1], argv[2]);

  BimodalSingleBit Single(4);
  Single.runBimodalSingleBit(argv[1], argv[2]);
  BimodalSingleBit Single2(8);
  Single2.runBimodalSingleBit(argv[1], argv[2]);
  BimodalSingleBit Single3(32);
  Single3.runBimodalSingleBit(argv[1], argv[2]);
  BimodalSingleBit Single4(64);
  Single4.runBimodalSingleBit(argv[1], argv[2]);
  BimodalSingleBit Single5(256);
  Single5.runBimodalSingleBit(argv[1], argv[2]);
  BimodalSingleBit Single6(1024);
  Single6.runBimodalSingleBit(argv[1], argv[2]);
  BimodalSingleBit Single7(4096);
  Single7.runBimodalSingleBit(argv[1], argv[2]);

  BimodalTwoBit Two(4);
  Two.runBimodalTwoBit(argv[1], argv[2]);
  BimodalTwoBit Two2(8);
  Two2.runBimodalTwoBit(argv[1], argv[2]);
  BimodalTwoBit Two3(32);
  Two3.runBimodalTwoBit(argv[1], argv[2]);
  BimodalTwoBit Two4(64);
  Two4.runBimodalTwoBit(argv[1], argv[2]);
  BimodalTwoBit Two5(256);
  Two5.runBimodalTwoBit(argv[1], argv[2]);
  BimodalTwoBit Two6(1024);
  Two6.runBimodalTwoBit(argv[1], argv[2]);
  BimodalTwoBit Two7(4096);
  Two7.runBimodalTwoBit(argv[1], argv[2]);

  BimodalThreeBit Three(4);
  Three.runBimodalThreeBit(argv[1], argv[2]);
  BimodalThreeBit Three2(8);
  Three2.runBimodalThreeBit(argv[1], argv[2]);
  BimodalThreeBit Three3(32);
  Three3.runBimodalThreeBit(argv[1], argv[2]);
  BimodalThreeBit Three4(64);
  Three4.runBimodalThreeBit(argv[1], argv[2]);
  BimodalThreeBit Three5(256);
  Three5.runBimodalThreeBit(argv[1], argv[2]);
  BimodalThreeBit Three6(1024);
  Three6.runBimodalThreeBit(argv[1], argv[2]);
  BimodalThreeBit Three7(4096);
  Three7.runBimodalThreeBit(argv[1], argv[2]);
}

  
