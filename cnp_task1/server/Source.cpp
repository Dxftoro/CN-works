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

#include "human.h"

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

		while (!fin.eof() && fin >> tmp) {
			deltaString += " " + tmp;
		}

		return deltaString;
	}

	return "";
}

void waitForExit(char *exitKey) {
	while (*exitKey != LOOP_EXIT_CODE) {
		*exitKey = _getch();
	}
}

int main() {
	std::string serverFilePath = "D:\\Programming\\testing\\test_server.txt";
	std::string clientFilePath = "D:\\Programming\\testing\\test_client.txt";
	ifstream fin(serverFilePath);
	ofstream fout;

	if (!fin) {
		std::cerr << "File not found!" << endl;
		return -1;
	}

	char exitKey = '\0';
	std::thread keyReceiver(waitForExit, &exitKey);

	while (true) {
		const int size1 = getFileSize(fin);
		int delta = 0, size2;

		cout << "Listening file " << serverFilePath << endl;
		cout << "Current file size: " << size1 << " b." << endl;

		while (delta == 0 && exitKey != LOOP_EXIT_CODE) {
			size2 = getFileSize(fin);
			delta = size1 - size2;
		}

		if (exitKey == LOOP_EXIT_CODE) {
			cout << "Exit key caught. Stopping..." << endl;
			keyReceiver.join();
			break;
		}

		cout << "File changed! " << size2 << " b." << endl;
		std::string content = getDeltaString(fin, size1, size2);

		Human human(content);
		human.print();

		fout.open(clientFilePath, std::ios::app);
		fout << human.getPrintString();
		fout.close();

		cout << "--------------------------------------------" << endl;
		fin.clear();
	}

	fin.close();
	system("pause");
	return 0;
}