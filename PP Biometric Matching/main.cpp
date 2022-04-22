/*
#To compile the code (after modifying .bashrc file):
#   g++ main.cpp -o main -ltfhe-spqlios-fma 
*/
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <stdio.h>

#include "circuits.cpp"

#include "client.hpp"
#include "server.hpp"

// -- Declarat
void protocol();
void computation ();
void precomputation();

void protocol() {
    precomputation();
    computation();
}

void precomputation() {
    // client precomputation

    // sending message to server
}

void computation () {
    // client encryption

    // sending template : client -> server 

    // server stuff

    // sending y' : server -> client

    // sending y : client -> server

    // sending r : server -> client
}

int main() {
    
}


// int temp(){

//     LweSample* result = new_gate_bootstrapping_ciphertext_array(size, params); // Result of circuit will be stored here

//     // TODO : Use some circuit on samples

//     // Generate random r_0 and r_1 and encrypt them
//     unsigned r_0 = rand()%m;
//     unsigned r_1 = rand()%m;
//     LweSample* r_0_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
//     LweSample* r_1_cipher = new_gate_bootstrapping_ciphertext_array(cipher_size, params);
//     for(unsigned i=0; i<cipher_size; ++i){
//         bootsSymEncrypt(&r_0_cipher[i], (r_0>>i)&1, key);
//         bootsSymEncrypt(&r_1_cipher[i], (r_1>>i)&1, key);
//     }

//     // TODO: Use some circuit which uses r_0 and r_1 ciphers

//     // Free
//     delete_gate_bootstrapping_secret_keyset(key); // client + server
//     delete_gate_bootstrapping_parameters(params);  // client + server
//     delete_gate_bootstrapping_ciphertext_array(size, result); // server
//     delete_gate_bootstrapping_ciphertext_array(cipher_size, r_0_cipher); //server
//     delete_gate_bootstrapping_ciphertext_array(cipher_size, r_1_cipher);  //server
//     //Client
//     for (int i = 0; i < size; ++i) {
//         delete_gate_bootstrapping_ciphertext_array(cipher_size, template_cipher[i]);
//         delete_gate_bootstrapping_ciphertext_array(cipher_size, sample_true_cipher[i]);
//         delete_gate_bootstrapping_ciphertext_array(cipher_size, sample_false_cipher[i]);
//     }
    
//     return 0;
// }
