#include "../headers/server.hpp"

//Server::Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* sec_key){
Server::Server(TFheGateBootstrappingParameterSet* params, const TFheGateBootstrappingCloudKeySet* key){
    this->params = params;
    this->cloud_key = key;
    this->result = new_gate_bootstrapping_ciphertext_array(this->size, this->params);
}

Server::~Server(){
    
}

void Server::initRandomNumbers(){
    this->r_0 = rand()%m;
    this->r_1 = rand()%m;
    this->r_0_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    this->r_1_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    for(unsigned i=0; i<cipher_size; ++i){
        bootsSymEncrypt(&this->r_0_cipher[i], (r_0>>i)&1, key);
        bootsSymEncrypt(&this->r_1_cipher[i], (r_1>>i)&1, key);
    }
}