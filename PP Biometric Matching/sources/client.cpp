#include "../headers/client.hpp"

Client::Client(){
    this->params = new_default_gate_bootstrapping_parameters(minimum_lambda);
    this->key = new_random_gate_bootstrapping_secret_keyset(params);
    this->template_client = std::vector<uint8_t>(this->nslots);
    this->sample_client_false = std::vector<uint8_t>(this->nslots);
    this->sample_client_true = std::vector<uint8_t>(this->nslots);
    this->enc_template_client = std::vector<LweSample*>(nslots);
    this->enc_sample_client_false = std::vector<LweSample*>(nslots);
    this->enc_sample_client_true = std::vector<LweSample*>(nslots);
    this->cloud_key = &key->cloud;
}

Client::~Client(){
    for (unsigned i = 0; i < this->nslots; ++i) {
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_template_client[i]);
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_sample_client_true[i]);
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_sample_client_false[i]);
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
    for (unsigned i = 0; i < this->nslots; ++i) {
        srand(time(NULL)*i^3);  //srand used to seed the random generator
        this->template_client[i] =  rand() %this->m-1;
    }
}

void Client::initSamples(){
    for (unsigned i = 0; i < this->nslots; ++i) {
        srand(time(NULL)*i^3);  //srand used to seed the random generator
        this->sample_client_true[i] = this->template_client[i] + 1;
        srand(time(NULL)+i);
        this->sample_client_false[i] = rand() % this->m;
    }
}

void Client::encryptTemplate(){
    for(unsigned i=0; i<this->nslots; ++i){
        this->enc_template_client[i] = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
        for(unsigned j=0; j<this->bitsize; ++j){
            bootsSymEncrypt(&this->enc_template_client[i][j], (this->template_client[i]>>j)&1, this->key);
        }
    }
}

void Client::encryptSamples(){
    for(unsigned i=0; i<this->nslots; ++i){
        this->enc_sample_client_true[i] = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
        this->enc_sample_client_false[i] = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
        for(unsigned j=0; j<this->bitsize; ++j){
            bootsSymEncrypt(&this->enc_sample_client_true[i][j], (this->sample_client_true[i]>>j)&1, this->key);
            bootsSymEncrypt(&this->enc_sample_client_false[i][j], (this->sample_client_false[i]>>j)&1, this->key);
        }
    }  

}

void Client::decryptMatchingResult(LweSample* token){
    for (int i=0; i<this->bitsize; i++) {
        int ai = bootsSymDecrypt(&token[i], this->key);
        this->y |= (ai<<i);
    }
}

void Client::sendTemplate(Server& server){
    server.setTemplate(this->enc_template_client);
}

void Client::sendTrueSample(Server& server){
    server.setSample(this->enc_sample_client_true);
}

void Client::sendFalseSample(Server& server){
    server.setSample(this->enc_sample_client_false);
}

void Client::sendDecToken(Server& server){
    server.setMatchingResult(this->y);
}