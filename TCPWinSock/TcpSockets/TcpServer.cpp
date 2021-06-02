#define TSP_SOKETS_DLL
#include "Header.h"

TcpServer::TcpServer()
{
    //2. создание серверного сокета
    _listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_listenSocket == INVALID_SOCKET) {
        throw SocketException("socket failed with error: ");          
    }
}


TcpServer::~TcpServer()
{
    if (_listenSocket != INVALID_SOCKET)
    {
        closesocket(_listenSocket);
    }
}

void TcpServer::bind(string ipAddress, unsigned short port)
{
    //инициализация структуры, для указания ip адреса и порта который мы будем слушать
    sockaddr_in addr;
    addr.sin_family = AF_INET;//IPv4
    inet_pton(AF_INET, ipAddress.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);

    //3. Привязываем к ip адресу и порту
    //::bind - функция из глобальной области видимости
    if (::bind(_listenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) 
    {
        throw SocketException("bind failed with error ");  
    }
}

void TcpServer::listen()
{
    //4. Первести сокет в состояние прослушки
    if (::listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR) 
    {
        throw SocketException( "Listen failed with error: ");       
    }
}

TcpClient* TcpServer::accept()
{
    SOCKET clientSocket = ::accept(_listenSocket, NULL, NULL);
    
    if (clientSocket == INVALID_SOCKET) {

        throw SocketException("accept failed: ");
    }

    return new TcpClient(clientSocket);
}