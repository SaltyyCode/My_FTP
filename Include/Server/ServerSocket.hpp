/*
** EPITECH PROJECT, 2025
** ServerSocket.hpp
** File description:
** ServerSocket for FTP
*/

#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

class ServerSocket {
public:
    ServerSocket(int port);
    ~ServerSocket();
    bool setup();
    int acceptConnection();
    int getSocket() const;

private:
    int port;
    int socketFd;
};

#endif // SERVER_SOCKET_HPP