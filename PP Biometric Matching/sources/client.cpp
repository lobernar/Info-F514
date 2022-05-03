#include "../headers/client.hpp"

Client::Client(){
    this->template_client = new int[size];
    this->sample_false = new int[size];
    this->sample_true = new int[size];
    this->template_cipher = new LweSample* [this->size];
    this->sample_true_cipher = new LweSample* [this->size];
    this->sample_false_cipher = new LweSample* [this->size];
    this->params = new_default_gate_bootstrapping_parameters(sec_param);
    this->key = new_random_gate_bootstrapping_secret_keyset(params);
    this->cloud_key = &key->cloud;
}

Client::~Client(){
    delete this->template_client;
    delete this->sample_false;
    delete this->sample_true;
    for (unsigned i = 0; i < this->size; ++i) {
        delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->template_cipher[i]);
        delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->sample_true_cipher[i]);
        delete_gate_bootstrapping_ciphertext_array(this->cipher_size, this->sample_false_cipher[i]);
    }
    delete_gate_bootstrapping_secret_keyset(this->key);
    delete_gate_bootstrapping_parameters(this->params); 
}

const TFheGateBootstrappingCloudKeySet* Client::getCloudKey(){ return this->cloud_key;}

TFheGateBootstrappingSecretKeySet* Client::getSecretKey() {return this->key;}

TFheGateBootstrappingParameterSet* Client::getParams() {return this->params;}

void Client::setIdToken(int token){
    this->id_token = token;
}

void Client::initTemplate(){
    for (unsigned i = 0; i < this->size; ++i) {
        //srand(time(NULL)*i^3);  //srand used to seed the random generator
        this->template_client[i] =  rand() %this->m-1;
    }
}

void Client::initSamples(){
    for (unsigned i = 0; i < this->size; ++i) {
        //srand(time(NULL)*i^3);  //srand used to seed the random generator
        this->sample_true[i] = this->template_client[i] + 1;
        // srand(time(NULL)+i);
        this->sample_false[i] = rand() % this->m;
    }
}

void Client::encryptTemplate(){
    for(unsigned i=0; i<this->size; ++i){
        this->template_cipher[i] = new_gate_bootstrapping_ciphertext_array(this->cipher_size, this->params);
        for(unsigned j=0; j<this->cipher_size; ++j){
            bootsSymEncrypt(&this->template_cipher[i][j], (this->template_client[i]>>i)&1, this->key);
        }
    }
}

void Client::encryptSamples(){
    for(unsigned i=0; i<this->size; ++i){
        this->sample_true_cipher[i] = new_gate_bootstrapping_ciphertext_array(this->cipher_size, this->params);
        this->sample_false_cipher[i] = new_gate_bootstrapping_ciphertext_array(this->cipher_size, this->params);
        for(unsigned j=0; j<this->cipher_size; ++j){
            bootsSymEncrypt(&this->sample_true_cipher[i][j], (this->sample_true[i]>>i)&1, this->key);
            bootsSymEncrypt(&this->sample_false_cipher[i][j], (this->sample_false[i]>>i)&1, this->key);
        }
    }  

}

void Client::decryptMatchingResult(LweSample* token){
    for (int i=0; i<this->cipher_size; i++) {
        int ai = bootsSymDecrypt(&token[i], this->key);
        this->matching_result |= (ai<<i);
    }
}

void Client::sendTemplate(Server& server){
    server.setTemplate(this->template_cipher);
}

void Client::sendTrueSample(Server& server){
    server.setSample(this->sample_true_cipher);
}

void Client::sendFalseSample(Server& server){
    server.setSample(this->sample_false_cipher);
}

void Client::sendDecToken(Server& server){
    server.setMatchingResult(this->matching_result);
}