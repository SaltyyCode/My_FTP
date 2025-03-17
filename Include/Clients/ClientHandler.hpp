/*
** EPITECH PROJECT, 2025
** ClientHandler.hpp
** File description:
** ClientHandler for FTP
*/

#ifndef CLIENT_HANDLER_HPP
#define CLIENT_HANDLER_HPP

#include "IClientHandler.hpp"

#include <string>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

class ClientHandler : public IClientHandler {
public:
    ClientHandler(int socket, const std::string& homePath);
    ~ClientHandler();

    bool handleRequest() override;
    void sendResponse(int code, const std::string& message) override;
    bool isActive() const override;

    bool isAuthenticated() const { return authenticated; }
    const std::string& getCurrentPath() const { return currentPath; }
    const std::string& getHomePath() const { return homePath; }
    void setCurrentPath(const std::string& path) { currentPath = path; }
    void setUsername(const std::string& name) { username = name; }
    const std::string& getUsername() const { return username; }
    void setAuthenticated(bool auth) { authenticated = auth; }
    bool setupPassiveMode();
    int acceptPassiveConnection();
    void closePassiveConnection();
    bool isPassiveMode() const;
    int getPassivePort() const;
    int getControlSocket() const;
    bool setupActiveMode(const std::string& host, int port);
    int connectToClient();
    bool isActiveMode() const;

private:
    int controlSocket;
    std::string homePath;
    std::string currentPath;
    std::string username;
    bool authenticated;
    bool active;
    int passiveSocket;
    int passivePort;
    bool passiveMode;
    std::string activeHost;
    int activePort;
    bool activeMode;

};

#endif // CLIENT_HANDLER_HPP
