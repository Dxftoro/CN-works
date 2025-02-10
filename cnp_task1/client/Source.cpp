#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <conio.h>
#include <stdio.h>
#include <thread>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

#define LOOP_EXIT_CODE 27

int getFileSize(ifstream& fin) {
	fin.seekg(0, std::ios::end);
	int endPos = fin.tellg();
	return endPos;
}

std::string getDeltaString(ifstream& fin, int startSize, int newSize) {
	int latestPos = fin.tellg();

	fin.seekg(startSize);

	int delta = newSize - startSize;

	if (delta > 0) {
		std::string deltaString, tmp;

		while (!fin.eof() && getline(fin, tmp)) {
			deltaString += tmp + "\n";
		}
		return deltaString;
	}

	return "";
}

void handleResponse(std::string* clientFilePath, char* exitKey) {
	ifstream fin(*clientFilePath);

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

		std::string response = getDeltaString(fin, size1, size2);
		printf("\nServer response:\n-----------------------------------------\n%s\n",
				response.c_str());

		fin.clear();
	}
}

int main() {
	std::string serverFilePath = "D:\\Programming\\testing\\test_server.txt";
	std::string clientFilePath = "D:\\Programming\\testing\\test_client.txt";
	ofstream fout;

	char exitKey = '\0';
	std::thread serverResponseHandler(handleResponse, &clientFilePath, &exitKey);

	cout << "Waiting for input (name, age, weight)..." << endl;

	std::string userInput;
	std::getline(cin, userInput, '\n');

	while (userInput != "exit" && exitKey != LOOP_EXIT_CODE) {
		fout.open(serverFilePath, std::ios::app);
		fout << userInput << "\n";
		fout.close();

		std::getline(cin, userInput, '\n');
	}

	exitKey = LOOP_EXIT_CODE;
	serverResponseHandler.join();

	if (fout) fout.close();
	system("pause");
	return 0;
}