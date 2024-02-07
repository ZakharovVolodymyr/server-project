//Code.cpp
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include<stdlib.h>
#include <string.h>
#include <thread>

#define _CRT_SECURE_NO_WARNINGS
//server's port number for bind()
#define SERVER_PORT_NUM         2022

//priority of server's work task
#define SERVER_WORK_PRIORITY    100

//stack size of server's work task
#define SERVER_STACK_SIZE       10000

//max clients connected at a time
#define SERVER_MAX_CONNECTIONS  4

//max size of request message  
#define REQUEST_MSG_SIZE        200000

#pragma comment(lib,"Ws2_32.lib")


using namespace::std;

void KilSlov(string s, string& str)
{
	setlocale(LC_ALL, "Ukrainian");
	ifstream f2(s, ios::in);
	char tmp;
	int n = 50;
	int sizeOfArr = 0;
	int* counter = new int[n];
	string StartString = " lol ";
	string* lit = new string[n];
	for (int i = 0; i < n; i++)
		lit[i] = " lol ";

	while (!f2.eof())
	{
		tmp = f2.get();
		if(!isdigit(tmp) && !ispunct(tmp))
		{
			tmp = tolower(tmp);
			bool konZapLit = true;
			std::string tmptmp(1, tmp);
			//if (strcmp(" ", tmptmp.c_str()) != 0)
			for (int i = 0; i < n && konZapLit; i++)
			{
				if (strcmp(tmptmp.c_str(), " ") != 0 && strcmp(tmptmp.c_str(), "\n") != 0)
					if (strcmp(lit[i].c_str(), StartString.c_str()) == 0)
					{
						lit[i] = tmp;
						counter[i] = 1;
						sizeOfArr++;
						konZapLit = false;
					}
					else
					if (strcmp(lit[i].c_str(), tmptmp.c_str()) == 0)
					{
						counter[i]++;
						konZapLit = false;
					}
			}
		}
	}
	int tmpForCounter;
	string tmpForLit;
	for (int i = 0; i < sizeOfArr; i++)
	{
		int MaxN = 0, MaxO;
		for (int o = i; o < sizeOfArr; o++)
		{
			if (MaxN < counter[o]) 
			{
				MaxN = counter[o];
				MaxO = o;
			}
		}
		tmpForCounter = counter[i];
		tmpForLit = lit[i];
		counter[i] = counter[MaxO];
		lit[i] = lit[MaxO];
		counter[MaxO] = tmpForCounter;
		lit[MaxO] = tmpForLit;
	}

	string ret;
	for (int i = 0; i < n && strcmp(lit[i].c_str(), StartString.c_str()) != 0; i++) 
	{
		if (lit[i][0] == -1)
		{
			if (counter[i] == 1);
			else
			{
				ret += lit[i];
				ret += " ";
				ret += std::to_string(counter[i] - 1);
				ret += "\n";
			}

		}
		else 
		{
			ret += lit[i];
			ret += " ";
			ret += std::to_string(counter[i]);
			ret += "\n";
		}
	}
	str = ret;
	
	delete[] counter;
	delete[] lit;
	f2.close();
	//return bl;
}

extern "C" _declspec(dllexport) void KilASlov(string& str)
{
	KilSlov("txtA.txt", str);
}
extern "C" _declspec(dllexport) void KilUSlov(string& str)
{
	KilSlov("txtU.txt", str);
}

extern "C" _declspec(dllexport) void out(string& FullMessage)
{
	setlocale(LC_ALL, "Ukrainian");
	char clientRequest[REQUEST_MSG_SIZE];
	memcpy(clientRequest, FullMessage.c_str(), 20000);
	sockaddr_in  serverAddr;
	int sockAddrSize;
	SOCKET sHandle;
	unsigned nbyte;
	double starttime, endtime;
	unsigned mlen;
	char serverName[11];
	WSADATA wsaData;
	strcpy_s(serverName, "127.0.0.1");
	WSAStartup(MAKEWORD(1, 1), &wsaData);
	sHandle = socket(AF_INET, SOCK_STREAM, 0);
	sockAddrSize = sizeof(sockaddr_in);
	memset(&serverAddr, 0, sockAddrSize);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT_NUM);
	serverAddr.sin_addr.s_addr = inet_addr(serverName);
	connect(sHandle, (sockaddr*)&serverAddr, sockAddrSize);

	mlen = strlen(clientRequest);
	mlen++;
	nbyte = send(sHandle, (char*)clientRequest, mlen, MSG_DONTROUTE);
	closesocket(sHandle);
}