#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <WinBase.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4996)
#define LOOP_EXIT_CODE	"exit"
#define MAX_MSG_SIZE	4096

using std::cout;
using std::endl;
using std::cerr;
using std::cin;

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

	Address address("127.0.0.1:27015");

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr(address.ip.c_str());
	addr.sin_port = htons(address.port);
	addr.sin_family = AF_INET;
	int addrSize = sizeof(addr);

	SOCKET selfSocket = socket(AF_INET, SOCK_DGRAM, NULL);

	char msg[MAX_MSG_SIZE];
	while (true) {
		std::string message;

		cout << "Input message:" << endl;
		std::getline(cin, message);

		if (message == LOOP_EXIT_CODE) break;

		sendto(selfSocket, (char*)&message[0], message.size(), NULL,
			(SOCKADDR*)&addr, sizeof(addr));

		SOCKADDR_IN serverAddr;
		int serverAddrSize = sizeof(serverAddr);

		int msgSize = recvfrom(selfSocket, msg, MAX_MSG_SIZE - 1, NULL,
			(SOCKADDR*)&serverAddr, &serverAddrSize);

		if (msgSize == SOCKET_ERROR) {
			cout << "Receive error!" << endl;
			continue;
		}

		msg[msgSize] = '\0';
		cout << "\nServer echo:\n-----------------------------------------" << endl;
		cout << msg << endl << endl;
	}

	closesocket(selfSocket);
	WSACleanup();
	return 0;
}