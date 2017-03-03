// Roguelike.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "Roguelike.h"
#include "Parameters.h"
#include <atlimage.h>

#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;								// 目前執行個體
TCHAR szTitle[MAX_LOADSTRING];					// 標題列文字
TCHAR szWindowClass[MAX_LOADSTRING];			// 主視窗類別名稱

// 這個程式碼模組中所包含之函式的向前宣告:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此置入程式碼。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全域字串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ROGUELIKE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 執行應用程式初始設定:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ROGUELIKE));

	// 主訊息迴圈:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  函式: MyRegisterClass()
//
//  用途: 註冊視窗類別。
//
//  註解:
//
//    只有當您希望此程式碼能相容比 Windows 95 的 
//    'RegisterClassEx' 函式更早的 Win32 系統時，
//    才會需要加入及使用這個函式。
//    您必須呼叫這個函式，讓應用程式取得與它相關的 
//    「格式正確」的圖示。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ROGUELIKE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_ROGUELIKE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函式: InitInstance(HINSTANCE, int)
//
//   用途: 儲存執行個體控制代碼並且建立主視窗
//
//   註解:
//
//        在這個函式中，我們會將執行個體控制代碼儲存在全域變數中，
//        並且建立和顯示主程式視窗。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
   
	m_hMainWnd = hWnd;
	CRandomMap RandomMap;
	InitializePlayer();
	InitializeName();

	m_nShowPlayerPosX = (ShowMapX - 1) / 2;
	m_nShowPlayerPosY = (ShowMapY - 1) / 2;
	m_nMapMaxX = RandomMap.GetRandom(10, 30);
	m_nMapMaxY = RandomMap.GetRandom(10, 20);
	RandomMap.CreateMap(m_nMapMaxX, m_nMapMaxY);

	DungeonMap = RandomMap.GetDungeonMap();
	MonsterMap = RandomMap.GetMonsterMap();	
	TreasureMap = RandomMap.GetTreasureMap();

	ShowDungeonMap = vector<vector<TERRAIN> >(ShowMapX, vector<TERRAIN>(ShowMapY));
	ShowMonsterMap = vector<vector<MONSTER> >(ShowMapX, vector<MONSTER>(ShowMapY));
	ShowTreasureMap = vector<vector<TREASURE> >(ShowMapX, vector<TREASURE>(ShowMapY));

	NowLightMap = vector<vector<BOOL> >(m_nMapMaxX, vector<BOOL>(m_nMapMaxY));
	for (int x = 0; x < ShowMapX; x++)
	{
		for (int y = 0; y < ShowMapY; y++)
		{
			ShowDungeonMap[x][y] = None;
			ShowMonsterMap[x][y] = NoMonster;
			ShowTreasureMap[x][y] = NoTreasure;
		}
	}
	for (int x = 0; x < m_nMapMaxX; x++) for (int y = 0; y < m_nMapMaxY; y++) NowLightMap[x][y] = FALSE;
	RandomMap.GetStartPoint(m_nMapStartX, m_nMapStartY);
	
	m_nPlayerPosX = m_nMapStartX;
	m_nPlayerPosY = m_nMapStartY;
	m_nPlayerLastPosX = -1;
	m_nPlayerLastPosY = -1;
	LightMap(m_nPlayerPosX, m_nPlayerPosY);
	
	MoveWindow(hWnd, 0, 0, WindowSizeX, WindowSizeY, TRUE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	return TRUE;
}

//
//  函式: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  用途:  處理主視窗的訊息。
//
//  WM_COMMAND	- 處理應用程式功能表
//  WM_PAINT	- 繪製主視窗
//  WM_DESTROY	- 顯示結束訊息然後返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	//MoveWindow(hWnd, 0, 0, WindowSizeX, WindowSizeY, TRUE);

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 剖析功能表選取項目:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此加入任何繪圖程式碼...
		{
			// Gathering current client area's info.
			RECT rectClient;
			GetClientRect(hWnd, &rectClient);
			int cx = rectClient.right - rectClient.left;
			int cy = rectClient.bottom - rectClient.top;

			// For comparing the effect, we use if statement to select different code.
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP bmpMem = CreateCompatibleBitmap(hdc, cx, cy);
			SelectObject(hdcMem, bmpMem);
			FillRect(hdcMem, &rectClient, (HBRUSH) GetStockObject(BLACK_BRUSH));
			DrawBackground(hWnd, hdcMem);
			RedrawMap(hWnd, hdcMem);
			DrawPlayer(hWnd, hdcMem);
			m_bRedraweMap = FALSE;
			DrawPlayerStats(hWnd, hdcMem);
			BitBlt(hdc, 0, 0, cx, cy, hdcMem, 0, 0, SRCCOPY);

			// end
			// prevent from GDI objects Leak (can detect by "GDIView, nirsoft")
			DeleteObject(bmpMem);
			DeleteDC(hdcMem);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		KeyDownEvent(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_LBUTTONUP:
		{
		}
		break;
	case WM_ERASEBKGND:
		//避免重繪畫面時因背景重繪造成閃爍
		// Using if statement to enable/disable the erasing.
		if ( !TRUE ) {
		  DefWindowProc(hWnd, message, wParam, lParam);
		} else {
		  return 0;
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// [關於] 方塊的訊息處理常式。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void ShowImageFile(HDC hdc, CString strFile, int nPosX, int nPosY, int nWidth, int nHeight)
{
	WIN32_FIND_DATA pFindFileData;
	HANDLE hFind = FindFirstFile(strFile, &pFindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return;

	CImage img;
	img.Load(strFile);
	if (nWidth = -1) nWidth = img.GetWidth();
	if (nHeight = -1) nHeight = img.GetHeight();                
    img.Draw(hdc, nPosX, nPosY, nWidth, nHeight);
    img.Destroy();
}

void ShowPlayerInMap(HDC hdc, CString strFile, int nPosX, int nPosY, int nWidth, int nHeight)
{
	WIN32_FIND_DATA pFindFileData;
	HANDLE hFind = FindFirstFile(strFile, &pFindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return;
	hFind = FindFirstFile(_T(""), &pFindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return;

	CImage img;
	img.Load(strFile);
	if (nWidth = -1) nWidth = img.GetWidth();
	if (nHeight = -1) nHeight = img.GetHeight();                
    img.Draw(hdc, nPosX, nPosY, nWidth, nHeight);
    img.Destroy();
}

void DrawBackground(HWND hWnd, HDC hdc)
{
	CString Backgroundfile;
	WIN32_FIND_DATA pFindFileData;
	HANDLE hFind;

	Backgroundfile = strResourcePath + _T("mapwindow.png");
	hFind = FindFirstFile(Backgroundfile, &pFindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		ShowImageFile(hdc, Backgroundfile, MapWindowLeft, MapWindowUpper, MapWindowWidth, MapWindowHeight);
	}

	Backgroundfile = strResourcePath + _T("statswindow.png");
	hFind = FindFirstFile(Backgroundfile, &pFindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		ShowImageFile(hdc, Backgroundfile, StatsWindowLeft, StatsWindowUpper, StatsWindowWidth, StatsWindowHeight);
	}
}

void DrawPlayer(HWND hWnd, HDC hdc)
{
	CString playerfile = strResourcePath + _T("player.png");
	WIN32_FIND_DATA pFindFileData;
	HANDLE hFind = FindFirstFile(playerfile, &pFindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		ShowImageFile(hdc, playerfile, m_nShowPlayerPosX * 32 + ShowMapLeft, m_nShowPlayerPosY * 32 + ShowMapUpper, 32, 32);
	}
}

void DrawPlayerStats(HWND hWnd, HDC hdc)
{
	SetTextColor(hdc, RGB(255, 255, 255)); 
	SetBkMode(hdc, TRANSPARENT);
	int nStatsTextX = StatsTextLeft;
	int nStatsTextY = StatsTextUpper;
	int nLength = 60;
	CString strShowStats = _T("");

	//等級
	nStatsTextY += 20;
	strShowStats.Format(_T("Level: %d"), m_Player.Level);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());

	//經驗值
	nStatsTextY += 20;
	strShowStats.Format(_T("Experience: %d / %d"), m_Player.Experience, (m_Player.Level ^ 2) * 10);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());

	//生命
	nStatsTextY += 20;
	strShowStats.Format(_T("Life: %d / %d"), m_Player.Life, m_Player.Level * 10);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());

	//攻擊
	nStatsTextY += 20;
	strShowStats.Format(_T("Attack: %d"), m_Player.Attack);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());

	//防禦
	nStatsTextY += 20;
	strShowStats.Format(_T("Defense: %d"), m_Player.Defense);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());

	//武器
	nStatsTextY += 20;
	strShowStats.Format(_T("Weapon: %s"), mapWeaponName[m_Player.Weapon]);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());

	//防具
	nStatsTextY += 20;
	strShowStats.Format(_T("Armour: %s"), mapArmourName[m_Player.Armour]);
	TextOut(hdc, nStatsTextX, nStatsTextY, strShowStats, strShowStats.GetLength());
}

void RedrawMap(HWND hWnd, HDC hdc)
{
	for (int x = 0; x < ShowMapX; x++)
	{
		for (int y = 0; y < ShowMapY; y++)
		{
			CString Imagefile;
			Imagefile.Format(_T("%smap%d.png"), strResourcePath, ShowDungeonMap[x][y]);		//地圖
			ShowImageFile(hdc, Imagefile, x * 32 + ShowMapLeft, y * 32 + ShowMapUpper, 32, 32);
			if (ShowMonsterMap[x][y] != NoMonster)
			{
				Imagefile.Format(_T("%smonster%d.png"), strResourcePath, ShowMonsterMap[x][y]);		//怪物
				ShowImageFile(hdc, Imagefile, x * 32 + ShowMapLeft, y * 32 + ShowMapUpper, 32, 32);
			}
		}
	}
}

void DoInvalidateRect(HWND hWnd, int nPosX, int nPosY)
{
	CRect Rect;
	Rect = CRect(nPosX * 32, nPosY * 32, (nPosX + 1) * 32, (nPosY + 1) * 32);
	InvalidateRect(hWnd, &Rect, TRUE);
}

void KeyDownEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CRandomMap RandomMap;
	//::MessageBox(hWnd, _T("R") , _T("Key Pressed"), MB_OK);
	CRect TextRect(StatsTextLeft, StatsTextUpper, StatsTextLeft + StatsTextWidth, StatsTextUpper + StatsTextHeight);

	switch (wParam)
	{
	case _T('R'):
	case _T('r'):
		{
			InitializePlayer();
			m_nMapMaxX = RandomMap.GetRandom(10, 30);
			m_nMapMaxY = RandomMap.GetRandom(10, 20);
			InvalidateRect(hWnd, NULL, TRUE);
			RandomMap.CreateMap(m_nMapMaxX, m_nMapMaxY);

			DungeonMap = RandomMap.GetDungeonMap();
			ShowDungeonMap = vector<vector<TERRAIN> >(m_nMapMaxX, vector<TERRAIN>(m_nMapMaxY));
		
			MonsterMap = RandomMap.GetMonsterMap();
			ShowMonsterMap = vector<vector<MONSTER> >(m_nMapMaxX, vector<MONSTER>(m_nMapMaxY));
		
			TreasureMap = RandomMap.GetTreasureMap();
			ShowTreasureMap = vector<vector<TREASURE> >(m_nMapMaxX, vector<TREASURE>(m_nMapMaxY));

			NowLightMap = vector<vector<BOOL> >(m_nMapMaxX, vector<BOOL>(m_nMapMaxY));
			for (int x = 0; x < m_nMapMaxX; x++) for (int y = 0; y < m_nMapMaxY; y++) NowLightMap[x][y] = FALSE;
			RandomMap.GetStartPoint(m_nMapStartX, m_nMapStartY);
			m_nPlayerPosX = m_nMapStartX;
			m_nPlayerPosY = m_nMapStartY;
			m_nPlayerLastPosX = -1;
			m_nPlayerLastPosY = -1;
			LightMap(m_nPlayerPosX, m_nPlayerPosY);
			
			m_bRedraweMap = TRUE;
		}
		break;
	case VK_LEFT:
		// Process the LEFT ARROW key.
		if (CheckLink(m_nPlayerPosX, m_nPlayerPosY, LEFT) == TRUE)
		{
			InvalidateRect(hWnd, &TextRect, TRUE);
			if (HitMonster(m_nPlayerPosX, m_nPlayerPosY, LEFT) == TRUE)
			{
				m_nPlayerPosX = m_nPlayerPosX - 1;
				if (m_nPlayerPosX < 0) m_nPlayerPosX = 0;
			}

			m_bRedraweMap = TRUE;
		}
		break;
	case VK_RIGHT:
		// Process the RIGHT ARROW key.
		if (CheckLink(m_nPlayerPosX, m_nPlayerPosY, RIGHT) == TRUE)
		{
			InvalidateRect(hWnd, &TextRect, TRUE);
			if (HitMonster(m_nPlayerPosX, m_nPlayerPosY, RIGHT) == TRUE)
			{
				m_nPlayerPosX = m_nPlayerPosX + 1;
				if (m_nPlayerPosX >= m_nMapMaxX) m_nPlayerPosX = m_nMapMaxX - 1;
			}

			m_bRedraweMap = TRUE;
		}
		break;
	case VK_UP:
		// Process the UP ARROW key.
		if (CheckLink(m_nPlayerPosX, m_nPlayerPosY, UPPER) == TRUE)
		{
			InvalidateRect(hWnd, &TextRect, TRUE);
			if (HitMonster(m_nPlayerPosX, m_nPlayerPosY, UPPER) == TRUE)
			{
				m_nPlayerPosY = m_nPlayerPosY - 1;
				if (m_nPlayerPosY < 0) m_nPlayerPosY = 0;
			}

			m_bRedraweMap = TRUE;
		}
		break;
	case VK_DOWN:
		// Process the DOWN ARROW key.
		if (CheckLink(m_nPlayerPosX, m_nPlayerPosY, LOWER) == TRUE)
		{
			InvalidateRect(hWnd, &TextRect, TRUE);
			if (HitMonster(m_nPlayerPosX, m_nPlayerPosY, LOWER) == TRUE)
			{
				m_nPlayerPosY = m_nPlayerPosY + 1;
				if (m_nPlayerPosY >= m_nMapMaxY) m_nPlayerPosY = m_nMapMaxY - 1;
			}

			m_bRedraweMap = TRUE;
		}
		break;
	case VK_HOME:
		// Process the HOME key.
		break;
	case VK_END:
		// Process the END key.
		break;
	case VK_INSERT:
		// Process the INS key.
		break;
	case VK_DELETE:
		// Process the DEL key.
		break;
	case VK_F2:
		// Process the F2 key.
		break;
	// Process other non-character keystrokes.
	default:
		break; 
	}
	
	LightMap(m_nPlayerPosX, m_nPlayerPosY);
}

BOOL CheckLink(int nPosX, int nPosY, Direction nDir)
{
	BOOL bReturn = FALSE;

	CRandomMap RandomMap;
	if (nDir == LEFT)
	{
		bReturn = RandomMap.CheckLeftLink(DungeonMap[nPosX][nPosY]);
	}
	if (nDir == RIGHT)
	{
		bReturn = RandomMap.CheckRightLink(DungeonMap[nPosX][nPosY]);
	}
	if (nDir == UPPER)
	{
		bReturn = RandomMap.CheckUpperLink(DungeonMap[nPosX][nPosY]);
	}
	if (nDir == LOWER)
	{
		bReturn = RandomMap.CheckLowerLink(DungeonMap[nPosX][nPosY]);
	}

	return bReturn;
}

BOOL HitMonster(int nPosX, int nPosY, Direction nDir)
{
	BOOL bReturn = FALSE;
	
	if (nDir == LEFT)
	{
		nPosX = nPosX - 1;
		if (nPosX < 0) nPosX = 0;
	}
	if (nDir == RIGHT)
	{
		nPosX = nPosX + 1;
		if (nPosX >= m_nMapMaxX) nPosX = m_nMapMaxX - 1;
	}
	if (nDir == UPPER)
	{
		nPosY = nPosY - 1;
		if (nPosY < 0) nPosY = 0;
	}
	if (nDir == LOWER)
	{
		nPosY = nPosY + 1;
		if (nPosY >= m_nMapMaxY) nPosY = m_nMapMaxY - 1;
	}
	
	MONSTER nMonster = MonsterMap[nPosX][nPosY];

	if (nMonster == NoMonster)
	{
		bReturn = TRUE;
	}
	else
	{
		bReturn = DoFighting(nMonster);
		if (bReturn == TRUE)
		{
			MonsterMap[nPosX][nPosY] = NoMonster;
		}
	}

	return bReturn;
}

BOOL DoFighting(MONSTER nMonster)
{
	BOOL bWinner = TRUE;

	Attribute strMonster = GetMonsterDate(nMonster);

	int nMLife = strMonster.Life;
	int nPLife = m_Player.Life;
	while(nPLife > 0 && nMLife > 0)
	{
		nMLife = nMLife - (m_Player.Attack - strMonster.Defense);
		if (nMLife > 0)
		{
			nPLife = nPLife - (strMonster.Attack - m_Player.Defense);
			if (nPLife <= 0)
			{
				bWinner = FALSE;
			}
		}
	};
	m_Player.Life = nPLife;
	m_Player.Experience = m_Player.Experience + strMonster.Experience;

	//升級
	if (m_Player.Experience >= (m_Player.Level ^ 2) * 10)
	{
		m_Player.Experience = m_Player.Experience - ((m_Player.Level ^ 2) * 10);
		m_Player.Level = m_Player.Level + 1;
	}

	return bWinner;
}

void LightMap(int nPosX, int nPosY)
{
	if (m_nPlayerLastPosX == nPosX && m_nPlayerLastPosY == nPosY) return;
	
	CRandomMap RandomMap;

	NowLightMap[nPosX][nPosY] = TRUE;
	CRect Rect;
	Rect = CRect(0, 0, (ShowMapX + 1) * 32 + ShowMapLeft, (ShowMapY + 1) * 32 + ShowMapUpper);
	InvalidateRect(m_hMainWnd, &Rect, TRUE);
	
	if (RandomMap.CheckLeftLink(DungeonMap[nPosX][nPosY]) == TRUE && nPosX > 0)
	{
		NowLightMap[nPosX - 1][nPosY] = TRUE;
	}

	if (RandomMap.CheckRightLink(DungeonMap[nPosX][nPosY]) == TRUE && nPosX < m_nMapMaxX - 1)
	{
		NowLightMap[nPosX + 1][nPosY] = TRUE;
	}

	if (RandomMap.CheckUpperLink(DungeonMap[nPosX][nPosY]) == TRUE && nPosY > 0)
	{
		NowLightMap[nPosX][nPosY - 1] = TRUE;
	}

	if (RandomMap.CheckLowerLink(DungeonMap[nPosX][nPosY]) == TRUE && nPosY < m_nMapMaxY - 1)
	{
		NowLightMap[nPosX][nPosY + 1] = TRUE;
	}
	
	int nShowLeftX = nPosX - int((ShowMapX - 1) / 2);
	int nShowRightX = nPosX + int((ShowMapX - 1) / 2);
	if (nShowLeftX <= 0)
	{
		nShowLeftX = 0;
		m_nShowPlayerPosX = m_nPlayerPosX;
	}
	if (nShowRightX >= m_nMapMaxX - 1)
	{
		nShowRightX = m_nMapMaxX - 1;
		nShowLeftX = nShowRightX - ShowMapX + 1;
		m_nShowPlayerPosX = m_nPlayerPosX - nShowLeftX;
	}

	int nShowUpperY = nPosY - int((ShowMapY - 1) / 2);
	int nShowDownY = nPosY + int((ShowMapY - 1) / 2);
	if (nShowUpperY <= 0)
	{
		nShowUpperY = 0;
		m_nShowPlayerPosY = m_nPlayerPosY;
	}
	if (nShowDownY >= m_nMapMaxY - 1)
	{
		nShowDownY = m_nMapMaxY - 1;
		nShowUpperY = nShowDownY - ShowMapY + 1;
		m_nShowPlayerPosY = m_nPlayerPosY - nShowUpperY;
	}

	for (int x = 0; x < ShowMapX; x++)
	{
		for (int y = 0; y < ShowMapY; y++)
		{
			if (NowLightMap[x + nShowLeftX][y + nShowUpperY] == TRUE)
			{
				ShowDungeonMap[x][y] = DungeonMap[x + nShowLeftX][y + nShowUpperY];
				ShowMonsterMap[x][y] = MonsterMap[x + nShowLeftX][y + nShowUpperY];
				ShowTreasureMap[x][y] = TreasureMap[x + nShowLeftX][y + nShowUpperY];
			}
			else
			{
				ShowDungeonMap[x][y] = None;
				ShowMonsterMap[x][y] = NoMonster;
				ShowTreasureMap[x][y] = NoTreasure;
			}
		}
	}

	m_nPlayerLastPosX = m_nPlayerPosX;
	m_nPlayerLastPosY = m_nPlayerPosY;
}


void InitializePlayer()
{
	m_Player.Level = 1;
	m_LastPlayer.Level = -1;
	m_Player.Experience = 0;
	m_LastPlayer.Experience = -1;
	m_Player.Life = 10;
	m_LastPlayer.Life = -1;
	m_Player.Attack = 1;
	m_LastPlayer.Attack = -1;
	m_Player.Defense = 1;
	m_LastPlayer.Defense = -1;
	m_Player.Money = 0;
	m_LastPlayer.Money = -1;
	m_Player.Weapon = NoWeapon;
	m_LastPlayer.Weapon = WEAPON_MAX;
	m_Player.Armour = NoArmour;
	m_LastPlayer.Armour = ARMOUR_MAX;
}

void SetupPlayerDate(int nLevel)
{
	m_Player.Level = nLevel;
	m_Player.Life = nLevel * 10;
	m_Player.Attack = 1 + (nLevel - 1) + m_Player.Weapon;
	m_Player.Defense = 1 + (nLevel - 1) + m_Player.Armour;
}

Attribute GetMonsterDate(MONSTER nMonster)
{
	Attribute tReturn;
		
	tReturn.Level = int(nMonster);

	tReturn.Money = 1 + (tReturn.Level - 1) * 3;
	tReturn.Experience = 1 + (tReturn.Level - 1) * 2;
	tReturn.Weapon = NoWeapon;
	tReturn.Armour = NoArmour;

	tReturn.Life = tReturn.Level * 2;
	tReturn.Attack = tReturn.Level * 2 + tReturn.Weapon;
	tReturn.Defense = (tReturn.Level - 1) * 2 + tReturn.Armour;

	return tReturn;
}

void InitializeName()
{
	mapMonsterName[NoMonster] = _T("");
	mapMonsterName[LV1Monster] = _T("");
	mapMonsterName[MONSTER_MAX] = _T("");
	
	mapTreasureName[NoTreasure] = _T("");
	mapTreasureName[Gold] = _T("");
	mapTreasureName[Equipment] = _T("");
	mapTreasureName[Item] = _T("");
	mapTreasureName[TREASURE_MAX] = _T("");
	
	mapWeaponName[NoWeapon] = _T("");
	mapWeaponName[WEAPON_MAX] = _T("");

	mapArmourName[NoArmour] = _T("");
	mapArmourName[ARMOUR_MAX] = _T("");
}
