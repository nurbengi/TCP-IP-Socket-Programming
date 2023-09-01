#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <random>
#include <bitset>
#include <iostream>
#include <sstream>

class Server {
public:
Server(unsigned short port);
void run();

private:
boost::asio::io_service io;
boost::asio::ip::tcp::acceptor acceptor;
boost::asio::ip::tcp::socket socket;
std::vector<unsigned int> receivedNumbers;

std::string decimalToBinary(unsigned int decimal);
int generateRandomInRange(int lower, int upper);
void process();
std::string readData();

};

#endif // SERVER_H
