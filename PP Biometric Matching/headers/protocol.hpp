#ifndef __Protocol_HPP__
#define __Protocol_HPP__
#include <iostream>
#include "server.hpp"
#include "client.hpp"
class Protocol
{
private:
    /* data */
    Client* client;
    Server* server;
public:
    Protocol();
    ~Protocol();

    void precomputation();
    void computation();
};


#endif