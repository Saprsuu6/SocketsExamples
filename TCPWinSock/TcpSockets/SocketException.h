#pragma once

#include "Header.h"

class EXPORT SocketException : public exception
{
public:

	SocketException() {
		_message = "Socket exception";
		_errCode = 0;
	}

	SocketException(string message) {
		_message = message;
		_errCode = WSAGetLastError();
	}

	SocketException(string message, int errCode) {
		_message = message;
		_errCode = errCode;
	}

	const char* what() const override {
		static char str[1024];

		if (_errCode == 0) {
			sprintf_s(str, "Exception: %s", _message.c_str());
		}
		else {
			sprintf_s(str, "Exception: %s Error code: %d", _message.c_str(), _errCode);
		}
		
		return str;
	}

	int getCode() { return _errCode; }
private:
	string _message;
	int _errCode;
};

