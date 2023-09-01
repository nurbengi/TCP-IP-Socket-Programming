#include "server.h"


    Server::Server(unsigned short port) : acceptor(io), socket(io) { //soket oluşturma ve bağlantı kabul etme işlemleri için kullanılır.
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port);
        acceptor.open(ep.protocol());
        acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor.bind(ep);
        acceptor.listen();
            std::cout << "Listening on port: " << port << std::endl;
        acceptor.accept(socket);
            std::cout << "Connected to client" << std::endl;
    }
        std::string Server::decimalToBinary(unsigned int decimal) {
        std::string binaryStr = std::bitset<16>(decimal).to_string();
        // std::bitset<16>(decimal): Ondalık sayıyı 16 bitlik ikili bir sayıya dönüştürür.
        return binaryStr.substr(binaryStr.size() - 16);
        }
   int Server::generateRandomInRange(int lower, int upper) {
    // generateRandomInRange adında bir fonksiyon oluşturuldu, verilen alt ve üst sınırlar arasında rastgele bir tamsayı üreticek.
    std::random_device rd;
    // std::random_device, rastgele sayı üretmek için donanım tabanlı bir rastgele sayı üreteci oluşturur.
    std::mt19937 gen(rd());
    // std::mt19937, Mersenne Twister algoritması kullanarak daha iyi rastgele sayılar üretmek için kullanılır.
    // Bu algoritma daha öngörülemez ve daha düşük korelasyona sahip rastgele sayılar üretir.
    std::uniform_int_distribution<int> distribution(lower, upper);
    // std::uniform_int_distribution, belirli bir aralıkta eşit olasılıkla dağılmış rastgele tamsayılar üretir.
    // lower ve upper, bu dağılımın üretebileceği tamsayı aralığını belirler.
    return distribution(gen);
    }
    void Server::process() {
        while (true) {
            std::string data = readData();
            std::istringstream ss(data);
            std::string header;
            ss >> header;
            if (header == "exit") {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            if (header[0] == 'b') {
                unsigned int num;
                
                try {
                    num = std::bitset<16>(header.substr(1)).to_ulong();
                } 
                catch (const std::invalid_argument&) {
                    std::cout << "Error Data" << std::endl;
                    continue;
                }
                std::string binaryStr = decimalToBinary(num);
                receivedNumbers.push_back(num);
                std::cout << "--> Binary: " << binaryStr << " Decimal: " << num << std::endl;
                int lower = ((num - 1) / 16) * 16 + 1;
                int upper = std::min(lower + 16, 65536);
                // Bir alt ve üst sınır belirlenir bu sınırlar rastgele üretilecek sayının aralığını belirleriz
                // Her 16 sayıda bir alt sınır arttırılır ve üst sınır 65536'ya sabitlenir
                int randomNum = generateRandomInRange(lower, upper);
                // generateRandomInRange fonksiyonu verilen alt ve üst sınırlar arasında rastgele bir tamsayı üretir
                std::string randomMsg = "--> random number: " + std::to_string(randomNum) + "\n";
                boost::asio::write(socket, boost::asio::buffer(randomMsg));
            } 
            else if (header == "decimal:") {
                unsigned int num;
                ss >> num;
                std::string binaryStr = decimalToBinary(num);
                receivedNumbers.push_back(num);
                std::cout << "--> Binary: " << binaryStr << " Decimal: " << num << std::endl;
                int lower = ((num - 1) / 16) * 16 + 1;
                int upper = std::min(lower + 16, 65536);
                std::string responseMsg;
                
                if (num >= 1 && num <= 512) {
                    int lowerRandom = (num - 1) / 16 * 16 + 1;
                    int upperRandom = lowerRandom + 16;

                    int randomNum = generateRandomInRange(lowerRandom, upperRandom);
                    responseMsg = "--> random number: " + std::to_string(randomNum) + "\n";
                } 
                else if (num > 512 && num <= 65536) {
                    responseMsg = "--> data received\n";
                } 
                else {
                    responseMsg = "--> invalid number range\n";
                }
                boost::asio::write(socket, boost::asio::buffer(responseMsg));
            }
        }
    }

        std::string Server::readData() {
        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n");
        std::string data = boost::asio::buffer_cast<const char*>(buf.data());
        
            if (!data.empty() && (data.back() == '\n')) {
                    data.pop_back();
            }
            return data;
    }

    void Server::run() {
        process();
    }

    int main() {
        unsigned short port_num = 3333;
        Server server(port_num);
        server.run();
        return 0;
    }
