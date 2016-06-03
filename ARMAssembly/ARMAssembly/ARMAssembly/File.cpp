#include "File.h"
#include <iostream>
#include <fstream>
#include "InvalidFilePathException.h"

using namespace std;


string to_hex(char c)
{
	switch (c){
	case 0:
		return "0";
	case 1:
		return "1";
	case 2:
		return "2";
	case 3:
		return "3";
	case 4:
		return "4";
	case 5:
		return "5";
	case 6:
		return "6";
	case 7:
		return "7";
	case 8:
		return "8";
	case 9:
		return "9";
	case 10:
		return "A";
	case 11:
		return "B";
	case 12:
		return "C";
	case 13:
		return "D";
	case 14:
		return "E";
	case 15:
		return "F";
	}
}

string hex(unsigned char _c)
{
	char f = _c % 16;
	char s = _c / 16;
	if (s > 15)
	{
		cout << "HEHE";
	}

	return "" + to_hex(s) + to_hex(f);
}



File::File(const char* path, bool toRead)
{
	this->path = path;
	
	if (toRead)
	{
		readFile.open(path);

		if (!readFile.is_open())
		{
			throw InvalidFilePathException("Invalid path");
		}
	}
	else
	{
		writeFile.open(path);

		if (!writeFile.is_open())
		{
			throw InvalidFilePathException("Invalid path");
		}
	}
}

void File::write(string s)
{
	writeFile << s;
}

void File::write(char c)
{
	writeFile << c;
}

void File::write(char* c, int size)
{
	for (int i = 0; i < size; i++)
		writeFile << hex(c[i]) << " ";
}

File::~File()
{
	if (readFile.is_open())
	{
		readFile.close();
	}

	if (writeFile.is_open())
	{
		writeFile.close();
	}
}

char File::read()
{
	return readFile.get();
}

string File::readWord()
{
	string result = "";

	char c;

	//skip all blanco spaces, tabs and new line symbols
	while (((c = readFile.get()) == ' ' || c == '\n' || c == '\t') && !readFile.eof())
	{
		if (readFile.eof())
		{
			break;
		}
	}

	if (!readFile.eof()) { result += c; }

	//read word
	while ((c = readFile.get()) != ' ' && c != '\n' && c != '\t' && !readFile.eof())
	{
		result += c;
	}

	return result;
}

//read from cursor to the end of the line
string File::readLine()
{
	string result = "";
	char c;

	while ((c = readFile.get()) != '\n' && !readFile.eof())
	{
		result += c;
	}

	return result;
}

bool File::eof()
{
	if (readFile.is_open())
	{
		return readFile.eof();
	}
	
	if (writeFile.is_open())
	{
		return writeFile.eof();
	}
}

const char* File::get_name()
{
	return path;
}