#ifndef _Server_HPP
#define _Server_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>
#include <vector>
#include "circuits.hpp"
#include "client.hpp"

class Client;
class Server{
private:
    static const uint32_t nslots = 128; // Biometric vector length
    static const uint32_t bitsize = 8; //8 significant bits, however a sign bit will be used
    static const uint32_t max_bitsize = 24; // the final result will be on 24 bits, in order to ensure correctness of the euclidian distance
    const unsigned m = 256;  // plaintext space Z_m
    uint64_t bound_match_clear = nslots*2; // arbitrary choice
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    std::vector<LweSample*> enc_template_client;
    std::vector<LweSample*> sample_client;
    int matching_result;
    uint64_t r0;
    uint64_t r1;
    LweSample* enc_r0;
    LweSample* enc_r1;
    LweSample* enc_b;
    LweSample* enc_y;
    int id_token;

public:
    Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* key);
    ~Server();
    void setTemplate(std::vector<LweSample*> vect);
    void setSample(std::vector<LweSample*> vect);
    void setMatchingResult(int token);
    void initAndEncRandomNumbers();
    void computeF();
    void computeG();
    void sendMatchingToken(Client& client);
    void identifyUser();
    void sendIdToken(Client& client);
};

#endif