#ifndef _Client_HPP
#define _Client_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>
class Client
{
private:
    /* data */
    const int sec_param = 110;  // Security parameter
    const unsigned size = 128;  // Biometric vector length
    const unsigned m = 256;  // plaintext space Z_m
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    int cipher_size = 8;   // size of the ciphertext
    int* template_client;  // Oroginal template
    int* sample_true;  // Trustworthy user
    int* sample_false;  // Untrustworthy user
    LweSample** template_cipher;
    LweSample** sample_true_cipher;
    LweSample** sample_false_cipher;

public:
    Client(/* args */);
    void keyGeneration(); // ! fixme return (sk, pk) pair
    void templateEncryption();
    void initVectors();
    void encryptVectors();
    ~Client();
};

#endif