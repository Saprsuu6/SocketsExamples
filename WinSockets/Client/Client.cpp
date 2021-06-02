#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
using namespace std;

int main()
{
    //1. инициализация "Ws2_32.dll" для текущего процесса
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        cout << "WSAStartup failed with error: " << err << endl;
        return 1;
    }
    
    //2. создание клиентского сокета
    SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        cout << "socket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 2;
    }

    //инициализация структуры, для указания ip адреса и порта сервера с которым мы хотим соединиться
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    addr.sin_port = htons(27015);

    //3. Соединяемся с сервером
    int iResult = connect(connectSocket, (SOCKADDR*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
        cout << "Unable to connect to server!\n";        
        closesocket(connectSocket);
        WSACleanup();
        return 3;        
    }

    char sendData[100] = "send data";
   //отправка сообщения
    if (send(connectSocket, sendData, 100, 0) == SOCKET_ERROR) {
        cout << "send failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 4;
    }
    cout << "send data" << endl;

    //получение ответа
    char serverAnswer[1024];
    if (recv(connectSocket, serverAnswer, 1024, 0) >= 0) {
        cout << "Server sent: " << serverAnswer << endl;
    }
    else {
        cout << "recv failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 5;
    }

    //отключает отправку и получение сообщений сокетом
    iResult = shutdown(connectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        cout << "shutdown failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 8;
    }

    closesocket(connectSocket);
    WSACleanup();

    return 0;
}
