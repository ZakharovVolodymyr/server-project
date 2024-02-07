//Header.h
#include <iostream>
#include <Windows.h>
#pragma once

extern "C" _declspec(dllexport) void KilASlov(std::string&);
extern "C" _declspec(dllexport) void KilUSlov(std::string&);
extern "C" _declspec(dllexport) void out(std::string&);