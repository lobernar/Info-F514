/*
#To compile the code (after modifying .bashrc file):
#   g++ your-program.cpp -o your-program -ltfhe-spqlios-fma 
*/
#include <tfhe.h>
#include <tfhe_io.h>
#include <stdio.h>
#include <math.h>
#include "circuits.cpp"

int euclidean_dist(int x[], int y[], int size){
    unsigned sum = 0;
    for(int i=0; i<size; ++i) sum = sum +  pow(y[i]-x[i], 2);
    return sqrt(sum);
}

bool matching(int sample[], int templt[], int size, unsigned B){
    int dist = euclidean_dist(templt, sample, size);
    if(pow(dist, 2) <= B) return 1;  // Match
    return 0;  // No match
}

int g(int b, int r_0, int r_1){
    return (1-b)*r_0 + b*r_1;
}


int main(){
    int size = 128; // Biometric vector length
    FILE* could_key = fopen("cloud.key", "rb");
    TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(could_key);
    fclose(could_key);
    const TFheGateBootstrappingParameterSet* params = bk->params;

    // Read template and sample (both are ciphertexts)
    LweSample* templt = new_gate_bootstrapping_ciphertext_array(size, params);
    LweSample* sample = new_gate_bootstrapping_ciphertext_array(size, params);

    /*
     * TODO: initial template
     * For now, assume the initial template is already stored on the server
     */
    // Read template from file
    FILE* cloud_template = fopen("cloud.template", "rb");
    for(int i=0; i<size; ++i) import_gate_bootstrapping_ciphertext_fromFile(cloud_template, &templt[i], params);
    fclose(cloud_template);

    // Read sample from file
    FILE* cloud_data = fopen("cloud.data", "rb");
    for(int i=0; i<size; ++i) import_gate_bootstrapping_ciphertext_fromFile(cloud_data, &sample[i], params);
    fclose(cloud_data);

    
    LweSample* result = new_gate_bootstrapping_ciphertext_array(size, params); // Result of circuit will be stored here

    /*
     * TODO: implemenmt circuit
     */



    //export the resulting ciphertext to a file (for the cloud)
    FILE* answer_data = fopen("answer.data","wb");
    for (int i=0; i<size; i++) export_gate_bootstrapping_ciphertext_toFile(answer_data, &result[i], params);
    fclose(answer_data);


    //clean up all pointers
    delete_gate_bootstrapping_ciphertext_array(size, result);
    delete_gate_bootstrapping_ciphertext_array(size, templt);
    delete_gate_bootstrapping_ciphertext_array(size, sample);
    delete_gate_bootstrapping_cloud_keyset(bk);

    return 0;
}