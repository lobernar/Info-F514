
#include "../headers/server.hpp"

Server::Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* key){
    this->params = params;
    this->key = key;
    this->enc_template_client = std::vector<LweSample*>(nslots);
    this->enc_sample_client = std::vector<LweSample*>(nslots);
    this->cloud_key = &key->cloud;
    this->tmp = new_gate_bootstrapping_ciphertext_array(max_bitsize, params);
    this->tmp_carry = new_gate_bootstrapping_ciphertext_array(1, params);
}

Server::~Server(){
    delete_gate_bootstrapping_secret_keyset(this->key);
    delete_gate_bootstrapping_parameters(this->params);
    delete_gate_bootstrapping_ciphertext_array(this->nslots, this->enc_b);
    delete_gate_bootstrapping_ciphertext_array(this->nslots, this->enc_y);
    delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_r0);
    delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_r1);
    delete_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->tmp);
    delete_gate_bootstrapping_ciphertext_array(1, this->tmp_carry);
    for (unsigned i = 0; i < this->nslots; ++i) {
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_template_client[i]);
        delete_gate_bootstrapping_ciphertext_array(this->bitsize, this->enc_sample_client[i]);
    }
}

void Server::setTemplate(std::vector<LweSample*> templ){
    this->enc_template_client = templ;
}

void Server::setSample(std::vector<LweSample*> sample){
    this->enc_sample_client = sample;
}

void Server::setMatchingResult(int token){
    this->y = token;
}

void Server::initTmp(){
    srand(time(NULL));  // Without this, the "random" numbers are always the same
    x1 = rand() % nslots;
    srand(time(NULL)*2);
    x2 = rand() % nslots;
    for (int i = 0; i < max_bitsize; ++i) {
        bootsCONSTANT(&tmp[i], 0, cloud_key);
    }
}

void Server::initAndEncRandomNumbers(){
    // Generate 2 random numbers
    srand(time(NULL));
    this->r0 = rand()%m;
    srand(time(NULL)*(this->r0+1));
    this->r1 = rand()%m;
    // Encrypting them
    this->enc_r0 = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
    this->enc_r1 = new_gate_bootstrapping_ciphertext_array(this->bitsize, this->params);
    for(unsigned i=0; i<this->bitsize; ++i){
        bootsSymEncrypt(&this->enc_r0[i], (this->r0>>i)&1, this->key);
        bootsSymEncrypt(&this->enc_r1[i], (this->r1>>i)&1, this->key);
    }
}

void Server::sendMatchingToken(Client& client){
    client.decryptMatchingResult(this->enc_y);
}

void Server::identifyUser(){
    // Accept user
    if(this->y == this->r1) this->id_token = 1;
    // Reject user
    else this->id_token = 0;
}

void Server::sendIdToken(Client& client){
    client.setIdToken(this->id_token);
}

void Server::nBitAddition(){
    initTmp(); // generate random x1 and x2 and initialize tmp
    tmp_result = 0;
    bootsADDNbit(tmp, enc_template_client[x1], enc_sample_client[x2], tmp_carry, bitsize, cloud_key);
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu << m);
    }
    printf("The result of the addition is %lu\n", tmp_result);
}

void Server::twosComplement(){
    initTmp();
    tmp_result = 0;
    bootsTwoSComplement(tmp, enc_sample_client[x1], bitsize, cloud_key);
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu << m);
    }
    printf("The result of the two's complement is %lu\n", tmp_result);
}

void Server::absoluteValue(){
    initTmp();
    tmp_result = 0;
    bootsABS(tmp, enc_sample_client[x1], bitsize, cloud_key);
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu << m);
    }
    printf("The result of the absolute value is %lu\n", tmp_result);
}

void Server::nBitSubstraction(){
    initTmp();
    tmp_result = 0;
    bootsSUBNbit(tmp, enc_template_client[x1], enc_sample_client[x2], bitsize, cloud_key);
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu << m);
    }
    printf("The result of the substraction is %lu\n", tmp_result);
}

void Server::nBitMultiplication(){
    initTmp();
    tmp_result = 0;
    bootsMultiply(tmp, enc_template_client[x1], enc_sample_client[x2], bitsize, cloud_key);
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu << m);
    }
    printf("The result of the multiplication is %lu\n", tmp_result);
}

void Server::manhattanDistance(){
    initTmp();
    tmp_result = 0;
    HE_ManhattanDistance(tmp, enc_template_client, enc_sample_client, bitsize, cloud_key);
    // Decrypt result
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu<<m);
    }
    printf("The result for Manhattan distance is %lu \n", tmp_result);
}

void Server::euclideanDistance(){
    initTmp();
    tmp_result = 0;
    HE_EuclideanDistance(tmp, enc_template_client, enc_sample_client, bitsize, cloud_key);
    // Decrypt result
    for (int m = 0; m < max_bitsize; ++m) {
        uint8_t dec_mu = bootsSymDecrypt(&tmp[m], key);
        tmp_result |= (dec_mu<<m);
    }
    printf("The result for Euclidean distance is %lu \n", tmp_result);
}

void Server::computeF(){
    LweSample* enc_bound_match = new_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->params);
    for (int i=0; i < this->max_bitsize; i++) {
        bootsSymEncrypt(&enc_bound_match[i], (bound_match_clear>>i)&1, this->key);
    }
    this->enc_b = new_gate_bootstrapping_ciphertext_array(this->max_bitsize, this->params);
    Function_f(this->enc_b, this->enc_template_client, this->enc_sample_client, enc_bound_match, this->bitsize, this->cloud_key);
    delete_gate_bootstrapping_ciphertext_array(bound_match_clear, enc_bound_match);
}

void Server::computeG(){
    this->enc_y = new_gate_bootstrapping_ciphertext_array(this->max_bitsize+1, params);
    Function_g(this->enc_y, this->enc_b, this->enc_r0, this->enc_r1, this->bitsize, this->cloud_key);
}

