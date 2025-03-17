/*
** EPITECH PROJECT, 2025
** Utils.hpp
** File description:
** Utils for FTP
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "Clients/CommandHandler.hpp"

std::vector<std::string> split(const std::string& str, char delimiter);
std::string normalizePath(const std::string& path);
bool isValidDirectory(const std::string& path);

#endif // UTILS_HPP