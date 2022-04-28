#ifndef _Server_HPP
#define _Server_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>
#include "../sources/circuits.cpp"

class Server
{
private:
    /* data */
    const unsigned size = 128;  // Biometric vector length
    const unsigned m = 256;  // plaintext space Z_m
    const unsigned cipher_size = 8; // size of the ciphertext
    const unsigned match_lim = size*2; // arbitrary choice
    const unsigned max_bitsize = 24; // the final result will be on 24 bits, in order to ensure correctness of the euclidian distance
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    LweSample* match_lim_cipher;
    LweSample** template_client;
    LweSample** sample_client;
    LweSample* result;
    LweSample* b;
    unsigned r_0;
    unsigned r_1;
    LweSample* r_0_cipher;
    LweSample* r_1_cipher;

public:
    Server(TFheGateBootstrappingParameterSet* params, const TFheGateBootstrappingCloudKeySet* key);
    ~Server();
    void setTemplate(LweSample* templ[]);
    void setSample(LweSample* sample[]);
    void initRandomNumbers();
    void computeF(LweSample* result_b, LweSample* x[], LweSample* y[]);
    void computeG(LweSample* result, LweSample* b, LweSample* r0, LweSample* r1);
};

#endif