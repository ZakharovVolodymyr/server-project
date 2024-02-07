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
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include "framework.h"
#include "WindowsProject2.h"
#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <sstream>
#include <thread>

#include <mutex>


#pragma comment(lib,"Ws2_32.lib")

#define MAX_LOADSTRING 100
#include <windows.h> // підключення бібліотеки з функціями API
#include <windowsx.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <fstream> 
#include <iostream>

#include <winsock2.h>

#define OnMenuAction4 4
// Глобальні змінні :
HINSTANCE hInst; // покажчик додатку
PAINTSTRUCT ps;
LPCTSTR szWindowClass = L"QWERTY";
HHOOK hook;
HWND hWnd;
DWORD WINAPI network_server(LPVOID);

LPCTSTR szTitle = L"Захаров В.В.";
std::string OneStringToRuleThemAll;//one ring to rule them all

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void MainWndAddMenus(HWND hWnd);
LRESULT CALLBACK Hook(int code, WPARAM wParam, LPARAM lParam);

int n = 80;
std::string* lit = new std::string[n];
int* counter = new int[n];
int MaxI = 0;
std::mutex mtx;
bool LiveOfThread1 = true;
std::condition_variable cv;

using namespace std;

std::wstring s2ws(const std::string& s)
{
	WCHAR wchOut[MAX_PATH + 1]; //I know s will always be a path

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, wchOut, sizeof(wchOut) / sizeof(wchOut[0]));

	return wchOut;
}

// Основна програма 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::string* tmptmp = new std::string[n];
	char realyTmp[10];
	sockaddr_in serverAddr;       // server's socket address
	sockaddr_in clientAddr;       // client's socket address
	int sockAddrSize;			  // size of socket address structure
	SOCKET sHandle;               // socket file descriptor
	SOCKET newSHandle;            // socket descriptor from accept
	WSADATA wsaData;			  //data structure that is to receive details
	int len;                      //of the Windows Sockets implementation
	int nbyte = 0;				  //number of sended bytes
	char clientRequest[REQUEST_MSG_SIZE];
	sockAddrSize = sizeof(sockaddr_in);
	HANDLE theadServer;
	HANDLE theadServer2;
	char buf[80];

	std::stringstream strStrTmp;
	std::stringstream anotherTmp;

	std::thread t1([&]() {
		memset(&serverAddr, 0, sockAddrSize);
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(SERVER_PORT_NUM);
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		WSAStartup(MAKEWORD(1, 1), &wsaData);
		sHandle = socket(AF_INET, SOCK_STREAM, 0);
		bind(sHandle, (sockaddr*)&serverAddr, sockAddrSize);
		listen(sHandle, SERVER_MAX_CONNECTIONS);

		u_long mode = 1;
		ioctlsocket(sHandle, FIONBIO, &mode);

		//newSHandle = accept(sHandle, (sockaddr*)&clientAddr, &sockAddrSize);//проблема тут
		//HANDLE HF;
		//nbyte = recv(newSHandle, (char*)clientRequest, sizeof(clientRequest), MSG_PEEK);
		
		for(; !(nbyte > 0) && LiveOfThread1;)
		//while (!(nbyte > 0) && LiveOfThread1 && iiii < 10) 
		{
			std::lock_guard<std::mutex> lock(mtx);
			newSHandle = accept(sHandle, (sockaddr*)&clientAddr, &sockAddrSize);
			nbyte = recv(newSHandle, (char*)clientRequest, sizeof(clientRequest), 0);
			cv.notify_one();
		}

		if (nbyte > 0) {
			strStrTmp << clientRequest;
			for (int i = 0; !strStrTmp.eof() && i < n; i++)
			{
				strStrTmp.getline(realyTmp, 10, '\n');
				tmptmp[i] = realyTmp;
				MaxI = i;
			}
			tmptmp[MaxI + 1] = "@";
			for (int i = 0; strcmp(tmptmp[i].c_str(), "@") != 0 && i < n; i++)
			{
				anotherTmp << tmptmp[i];
				anotherTmp >> lit[i];
				anotherTmp >> realyTmp;
				counter[i] = atoi(realyTmp);
				anotherTmp.str("");
				anotherTmp.clear();
				anotherTmp.seekg(0);
			}
		}



		});


	setlocale(LC_ALL, "Ukrainian");
	MSG msg;
	// Регістрація класа вікна 
	MyRegisterClass(hInstance);
	// Створення вікна додатку
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обробки повідомлень
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//closesocket(sHandle);
	std::unique_lock<std::mutex> lock(mtx);
	LiveOfThread1 = false;
	cv.notify_one();
	lock.unlock();
	t1.join();
	return msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; // стиль вікна
	wcex.style = CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC)WndProc; // вікона процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_ERROR);
	wcex.hCursor = LoadCursor(NULL, IDC_ICON);
	wcex.hbrBackground = GetSysColorBrush(8);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;// им’я класа
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}
// Створює вікно додатку
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance;
	hWnd = CreateWindow(szWindowClass, // им’я класу вікна
		szTitle, // им’я додатку
		WS_OVERLAPPEDWINDOW |
		WS_VSCROLL, // стиль вікна
		CW_USEDEFAULT, // положення по Х
		CW_USEDEFAULT, // положення по Y
		1500, // розмір по Х
		800, // розмір по Y
		NULL,
		NULL,
		hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

// Вікона процедура. Приймає та обробляє всі повідомлення
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	unsigned int key;
	setlocale(LC_ALL, "Ukrainian");

	hook = SetWindowsHookEx(WH_KEYBOARD_LL, Hook, GetModuleHandle(NULL), 0);


	int max1 = 0, max2 = 0, max3 = 0;
	int maxi1, maxi2, maxi3;
	


	switch (message)
	{
	case WM_LBUTTONDBLCLK:
		for(int i = 0; i < MaxI +1; i++)
		{
			if(counter[i] >max1)
			{
				max1 = counter[i];
				maxi1 = i;
			}
		}
		for (int i = 0; i < MaxI + 1; i++)
		{
			if (counter[i] > max2 && max1 > counter[i])
			{
				max2 = counter[i];
				maxi2 = i;
			}
		}
		for (int i = 0; i < MaxI + 1; i++)
		{
			if (counter[i] > max3 && max1 > counter[i] && max2 > counter[i])
			{
				max3 = counter[i];
				maxi3 = i;
			}
		}
		HKEY MyKey;
		LONG ErrorCode;
		DWORD Disp;

		ErrorCode = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\LAB"),
			0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &MyKey, &Disp);

		ErrorCode = RegSetValueEx(MyKey,(LPCWSTR)lit[maxi1].c_str(), 0, REG_DWORD, (const BYTE*)&counter[maxi1], sizeof(n));
		ErrorCode = RegSetValueEx(MyKey, (LPCWSTR)lit[maxi2].c_str(), 0, REG_DWORD, (const BYTE*)&counter[maxi2], sizeof(n));
		ErrorCode = RegSetValueEx(MyKey, (LPCWSTR)lit[maxi3].c_str(), 0, REG_DWORD, (const BYTE*)&counter[maxi3], sizeof(n));
		RegCloseKey(MyKey);
		break;
	case WM_COMMAND:
		switch (wParam) {
		case OnMenuAction4:
			MessageBoxA(hWnd, "Програму створив Захаров Володимир Вадимович студент ХНУРЕ з групи ІТІНФ-21-2", "Menu worked", MB_OK);
			break;
		default: break;
		}

	case WM_CREATE:
		MainWndAddMenus(hWnd);
		break;
	case WM_DESTROY:
		UnhookWindowsHookEx(hook);
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);

			//MessageBox(NULL, tchar1, L"Server data", MB_OK);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void MainWndAddMenus(HWND hwnd) {
	HMENU RootMenu = CreateMenu();

	HMENU SubMenu = CreateMenu();
	AppendMenu(SubMenu, MF_STRING, OnMenuAction4, L"Про програму");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
	SetMenu(hwnd, RootMenu);
}



LRESULT CALLBACK Hook(int code, WPARAM wParam, LPARAM lParam)
{
	setlocale(LC_ALL, "Ukrainian");
	BeginPaint(hWnd, &ps);
	LPKBDLLHOOKSTRUCT l = (LPKBDLLHOOKSTRUCT)lParam;
	if (wParam == WM_KEYDOWN)
	{
		if (l->vkCode == 'S' && GetKeyState(VK_CONTROL))
		{
			HDC hDC;
			hDC = GetDC(hWnd);
			for (int i = 0; i < MaxI + 1; i++) {
				std::wstring tmp;
				tmp = s2ws(lit[i]);
				Rectangle(ps.hdc, 10 + 20 * i, 600 - 3 * counter[i], 20 + 20 * i, 600);
				TextOut(ps.hdc, 10 + 20 * i, 610, (LPCWSTR)tmp.c_str(), 2);
			}
		}
	}

	// Передадим перехваченное сообщение дальше.
	return CallNextHookEx(NULL, code, wParam, lParam);
}
