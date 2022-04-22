#include "../headers/protocol.hpp"

Protocol::Protocol(/* args */)
{
    this->client = new Client();
    this->server = new Server();
}

Protocol::~Protocol()
{
}

void Protocol::precomputation() {

    // client precomputation
    client->keyGeneration();
    client->templateEncryption();

    // sending message to server
    
}

void Protocol::computation() {
    // client encryption

    // sending template : client -> server 

    // server stuff

    // sending y' : server -> client

    // sending y : client -> server

    // sending r : server -> client
    
}