#define TSP_SOKETS_DLL
#include "Header.h"

WinSockInitializer* WinSockInitializer::winSock = nullptr;

WinSockInitializer* WinSockInitializer::GetInstance() {

	if (winSock == nullptr)
	{
		winSock = new WinSockInitializer();
	}

	return winSock;
}

WinSockInitializer::~WinSockInitializer() {
	WSACleanup();
}

WinSockInitializer::WinSockInitializer() {
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);

	//1. инициализация "Ws2_32.dll" для текущего процесса
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {

		throw SocketException("WSAStartup failed with error: ", err);
	}
}