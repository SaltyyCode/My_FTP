/*
** EPITECH PROJECT, 2025
** FTPServer.cpp
** File description:
** FTP Server for FTP
*/

#include "Server/FTPServer.hpp"
#include "Server/SignalHandler.hpp"

FtpServer::FtpServer(int port, const std::string& homePath)
    : port(port), homePath(homePath), serverSocket(port), running(false)
{
    SignalHandler::setup();
}

FtpServer::~FtpServer() 
{
    for (auto client : clients)
        delete client;
}

bool FtpServer::start()
{
    if (!serverSocket.setup())
        return false;

    running = true;
    pollFds.push_back({serverSocket.getSocket(), POLLIN, 0});

    return true;
}

void FtpServer::run()
{
    while (running) {
        int pollResult = poll(pollFds.data(), pollFds.size(), 100);

        if (pollResult < 0 && errno != EINTR)
            break;
        if (pollFds[0].revents & POLLIN)
            acceptNewClient();
        for (size_t i = 1; i < pollFds.size(); i++) {
            size_t clientIndex = i - 1;
            if (pollFds[i].revents & POLLIN) {
                if (!clients[clientIndex]->handleRequest() || !clients[clientIndex]->isActive()) {
                    removeClient(clientIndex);
                    i--;
                }
            }
        }
        cleanupClients();
    }
}

void FtpServer::acceptNewClient()
{
    int clientSocket = serverSocket.acceptConnection();

    if (clientSocket < 0)
        return;
    
    ClientHandler* client = new ClientHandler(clientSocket, homePath);

    clients.push_back(client);
    pollFds.push_back({clientSocket, POLLIN, 0});
    client->sendResponse(220, "Service ready for new user.");
}

void FtpServer::removeClient(size_t index)
{
    if (index >= clients.size())
        return;

    close(pollFds[index + 1].fd);
    delete clients[index];
    clients.erase(clients.begin() + index);
    pollFds.erase(pollFds.begin() + index + 1);
}

void FtpServer::cleanupClients()
{
    for (size_t i = 0; i < clients.size(); i++) {
        if (!clients[i]->isActive()) {
            removeClient(i);
            i--;
        }
    }
}
