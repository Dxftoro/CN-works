#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WinBase.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4996)
#define LOOP_EXIT_CODE	27
#define MAX_MSG_SIZE	4096

using std::cout;
using std::endl;
using std::cerr;
using std::cin;

#include "response.h"

struct Address {
	std::string ip;
	int port = 0;

	Address() {}
	Address(const std::string& strAddress);

	inline void fromString(const std::string& strAddress);
};

inline void Address::fromString(const std::string& strAddress) {
	std::string portStr = "";
	size_t ddotPos = strAddress.find(':');

	if (ddotPos == SIZE_MAX) {
		throw std::runtime_error("Server address must contain ':'!!!");
	}

	for (size_t i = 0; i < strAddress.size(); i++) {
		if (i < ddotPos) ip += strAddress[i];
		if (i > ddotPos) portStr += strAddress[i];
	}

	port = std::stoi(portStr);
}

Address::Address(const std::string& strAddress) {
	fromString(strAddress);
}

int main() {
	WSAData wsalib_d;
	WORD DllVersion = MAKEWORD(1, 3);

	if (WSAStartup(DllVersion, &wsalib_d) != 0) {
		cerr << "WinSocket startup error!" << endl;
		return -1;
	}

	std::string strAddress = "127.0.0.1:27015";
	//cout << "Input server address (IP:PORT): "; cin >> strAddress;

	Address address(strAddress);
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(address.ip.c_str());
	addr.sin_port = htons(address.port);
	addr.sin_family = AF_INET;
	size_t addrSize = sizeof(addr);

	SOCKET serverSocket = INVALID_SOCKET;
	
	bool canConnect = true;
	while (canConnect) {
		Request request;
		cout << "Input surname, name, group number and 4 marks:" << endl;
		request.input();

		serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(serverSocket, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			cerr << "Can't connect to the server!" << endl;
			return -1;
		}

		send(serverSocket, (char*)&request, sizeof(request), NULL);

		ServerResponse response;
		if (recv(serverSocket, (char*)&response, sizeof(response), NULL) < 0) {
			cout << "Connection aborted: " << WSAGetLastError() << endl;
			return -1;
		}

		cout << "\nServer response:\n-----------------------------------------" << endl;
		cout << response.getMessage() << endl << endl;
	}

	closesocket(serverSocket);
	WSACleanup();
	return 0;
}