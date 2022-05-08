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


    // -- server computations -- //
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    server.nBitAddition();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Addition done in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    begin = std::chrono::steady_clock::now();
    server.twosComplement();
    end = std::chrono::steady_clock::now();
    std::cout << "Two's complement done in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    begin = std::chrono::steady_clock::now();
    server.absoluteValue();
    end = std::chrono::steady_clock::now();
    std::cout << "Absolute value done in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    server.nBitSubstraction();
    end = std::chrono::steady_clock::now();
    std::cout << "Subtraction done in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    begin = std::chrono::steady_clock::now();
    server.nBitMultiplication();
    end = std::chrono::steady_clock::now();
    std::cout << "Multiplication done in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    begin = std::chrono::steady_clock::now();
    server.computeF();
    end = std::chrono::steady_clock::now();
    std::cout << "Function f computed in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    server.initAndEncRandomNumbers();
    begin = std::chrono::steady_clock::now();
    server.computeG();
    end = std::chrono::steady_clock::now();
    std::cout << "Function g computed in " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;

    // // sending y' : server -> client
    std::cout << "Sending encrypted y to client" << std::endl;
    server.sendMatchingToken(client);
    // // sending y : client -> server
    std::cout << "Sending decrypted y to server" << std::endl;
    client.sendDecToken(server);
    // // sending r : server -> client
    std::cout << "Verifying identiy of user" << std::endl;
    server.identifyUser();
    std::cout << "Sending ID token to client" << std::endl;
    server.sendIdToken(client);
}

int main() {
    std::cout << "START" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    Client* client = new Client();
    Server* server = new Server(client->getParams(), client->getSecretKey());
    protocol(*client, *server);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds> (end - begin).count() << "[s]" << std::endl;
    std::cout << "END" << std::endl;
}