#include "server.hpp"

Server::Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* sec_key){
    this->params = params;
    this->key = sec_key;
}

Server::~Server(){
    
}