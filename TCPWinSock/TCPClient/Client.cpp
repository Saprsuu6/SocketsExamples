#pragma comment(lib, "../Debug/TcpSockets.lib")
#include "../TcpSockets/Header.h"

#include <algorithm>
using namespace std;

int main()
{	
	try {		

		TcpClient client;
		cout << "Client started" << endl;
		
		client.connect("127.0.0.1", 23000);

		while (true)
		{
			string request = "Client request";
			cout << "Enter request text: ";
			getline(cin, request);	

			transform(request.begin(), request.end(), request.begin(), [](char c) {
				return tolower(c);
			});

			if (request == "exit") {
				break;
			}

			client.send(request);

			string response = client.receive();
			cout << "Response: " << response << endl;
		}
		
		client.shutdown();
		
	}
	catch (SocketException& ex)
	{
		cout << ex.what() << endl;		
	}
}

