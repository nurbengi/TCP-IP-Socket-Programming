#include "client.h"

    Client::Client(unsigned short port_num) : socket(io_service) {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::loopback(), port_num);
        socket.connect(ep);
    }
    bool Client::processInput(const std::string& input) {
        try {
            unsigned int num;
            
             if (input[0] == 'b') {
                    num = std::bitset<16>(input.substr(1)).to_ulong();
             } 
             else {
                    num = std::stoul(input);
            if (num < 0 || num > 65536) {
                    std::cout << "You entered an incorrect number. Please enter a number between 0-65536." << std::endl;
        
                return false; // Yanlış girişi işaretle ve işlemi sonlandır
            }
             }

        std::string msg;
        
            if (num > 0) {
                    msg = "decimal: " + std::to_string(num) + "\n";
                    sendData(msg);
             }

        readData(); // Random sayıları okuma işlemi
        return true;
        } 
        catch (const std::invalid_argument& ex) {
            std::cout << "Hatalı giriş. Lütfen sayı giriniz." << std::endl;
            return false;
        }
    }

    void Client::sendData(const std::string& data) {
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(data), error);
   
         if (error) {
             std::cout << "Sending data failed: " << error.message() << std::endl;
        }
    }

    void Client::readData() {
        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n");
        std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    if (!data.empty() && (data.back() == '\n')) {
         data.pop_back();
    }
         std::cout << data << std::endl;
    }

    void Client::run() {
    std::cout << "Hello! Please enter a number between 0-65536. You can type 'exit' to exit." << std::endl;
            while (true) {
                std::string input;
                std::cout << "Data entry ('exit' to exit the function): ";
                std::getline(std::cin, input);
                if (input == "exit") {
                    sendData("exit\n");
                    break;
                } 
                    else {
                        processInput(input);
                    }
                }
    }

    int main() {
        unsigned short port_num = 3333;
        Client client(port_num);
        client.run();
        
        return 0;
    }