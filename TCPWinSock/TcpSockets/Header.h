#pragma once

#ifdef TSP_SOKETS_DLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include <exception> 
using namespace std;

#include "SocketException.h"
#include "WinSockInitializer.h"
#include "TcpClient.h"
#include "TcpServer.h"
