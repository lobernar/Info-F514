#ifndef _CIRCUITS_HPP
#define _CIRCUITS_HPP

#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <tfhe/tfhe_core.h>
#include <tfhe/tfhe_gate_bootstrapping_functions.h>


void bootsADD1bit(LweSample* result, LweSample* a, LweSample* b, LweSample* carry, const TFheGateBootstrappingCloudKeySet* cloud_key);
void bootsADDNbit(LweSample* result, LweSample* a, LweSample* b, LweSample* carry, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);

void bootsTwoSComplement(LweSample* result, LweSample* a, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);
void bootsABS(LweSample* result, LweSample* a, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);
void bootsSUBNbit(LweSample* result, LweSample* a, LweSample* b, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);

void bootsShiftLeft(LweSample* result, LweSample* a, const int bitsize, const int n, const TFheGateBootstrappingCloudKeySet* cloud_key);
void bootsShiftRight(LweSample* result, LweSample* a, const int bitsize, const int n, const TFheGateBootstrappingCloudKeySet* cloud_key);
void bootsShiftLeftNR(LweSample* a, const int bitsize, const int n, const TFheGateBootstrappingCloudKeySet* cloud_key);

void bootsMultiply(LweSample* result, LweSample* a, LweSample* b, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);

double approxEquals(Torus32 a, Torus32 b);
void compare_bit(LweSample* result, const LweSample* a, const LweSample* b, const LweSample* lsb_carry, LweSample* tmp, const TFheGateBootstrappingCloudKeySet* bk);
void minimum(LweSample* result, LweSample* bit, const LweSample* a, const LweSample* b, const int nb_bits, const TFheGateBootstrappingCloudKeySet* bk);



void HE_ManhattanDistance(LweSample* result,  LweSample* a[], LweSample* b[], const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);
void HE_EuclideanDistance(LweSample* result, LweSample* a[], LweSample* b[], const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);

void f(LweSample* result_b, LweSample* a[], LweSample* b[], LweSample* bound_match, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);
void g(LweSample* result, LweSample* result_b, LweSample* r0, LweSample* r1, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key);

#endif //TFHE_PROTOCOL_MATH_H