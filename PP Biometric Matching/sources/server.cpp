
#include "../headers/server.hpp"

Server::Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* key){
    this->params = params;
    this->key = key;
    this->template_client = new LweSample* [this->size];
    this->sample_client = new LweSample* [this->size];
    this->cloud_key = &key->cloud;
}

Server::~Server(){
    delete_gate_bootstrapping_secret_keyset(this->key);
    delete_gate_bootstrapping_parameters(this->params);
    delete_gate_bootstrapping_ciphertext_array(this->size, this->result_f);
    delete_gate_bootstrapping_ciphertext_array(this->size, this->result_g);
    delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->r_0_cipher);
    delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->r_1_cipher);
    for (unsigned i = 0; i < this->size; ++i) {
        delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->template_client[i]);
        delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->sample_client[i]);
    }
}

void Server::setTemplate(LweSample** templ){
    this->template_client = templ;
}

void Server::setSample(LweSample** sample){
    this->sample_client = sample;
}

void Server::setMatchingResult(int token){
    this->matching_result = token;
}

void Server::initAndEncRandomNumbers(){
    // Generate 2 random numbers
    this->r_0 = rand()%m;
    this->r_1 = rand()%m;
    // Encrypting them
    this->r_0_cipher = new_gate_bootstrapping_ciphertext_array(this->cipher_size, this->params);
    this->r_1_cipher = new_gate_bootstrapping_ciphertext_array(this->cipher_size, this->params);
    for(unsigned i=0; i<this->cipher_size; ++i){
        bootsSymEncrypt(&this->r_0_cipher[i], (this->r_0>>i)&1, this->key);
        bootsSymEncrypt(&this->r_1_cipher[i], (this->r_1>>i)&1, this->key);
    }
}

void Server::computeF(){
    LweSample* match_lim_cipher = new_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->params);
    for (int i=0; i < this->max_bitsize; i++) {
        bootsSymEncrypt(&match_lim_cipher[i], (match_lim>>i)&1, this->key);
    }
    this->result_f = new_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->params);
    f(this->result_f, this->template_client, this->sample_client, match_lim_cipher, this->cipher_size, this->cloud_key);
    delete_gate_bootstrapping_ciphertext_array(this->match_lim, match_lim_cipher);
}

void Server::computeG(){
    this->result_g = new_gate_bootstrapping_ciphertext_array(this->max_bitsize+1, params);
    g(this->result_g, this->result_f, this->r_0_cipher, this->r_1_cipher, this->cipher_size, this->cloud_key);
}

void Server::sendMatchingToken(Client& client){
    client.decryptMatchingResult(this->result_g);
}

void Server::identifyUser(){
    // Accept user
    if(this->matching_result == this->r_1) this->id_token = 1;
    // Reject user
    else this->id_token = 0;
}

void Server::sendIdToken(Client& client){
    client.setIdToken(this->id_token);
}
