#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>
#include <chrono>

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
    //computation(client, server, false); // Launch protocol with false sample
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

    // // sending sample : client -> server
    if(sample)
        client.sendTrueSample(server);
    else
        client.sendFalseSample(server);


    // // server computations
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
    std::cout << "SUSHI" << std::endl;
}

int main() {
    std::cout << "START" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    Client* client = new Client(); std::cout << "ramen" << std::endl;
    Server* server = new Server(client->getParams(), client->getSecretKey()); std::cout << "gyoza" << std::endl;
    protocol(*client, *server);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;
    std::cout << "END" << std::endl;
}