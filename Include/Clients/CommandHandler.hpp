/*
** EPITECH PROJECT, 2025
** CommandHandler.hpp
** File description:
** CommandHandler for FTP
*/

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <sys/stat.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <fstream>

#include "Utils/Utils.hpp"
#include "ClientHandler.hpp"


class CommandHandler {
public:
    explicit CommandHandler(ClientHandler& clientHandler);
    void executeCommand(const std::string& commandLine);
    
private:
    ClientHandler& client;

    void handleUSER(const std::string& param);
    void handlePASS(const std::string& param);  
    void handleQUIT(const std::string& param);
    void handleNOOP(const std::string& param);
    void handlePWD(const std::string& param);
    void handleCWD(const std::string& param);
    void handleCDUP(const std::string& param);
    void handleUnknown(const std::string& command);
    void handleHELP(const std::string& param);
    void handlePASV(const std::string& param);
    void handleLIST(const std::string& param);
    void handlePORT(const std::string& param);
    void handleDELE(const std::string& param);
    void handleRETR(const std::string& param);
    void handleSTOR(const std::string& param);
    std::string getDirectoryListing(const std::string& path);
    std::string buildFilePath(const std::string& param);
    int establishDataConnection();
};

#endif // COMMAND_HANDLER_HPP
