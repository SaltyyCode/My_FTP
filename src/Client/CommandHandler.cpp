/*
** EPITECH PROJECT, 2025
** CommandHandler.cpp
** File description:
** CommandHandler for FTP
*/

#include "Clients/CommandHandler.hpp"

CommandHandler::CommandHandler(ClientHandler& clientHandler) : client(clientHandler) {}

void CommandHandler::executeCommand(const std::string& commandLine)
{
    std::string command;
    std::string param;
    size_t spacePos;

    static const std::unordered_map<std::string, std::function<void(CommandHandler&, const std::string&)>> commandMap = {
        {"USER", [](CommandHandler& cmd, const std::string& p) { cmd.handleUSER(p); }},
        {"PASS", [](CommandHandler& cmd, const std::string& p) { cmd.handlePASS(p); }},
        {"QUIT", [](CommandHandler& cmd, const std::string& p) { cmd.handleQUIT(p); }},
        {"NOOP", [](CommandHandler& cmd, const std::string& p) { cmd.handleNOOP(p); }},
        {"PWD",  [](CommandHandler& cmd, const std::string& p) { cmd.handlePWD(p); }},
        {"CWD",  [](CommandHandler& cmd, const std::string& p) { cmd.handleCWD(p); }},
        {"CDUP", [](CommandHandler& cmd, const std::string& p) { cmd.handleCDUP(p); }},
        {"HELP", [](CommandHandler& cmd, const std::string& p) { cmd.handleHELP(p); }},
        {"PASV", [](CommandHandler& cmd, const std::string& p) { cmd.handlePASV(p); }},
        {"LIST", [](CommandHandler& cmd, const std::string& p) { cmd.handleLIST(p); }},
        {"PORT", [](CommandHandler& cmd, const std::string& p) { cmd.handlePORT(p); }},
        {"DELE", [](CommandHandler& cmd, const std::string& p) { cmd.handleDELE(p); }},
        {"RETR", [](CommandHandler& cmd, const std::string& p) { cmd.handleRETR(p); }},
        {"STOR", [](CommandHandler& cmd, const std::string& p) { cmd.handleSTOR(p); }}
    };

    spacePos = commandLine.find(' ');
    if (spacePos != std::string::npos) {
        command = commandLine.substr(0, spacePos);
        param = commandLine.substr(spacePos + 1);
    } else {
        command = commandLine;
        param = "";
    }
    
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);
    
    auto it = commandMap.find(command);
    if (it != commandMap.end()) {
        it->second(*this, param);
    } else {
        handleUnknown(command);
    }
}