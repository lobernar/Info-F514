#ifndef _Server_HPP
#define _Server_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>

class Server
{
private:
    /* data */
    const unsigned size = 128;  // Biometric vector length
    const unsigned m = 256;  // plaintext space Z_m
    const unsigned cipher_size = 8; // size of the ciphertext
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    LweSample** template_client;
    LweSample** sample_client;
    LweSample* result;
    unsigned r_0;
    unsigned r_1;
    LweSample* r_0_cipher;
    LweSample* r_1_cipher;

public:
    Server(TFheGateBootstrappingParameterSet* params, const TFheGateBootstrappingCloudKeySet* key);
    ~Server();
    void initRandomNumbers();
};

#endif