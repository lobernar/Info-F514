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
    static const uint32_t nslots = 32;//128; // biometric vector length
    static const uint32_t bitsize = 8; //8 significant bits, however a sign bit will be used
    static const uint32_t max_bitsize = 24; // the final result will be on 24 bits, in order to ensure correctness of the euclidian distance
    const unsigned m = 256;  // plaintext space Z_m
    int bound_match = nslots*2; // arbitrary choice
    int x1, x2; // used to initialize ciphertext tmp
    uint64_t y = 0;
    uint64_t res;
    uint64_t tmp_result;
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
    std::vector<LweSample*> enc_template_client;
    std::vector<LweSample*> enc_sample_client;
    LweSample* tmp;
    LweSample* tmp_carry;
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
    void initTmp();
    void initAndEncRandomNumbers();
    void sendMatchingToken(Client& client);
    void identifyUser();
    void sendIdToken(Client& client);
    // Circuits
    void nBitAddition();
    void twosComplement();
    void absoluteValue();
    void nBitSubstraction();
    void nBitMultiplication();
    void manhattanDistance();
    void euclideanDistance();
    // Protocol functions
    void computeF();
    void computeG();

};

#endif