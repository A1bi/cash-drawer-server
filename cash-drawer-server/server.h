#ifndef _SERVER_H_
#define _SERVER_H_

#include <set>
#include <boost/asio.hpp>

class Connection;

typedef boost::shared_ptr<Connection> connectionPtr;
enum CashDrawerCommandAction: unsigned short {
    CashDrawerCommandActionIdentify,
    CashDrawerCommandActionOpen
};
enum CashDrawerState: unsigned short {
    CashDrawerStateClosed,
    CashDrawerStateOpened
};
enum CashDrawerEvent: unsigned short {
    CashDrawerEventStateChanged
};

class Server {
public:
    Server(const int port);
    void start();
    void closedConnection(connectionPtr);
    void openDrawer();
    void identifyDrawer();
    boost::asio::io_service *getIO() {
        return &ioService;
    }
    
private:
    boost::asio::io_service ioService;
    boost::asio::ip::tcp::acceptor acceptor;
    connectionPtr newConnection;
    std::set<connectionPtr> connections;
    
    void listen();
    void accept();
    void handleAccept();
};

#endif
