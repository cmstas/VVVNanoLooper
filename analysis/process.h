#ifndef process_h
#define process_h

#include "rooutil.h"
#include "cxxopts.h"

// helper functions
void setup_SRBranch();
void setupBDTBranches_test();
void setupBDTBranches_train();
void fillBDTBranches_test();
void fillBDTBranches_train();
void setupBDTBranches_SF_test();
void setupBDTBranches_SF_train();
void fillBDTBranches_SF_test();
void fillBDTBranches_SF_train(); 
double helicity(const LorentzVector particle_1, const LorentzVector particle_2);
double computeMT(const LorentzVector particle, const LorentzVector met); 

#endif 
