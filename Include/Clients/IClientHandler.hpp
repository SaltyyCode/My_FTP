/*
** EPITECH PROJECT, 2025
** IClientHandler.hpp
** File description:
** IClientHandler for FTP
*/

#ifndef ICLIENT_HANDLER_HPP
#define ICLIENT_HANDLER_HPP

#include <string>

class IClientHandler {
public:
    virtual ~IClientHandler() = default;
    virtual bool handleRequest() = 0;
    virtual void sendResponse(int code, const std::string& message) = 0;
    virtual bool isActive() const = 0;
};

#endif // ICLIENT_HANDLER_HPP