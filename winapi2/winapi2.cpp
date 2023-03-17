// winapi2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "winapi2.h"
#include <set>
#include <string>
#include <map>
#include <time.h>

#define MAX_LOADSTRING 100

#define GAMETIME_ID 1

#define LETTERSAMOUNT 5
#define WINDOWAMOUNT 4
#define MAXWORDS 10

#define TILESIZE 55
#define MARGIN 6

#define ROWHEIGHT TILESIZE + MARGIN
#define ROWWIDTH (TILESIZE * LETTERSAMOUNT) + ((LETTERSAMOUNT + 1) * MARGIN)

#define KEYBOARDWIDTH 700
#define KEYBOARDHEIGHT 300


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterClassGame(HINSTANCE hInstance);
ATOM                MyRegisterClassOverlay(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcGame(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcOverlay(HWND, UINT, WPARAM, LPARAM);



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    srand(time(NULL));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClassGame(hInstance);
    MyRegisterClassOverlay(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPI2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM MyRegisterClassGame(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcGame;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255,255,255)));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"GAME CLASS";
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}
ATOM MyRegisterClassOverlay(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcOverlay;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(0, 255, 0)));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"OVERLAY WINDOW";
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   RECT rc;
   SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      (rc.right + rc.left)/2 - KEYBOARDWIDTH/2, rc.bottom - KEYBOARDHEIGHT, KEYBOARDWIDTH, KEYBOARDHEIGHT, nullptr, nullptr, hInstance, nullptr);
   
   rc = { 0, 0, ROWWIDTH, 0};
   AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, false, 0);
  
   for (int i = 0; i < WINDOWAMOUNT; i++)
   {
       gameWindows[i] = CreateWindowW(L"GAME CLASS", L"WORDLE - PUZZLE",  WS_CAPTION,
           0, 0, rc.right - rc.left, 0, hWnd, nullptr, hInst, nullptr);
   }

   if (!hWnd)
   {
      return FALSE;
   }
   SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE ) | WS_EX_LAYERED);
   SetLayeredWindowAttributes(hWnd, 0, (255 * 50) / 100, LWA_ALPHA);
   
   loadConfig(currentlyChecked, hWnd, wordCount);
   LoadDictionary(dictionary);

   if(currentlyChecked == IDM_DIFFICULTY_EASY)
    setEasyLevel(gameWindows, nCmdShow, WINDOWAMOUNT, ROWHEIGHT, wordCount);
   if (currentlyChecked == IDM_DIFFICULTY_MEDIUM)
       setMediumLevel(gameWindows, nCmdShow, WINDOWAMOUNT, ROWHEIGHT, wordCount);
   if (currentlyChecked == IDM_DIFFICULTY_HARD)
       setHardLevel(gameWindows, nCmdShow, WINDOWAMOUNT, ROWHEIGHT, wordCount);
   StartGame(currentLine, currentLetter, chosenWords, currentlyChecked, dictionary, letters, color, finished, letterColor);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            HMENU menu = GetMenu(hWnd);
            if (currentlyChecked != wmId)
            {
                CheckMenuItem(menu, wmId, MF_CHECKED);
                CheckMenuItem(menu, currentlyChecked, MF_UNCHECKED);
                currentlyChecked = wmId;
            }
            switch (wmId)
            {
            case IDM_DIFFICULTY_EASY: 

                setEasyLevel(gameWindows, SW_SHOW, WINDOWAMOUNT, ROWHEIGHT, wordCount);
                break;
            case IDM_DIFFICULTY_MEDIUM:

                setMediumLevel(gameWindows, SW_SHOW, WINDOWAMOUNT, ROWHEIGHT, wordCount);
                break;
            case IDM_DIFFICULTY_HARD:

                setHardLevel(gameWindows, SW_SHOW, WINDOWAMOUNT, ROWHEIGHT, wordCount);
                break;
            default: 
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        StartGame(currentLine, currentLetter, chosenWords, currentlyChecked, dictionary, letters, color, finished, letterColor);
        InvalidateRect(hWnd, NULL, true);
        for (int j = 0; j < WINDOWAMOUNT; j++)
            InvalidateRect(gameWindows[j], NULL, true);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC offDC;
            HBITMAP offOldBitmap;
            HBITMAP offBitmap;
            RECT rc;

            GetClientRect(hWnd, &rc);
            offDC = CreateCompatibleDC(hdc);
            offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
            offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);
            HPEN oldPen = (HPEN)(SelectObject(offDC, GetStockObject(DC_PEN)));
            HBRUSH oldBrush = (HBRUSH)SelectObject(offDC, GetStockObject(DC_BRUSH));

            SetDCBrushColor(offDC, RGB(255, 255, 255));
            SetDCPenColor(offDC, RGB(255, 255, 255));
            Rectangle(offDC, 0, 0, rc.right, rc.bottom);
            
            for (int i = 0; i < 26; i++)
            {
                HPEN oldPen = (HPEN)(SelectObject(offDC, GetStockObject(DC_PEN)));
                HBRUSH oldBrush = (HBRUSH)SelectObject(offDC, GetStockObject(DC_BRUSH));
                SetDCPenColor(offDC, RGB(164, 174, 196));
                SetDCBrushColor(offDC, RGB(251, 251, 255));
                RoundRect(offDC, keyboard[keyboardLetters[i]][WINDOWAMOUNT].left, keyboard[keyboardLetters[i]][WINDOWAMOUNT].top,
                    keyboard[keyboardLetters[i]][WINDOWAMOUNT].right, keyboard[keyboardLetters[i]][WINDOWAMOUNT].bottom,
                    MARGIN, MARGIN);
                oldPen = (HPEN)(SelectObject(offDC, GetStockObject(NULL_PEN)));
                for (int j = 0; j < WINDOWAMOUNT; j++)
                {
                    if(letterColor[j][keyboardLetters[i]] == 2)
                        SetDCBrushColor(offDC, RGB(121, 184, 81));
                    else if (letterColor[j][keyboardLetters[i]] == 1)
                        SetDCBrushColor(offDC, RGB(243, 194, 55));
                    else if (letterColor[j][keyboardLetters[i]] == -1)
                        SetDCBrushColor(offDC, RGB(164, 174, 196));
                    RoundRect(offDC, keyboard[keyboardLetters[i]][j].left, keyboard[keyboardLetters[i]][j].top,
                        keyboard[keyboardLetters[i]][j].right, keyboard[keyboardLetters[i]][j].bottom, MARGIN/4, MARGIN/4);

                }
                wchar_t s[2];
                swprintf_s(s, 2, L"%c", toupper(keyboardLetters[i]));
                SetBkMode(offDC, TRANSPARENT);
                DrawText(offDC, s, 1, &keyboard[keyboardLetters[i]][WINDOWAMOUNT], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);
                SelectObject(offDC, oldBrush);
                SelectObject(offDC, oldPen);
            }
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_CREATE:
    {
        int posY = MARGIN;
        for (int i = 0; i < MAXWORDS; i++)
        {
            int posX = MARGIN;
            for (int j = 0; j < LETTERSAMOUNT; j++)
            {
                RECT rc = { posX, posY, posX + TILESIZE, posY + TILESIZE };
                letterRect[i][j] = rc;
                posX += TILESIZE + MARGIN;
            }
            posY += TILESIZE + MARGIN;
        }


        RECT rc;
        GetClientRect(hWnd, &rc);

        int start = ((rc.right - rc.left) - 10 * (TILESIZE + MARGIN)) / 2;
        for (int i = 0; i < 10; i++)
        {
            RECT rect = { start, MARGIN, start + TILESIZE, TILESIZE + MARGIN };
            keyboard[keyboardLetters[i]][WINDOWAMOUNT] = rect;
            RECT rect1 = { rect.left + 1, rect.top + 1, (rect.left + rect.right) / 2 + 1, (rect.bottom + rect.top) / 2 + 1 };
            keyboard[keyboardLetters[i]][0] = rect1;
            RECT rect2 = { (rect.left + rect.right) / 2 - 1, rect.top + 1, rect.right, (rect.bottom + rect.top) / 2 + 1};
            keyboard[keyboardLetters[i]][1] = rect2;
            RECT rect3 = { rect.left + 1, (rect.bottom + rect.top) / 2, (rect.left + rect.right) / 2 + 1, rect.bottom };
            keyboard[keyboardLetters[i]][2] = rect3;
            RECT rect4 = { (rect.left + rect.right) / 2 - 1, (rect.bottom + rect.top) / 2 - 1, rect.right, rect.bottom };
            keyboard[keyboardLetters[i]][3] = rect4;

            start += TILESIZE + MARGIN;
        }
        start = ((rc.right - rc.left) - 9 * (TILESIZE + MARGIN)) / 2;
        for (int i = 0; i < 9; i++)
        {
            RECT rect = { start, TILESIZE + 2 * MARGIN, start + TILESIZE,  2 * TILESIZE + 2 * MARGIN };
            keyboard[keyboardLetters[i + 10]][WINDOWAMOUNT] = rect;
            RECT rect1 = { rect.left + 1, rect.top + 1, (rect.left + rect.right) / 2 + 1, (rect.bottom + rect.top) / 2 + 1};
            keyboard[keyboardLetters[i + 10]][0] = rect1;
            RECT rect2 = { (rect.left + rect.right) / 2 - 1, rect.top + 1, rect.right, (rect.bottom + rect.top) / 2 + 1};
            keyboard[keyboardLetters[i + 10]][1] = rect2;
            RECT rect3 = { rect.left + 1, (rect.bottom + rect.top) / 2, (rect.left + rect.right) / 2 + 1, rect.bottom };
            keyboard[keyboardLetters[i + 10]][2] = rect3;
            RECT rect4 = { (rect.left + rect.right) / 2 - 1, (rect.bottom + rect.top) / 2 - 1, rect.right, rect.bottom };
            keyboard[keyboardLetters[i + 10]][3] = rect4;
            start += TILESIZE + MARGIN;
        }
        start = ((rc.right - rc.left) - 7 * (TILESIZE + MARGIN)) / 2;
        for (int i = 0; i < 7; i++)
        {
            RECT rect = { start, 2 * TILESIZE + 3 * MARGIN, start + TILESIZE, 3 * TILESIZE + 3 * MARGIN };
            keyboard[keyboardLetters[i + 19]][WINDOWAMOUNT] = rect;
            RECT rect1 = { rect.left + 1, rect.top + 1, (rect.left + rect.right) / 2 + 1, (rect.bottom + rect.top) / 2 + 1 };
            keyboard[keyboardLetters[i + 19]][0] = rect1;
            RECT rect2 = { (rect.left + rect.right) / 2 - 1, rect.top + 1, rect.right, (rect.bottom + rect.top) / 2 + 1};
            keyboard[keyboardLetters[i + 19]][1] = rect2;
            RECT rect3 = { rect.left + 1, (rect.bottom + rect.top) / 2, (rect.left + rect.right) / 2 + 1, rect.bottom };
            keyboard[keyboardLetters[i + 19]][2] = rect3;
            RECT rect4 = { (rect.left + rect.right) / 2 - 1, (rect.bottom + rect.top) / 2 - 1, rect.right, rect.bottom };
            keyboard[keyboardLetters[i + 19]][3] = rect4;
            start += TILESIZE + MARGIN;
        }
    }
        break;
    
    case WM_CHAR:
        if (wParam == VK_RETURN)
        {
            if (currentLetter == LETTERSAMOUNT)
            {
                for (int j = 0; j < WINDOWAMOUNT; j++)
                {
                    if (finished[j]) continue;
                    std::string s{ letters[currentLine[j]] };
            
                    auto t = dictionary.find(s);
                    if (t != dictionary.end())
                    {
                        for (int i = 0; i < LETTERSAMOUNT; i++)
                        {
                            char let = s[i];
                            if (contains(s[i], chosenWords[j]))
                            {
                                if (s[i] == chosenWords[j][i])
                                {
                                    color[j][currentLine[j]][i] = 2;
                                    letterColor[j][let] = 2;
                                }
                                else
                                {
                                    color[j][currentLine[j]][i] = 1 ;
                                    letterColor[j][let] = max(1, letterColor[j][let]);
                                }
                            }
                            else
                            {
                                color[j][currentLine[j]][i] = -1;
                                letterColor[j][let] = -1;
                            }
                        }
                        if(finished[j] == false)
                            animate[j] = true;
                        if (s == chosenWords[j])
                        {
                            finished[j] = true;
                            animate[j] = true;
                        }
                        currentLine[j]++;
                        currentLetter = 0;
                        
                        percent = 0;
                        SetTimer(hWnd, 1, 10, NULL);
                        InvalidateRect(gameWindows[j], NULL, TRUE);
                        InvalidateRect(hWnd, NULL, true);
                        
                    }
                    else
                    {
                        currentLetter = 0;    
                        InvalidateRect(gameWindows[j], NULL, TRUE);
                    }
                }
                
            }
        }
        else if (wParam == VK_BACK)
        {
            if (currentLetter > 0)
            {
                currentLetter--;
                for (int i = 0; i < WINDOWAMOUNT; i++)
                {
                    if (finished[i]) continue;
                    letters[currentLine[i]][currentLetter] = NULL;
                    InvalidateRect(gameWindows[i], NULL, TRUE);
                }
            }
        }
        else if (currentLetter < LETTERSAMOUNT && isalpha((char)wParam))
        {
            currentLetter++;
            for (int i = 0; i < WINDOWAMOUNT; i++)
            {
                if (finished[i]) continue;

                letters[currentLine[i]][currentLetter - 1] = (char)wParam;
                InvalidateRect(gameWindows[i], NULL, TRUE);
            }
        }
        break;
    case WM_DESTROY:
        saveConfig(currentlyChecked);
        break;

    case WM_TIMER:
        
        if (percent > 1)
        {
            percent = 0;
            animationLetter++;
        }

        if (animationLetter >= LETTERSAMOUNT)
        {
                KillTimer(hWnd, 1);
                percent = 0;
                animationLetter = 0;
                for(int i = 0; i < WINDOWAMOUNT; i++)
                    animate[i] = false;
        }
        else
            percent += 0.1;
        for (int i = 0; i < WINDOWAMOUNT; i++)
        {
            InvalidateRect(gameWindows[i], NULL, true);
        }
        
        
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcGame(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_NCHITTEST: {
        LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
        if (hit == HTCLIENT) 
            hit = HTCAPTION;
        return hit;
    }
    case WM_PAINT:
    {
        int k = 0;
        if (hWnd == gameWindows[1]) k = 1;
        if (hWnd == gameWindows[2]) k = 2;
        if (hWnd == gameWindows[3]) k = 3;
        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        
        HDC offDC;
        HBITMAP offOldBitmap;
        HBITMAP offBitmap;
        RECT rc;

        GetClientRect(hWnd, &rc);
        offDC = CreateCompatibleDC(hdc);
        offBitmap = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
        offOldBitmap = (HBITMAP)SelectObject(offDC, offBitmap);
        HPEN oldPen = (HPEN) (SelectObject(offDC, GetStockObject(DC_PEN)));
        HBRUSH oldBrush = (HBRUSH) SelectObject(offDC, GetStockObject(DC_BRUSH));
        
        SetDCBrushColor(offDC, RGB(255, 255, 255));
        SetDCPenColor(offDC, RGB(255, 255, 255));
        Rectangle(offDC, 0, 0, rc.right, rc.bottom);


        for (int i = 0; i < wordCount; i++)
        {
            for (int j = 0; j < LETTERSAMOUNT; j++)
            {
                SetDCBrushColor(offDC, RGB(251, 251, 255));
                SetDCPenColor(offDC, RGB(164, 174, 196));
                RoundRect(offDC, letterRect[i][j].left, letterRect[i][j].top, letterRect[i][j].right, letterRect[i][j].bottom, MARGIN, MARGIN);

                if (color[k][i][j] == -1 && (!(i == currentLine[k] -1 && j > animationLetter) ||  animate[k] == false))
                {
                    SetDCBrushColor(offDC, RGB(164, 174, 196));
                    SetDCPenColor(offDC, RGB(164, 174, 196));
                }
                if (color[k][i][j] == 1 && (!(i == currentLine[k] -1 && j > animationLetter) || animate[k] == false))
                {
                    SetDCBrushColor(offDC, RGB(243, 194, 55));
                    SetDCPenColor(offDC, RGB(243, 194, 55));
                }
                if (color[k][i][j] == 2 && (!(i == currentLine[k] - 1&& j > animationLetter) || animate[k] == false))
                {
                    SetDCBrushColor(offDC, RGB(121, 184, 81));
                    SetDCPenColor(offDC, RGB(121, 184, 81));
                }

                if(i == currentLine[k] - 1 && j == animationLetter && animate[k] == true)
                    RoundRect(offDC, letterRect[i][j].left,
                        (letterRect[i][j].top + letterRect[i][j].bottom) / 2 - (percent * ((letterRect[i][j].bottom - letterRect[i][j].top) / 2)),
                        letterRect[i][j].right, 
                        (letterRect[i][j].top + letterRect[i][j].bottom) / 2 + (percent) * (letterRect[i][j].bottom - letterRect[i][j].top)/2,
                        MARGIN, MARGIN);
                else
                    RoundRect(offDC, letterRect[i][j].left, letterRect[i][j].top , letterRect[i][j].right, letterRect[i][j].bottom, MARGIN, MARGIN);



                if (i < currentLine[k] || (i == currentLine[k] && j < currentLetter))
                {
                    wchar_t s[2];
                    swprintf_s(s, 2, L"%c", toupper(letters[i][j]));
                    SetBkMode(offDC, TRANSPARENT);
                    DrawText(offDC, s, 1, &letterRect[i][j], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
        }
        if (finished[k] && animate[k] == false)
            setOverlayWin(hWnd, offDC);
        else if (currentLine[k] >= wordCount && animate[k] == false)
        {
            setOverlayLose(hWnd, offDC, chosenWords[k]);
        }
        BitBlt(hdc, 0, 0, rc.right, rc.bottom, offDC, 0, 0, SRCCOPY);
        SelectObject(offDC, oldPen);
        SelectObject(offDC, oldBrush);

        
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
    {
        HDC hdc = GetDC(hWnd);
        
        ReleaseDC(hWnd, hdc);
        break;
 
    }
    case WM_TIMER:
    {
       
    }
    break;
    case WM_ERASEBKGND:
        return 1;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK WndProcOverlay(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}