#pragma comment(lib, "../Debug/TcpSockets.lib")
#include "../TcpSockets/Header.h"

#include <Windows.h>
#include <list>
#include <algorithm>

list<TcpClient*> clients;
CRITICAL_SECTION csClients;

DWORD WINAPI ThreadProc(LPVOID lpParameter);

int main()
{
    InitializeCriticalSection(&csClients);   
    try {       

        TcpServer server;
        server.bind("127.0.0.1", 23000);

        cout << "server bind 127.0.0.1 : 23000" << endl;
        
        server.listen();
        cout << "server listening..." << endl;    

        while (true)
        {    
            //принимаем запрос на соединение
            TcpClient* client = server.accept();

            EnterCriticalSection(&csClients);
            clients.push_back(client);  
            LeaveCriticalSection(&csClients);

            cout << "client accepted" << endl;

            HANDLE hThread = CreateThread(NULL, 0, ThreadProc, client, 0, NULL);
            CloseHandle(hThread);           
        }
       
        DeleteCriticalSection(&csClients);
    }
    catch (SocketException& ex)
    {
        cout << ex.what() << endl;  

        DeleteCriticalSection(&csClients);
    }

}

DWORD __stdcall ThreadProc(LPVOID lpParameter)
{
    TcpClient* client = (TcpClient*)lpParameter;

    while (true)
    {
        try {
            //принимаем сообщение от клиента
            string request = client->receive();
            cout << "Reqest: " << request << endl;

            //отправляем ответ
            string response = "Server response";
            client->send(response);
        }
        catch (SocketException& ex)
        {
            if (ex.getCode() == 10054 || ex.getCode() == 50000)
            {
                cout << "client closed" << endl;

                auto it = find(clients.begin(), clients.end(), client);
                delete* it;

                EnterCriticalSection(&csClients);
                clients.erase(it);
                LeaveCriticalSection(&csClients);              

                return 0;
            }
        }
    }
    return 0;
}
