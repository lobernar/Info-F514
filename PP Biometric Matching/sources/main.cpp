/*
#To compile the code (after modifying .bashrc file):
#   g++ main.cpp -o main -ltfhe-spqlios-fma 
*/
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>

#include "circuits.cpp"

#include "../headers/client.hpp"
#include "../headers/server.hpp"

// -- Declarations -- //
void protocol(Client& client, Server& server);
void computation (Client& client, Server& server, bool sample);
void precomputation(Client& client, Server& server);
// -----------------  //

void protocol(Client& client, Server& server) {
    precomputation(client, server);
    computation(client, server, true); // Launch protocol with true sample
    computation(client, server, false); // Launch protocol with false sample
}

void precomputation(Client& client, Server& server) {
    // client precomputation
    client.initTemplate(); // Generate template, true sample and false sample
    client.encryptTemplate(); // Encrypting the previously generated vectors

    // sending template to server
    client.sendTemplate(server);
}

void computation(Client& client, Server& server, bool sample) {
    // client encryption
    client.initSamples();
    client.encryptSamples();

    // sending sample : client -> server
    if(sample) {
        client.sendTrueSample(server);

    }else{
        client.sendFalseSample(server);
    }

    // server computations
    server.computeF();
    server.initAndEncRandomNumbers();
    server.computeG();

    // sending y' : server -> client
    server.sendMatchingToken(client);
    // sending y : client -> server
    client.sendDecToken(server);
    // sending r : server -> client
    server.identifyUser();
    server.sendIdToken(client);
}

int main() {
    Client* client = new Client();
    Server* server = new Server(client->getParams(), client->getCloudKey());
    protocol(*client, *server);
}