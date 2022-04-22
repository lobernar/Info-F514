#include "../headers/client.hpp"

Client::Client(){
    this->template_client = new int[size];
    this->sample_false = new int[size];
    this->sample_true = new int[size];
    this->params = new_default_gate_bootstrapping_parameters(sec_param);
    this-> key = new_random_gate_bootstrapping_secret_keyset(params);
    this->cloud_key = &key->cloud;
}

Client::~Client(){
    delete this->template_client;
    delete this->sample_false;
    delete this->sample_true;
}

void Client::encryptVectors() {
    for(unsigned i=0; i<size; ++i){
    template_cipher[i] = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    sample_true_cipher[i] = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    sample_false_cipher[i] = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    for(unsigned j=0; j<cipher_size; ++j){
        bootsSymEncrypt(&template_cipher[i][j], (template_client[i]>>i)&1, key);
        bootsSymEncrypt(&sample_true_cipher[i][j], (sample_true[i]>>i)&1, key);
        bootsSymEncrypt(&sample_false_cipher[i][j], (sample_false[i]>>i)&1, key);
    }
}

}

void Client::initVectors() {
   // Initialize biometric vectors
    for (unsigned i = 0; i < size; ++i) {
        //srand(time(NULL)*i^3);  //srand used to seed the random generator
        template_client[i] =  rand() %m-1;
        sample_true[i] = template_client[i] + 1;
        // srand(time(NULL)+i);
        sample_false[i] = rand() %m;
    } 
}