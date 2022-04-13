#include <tfhe_gate_bootstrapping_functions.h>


/*
 * CIPHERTEXT IMPLEMENTATION ONLY 
 * n-bit addition
*/
void add1Bit(LweSample* result, LweSample* a, LweSample* b, LweSample* carry, const TFheGateBootstrappingCloudKeySet* cloud_key){
    LweSample* tmp = new_gate_bootstrapping_ciphertext_array(1, cloud_key->params);
    LweSample* a_carry = new_gate_bootstrapping_ciphertext_array(1, cloud_key->params);
    LweSample* b_carry = new_gate_bootstrapping_ciphertext_array(1, cloud_key->params);

    // Add a, b and carry
    bootsXOR(&tmp[0], a, b, cloud_key);
    bootsXOR(result, &tmp[0], carry, cloud_key);

    // a AND b
    bootsAND(&tmp[0], a, b, cloud_key);
    // a AND carry_in
    bootsAND(&a_carry[0], a, carry, cloud_key);
    // b AND carry_in
    bootsAND(&b_carry[0], b, carry, cloud_key);
    // (a AND b) XOR (a AND carry_in)
    bootsXOR(carry, &tmp[0], a_carry, cloud_key);
    // (a AND b) XOR (a AND carry_in) XOR (b AND carry_in)
    bootsXOR(&tmp[0], carry, b_carry, cloud_key);
    bootsCOPY(carry, &tmp[0], cloud_key);

    delete_gate_bootstrapping_ciphertext_array(1, tmp);
    delete_gate_bootstrapping_ciphertext_array(1, a_carry);
    delete_gate_bootstrapping_ciphertext_array(1, b_carry);
}

void addNNbit(LweSample* result, LweSample* a, LweSample* b, LweSample* carry, const int bitsize, const TFheGateBootstrappingCloudKeySet* cloud_key){
    LweSample* carry = new_gate_bootstrapping_ciphertext_array(1, cloud_key->params);
    // Assign 0 to carry
    bootsCONSTANT(&carry[0], 0, cloud_key);

    for(int i=0; i<bitsize; ++i){
        add1Bit(&result[i], &a[i], &b[i], &carry[0], cloud_key);
    }

    delete_gate_bootstrapping_ciphertext_array(1, carry);
}


/*
 * Ciphertext functions

int euclidean_dist(int x[], int y[], int size){
    unsigned sum = 0;
    for(int i=0; i<size; ++i) sum = sum +  pow(y[i]-x[i], 2);
    return sqrt(sum);
}

bool matching(int sample[], int templt[], int size, unsigned B){
    int dist = euclidean_dist(templt, sample, size);
    if(pow(dist, 2) <= B) return 1;  // Match
    return 0;  // No match
}

*/
