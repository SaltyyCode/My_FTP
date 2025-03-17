/*
** EPITECH PROJECT, 2025
** MiscCommand.cpp
** File description:
** MiscCommand for FTP
*/

#include "Clients/CommandHandler.hpp"

void CommandHandler::handleQUIT(const std::string&)
{
    client.sendResponse(221, "Service closing control connection.");
}

void CommandHandler::handleNOOP(const std::string&)
{
    client.sendResponse(200, "Command okay.");
}

void CommandHandler::handleUnknown(const std::string&)
{
    client.sendResponse(500, "Command not implemented.");
}

void CommandHandler::handleHELP(const std::string& param)
{
    std::string command;

    static const std::unordered_map<std::string, std::string> helpTexts = {
        {"USER", "USER <username>: Specify user for authentication."},
        {"PASS", "PASS <password>: Specify password for authentication."},
        {"CWD", "CWD <path>: Change working directory."},
        {"CDUP", "CDUP: Change to parent directory."},
        {"QUIT", "QUIT: Disconnect from server."},
        {"DELE", "DELE <path>: Delete file on server."},
        {"PWD", "PWD: Print current working directory."},
        {"PASV", "PASV: Enter passive mode for data transfer."},
        {"PORT", "PORT <h1,h2,h3,h4,p1,p2>: Enter active mode for data transfer."},
        {"HELP", "HELP [<command>]: List available commands or get help on specific command."},
        {"NOOP", "NOOP: No operation, server returns 200 OK."},
        {"RETR", "RETR <path>: Download file from server to client."},
        {"STOR", "STOR <path>: Upload file from client to server."},
        {"LIST", "LIST [<path>]: List files in current or specified directory."}
    };

    if (param.empty()) {
        client.sendResponse(214, "The following commands are recognized:"
                            " USER PASS CWD CDUP QUIT DELE PWD"
                            " PASV PORT HELP NOOP RETR STOR LIST");
        return;
    }
    command = param;
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    auto it = helpTexts.find(command);
    if (it != helpTexts.end()) {
        client.sendResponse(214, it->second);
    } else {
        client.sendResponse(214, "Unknown command. Type HELP for command list.");
    }
}

void CommandHandler::handleDELE(const std::string& param)
{
    struct stat st;
    std::string path;
    std::string filePath;
    std::string fullPath;

    if (!client.isAuthenticated()) {
        client.sendResponse(530, "Not logged in.");
        return;
    }
    
    if (param.empty()) {
        client.sendResponse(501, "Syntax error in parameters or arguments.");
        return;
    }
    
    path = client.getCurrentPath();
    
    if (param[0] == '/') {
        filePath = param;
    } else {
        filePath = (path == "/") ? "/" + param : path + "/" + param;
    }
    
    filePath = normalizePath(filePath);
    fullPath = client.getHomePath() + filePath;

    if (stat(fullPath.c_str(), &st) != 0) {
        client.sendResponse(550, "File not found.");
        return;
    }
    
    if (S_ISDIR(st.st_mode)) {
        client.sendResponse(550, "Not a regular file.");
        return;
    }

    if (unlink(fullPath.c_str()) == 0) {
        client.sendResponse(250, "File deleted successfully.");
    } else {
        client.sendResponse(550, "Failed to delete file: " + std::string(strerror(errno)));
    }
}