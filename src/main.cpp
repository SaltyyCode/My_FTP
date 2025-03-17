/*
** EPITECH PROJECT, 2025
** main.cpp
** File description:
** main for FTP
*/

#include <iostream>
#include <string>
#include "Server/FTPServer.hpp"
#include "Utils/Utils.hpp"

void runServer(int port, const std::string& homePath)
{
    FtpServer server(port, homePath);

    if (!server.start()) {
        std::cerr << "Error: Failed to start FTP server" << std::endl;
        exit(84);
    }
    server.run();
}

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: ./myftp <port> <home_path>" << std::endl;
        return 84;
    }
    
    try {
        int port = std::stoi(argv[1]);
    
        if (port <= 0 || port > 65535) throw std::out_of_range("Port must be between 1 and 65535");

        if (!isValidDirectory(argv[2])) {
            std::cerr << "Error: Directory '" << argv[2] << "' does not exist, is not accessible, or is not a directory" << std::endl;
            return 84;
        }

        runServer(port, argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid port number - " << e.what() << std::endl;
        return 84;
    }
    
    return 0;
}
