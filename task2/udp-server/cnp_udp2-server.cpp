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

	int port = 27015;

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	int addrSize = sizeof(addr);

	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, NULL);
	if (bind(serverSocket, (SOCKADDR*)&addr, addrSize) != 0) {
		cerr << "BINDING ERROR: " << WSAGetLastError() << endl;
		return -1;
	}

	cout << "Server is ready!" << endl << endl;

	char msg[MAX_MSG_SIZE];
	while (true) {
		SOCKADDR_IN clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		int msgSize = recvfrom(serverSocket, msg, MAX_MSG_SIZE - 1, NULL,
			(SOCKADDR*)&clientAddr, &clientAddrSize);

		if (msgSize == SOCKET_ERROR) {
			cout << "Receive error!" << endl;
			continue;
		}

		hostent* host = gethostbyaddr((char*)&clientAddr.sin_addr, 4, AF_INET);
		cout << (host ? host->h_name : "Unknown host") << " sent a new datagram!" << endl;
		cout << inet_ntoa(clientAddr.sin_addr) << endl << ntohs(clientAddr.sin_port) << endl;

		msg[msgSize] = '\0';
		cout << msg << endl << endl;
		sendto(serverSocket, msg, msgSize, 0, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
	}

	closesocket(serverSocket);
	WSACleanup();
	return 0;
}