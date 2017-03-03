#pragma once

#include "resource.h"
#include "RandomMap.h"

enum Direction
{
	LEFT = 1,
	RIGHT = 2,
	UPPER = 3,
	LOWER = 4
};

HWND m_hMainWnd;

CString strResourcePath = _T("Resource\\");
vector<vector<TERRAIN> > DungeonMap;
vector<vector<TERRAIN> > ShowDungeonMap;

vector<vector<MONSTER> > MonsterMap;
vector<vector<MONSTER> > ShowMonsterMap;

vector<vector<TREASURE> > TreasureMap;
vector<vector<TREASURE> > ShowTreasureMap;

vector<vector<BOOL> > NowLightMap;

Attribute m_Player;
Attribute m_LastPlayer;

int m_nMapStartX;
int m_nMapStartY;
int m_nMapMaxX;
int m_nMapMaxY;
int m_nPlayerPosX;
int m_nPlayerPosY;
int m_nPlayerLastPosX;
int m_nPlayerLastPosY;
int m_nShowPlayerPosX;
int m_nShowPlayerPosY;

BOOL m_bRedraweMap;

map<MONSTER, CString> mapMonsterName;
map<TREASURE, CString> mapTreasureName;
map<WEAPON, CString> mapWeaponName;
map<ARMOUR, CString> mapArmourName;

void ShowImageFile(HDC hdc, CString strFile, int nPosX, int nPosY, int nWidth = -1, int nHeight = -1);
void ShowPlayerInMap(HDC hdc, CString strFile, int nPosX, int nPosY, int nWidth = -1, int nHeight = -1);
void RedrawMap(HWND hWnd, HDC hdc);
void KeyDownEvent(HWND hWnd, WPARAM wParam, LPARAM lParam);

void DrawBackground(HWND hWnd, HDC hdc);
void DrawPlayer(HWND hWnd, HDC hdc);
void DrawPlayerStats(HWND hWnd, HDC hdc);
void DoInvalidateRect(HWND hWnd, int nPosX, int nPosY);

BOOL CheckLink(int nPosX, int nPosY, Direction nDir);
BOOL HitMonster(int nPosX, int nPosY, Direction nDir);
BOOL DoFighting(MONSTER nMonster);
void LightMap(int nPosX, int nPosY);
BOOL CheckRedrawPlayerStats();

void InitializePlayer();
void InitializeName();
void SetupPlayerDate(int nLevel);
Attribute GetMonsterDate(MONSTER nMonster);
