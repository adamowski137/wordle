#pragma once

#include "resource.h"
#include <fstream>
#include <set>
#include <string>


void setEasyLevel(HWND gameWindows[], int nCmdShow, int windowAmount, int rowHeight)
{
	for (int i = 0; i < windowAmount; i++)
	{
		ShowWindow(gameWindows[i], SW_HIDE);
	}
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	RECT windowRc;
	GetWindowRect(gameWindows[0], &windowRc);

	int windowWidth = windowRc.right - windowRc.left;
	int windowHeight = 6 * rowHeight + 6;
	windowRc = { 0, 0, 0, windowHeight};
	AdjustWindowRectEx(&windowRc, WS_OVERLAPPEDWINDOW, false, 0);
	windowHeight = windowRc.bottom - windowRc.top;
	
	MoveWindow(gameWindows[0], ((rc.left + rc.right) / 2) - (windowWidth) / 2, ((rc.bottom + rc.top) / 2) - ((windowHeight) / 2), windowWidth, windowHeight, true);
	
	ShowWindow(gameWindows[0], nCmdShow);
	
	UpdateWindow(gameWindows[0]);
}
void setMediumLevel(HWND gameWindows[], int nCmdShow, int windowAmount, int rowHeight)
{
	for (int i = 0; i < windowAmount; i++)
	{
		ShowWindow(gameWindows[i], SW_HIDE);
	}

	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	RECT windowRc;
	GetWindowRect(gameWindows[0], &windowRc);

	int windowWidth = windowRc.right - windowRc.left;
	int windowHeight = 8 * rowHeight + 6;
	windowRc = { 0, 0, 0, windowHeight };
	AdjustWindowRectEx(&windowRc, WS_OVERLAPPEDWINDOW, false, 0);
	windowHeight = windowRc.bottom - windowRc.top;

	MoveWindow(gameWindows[0], (  (rc.left + rc.right) / 4) - (windowWidth) / 2, ((rc.bottom + rc.top) / 2) - ((windowHeight) / 2), windowWidth, windowHeight, true);
	MoveWindow(gameWindows[1], (3*(rc.left + rc.right) / 4) - (windowWidth) / 2, ((rc.bottom + rc.top) / 2) - ((windowHeight) / 2), windowWidth, windowHeight, true);
	
	ShowWindow(gameWindows[0], nCmdShow);
	ShowWindow(gameWindows[1], nCmdShow);
	
	UpdateWindow(gameWindows[0]);
	UpdateWindow(gameWindows[1]);
}
void setHardLevel(HWND gameWindows[], int nCmdShow, int windowAmount, int rowHeight)
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	
	RECT windowRc;
	GetWindowRect(gameWindows[0], &windowRc);
	
	int windowWidth = windowRc.right - windowRc.left;
	int windowHeight = 10 * rowHeight + 6;
	windowRc = { 0, 0, 0, windowHeight };
	AdjustWindowRectEx(&windowRc, WS_OVERLAPPEDWINDOW, false, 0);
	windowHeight = windowRc.bottom - windowRc.top;
	
	MoveWindow(gameWindows[0], (    (rc.left + rc.right) / 4) - (windowWidth) / 2, 0, windowWidth, windowHeight, true);
	MoveWindow(gameWindows[1], (3 * (rc.left + rc.right) / 4) - (windowWidth) / 2, 0, windowWidth, windowHeight, true);
	MoveWindow(gameWindows[2], (    (rc.left + rc.right) / 4) - (windowWidth) / 2, (rc.bottom + rc.left)/2, windowWidth, windowHeight, true);
	MoveWindow(gameWindows[3], (3 * (rc.left + rc.right) / 4) - (windowWidth) / 2, (rc.bottom + rc.left)/2, windowWidth, windowHeight, true);
	
	ShowWindow(gameWindows[0], nCmdShow);
	ShowWindow(gameWindows[1], nCmdShow);
	ShowWindow(gameWindows[2], nCmdShow);
	ShowWindow(gameWindows[3], nCmdShow);

	UpdateWindow(gameWindows[0]);
	UpdateWindow(gameWindows[1]);
	UpdateWindow(gameWindows[2]);
	UpdateWindow(gameWindows[3]);

}
void loadConfig(UINT& currentlyChecked, HWND hWnd, int& wordCount)
{
	const char hard[5] = "HARD";
	const char medium[7] = "MEDIUM";
	const char easy[5] = "EASY";


	char sResult[255] = {};
	int copied = GetPrivateProfileStringA("start", "config", NULL, sResult, 255, "C:/Users/adam/Desktop/config.ini");

	if (sResult[0] == hard[0])
	{
		currentlyChecked = IDM_DIFFICULTY_HARD;
		wordCount = 10;
	}
	else if (sResult[0] == medium[0])
	{
		currentlyChecked = IDM_DIFFICULTY_MEDIUM;
		wordCount = 8;
	}
	else if (sResult[0] == easy[0])
	{
		currentlyChecked = IDM_DIFFICULTY_EASY;
		wordCount = 6;
	}
	else
	{
		currentlyChecked = IDM_DIFFICULTY_EASY;
		wordCount = 6;
	}
	HMENU menu = GetMenu(hWnd);
	CheckMenuItem(menu, currentlyChecked, MF_CHECKED);
	
}

void saveConfig(UINT& currentlyChecked)
{
	if (currentlyChecked == IDM_DIFFICULTY_EASY)
		WritePrivateProfileStringA("start", "config", "EASY", "C:/Users/adam/Desktop/config.ini");
	if (currentlyChecked == IDM_DIFFICULTY_MEDIUM)
		WritePrivateProfileStringA("start", "config", "MEDIUM", "C:/Users/adam/Desktop/config.ini");
	if (currentlyChecked == IDM_DIFFICULTY_HARD)
		WritePrivateProfileStringA("start", "config", "HARD", "C:/Users/adam/Desktop/config.ini");
}

void LoadDictionary(std::set<std::string>& dictionary)
{
	std::ifstream in{ "../../WinApi_Home/WinApi_Home/Wordle.txt" };
	std::string s;
	while(!in.eof())
	{
		in >> s;
		dictionary.insert(s);
	}
}

void StartGame(int currentLine[], int& currentLetter, std::string chosenWords[], UINT currentLevel, std::set<std::string>& dictionary)
{
	currentLine = {};
	currentLetter = 0;
	int n = dictionary.size();
	int r = rand() % n;

	auto it = dictionary.begin();
	for (int i = 0; i < r; i++)
	{
		it++;
	}
	chosenWords[0] = *it;
	chosenWords[1] = *it;
	chosenWords[2] = *it;
	chosenWords[3] = *it;

	if (currentLevel == IDM_DIFFICULTY_EASY)
		return;

	r = rand() % n;
	it = dictionary.begin();
	for (int i = 0; i < r; i++)
	{
		it++;
	}
	chosenWords[1] = *it;
	chosenWords[3] = *it;

	if (currentLevel == IDM_DIFFICULTY_MEDIUM)
		return;

	r = rand() % n;
	it = dictionary.begin();
	for (int i = 0; i < r; i++)
	{
		it++;
	}
	chosenWords[2] = *it;

	r = rand() % n;
	it = dictionary.begin();
	for (int i = 0; i < r; i++)
	{
		it++;
	}
	chosenWords[3] = (*it);
}

bool contains(char c, std::string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (c == s[i]) return true;
	}
	return false;
}

void setOverlay(HWND hWnd, HDC hdc, COLORREF color, HINSTANCE hInstance)
{
	RECT rc;
	GetWindowRect(hWnd, &rc);
	HWND overlayWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT,
		L"GAME CLASS", L"sad", NULL,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		hWnd, NULL, hInstance, NULL);
	SetLayeredWindowAttributes(overlayWindow, RGB(0, 0, 0), 0, LWA_COLORKEY);
	
	HDC hdcScreen = GetDC(NULL);
	HDC hdcOverlay = CreateCompatibleDC(hdcScreen);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdcOverlay, hBrush);

	RECT rect = { 0, 0, rc.right - rc.left, rc.bottom - rc.top};
	FillRect(hdcOverlay, &rect, hBrush);


	BLENDFUNCTION blendFunc;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = 255;
	blendFunc.AlphaFormat = AC_SRC_ALPHA;

	POINT ptSrc = { 0, 0 };
	SIZE sizeWnd = { rc.right - rc.left, rc.bottom - rc.top};
	POINT ptDst = { rc.left, rc.top };

	UpdateLayeredWindow(overlayWindow, hdcScreen, &ptDst, &sizeWnd, hdcOverlay, &ptSrc, RGB(0, 0, 0), &blendFunc, ULW_ALPHA);

	DeleteDC(hdcOverlay);
	DeleteObject(hBrush);
	ReleaseDC(NULL, hdcScreen);

	SetWindowPos(overlayWindow, hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOACTIVATE | SWP_NOREDRAW);
	ShowWindow(overlayWindow, SW_SHOW);
}