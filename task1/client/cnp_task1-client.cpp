#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <conio.h>
#include <stdio.h>
#include <thread>
#include <mutex>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

#define LOOP_EXIT_CODE 27

#include "response.h"

std::mutex mtx;

int getFileSize(ifstream& fin) {
	fin.seekg(0, std::ios::end);
	int endPos = fin.tellg();
	return endPos;
}

ServerResponse getDelta(ifstream& fin, int startSize, int newSize) {
	int latestPos = fin.tellg();

	fin.seekg(startSize);
	int delta = newSize - startSize;

	ServerResponse response;
	fin.read((char*)&response, sizeof(response));
	return response;
}

void handleResponse(std::string* clientFilePath, char* exitKey, bool* sended) {
	ifstream fin(*clientFilePath, std::ios::binary);

	if (!fin) {
		std::cerr << "File not found!" << endl;
		*exitKey = LOOP_EXIT_CODE;
		return;
	}

	while (true) {
		const int size1 = getFileSize(fin);
		int delta = 0, size2 = getFileSize(fin);

		while (delta == 0 && *exitKey != LOOP_EXIT_CODE) {
			size2 = getFileSize(fin);
			delta = size1 - size2;
		}

		if (*exitKey == LOOP_EXIT_CODE) {
			cout << "Exit code caught. Stopping..." << endl;
			break;
		}

		if (*sended == true) {
			ServerResponse response = getDelta(fin, size1, size2);
	
			printf("\nServer response:\n-----------------------------------------\n%s\n",
				response.getMessage().c_str());

			fin.clear();
			*sended = false;
		}
	}
}

void clientInput(Request& request, bool& sended) {
	mtx.lock();

	cin >> request.surname >> request.name >> request.group;
	for (int i = 0; i < MAX_STUDENT_MARKS; i++) {
		cin >> request.marks[i];
	}

	sended = true;
	mtx.unlock();
}

int main() {
	std::string serverFilePath = "D:\\Programming\\testing\\cnp_task2\\test_server.bin";
	std::string clientFilePath = "D:\\Programming\\testing\\cnp_task2\\test_client.bin";
	ofstream fout;

	char exitKey = '\0';
	bool sended = false;
	std::thread serverResponseHandler(handleResponse, &clientFilePath, &exitKey, &sended);

	cout << "Waiting for input (surname, name, group, 4 marks)..." << endl;

	Request request;
	clientInput(request, sended);

	while (exitKey != LOOP_EXIT_CODE) {
		fout.open(serverFilePath, std::ios::binary | std::ios::app);
		fout.write((char*)&request, sizeof(request));
		fout.close();

		clientInput(request, sended);
	}

	exitKey = LOOP_EXIT_CODE;
	serverResponseHandler.join();

	if (fout) fout.close();
	system("pause");
	return 0;
}