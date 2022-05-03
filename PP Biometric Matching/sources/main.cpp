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
    // computation(client, server, false); // Launch protocol with false sample
}

void precomputation(Client& client, Server& server) {

    // -- client precomputation -- //
    client.initTemplate(); // Generate template, true sample and false sample
    client.encryptTemplate(); // Encrypting the previously generated vectors

    // -- sending template to server -- //
    client.sendTemplate(server);
}

void computation(Client& client, Server& server, bool sample) {
    // -- client encryption -- //
    client.initSamples();
    client.encryptSamples();

    // -- sending sample : client -> server -- //
    if(sample)
        client.sendTrueSample(server);
    else
        client.sendFalseSample(server);


    // // -- server computations -- //
    std::cout << "Pay respects 0" << std::endl;
    server.computeF();
    std::cout << "Pay respects 1" << std::endl;
    server.initAndEncRandomNumbers();
    std::cout << "Pay respects 2" << std::endl;
    server.computeG();

    // // sending y' : server -> client
    std::cout << "Pay respects 3" << std::endl;
    server.sendMatchingToken(client);
    // // sending y : client -> server
    std::cout << "Pay respects 4" << std::endl;
    client.sendDecToken(server);
    // // sending r : server -> client
    std::cout << "Pay respects 5" << std::endl;
    server.identifyUser();
    std::cout << "Pay respects 6" << std::endl;
    server.sendIdToken(client);
}

int main() {
    std::cout << "START" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    Client* client = new Client();
    Server* server = new Server(client->getParams(), client->getSecretKey());
    protocol(*client, *server);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[µs]" << std::endl;
    std::cout << "END" << std::endl;
}