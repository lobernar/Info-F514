/*
#To compile the code (after modifying .bashrc file):
#   g++ your-program.cpp -o your-program -ltfhe-spqlios-fma 
*/
#include <tfhe.h>
#include <tfhe_io.h>
#include <stdio.h>

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

    /*
     * TODO: Encrypt template and sample
    */


    //Initialisation (generating parameters, key pairs, ...) 
    TFheGateBootstrappingParameterSet* params = new_default_gate_bootstrapping_parameters(sec_param);
    TFheGateBootstrappingSecretKeySet* key = new_random_gate_bootstrapping_secret_keyset(params);
    FILE* sec_key = fopen("secret.key", "wb");
    export_tfheGateBootstrappingSecretKeySet_toFile(sec_key, key);
    fclose(sec_key);
    FILE* cloud_key = fopen("cloud.key", "wb");
    export_tfheGateBootstrappingCloudKeySet_toFile(cloud_key, &key->cloud);
    fclose(cloud_key);

    // Encrypting
    LweSample* ciphertext = new_gate_bootstrapping_ciphertext_array(size, params);
    for(unsigned i=0; i<size; ++i){
        bootsSymEncrypt(&ciphertext[i], (template_client[i]>>i)&1, key);
    }

    // Exporting sample
    FILE* cloud_data = fopen("cloud.data", "wb");
    for(unsigned i=0; i<size; i++){
        export_gate_bootstrapping_ciphertext_toFile(cloud_data, &ciphertext[i], params);
    }
    fclose(cloud_data);

    delete_gate_bootstrapping_secret_keyset(key);
    delete_gate_bootstrapping_parameters(params);
    delete_gate_bootstrapping_ciphertext_array(size, ciphertext);
    
    return 0;
}