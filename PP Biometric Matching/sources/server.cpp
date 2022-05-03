
#include "../headers/server.hpp"

Server::Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* key){
    this->params = params;
    this->key = key;
    this->enc_template_client = std::vector<LweSample*>(nslots);
    this->sample_client = std::vector<LweSample*>(nslots);
    this->cloud_key = &key->cloud;
}

Server::~Server(){
    delete_gate_bootstrapping_secret_keyset(this->key);
    delete_gate_bootstrapping_parameters(this->params);
    delete_gate_bootstrapping_ciphertext_array(this->nslots, this->enc_b);
    delete_gate_bootstrapping_ciphertext_array(this->nslots, this->enc_y);
    delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_r0);
    delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_r1);
    for (unsigned i = 0; i < this->nslots; ++i) {
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_template_client[i]);
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->sample_client[i]);
    }
}

void Server::setTemplate(std::vector<LweSample*> templ){
    this->enc_template_client = templ;
}

void Server::setSample(std::vector<LweSample*> sample){
    this->sample_client = sample;
}

void Server::setMatchingResult(int token){
    this->matching_result = token;
}

void Server::initAndEncRandomNumbers(){
    // Generate 2 random numbers
    this->r0 = rand()%m;
    this->r1 = rand()%m;
    // Encrypting them
    this->enc_r0 = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
    this->enc_r1 = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
    for(unsigned i=0; i<this->bitsize; ++i){
        bootsSymEncrypt(&this->enc_r0[i], (this->r0>>i)&1, this->key);
        bootsSymEncrypt(&this->enc_r1[i], (this->r1>>i)&1, this->key);
    }
}

void Server::computeF(){
    LweSample* enc_bound_match = new_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->params);
    for (int i=0; i < this->max_bitsize; i++) {
        bootsSymEncrypt(&enc_bound_match[i], (bound_match_clear>>i)&1, this->key);
    }
    this->enc_b = new_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->params);
    Function_f(this->enc_b, this->enc_template_client, this->sample_client, enc_bound_match, this->bitsize, this->cloud_key);
    delete_gate_bootstrapping_ciphertext_array(bound_match_clear, enc_bound_match);
}

void Server::computeG(){
    this->enc_y = new_gate_bootstrapping_ciphertext_array(this->max_bitsize+1, params);
    Function_g(this->enc_y, this->enc_b, this->enc_r0, this->enc_r1, this->bitsize, this->cloud_key);
}

void Server::sendMatchingToken(Client& client){
    client.decryptMatchingResult(this->enc_y);
}

void Server::identifyUser(){
    // Accept user
    if(this->matching_result == this->r1) this->id_token = 1;
    // Reject user
    else this->id_token = 0;
}

void Server::sendIdToken(Client& client){
    client.setIdToken(this->id_token);
}
