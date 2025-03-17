# MyFTP - File Transfer Protocol Server
## Description
MyFTP is an C++ FTP server built following the RFC 959 standard. It allows multiple clients to connect, authenticate using an Anonymous account, and transfer files using active or passive modes. The server uses TCP sockets for communication and manages multiple clients using ```poll``` for command handling.

## Features
* Authentication system with an Anonymous account (empty password).
* Standard FTP commands as per RFC 959, including:
* Authentication: ```USER, PASS```
* Directory navigation: ```CWD, CDUP, PWD```
* File operations: ```STOR, RETR, DELE, LIST```
* Connection handling: ```QUIT, PASV, PORT```
* Miscellaneous: ```NOOP, HELP```
* Data transfer modes: Active (PORT) and Passive (PASV).
* Error handling with appropriate FTP status codes.


## Installation and Prerequisites

- A Linux system with GCC (or any C/C++ compiler).
Make sure Make is installed.

## Compilation
To build the project, run:
```make```

## Usage
Run the server with:
```./myftp <port> <path```
* ```<port>```: The port number where the server listens.
* ```<path>```: The path to the home directory for the Anonymous user.

Example:
```./myftp 2121 /home/ftp ```

| Command | Description |
| --- | --- |
| `USER <username>` | Specify user for authentication |
| `PASS <password>` | Specify password for authentication |
| `CWD <pathname>` | Change working directory |
| `CDUP` | Change to parent directory |
| `QUIT` | Disconnect from the server |
| `DELE <pathname>` | Delete a file on the server |
| `PWD` | Print the current directory |
| `PASV` | Enable **passive mode** for data transfer |
| `PORT <host-port>` | Enable **active mode** for data transfer |
| `HELP [command]` | List available commands |
| `NOOP` | Do nothing |
| `RETR <pathname>` | Download a file from the server |
| `STOR <pathname>` | Upload a file to the server |
| `LIST [pathname]` | List files in the current directory |
