#include "server.h"
#include "connection.h"
#include <boost/bind.hpp>
#include <iostream>

Server::Server(const int port) : acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

void Server::start() {
    listen();
    ioService.run();
}

void Server::listen() {
    accept();
}

void Server::accept() {
    if (newConnection) newConnection->read();
    newConnection.reset(new Connection(ioService, this));
    acceptor.async_accept(newConnection->getSocket(), boost::bind(&Server::handleAccept, this));
}

void Server::handleAccept() {
    connections.insert(newConnection);
    accept();
}

void Server::closedConnection(connectionPtr connection) {
    connections.erase(connection);
}

void Server::openDrawer() {
    std::cout << "open" << std::endl;
}

void Server::identifyDrawer() {
    std::cout << "identify" << std::endl;
}
