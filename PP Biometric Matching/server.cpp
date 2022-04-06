/*
#To compile the code (after modifying .bashrc file):
#   g++ your-program.cpp -o your-program -ltfhe-spqlios-fma 
*/
#include <tfhe.h>
#include <tfhe_io.h>
#include <stdio.h>
#include <math.h>

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
    FILE* could_key = fopen("cloud.key", "rb");
    TFheGateBootstrappingCloudKeySet* bk = new_tfheGateBootstrappingCloudKeySet_fromFile(could_key);
    fclose(could_key);
    const TFheGateBootstrappingParameterSet* params = bk->params;


    return 0;
}