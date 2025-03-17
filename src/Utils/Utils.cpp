/*
** EPITECH PROJECT, 2025
** Utils.cpp
** File description:
** Utils for FTP
*/

#include "Utils/Utils.hpp"

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    
    return tokens;
}

std::string normalizePath(const std::string& path)
{
    std::vector<std::string> components;
    std::istringstream iss(path);
    std::string component;
    
    while (std::getline(iss, component, '/')) {
        if (component.empty() || component == ".") {
            continue;
        } else if (component == "..") {
            if (!components.empty()) {
                components.pop_back();
            }
        } else {
            components.push_back(component);
        }
    }
    std::string result = "/";
    for (const auto& comp : components)
        result += comp + "/";
    
    if (result.length() > 1 && result.back() == '/')
        result.pop_back();
    
    return result;
}

std::string CommandHandler::getDirectoryListing(const std::string& fullPath)
{
    DIR* dir = opendir(fullPath.c_str());
    std::string listing;
    struct dirent* entry;

    if (dir == nullptr) {
        return "";
    }

    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        
        if (filename != "." && filename != "..") {
            listing += filename + "\r\n";
        }
    }
    closedir(dir);
    return listing;
}

bool isValidDirectory(const std::string& path)
{
    struct stat st;
    
    if (stat(path.c_str(), &st) != 0)
        return false;

    if (!S_ISDIR(st.st_mode))
        return false;

    if (access(path.c_str(), R_OK | X_OK) != 0)
        return false;
    
    return true;
}

std::string CommandHandler::buildFilePath(const std::string& param)
{
    std::string path = client.getCurrentPath();
    std::string filePath;
    
    if (param[0] == '/') {
        filePath = param;
    } else {
        filePath = (path == "/") ? "/" + param : path + "/" + param;
    }
    filePath = normalizePath(filePath);
    return client.getHomePath() + filePath;
}

int CommandHandler::establishDataConnection()
{
    int dataSocket;
    
    if (client.isPassiveMode()) {
        dataSocket = client.acceptPassiveConnection();
    } else if (client.isActiveMode()) {
        dataSocket = client.connectToClient();
    } else {
        client.sendResponse(425, "Use PASV or PORT first.");
        return -1;
    }
    
    if (dataSocket < 0) {
        client.sendResponse(425, "Can't open data connection.");
        return -1;
    }
    
    client.sendResponse(150, "Opening data connection for file transfer.");
    return dataSocket;
}