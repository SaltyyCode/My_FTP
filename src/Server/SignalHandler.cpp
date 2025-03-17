/*
** EPITECH PROJECT, 2025
** SignalHandler.cpp
** File description:
** SignalHandler for FTP
*/

#include "Server/SignalHandler.hpp"

void SignalHandler::sigchldHandler(int)
{
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}

void SignalHandler::setup()
{
    struct sigaction sa;

    sa.sa_handler = sigchldHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, nullptr);
}
