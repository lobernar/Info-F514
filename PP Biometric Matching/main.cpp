/*
#To compile the code (after modifying .bashrc file):
#   g++ main.cpp -o main -ltfhe-spqlios-fma 
*/
#include <tfhe.h>
#include <tfhe_io.h>
#include <stdio.h>
#include "circuits.cpp"

int main(){
    const int sec_param = 110;  // Security parameter
    const unsigned size = 128;  // Biometric vector length
    const unsigned m = 256;  // plaintext space Z_m
    int template_client[size];  // Oroginal template
    int sample_true[size];  // Trustworthy user
    int sample_false[size];  // Untrustworthy user

    // Initialize biometric vectors
    for (unsigned i = 0; i < size; ++i) {
        //srand(time(NULL)*i^3);  //srand used to seed the random generator
        template_client[i] =  rand() %m-1;
        sample_true[i] = template_client[i] + 1;
        // srand(time(NULL)+i);
        sample_false[i] = rand() %m;
    }

    //Initialisation (generating parameters, key pairs, ...) 
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(sec_param);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);
    const TFheGateBootstrappingCloudKeySet* cloud_key = &key->cloud;

    // Encrypting
    LweSample* template_cipher[size];
    LweSample* sample_true_cipher[size];
    LweSample* sample_false_cipher[size];
    int cipher_size = 8;

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


    LweSample* result = new_gate_bootstrapping_ciphertext_array(size, params); // Result of circuit will be stored here

    /*
     * TODO:
     * Use some circuit on samples
    */


    // Generate random r_0 and r_1 and encrypt them
    unsigned r_0 = rand()%m;
    unsigned r_1 = rand()%m;
    LweSample* r_0_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    LweSample* r_1_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
    for(unsigned i=0; i<cipher_size; ++i){
        bootsSymEncrypt(&r_0_cipher[i], (r_0>>i)&1, key);
        bootsSymEncrypt(&r_1_cipher[i], (r_1>>i)&1, key);
    }

    /*
     * TODO: 
     * Use some circuit which uses r_0 and r_1 ciphers
    */


    // Free
    delete_gate_bootstrapping_secret_keyset(key);
    //delete_gate_bootstrapping_cloud_keyset(cloud_key);
    delete_gate_bootstrapping_parameters(params);
    delete_gate_bootstrapping_ciphertext_array(size, result);
    delete_gate_bootstrapping_ciphertext_array(cipher_size, r_0_cipher);
    delete_gate_bootstrapping_ciphertext_array(cipher_size, r_1_cipher);
    for (int i = 0; i < size; ++i) {
        delete_gate_bootstrapping_ciphertext_array(cipher_size, template_cipher[i]);
        delete_gate_bootstrapping_ciphertext_array(cipher_size, sample_true_cipher[i]);
        delete_gate_bootstrapping_ciphertext_array(cipher_size, sample_false_cipher[i]);
    }
    
    return 0;
}