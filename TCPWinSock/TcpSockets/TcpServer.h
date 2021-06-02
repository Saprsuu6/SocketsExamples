#pragma once

class EXPORT TcpServer
{
public:

	TcpServer();
	~TcpServer();

	void bind(string ipAddress, unsigned short port);
	void listen();
	TcpClient* accept();

private:

	SOCKET _listenSocket;
};

