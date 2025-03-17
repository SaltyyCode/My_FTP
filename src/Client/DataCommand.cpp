/*
** EPITECH PROJECT, 2025
** DataCommand.cpp
** File description:
** DataCommand for FTP
*/

#include "Clients/CommandHandler.hpp"

void CommandHandler::handlePASV(const std::string&)
{
    struct sockaddr_in localAddr;
    socklen_t len;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }

    if (!client.setupPassiveMode()) {
        client.sendResponse(425, "Cannot open data connection.");
        return;
    }

    len = sizeof(localAddr);
    getsockname(client.getControlSocket(), (struct sockaddr*)&localAddr, &len);
    unsigned char* ip = (unsigned char*)&localAddr.sin_addr.s_addr;
    
    int port = client.getPassivePort();
    int p1 = port / 256;
    int p2 = port % 256;
    
    std::string response = "Entering Passive Mode (" + 
                          std::to_string(ip[0]) + "," + 
                          std::to_string(ip[1]) + "," + 
                          std::to_string(ip[2]) + "," + 
                          std::to_string(ip[3]) + "," + 
                          std::to_string(p1) + "," + 
                          std::to_string(p2) + ")";
    
    client.sendResponse(227, response);
}

void CommandHandler::handlePORT(const std::string& param)
{
    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    client.closePassiveConnection();
    
    std::vector<std::string> parts = split(param, ',');
    if (parts.size() != 6) {
        client.sendResponse(501, "Invalid PORT command.");
        return;
    }
    
    try {

        std::string ip = parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3];
        
        int p1 = std::stoi(parts[4]);
        int p2 = std::stoi(parts[5]);
        int port = p1 * 256 + p2;
        
        if (port < 1024 || port > 65535) {
            client.sendResponse(501, "Invalid port number.");
            return;
        }
        
        if (client.setupActiveMode(ip, port)) {
            client.sendResponse(200, "PORT command successful.");
        } else {
            client.sendResponse(425, "Can't setup active mode.");
        }
    } catch (const std::exception& e) {
        client.sendResponse(501, "Invalid PORT command parameters.");
    }
}

void CommandHandler::handleLIST(const std::string& param)
{
    std::string path;
    std::string fullPath;
    int dataSocket;
    std::string listing;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }

    if (!client.isPassiveMode()) {
        client.sendResponse(425, "Use PASV or PORT first.");
        return;
    }

    path = client.getCurrentPath();

    if (!param.empty()) {
        if (param[0] == '/') {
            path = param;
        } else {
            path = (path == "/") ? "/" + param : path + "/" + param;
        }
        path = normalizePath(path);
    }
    
    fullPath = client.getHomePath() + path;
    dataSocket = client.acceptPassiveConnection();

    if (dataSocket < 0) {
        client.sendResponse(425, "Can't open data connection.");
        return;
    }

    client.sendResponse(150, "Opening data connection for directory listing.");
    listing = getDirectoryListing(fullPath);

    if (!listing.empty()) {
        write(dataSocket, listing.c_str(), listing.length());
        client.sendResponse(226, "Directory send OK.");
    } else {
        client.sendResponse(550, "Failed to get directory listing.");
    }
    
    close(dataSocket);
}

void CommandHandler::handleRETR(const std::string& param)
{
    struct stat st;
    char buffer[8192];
    size_t totalSent = 0;
    int dataSocket;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    if (param.empty()) {
        client.sendResponse(501, "Syntax error in parameters.");
        return;
    }
    
    std::string fullPath = buildFilePath(param);
    
    if (stat(fullPath.c_str(), &st) != 0) {
        client.sendResponse(550, "File not found.");
        return;
    }
    
    if (!S_ISREG(st.st_mode)) {
        client.sendResponse(550, "Not a regular file.");
        return;
    }
    
    dataSocket = establishDataConnection();
    if (dataSocket < 0) {
        return;
    }
    
    std::ifstream file(fullPath, std::ios::binary);
    if (!file) {
        client.sendResponse(550, "Failed to open file.");
        close(dataSocket);
        return;
    }
    
    while (file) {
        file.read(buffer, sizeof(buffer));
        std::streamsize bytesRead = file.gcount();
        
        if (bytesRead > 0) {
            ssize_t bytesSent = write(dataSocket, buffer, bytesRead);
            if (bytesSent < 0) {
                break;
            }
            totalSent += bytesSent;
        }
    }
    
    file.close();
    close(dataSocket);
    
    if (totalSent == static_cast<size_t>(st.st_size)) {
        client.sendResponse(226, "Transfer complete.");
    } else {
        client.sendResponse(426, "Transfer aborted.");
    }
}

void CommandHandler::handleSTOR(const std::string& param)
{
    char buffer[8192];
    ssize_t bytesRead;
    size_t totalReceived = 0;
    int dataSocket;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    if (param.empty()) {
        client.sendResponse(501, "Syntax error in parameters.");
        return;
    }
    
    std::string fullPath = buildFilePath(param);
    
    dataSocket = establishDataConnection();
    if (dataSocket < 0) {
        return;
    }
    
    std::ofstream file(fullPath, std::ios::binary);
    if (!file) {
        client.sendResponse(550, "Failed to create file.");
        close(dataSocket);
        return;
    }
    
    while ((bytesRead = read(dataSocket, buffer, sizeof(buffer))) > 0) {
        file.write(buffer, bytesRead);
        if (!file) {
            break;
        }
        totalReceived += bytesRead;
    }
    
    file.close();
    close(dataSocket);
    
    if (bytesRead >= 0) {
        client.sendResponse(226, "Transfer complete.");
    } else {
        client.sendResponse(426, "Transfer aborted.");
        std::remove(fullPath.c_str());
    }
}