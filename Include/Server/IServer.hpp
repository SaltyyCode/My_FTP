/*
** EPITECH PROJECT, 2025
** IServer.hpp
** File description:
** IServer for FTP
*/

#ifndef ISERVER_HPP
#define ISERVER_HPP

#include <string>

class IServer {
public:
    virtual ~IServer() = default;
    virtual bool start() = 0;
    virtual void run() = 0;
};

#endif // ISERVER_HPP