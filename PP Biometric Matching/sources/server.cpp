#include "../headers/server.hpp"

Server::Server(TFheGateBootstrappingParameterSet* params, const TFheGateBootstrappingCloudKeySet* key){
    this->params = params;
    this->cloud_key = key;
    this->matching_result_cipher = new_gate_bootstrapping_ciphertext_array(this->size, this->params);
}

Server::~Server(){
    
}

void Server::setTemplate(LweSample* templ[]){
    this->template_client = templ;
}

void Server::setSample(LweSample* sample[]){
    this->sample_client = sample;
}

void Server::initRandomNumbers(){
    // Generate 2 random numbers
    this->r_0 = rand()%m;
    this->r_1 = rand()%m;
    // Encrypting them
    this->r_0_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    this->r_1_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    for(unsigned i=0; i<cipher_size; ++i){
        bootsSymEncrypt(&this->r_0_cipher[i], (r_0>>i)&1, key);
        bootsSymEncrypt(&this->r_1_cipher[i], (r_1>>i)&1, key);
    }
}

void Server::computeF(LweSample* result, LweSample* x[], LweSample* y[]){
    LweSample* match_lim_cipher = new_gate_bootstrapping_ciphertext_array(max_bitsize, params);
    for (int i=0; i < max_bitsize; i++) {
        bootsSymEncrypt(&match_lim_cipher[i], (this->match_lim>>i)&1, key);
    }
    f(result, x, y, match_lim_cipher, this->cipher_size, this->cloud_key);
}

void Server::computeG(LweSample* result, LweSample* b, LweSample* r0, LweSample* r1){
    g(result, b, r0, r1, this->cipher_size, this->cloud_key);
}

void Server::sendMatchingToken(Client& client){
    client.decryptMatchingResult(this->matching_result_cipher);
}

void Server::sendIdToken(Client& client){
    client.setIdToken(this->id_token);
}

void Server::setMatchingResult(int token){
    this->matching_result = token;
}

void Server::identifyUser(Client& client){
    // Accept user
    if(this->matching_result == this->r_1) this->id_token = 1;
    // Reject user
    else this->id_token = 0;
    this->sendIdToken(client);
}
