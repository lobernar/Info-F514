#ifndef _Server_HPP
#define _Server_HPP
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <iostream>

class Server
{
private:
    /* data */
    TFheGateBootstrappingParameterSet* params;
    TFheGateBootstrappingSecretKeySet* key;
    const TFheGateBootstrappingCloudKeySet* cloud_key;
public:
    //Server(TFheGateBootstrappingParameterSet* params, TFheGateBootstrappingSecretKeySet* sec_key);
    Server();
    ~Server();
};

#endif