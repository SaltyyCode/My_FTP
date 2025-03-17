/*
** EPITECH PROJECT, 2025
** FTPServer.hpp
** File description:
** FTP Server for FTP
*/


#ifndef FTP_SERVER_HPP
#define FTP_SERVER_HPP

#include "IServer.hpp"
#include "Clients/ClientHandler.hpp"
#include "ServerSocket.hpp"
#include "Server/SignalHandler.hpp"

#include <vector>
#include <poll.h>
#include <iostream>


class FtpServer : public IServer {
public:
    FtpServer(int port, const std::string& homePath);
    ~FtpServer();

    bool start() override;
    void run() override;

private:
    int port;
    std::string homePath;
    ServerSocket serverSocket;
    bool running;
    std::vector<ClientHandler*> clients;
    std::vector<struct pollfd> pollFds;

    void acceptNewClient();
    void removeClient(size_t index);
    void cleanupClients();
};

#endif // FTP_SERVER_HPP