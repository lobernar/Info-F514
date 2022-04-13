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
    int template_client[size];  // Oroginal template
    int sample_true[size];  // Trustworthy user
    int sample_false[size];  // Untrustworthy user

    // Initialize biometric vectors
    for (int i = 0; i < size; ++i) {
        //srand(time(NULL)*i^3);  //srand used to seed the random generator
        template_client[i] =  rand() %255;
        sample_true[i] = template_client[i] + 1;
        // srand(time(NULL)+i);
        sample_false[i] = rand() %256;
    }

    //Initialisation (generating parameters, key pairs, ...) 
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(sec_param);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);

    /*
     * TODO: Encrypt sample
    */
    // Encrypting
    LweSample* ciphertext = new_gate_bootstrapping_ciphertext_array(size, params);
    for(unsigned i=0; i<size; ++i){
        bootsSymEncrypt(&ciphertext[i], (template_client[i]>>i)&1, key);
    }


    LweSample* result = new_gate_bootstrapping_ciphertext_array(size, params); // Result of circuit will be stored here

    /*
     * Use some circuit
    */


    // Generate random r_0 and r_1
    unsigned r_0 = rand()%256;
    unsigned r_1 = rand()%256;

    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);
    delete_gate_bootstrapping_ciphertext_array(size, ciphertext);
    delete_gate_bootstrapping_ciphertext_array(size, result);
    
    return 0;
}