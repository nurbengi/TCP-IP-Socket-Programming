#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <random>
#include <bitset>
#include <iostream>
#include <sstream>

class Client {
public:
Client(unsigned short port_num);
void run();

private:
boost::asio::io_service io_service;
boost::asio::ip::tcp::socket socket;

bool processInput(const std::string& input);
void sendData(const std::string& data);
void readData();

};

#endif // CLIENT_H