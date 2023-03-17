#define LETTERSAMOUNT 5
#define WINDOWAMOUNT 4
#define MAXWORDS 10

#define TILESIZE 55
#define MARGIN 6

#define CHARSIZE 128

#pragma once

#include "resource.h"
#include <fstream>
#include <set>
#include <string>
#include <algorithm>

// curently checked level option 
UINT currentlyChecked = IDM_DIFFICULTY_EASY;
// amount of rows displayed in the windows
int wordCount = 6;
// handles to game windows
HWND gameWindows[WINDOWAMOUNT];
// blank that will be filled next
int currentLetter = 0;
// line we are in
int currentLine[WINDOWAMOUNT] = {};
// letters written to the windows
char letters[MAXWORDS][LETTERSAMOUNT];
// blanks rectangles
RECT letterRect[MAXWORDS][LETTERSAMOUNT];
// dictionary containing all the words
std::set<std::string> dictionary;
// keyboard rectangles
RECT keyboard[CHARSIZE][WINDOWAMOUNT + 1];
// letters in the right order
char keyboardLetters[26] =
{ 'q', 'w', 'e', 'r', 't', 'y','u', 'i', 'o', 'p',
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
  'z', 'x', 'c', 'v', 'b', 'n', 'm' };
// words to be guessed
std::string chosenWords[WINDOWAMOUNT];
// colors assigned to letter and window
int color[WINDOWAMOUNT][MAXWORDS][LETTERSAMOUNT] = {};
// is the game in this window finished
bool finished[WINDOWAMOUNT] = {};
// color of the letter on the keyboard
int letterColor[WINDOWAMOUNT][CHARSIZE] = {};
// animation rectagle filling percentage
float percent = 0;
// letter that is currently animated
int animationLetter = 0;

bool animate[WINDOWAMOUNT] = {};



void setEasyLevel(HWND gameWindows[], int nCmdShow, int windowAmount, int rowHeight, int& wordCount)
{
	wordCount = 6;
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
void setMediumLevel(HWND gameWindows[], int nCmdShow, int windowAmount, int rowHeight, int& wordCount)
{
	wordCount = 8;
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
void setHardLevel(HWND gameWindows[], int nCmdShow, int windowAmount, int rowHeight, int& wordCount)
{
	wordCount = 10;
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
	int copied = GetPrivateProfileStringA("start", "config", NULL, sResult, 255, "./config.ini");

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
	{
		WritePrivateProfileStringA("start", "config", "EASY", "./config.ini");
		return;
	}
	if (currentlyChecked == IDM_DIFFICULTY_MEDIUM)
	{
		WritePrivateProfileStringA("start", "config", "MEDIUM", "./config.ini");
		return;
	}
	if (currentlyChecked == IDM_DIFFICULTY_HARD)
	{
		WritePrivateProfileStringA("start", "config", "HARD", "./config.ini");
		return; 
	}
}

void LoadDictionary(std::set<std::string>& dictionary)
{
	std::ifstream in{ "Wordle.txt" };
	std::string s;
	while(!in.eof())
	{
		in >> s;
		dictionary.insert(s);
	}
}

void StartGame(int currentLine[], int& currentLetter, std::string chosenWords[], UINT currentLevel, std::set<std::string>& dictionary,
	char letters[MAXWORDS][LETTERSAMOUNT], int color[WINDOWAMOUNT][MAXWORDS][LETTERSAMOUNT], bool finished[], int letterColor[WINDOWAMOUNT][128])
{

	currentLetter = 0;

	for (int i = 0; i < WINDOWAMOUNT; i++)
	{
		currentLine[i] = 0;
		finished[i] = false;
	}
	for (int i = 0; i < WINDOWAMOUNT; i++)
	{
		for (int j = 0; j < MAXWORDS; j++)
		{
			for (int k = 0; k < LETTERSAMOUNT; k++)
			{
				color[i][j][k] = 0;
			}
		}
	}
	for (int i = 0; i < WINDOWAMOUNT; i++)
	{
		for (int j = 0; j < 128; j++)
			letterColor[i][j] = 0;
	}
	for (int i = 0; i < MAXWORDS; i++)
	{
		for (int j = 0; j < LETTERSAMOUNT; j++)
		{
			letters[i][j] = 0;
		}
	}

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


void setOverlayWin(HWND hwnd, HDC& hdc)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	rc = { 0, 0, rc.right - rc.left, rc.bottom - rc.top };


	HDC hdcOverlay = CreateCompatibleDC(hdc);

	//https://stackoverflow.com/questions/7502588/createcompatiblebitmap-and-createdibsection-memory-dcs

	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = rc.right;
	bmpInfo.bmiHeader.biHeight = rc.bottom;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;

	HBITMAP hBitmap = CreateDIBSection(hdcOverlay, &bmpInfo, DIB_RGB_COLORS, nullptr, nullptr, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcOverlay, hBitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0)); 
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdcOverlay, hBrush);
	HPEN oldPen = (HPEN)(SelectObject(hdcOverlay, GetStockObject(DC_PEN)));
	SetDCPenColor(hdcOverlay, RGB(0, 255, 0));

	Rectangle(hdcOverlay, rc.left, rc.top, rc.right, rc.bottom);

	BLENDFUNCTION blendFunc = {};
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = 150;

	AlphaBlend(hdc, 0, 0, rc.right, rc.bottom, hdcOverlay, 0, 0, rc.right, rc.bottom, blendFunc);
	DeleteObject(hBrush);
	SelectObject(hdcOverlay, oldBrush);
	SelectObject(hdcOverlay, oldPen);
}

void setOverlayLose(HWND hwnd, HDC& hdc, std::string word)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	rc = { 0, 0, rc.right - rc.left, rc.bottom - rc.top };


	HDC hdcOverlay = CreateCompatibleDC(hdc);
	//https://stackoverflow.com/questions/7502588/createcompatiblebitmap-and-createdibsection-memory-dcs

	BITMAPINFO bmpInfo = {};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = rc.right;
	bmpInfo.bmiHeader.biHeight = rc.bottom;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;

	HBITMAP hBitmap = CreateDIBSection(hdcOverlay, &bmpInfo, DIB_RGB_COLORS, nullptr, nullptr, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcOverlay, hBitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdcOverlay, hBrush);
	HPEN oldPen = (HPEN)(SelectObject(hdcOverlay, GetStockObject(DC_PEN)));
	SetDCPenColor(hdcOverlay, RGB(255, 0, 0));

	Rectangle(hdcOverlay, rc.left, rc.top, rc.right, rc.bottom);
	BLENDFUNCTION blendFunc = {};
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	blendFunc.SourceConstantAlpha = 150;

	AlphaBlend(hdc, 0, 0, rc.right, rc.bottom, hdcOverlay, 0, 0, rc.right, rc.bottom, blendFunc);

	SetBkMode(hdc, TRANSPARENT);
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	std::wstring widestr = std::wstring(word.begin(), word.end());
	DrawText(hdc, widestr.c_str(), 5, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DeleteObject(hBrush);
	SelectObject(hdcOverlay, oldBrush);
	SelectObject(hdcOverlay, oldPen);
}