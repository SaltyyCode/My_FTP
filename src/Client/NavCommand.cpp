/*
** EPITECH PROJECT, 2025
** NavCommand.cpp
** File description:
** NavCommand for FTP
*/

#include "Clients/CommandHandler.hpp"

void CommandHandler::handlePWD(const std::string&)
{
    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    client.sendResponse(257, "\"" + client.getCurrentPath() + "\" is the current directory.");
}

void CommandHandler::handleCWD(const std::string& param)
{
    struct stat st;
    std::string newPath;
    std::string fullPath;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    if (param.empty()) {
        client.sendResponse(550, "Failed to change directory.");
        return;
    }
    
    if (param[0] == '/') {
        newPath = param;
    } else {
        std::string currentPath = client.getCurrentPath();
        if (currentPath == "/") {
            newPath = currentPath + param;
        } else {
            newPath = currentPath + "/" + param;
        }
    }

    newPath = normalizePath(newPath);
    fullPath = client.getHomePath() + newPath;

    if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        client.setCurrentPath(newPath);
        client.sendResponse(250, "Directory successfully changed.");
    } else {
        client.sendResponse(550, "Failed to change directory.");
    }
}

void CommandHandler::handleCDUP(const std::string&)
{
    std::string currentPath;
    size_t pos;
    std::string newPath;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    currentPath = client.getCurrentPath();
    if (currentPath == "/") {
        client.sendResponse(550, "Already at root directory.");
        return;
    }
    
    pos = currentPath.find_last_of('/');
    newPath = (pos == 0) ? "/" : currentPath.substr(0, pos);
    client.setCurrentPath(newPath);
    client.sendResponse(250, "Moved to parent directory.");
}