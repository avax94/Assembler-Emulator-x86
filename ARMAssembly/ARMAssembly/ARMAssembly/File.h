#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class File
{
	const char* path;
	ifstream readFile;
	ofstream writeFile;
public:
	string readWord();
	string readLine();
	char read();
	void write(string toWrite);
	void write(char c);
	void write(char* c, int sz);
	bool eof();

	const char* get_name();

	File(const char* path, bool toRead);
	~File();
};

