/*
** EPITECH PROJECT, 2025
** ClientHandler.cpp
** File description:
** ClientHandler for FTP
*/

#include "Clients/ClientHandler.hpp"
#include "Clients/CommandHandler.hpp"

ClientHandler::ClientHandler(int socket, const std::string& homePath)
    : controlSocket(socket), homePath(homePath), currentPath("/"), 
      authenticated(false), active(true), 
      passiveSocket(-1), passivePort(0), passiveMode(false),
      activeHost(""), activePort(0), activeMode(false) {}

ClientHandler::~ClientHandler()
{
    if (controlSocket != -1) {
        close(controlSocket);
        controlSocket = -1;
    }
    
    if (passiveSocket != -1) {
        close(passiveSocket);
        passiveSocket = -1;
    }
}

bool ClientHandler::handleRequest()
{
    char buffer[1024] = {0};
    struct pollfd pfd{controlSocket, POLLIN, 0};
    
    if (poll(&pfd, 1, 0) <= 0 || !(pfd.revents & POLLIN))
        return true;
    
    ssize_t bytesRead = read(controlSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        active = false;
        return false;
    }
    
    buffer[bytesRead] = '\0';
    std::string commandLine(buffer);
    commandLine.erase(commandLine.find_last_not_of("\r\n") + 1);
    
    CommandHandler commandHandler(*this);
    commandHandler.executeCommand(commandLine);

    return true;
}

void ClientHandler::sendResponse(int code, const std::string& message)
{
    std::string response = std::to_string(code) + " " + message + "\r\n";
    write(controlSocket, response.c_str(), response.length());
}

bool ClientHandler::isActive() const
{
    return active;
}

bool ClientHandler::setupPassiveMode()
{
    if (passiveSocket != -1) {
        close(passiveSocket);
        passiveSocket = -1;
    }

    passiveSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (passiveSocket < 0) {
        return false;
    }
    
    int enable = 1;
    setsockopt(passiveSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
    
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(0);
    
    if (bind(passiveSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(passiveSocket);
        passiveSocket = -1;
        return false;
    }
    
    if (listen(passiveSocket, 1) < 0) {
        close(passiveSocket);
        passiveSocket = -1;
        return false;
    }
    
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);

    if (getsockname(passiveSocket, (struct sockaddr*)&sin, &len) < 0) {
        close(passiveSocket);
        passiveSocket = -1;
        return false;
    }
    
    passivePort = ntohs(sin.sin_port);
    passiveMode = true;
    
    return true;
}

int ClientHandler::acceptPassiveConnection()
{
    if (!passiveMode || passiveSocket == -1) {
        return -1;
    }
    
    struct pollfd pfd = {passiveSocket, POLLIN, 0};
    if (poll(&pfd, 1, 30000) <= 0) {
        return -1;
    }
    
    int dataSocket = accept(passiveSocket, nullptr, nullptr);
    close(passiveSocket);
    passiveSocket = -1;
    
    return dataSocket;
}

void ClientHandler::closePassiveConnection()
{
    if (passiveSocket != -1) {
        close(passiveSocket);
        passiveSocket = -1;
    }
    passiveMode = false;
}

bool ClientHandler::isPassiveMode() const
{
    return passiveMode;
}

int ClientHandler::getPassivePort() const
{
    return passivePort;
}

int ClientHandler::getControlSocket() const
{
    return controlSocket;
}

bool ClientHandler::setupActiveMode(const std::string& host, int port)
{
    activeHost = host;
    activePort = port;
    activeMode = true;
    return true;
}

int ClientHandler::connectToClient()
{
    if (!activeMode) {
        return -1;
    }
    
    int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataSocket < 0) {
        return -1;
    }
    
    struct sockaddr_in clientAddr = {};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(activePort);
    
    if (inet_pton(AF_INET, activeHost.c_str(), &clientAddr.sin_addr) <= 0) {
        close(dataSocket);
        return -1;
    }
    
    if (connect(dataSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
        close(dataSocket);
        return -1;
    }
    
    return dataSocket;
}

bool ClientHandler::isActiveMode() const
{
    return activeMode;
}