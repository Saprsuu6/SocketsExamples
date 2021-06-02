#pragma comment (lib, "Ws2_32.lib")
#include <Winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
using namespace std;

int main()
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    //1. инициализация "Ws2_32.dll" для текущего процесса
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        cout << "WSAStartup failed with error: " << err << endl;
        return 1;
    }

    //2. создание серверного сокета
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenSocket == INVALID_SOCKET) {
        cout << "socket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 2;
    }     

    //инициализация структуры, для указания ip адреса и порта который мы будем слушать
    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    inet_pton(AF_INET, "10.3.1.4", &addr.sin_addr);
    addr.sin_port = htons(27015);
   
    //3. Привязываем к ip адресу и порту
    if (bind(listenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 3;
    }

    //4. Первести сокет в состояние прослушки
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed with error: " << WSAGetLastError()<< endl;
        closesocket(listenSocket);
        WSACleanup();
        return 4;
    }

    cout << "Listening..." << endl;
    //5. Ожидание соединения с клиентом
    SOCKET clientSocket;

    while (clientSocket = accept(listenSocket, NULL, NULL))
    {
        if (clientSocket == INVALID_SOCKET) {

            cout << "accept failed: " << WSAGetLastError() << endl;
            closesocket(listenSocket);
            WSACleanup();
            return 5;
        }

        cout << "Client accepted" << endl;

        //получем сообщение от клиента
        char recvbuf[10];
        char data[1024] = "";
        int result;
        while(true){
            result = recv(clientSocket, recvbuf, 9, 0);
            if (result > 0)
            {
                recvbuf[9] = '\0';
                strcat_s(data, recvbuf);

                if (strstr(data, "<END>") != nullptr)
                {                    
                    cout << "Sent data: " << data << endl;
                    break;
                }                             
                
            }           
            else if (result == SOCKET_ERROR) {
                cout << "recv failed: " << WSAGetLastError() << endl;
                closesocket(clientSocket);
                WSACleanup();
                return 7;
            }
            else if (result == 0)
            {
                cout << "Connection closed" << endl;
                closesocket(clientSocket);
                break;
            }
        
        }      
       

        //отправляем ответ клиенту
        char answer[100] = "server answer";
        if (send(clientSocket, answer, 100, 0) == SOCKET_ERROR) {
            cout << "send failed: " << WSAGetLastError() << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 6;
        }
        cout << "send answer" << endl;

        char clientAcceptMessage[1024];
        if (recv(clientSocket, clientAcceptMessage, 1024, 0) >= 0) {
            cout << "Server sent: " << clientAcceptMessage << endl;
        }
        else {
            cout << "recv failed: " << WSAGetLastError() << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 5;
        }

        //отключает отправку и получение сообщений сокетом
        int iResult = shutdown(clientSocket, SD_BOTH);
        if (iResult == SOCKET_ERROR) {
            cout << "shutdown failed: " << WSAGetLastError() << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 8;
        }

        closesocket(clientSocket);
    }

    closesocket(listenSocket);
    WSACleanup();

    return 0;
}