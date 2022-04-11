#include <tfhe.h>
#include <tfhe_io.h>

void add1Bit(LweSample* result, LweSample* a, LweSample* b, LweSample* carry, const TFheGateBootstrappingCloudKeySet* cloud_key){
    LweSample* tmp = new_gate_bootstrapping_ciphertext_array(1, cloud_key->params);
    LweSample* tmp_carry = new_gate_bootstrapping_ciphertext_array(1, cloud_key->params);

    // Add a, b and carry
    bootsXOR(&tmp[0], a, b, cloud_key);
    bootsXOR(result, &tmp[0], carry, cloud_key);

    // Handling carry
    bootsAND(&tmp[0], a, b, cloud_key);
    bootsAND(&tmp_carry[0], &tmp[0],carry, cloud_key);

    delete_gate_bootstrapping_ciphertext_array(1, tmp);
    delete_gate_bootstrapping_ciphertext_array(1, tmp_carry);
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