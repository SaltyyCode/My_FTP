/*
** EPITECH PROJECT, 2025
** SignalHandler.hpp
** File description:
** SignalHandler for FTP
*/

#ifndef SIGNAL_HANDLER_HPP
#define SIGNAL_HANDLER_HPP

#include <signal.h>
#include <sys/wait.h>

class SignalHandler {
public:
    static void setup();
private:
    static void sigchldHandler(int);
};

#endif // SIGNAL_HANDLER_HPP