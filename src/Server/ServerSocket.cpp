/*
** EPITECH PROJECT, 2025
** ServerSocket.hpp
** File description:
** FTP Server Socket for FTP
*/

#include "Server/ServerSocket.hpp"

ServerSocket::ServerSocket(int port) : port(port), socketFd(-1) {}

ServerSocket::~ServerSocket()
{
    if (socketFd != -1)
        close(socketFd);
}

bool ServerSocket::setup()
{
    socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketFd == -1)
        return false;
    
    int enable = 1;
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    
    if (bind(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0 ||
        listen(socketFd, 5) < 0) {
        close(socketFd);
        socketFd = -1;
        return false;
    }
    return true;
}

int ServerSocket::acceptConnection()
{
    return accept(socketFd, nullptr, nullptr);
}

int ServerSocket::getSocket() const
{
    return socketFd;
}
