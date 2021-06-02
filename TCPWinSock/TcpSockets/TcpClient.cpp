#define TSP_SOKETS_DLL
#include "Header.h"

TcpClient::TcpClient()
{  
    //2. создание клиентского сокета
   _clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_clientSocket == INVALID_SOCKET) {
        throw SocketException("socket failed with error: ");       
    }
}

TcpClient::TcpClient(SOCKET socket)
{
	_clientSocket = socket;
}

TcpClient::TcpClient(TcpClient&& tcpClient)
{
    _clientSocket = tcpClient._clientSocket;
    tcpClient._clientSocket = INVALID_SOCKET;
}

void TcpClient::operator=(TcpClient&& tcpClient)
{
    if (this == &tcpClient)
        return;

    if (_clientSocket != INVALID_SOCKET)
        closesocket(_clientSocket);

    _clientSocket = tcpClient._clientSocket;
    tcpClient._clientSocket = INVALID_SOCKET;
}

TcpClient::~TcpClient()
{
    if(_clientSocket != INVALID_SOCKET)
        closesocket(_clientSocket);   
}

void TcpClient::connect(string ipAddress, unsigned short port)
{
    //инициализация структуры, для указания ip адреса и порта сервера с которым мы хотим соединиться
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddress.c_str(), &addr.sin_addr);
    addr.sin_port = htons(port);

    //3. Соединяемся с сервером
    int iResult = ::connect(_clientSocket, (SOCKADDR*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR)
    {
        throw SocketException("Unable to connect to server!");
    }
}

string TcpClient::receive()
{      
    int result = recv(_clientSocket, _recvbuf, 1024, 0);
    if (result > 0)
    {
        return string(_recvbuf);
    }
    else if (result == SOCKET_ERROR) 
    {
        throw SocketException("recv failed: ");
    }
    else if (result == 0)
    {
        throw SocketException("Connection closed", 50000);       
    }

    return string();
}

void TcpClient::send(string data)
{
    if (::send(_clientSocket, data.c_str(), data.length() + 1, 0) == SOCKET_ERROR) 
    {
        throw SocketException("send failed: ");       
    }
}

void TcpClient::shutdown()
{
    int iResult = ::shutdown(_clientSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        throw SocketException("shutdown failed: ");        
    }   
}
