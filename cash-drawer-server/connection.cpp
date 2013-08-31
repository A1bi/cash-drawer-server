#include "connection.h"
#include "server.h"
#include <boost/bind.hpp>

#define COMMAND_PACKET_SIZE 1

Connection::Connection(boost::asio::io_service &io_service, Server *s) : socket(io_service), server(s), readData(COMMAND_PACKET_SIZE) {}

void Connection::read() {
    boost::asio::async_read(socket, readData, boost::bind(&Connection::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::handleRead(const boost::system::error_code &error, std::size_t bytes) {
    if (!error) {
        std::stringstream data;
        data << &readData;
        char command;
        data.get(command);
        switch (command) {
            case CashDrawerCommandActionIdentify:
                server->identifyDrawer();
                break;
                
            case CashDrawerCommandActionOpen:
                server->openDrawer();
                break;
        }
    }
    
    continueReading(error);
}

void Connection::sendDrawerState(CashDrawerState state) {
    std::stringstream data;
    data << (unsigned short)CashDrawerEventStateChanged;
    data << (unsigned short)state;
    
    shutDownReceive();
    boost::asio::async_write(socket, boost::asio::buffer(data.str()), boost::bind(&Connection::handleWrite, this, boost::asio::placeholders::error));
}

void Connection::handleWrite(const boost::system::error_code &error) {
    continueReading(error);
}

void Connection::close() {
    shutDownReceive();
    socket.close();
    server->closedConnection(shared_from_this());
}

void Connection::shutDownReceive() {
    try {
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive);
    } catch (...) {}
}

void Connection::continueReading(const boost::system::error_code &error) {
    if (!error) {
        read();
    } else {
        close();
    }
}
