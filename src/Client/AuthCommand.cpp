/*
** EPITECH PROJECT, 2025
** AuthCommand.cpp
** File description:
** AuthCommand for FTP
*/

#include "Clients/CommandHandler.hpp"

void CommandHandler::handleUSER(const std::string& param)
{
    if (param == "Anonymous" || param == "anonymous") {
        client.setUsername(param);
        client.sendResponse(331, "User name okay, need password.");
    } else {
        client.sendResponse(331, "Invalid username. Use 'Anonymous' with an empty password.");
    }
}

void CommandHandler::handlePASS(const std::string&)
{
    if (client.getUsername().empty()) {
        client.sendResponse(530, "Login with USER first.");
        return;
    }
    
    if (client.getUsername() == "Anonymous" || client.getUsername() == "anonymous") {
        client.setAuthenticated(true);
        client.sendResponse(230, "User logged in, proceed.");
    } else {
        client.sendResponse(530, "Login incorrect.");
    }
}