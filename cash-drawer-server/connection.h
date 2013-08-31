#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

class Server;
enum CashDrawerState: unsigned short;

class Connection : public boost::enable_shared_from_this<Connection> {
public:
    Connection(boost::asio::io_service&, Server*);
    void read();
    void sendDrawerState(CashDrawerState state);
    boost::asio::ip::tcp::socket &getSocket() { return socket; }
  
private:
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf readData;
    Server *server;
    
    void write();
    void handleRead(const boost::system::error_code&, std::size_t);
    void handleWrite(const boost::system::error_code&);
    void close();
    void shutDownReceive();
    void continueReading(const boost::system::error_code&);
};

#endif
