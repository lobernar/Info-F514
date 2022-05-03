#ifndef _Client_HPP
#define _Client_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>
#include <vector>
#include "server.hpp"
#include "circuits.hpp"

class Server;
class Client{
private:
    const int minimum_lambda = 128;
    static const uint32_t nslots = 128; // Biometric vector length
    static const uint32_t bitsize = 8; //8 significant bits, however a sign bit will be used
    static const uint32_t max_bitsize = 24; // the final result will be on 24 bits, in order to ensure correctness of the euclidian distance
    const unsigned m = 256;  // plaintext space Z_m
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    std::vector<uint8_t> template_client;
    std::vector<uint8_t> sample_client_true;
    std::vector<uint8_t> sample_client_false;
    std::vector<LweSample*> enc_template_client;
    std::vector<LweSample*> enc_sample_client_true;
    std::vector<LweSample*> enc_sample_client_false;
    int matching_result;
    int id_token;

public:
    Client();
    ~Client();
    const TFheGateBootstrappingCloudKeySet* getCloudKey();
    TFheGateBootstrappingSecretKeySet* getSecretKey();
    TFheGateBootstrappingParameterSet* getParams();
    void setIdToken(int token);
    void initTemplate();
    void initSamples();
    void encryptTemplate();
    void encryptSamples();
    void decryptMatchingResult(LweSample* token);
    void sendTemplate(Server& server);
    void sendTrueSample(Server& server);
    void sendFalseSample(Server& server);
    void sendDecToken(Server& server);   
};

#endif