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

#include "student.h"

int main() {
	WSAData wsalib_d;
	WORD DllVersion = MAKEWORD(1, 3);

	if (WSAStartup(DllVersion, &wsalib_d) != 0) {
		cerr << "WinSocket startup error!" << endl;
		return -1;
	}

	std::string hostname = "127.0.0.1";
	int port = 27015;

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(hostname.c_str());
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	size_t addrSize = sizeof(addr);

	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (bind(serverSocket, (SOCKADDR*)&addr, addrSize) != 0) {
		cerr << "BINDING ERROR: " << WSAGetLastError() << endl;
		return -1;
	}

	listen(serverSocket, SOMAXCONN);
	cout << "Server is ready!" << endl;

	SOCKET clientSocket;

	while (true) {
		int addrSizeInt = (int)addrSize;
		clientSocket = accept(serverSocket, (SOCKADDR*)&addr, &addrSizeInt);

		if (!clientSocket) {
			cerr << "Something went wrong when client tried to connect..." << endl;
			continue;
		}

		Student student;
		recv(clientSocket, (char*)&student, sizeof(student), NULL);
		cout << "Client sent:" << endl;
		student.print();

		Response response = student.getResponse();
		send(clientSocket, (char*)&response, sizeof(response), NULL);
		closesocket(clientSocket);

		cout << "--------------------------------------------" << endl;
	}
	
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}