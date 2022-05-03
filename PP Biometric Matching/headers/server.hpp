#ifndef _Server_HPP
#define _Server_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>
#include "circuits.hpp"
#include "client.hpp"

class Client;
class Server{
private:
    static const unsigned size = 128;  // Biometric vector length
    const unsigned m = 256;  // plaintext space Z_m
    const unsigned cipher_size = 8; // size of the ciphertext
    const unsigned match_lim = size*2; // arbitrary choice
    const unsigned max_bitsize = 24; // the final result will be on 24 bits, in order to ensure correctness of the euclidian distance
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    LweSample** template_client;
    LweSample** sample_client;
    int matching_result;
    unsigned r_0;
    unsigned r_1;
    LweSample* r_0_cipher;
    LweSample* r_1_cipher;
    LweSample* result_f;
    LweSample* result_g;
    int id_token;

public:
    Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* key);
    ~Server();
    void setTemplate(LweSample* templ[]);
    void setSample(LweSample* sample[]);
    void setMatchingResult(int token);
    void initAndEncRandomNumbers();
    void computeF();
    void computeG();
    void sendMatchingToken(Client& client);
    void identifyUser();
    void sendIdToken(Client& client);
};

#endif