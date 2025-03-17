##
## EPITECH PROJECT, 2025
## NanoTekSpice
## File description:
## Makefile
##

NAME = myftp
CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++11 -I Include/

SRCS = src/main.cpp \
       src/Server/FTPServer.cpp \
	   src/Server/ServerSocket.cpp \
	   src/Server/SignalHandler.cpp \
	   src/Client/ClientHandler.cpp \
	   src/Client/CommandHandler.cpp \
	   src/Client/AuthCommand.cpp \
	   src/Client/DataCommand.cpp \
	   src/Client/MiscCommand.cpp \
	   src/Client/NavCommand.cpp \
       src/Utils/Utils.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
