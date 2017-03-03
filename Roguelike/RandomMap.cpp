#include "StdAfx.h"
#include "RandomMap.h"
#include "Parameters.h"

CRandomMap::CRandomMap(void)
{
	srand(::GetTickCount());
	SetLeftLink();
	SetRightLink();
	SetUpperLink();
	SetLowerLink();
}


CRandomMap::~CRandomMap(void)
{
}

void CRandomMap::SetLeftLink()
{	
	m_bLeftLink[None] = FALSE;
	m_bLeftLink[UpperLeft] = FALSE;
	m_bLeftLink[HorizontalLine] = TRUE;
	m_bLeftLink[TLine] = TRUE;
	m_bLeftLink[UpperRight] = TRUE;
	m_bLeftLink[StraightLine] = FALSE;
	m_bLeftLink[LeftTLine] = FALSE;
	m_bLeftLink[Cross] = TRUE;
	m_bLeftLink[RightTLine] = TRUE;
	m_bLeftLink[LowerLeft] = FALSE;
	m_bLeftLink[InvertedTLeft] = TRUE;
	m_bLeftLink[LowerRight] = TRUE;
	m_bLeftLink[UpperEnd] = FALSE;
	m_bLeftLink[LowerEnd] = FALSE;
	m_bLeftLink[LeftEnd] = FALSE;
	m_bLeftLink[RightEnd] = TRUE;
}

BOOL CRandomMap::CheckLeftLink(TERRAIN nTerrain)
{
	return m_bLeftLink[nTerrain];
}

void CRandomMap::SetRightLink()
{	
	m_bRightLink[None] = FALSE;
	m_bRightLink[UpperLeft] = TRUE;
	m_bRightLink[HorizontalLine] = TRUE;
	m_bRightLink[TLine] = TRUE;
	m_bRightLink[UpperRight] = FALSE;
	m_bRightLink[StraightLine] = FALSE;
	m_bRightLink[LeftTLine] = TRUE;
	m_bRightLink[Cross] = TRUE;
	m_bRightLink[RightTLine] = FALSE;
	m_bRightLink[LowerLeft] = TRUE;
	m_bRightLink[InvertedTLeft] = TRUE;
	m_bRightLink[LowerRight] = FALSE;
	m_bRightLink[UpperEnd] = FALSE;
	m_bRightLink[LowerEnd] = FALSE;
	m_bRightLink[LeftEnd] = TRUE;
	m_bRightLink[RightEnd] = FALSE;
}

BOOL CRandomMap::CheckRightLink(TERRAIN nTerrain)
{
	return m_bRightLink[nTerrain];
}

void CRandomMap::SetUpperLink()
{	
	m_bUpperLink[None] = FALSE;
	m_bUpperLink[UpperLeft] = FALSE;
	m_bUpperLink[HorizontalLine] = FALSE;
	m_bUpperLink[TLine] = FALSE;
	m_bUpperLink[UpperRight] = FALSE;
	m_bUpperLink[StraightLine] = TRUE;
	m_bUpperLink[LeftTLine] = TRUE;
	m_bUpperLink[Cross] = TRUE;
	m_bUpperLink[RightTLine] = TRUE;
	m_bUpperLink[LowerLeft] = TRUE;
	m_bUpperLink[InvertedTLeft] = TRUE;
	m_bUpperLink[LowerRight] = TRUE;
	m_bUpperLink[UpperEnd] = FALSE;
	m_bUpperLink[LowerEnd] = TRUE;
	m_bUpperLink[LeftEnd] = FALSE;
	m_bUpperLink[RightEnd] = FALSE;
}

BOOL CRandomMap::CheckUpperLink(TERRAIN nTerrain)
{
	return m_bUpperLink[nTerrain];
}

void CRandomMap::SetLowerLink()
{	
	m_bLowerLink[None] = FALSE;
	m_bLowerLink[UpperLeft] = TRUE;
	m_bLowerLink[HorizontalLine] = FALSE;
	m_bLowerLink[TLine] = TRUE;
	m_bLowerLink[UpperRight] = TRUE;
	m_bLowerLink[StraightLine] = TRUE;
	m_bLowerLink[LeftTLine] = TRUE;
	m_bLowerLink[Cross] = TRUE;
	m_bLowerLink[RightTLine] = TRUE;
	m_bLowerLink[LowerLeft] = FALSE;
	m_bLowerLink[InvertedTLeft] = FALSE;
	m_bLowerLink[LowerRight] = FALSE;
	m_bLowerLink[UpperEnd] = TRUE;
	m_bLowerLink[LowerEnd] = FALSE;
	m_bLowerLink[LeftEnd] = FALSE;
	m_bLowerLink[RightEnd] = FALSE;
}

BOOL CRandomMap::CheckLowerLink(TERRAIN nTerrain)
{
	return m_bLowerLink[nTerrain];
}

vector<vector<TERRAIN> > CRandomMap::GetDungeonMap()
{
	return vecDungeonMap;
}

vector<vector<MONSTER> > CRandomMap::GetMonsterMap()
{
	return vecMonsterMap;
}

vector<vector<TREASURE> > CRandomMap::GetTreasureMap()
{
	return vecTreasureMap;
}

void CRandomMap::CreateMap(int nFormatX, int nFormatY)
{
	m_nMapTotal = 0; 
	m_nMapX = nFormatX;
	m_nMapY = nFormatY;
	vecDungeonMap = vector<vector<TERRAIN> >(nFormatX, vector<TERRAIN>(nFormatY));
	vecMonsterMap = vector<vector<MONSTER> >(nFormatX, vector<MONSTER>(nFormatY));
	vecTreasureMap = vector<vector<TREASURE> >(nFormatX, vector<TREASURE>(nFormatY));
	for (int i = 0; i < nFormatX; i++)
	{
		for (int k = 0; k < nFormatY; k++)
		{
			vecDungeonMap[i][k] = TERRAIN_MAX;
			vecMonsterMap[i][k] = NoMonster;
			vecTreasureMap[i][k] = NoTreasure;
		}
	}

	//Start Position
	TERRAIN StartTerrain;
	m_nStartX = GetRandom(0, nFormatX - 1);
	if (m_nStartX != 0 && m_nStartX != (nFormatX - 1))
	{
		m_nStartY = GetRandom(0, 1);
		if (m_nStartY == 1) m_nStartY = nFormatY - 1;
	}
	else
	{
		m_nStartY = GetRandom(0, nFormatY - 1);
	}
	StartTerrain = GetRandomStartTerrain(m_nStartX, m_nStartY, nFormatX - 1, nFormatY - 1);	
	vecDungeonMap[m_nStartX][m_nStartY] = StartTerrain;

	for (int i = 0; i < (m_nMapX * m_nMapY) - 1; i++)
	{
		int nNextPosX = 0, nNextPosY = 0;
		GetNextRandomPosition(vecDungeonMap, nNextPosX, nNextPosY);

		if (nNextPosX != -1 && nNextPosY != -1)
			vecDungeonMap[nNextPosX][nNextPosY] = GetRandomTerrain(vecDungeonMap, nNextPosX, nNextPosY, nFormatX - 1, nFormatY - 1);
	}
	
	for (int x = 0; x < nFormatX; x++)
	{
		for (int y = 0; y < nFormatY; y++)
		{
			if (vecDungeonMap[x][y] == TERRAIN_MAX) vecDungeonMap[x][y] = None;
		}
	}

	RandomMonster(int(m_nMapTotal * MonsterProportion));
	RandomTreasure(int(m_nMapTotal * TreasureProportion));
}


void CRandomMap::GetNextRandomPosition(vector<vector<TERRAIN> > pMap, int& nPosX, int& nPosY)
{
	for (int x = 0; x < m_nMapX; x++)
	{
		for (int y = 0; y < m_nMapY; y++)
		{
			if (pMap[x][y] != TERRAIN_MAX)
			{
				if (m_bLeftLink[pMap[x][y]] == TRUE && x > 0)
				{
					if (pMap[x - 1][y] == TERRAIN_MAX)
					{
						nPosX = x - 1;
						nPosY = y;
						return;
					}
				}
				if (m_bRightLink[pMap[x][y]] == TRUE && x < m_nMapX - 1)
				{
					if (pMap[x + 1][y] == TERRAIN_MAX)
					{
						nPosX = x + 1;
						nPosY = y;
						return;
					}
				}
				if (m_bUpperLink[pMap[x][y]] == TRUE && y > 0)
				{
					if (pMap[x][y - 1] == TERRAIN_MAX)
					{
						nPosX = x;
						nPosY = y - 1;
						return;
					}
				}
				if (m_bLowerLink[pMap[x][y]] == TRUE && y < m_nMapY - 1)
				{
					if (pMap[x][y + 1] == TERRAIN_MAX)
					{
						nPosX = x;
						nPosY = y + 1;
						return;
					}
				}
			}
		}
	}

	nPosX = -1;
	nPosY = -1;
}

int CRandomMap::GetRandom(int nMin, int nMax)
{
	if (nMin > nMax)
	{
		int temp = nMax;
		nMax = nMin;
		nMin = temp;
	}

	if (nMin == nMax) return nMin;

	return (rand() % (nMax - nMin + 1)) + nMin;
}

float CRandomMap::GetRandom(float nMin, float nMax)
{
	if (nMin > nMax)
	{
		float temp = nMax;
		nMax = nMin;
		nMin = temp;
	}

	if (nMin == nMax) return nMin;

	return (rand() * (nMax - nMin + 1)) + nMin;
}

TERRAIN CRandomMap::GetRandomTerrain(vector<vector<TERRAIN> > pMap, int nPosX, int nPosY, int nFormatX, int nFormatY)
{
	int nRandom;
	vector<TERRAIN> RandomRange;
	BOOL nCheck[TERRAIN_MAX];
	for (int i = 0; i < TERRAIN_MAX; i++) nCheck[i] = TRUE;
	
	if (nPosX == 0 || nPosX == nFormatX || nPosY == 0 || nPosY == nFormatY)
	{
		for (int i = 0; i < TERRAIN_MAX; i++)
		{
			if (nPosX == 0 && m_bLeftLink[i] == TRUE)
				nCheck[i] = FALSE;
			if (nPosX == nFormatX && m_bRightLink[i] == TRUE)
				nCheck[i] = FALSE;
			if (nPosY == 0 && m_bUpperLink[i] == TRUE)
				nCheck[i] = FALSE;
			if (nPosY == nFormatY && m_bLowerLink[i] == TRUE)
				nCheck[i] = FALSE;
		}
	}

	for (int k = 0; k < TERRAIN_MAX; k++)
	{
		//左上
		if (nPosX > 0 && nPosY > 0 && nCheck[k] == TRUE)
			nCheck[k] = CheckUpperLeftTreeain(TERRAIN(k), pMap[nPosX - 1][nPosY - 1]);
		//左
		if (nPosX > 0 && nCheck[k] == TRUE)
			nCheck[k] = CheckLeftTreeain(TERRAIN(k), pMap[nPosX - 1][nPosY]);
		//左下
		if (nPosX > 0 && nPosY < m_nMapY - 1 && nCheck[k] == TRUE)
			nCheck[k] = CheckLowerLeftTreeain(TERRAIN(k), pMap[nPosX - 1][nPosY + 1]);
		//上
		if (nPosY > 0 && nCheck[k] == TRUE)
			nCheck[k] = CheckUpperTreeain(TERRAIN(k), pMap[nPosX][nPosY - 1]);
		//下
		if (nPosY < m_nMapY - 1 && nCheck[k] == TRUE)
			nCheck[k] = CheckLowerTreeain(TERRAIN(k), pMap[nPosX][nPosY + 1]);
		//右上
		if (nPosX < m_nMapX - 1 && nPosY > 0 && nCheck[k] == TRUE)
			nCheck[k] = CheckUpperRightTreeain(TERRAIN(k), pMap[nPosX + 1][nPosY - 1]);
		//右
		if (nPosX < m_nMapX - 1 && nCheck[k] == TRUE)
			nCheck[k] = CheckRightTreeain(TERRAIN(k), pMap[nPosX + 1][nPosY]);
		//右下
		if (nPosX < m_nMapX - 1 && nPosY < m_nMapY - 1 && nCheck[k] == TRUE)
			nCheck[k] = CheckLowerRightTreeain(TERRAIN(k), pMap[nPosX + 1][nPosY + 1]);
	}
	
	if (m_nMapTotal < int((m_nMapX * m_nMapY) * MinMapProportion) && CheckOnlyEndTreeain(nCheck) == FALSE)
	{
		nCheck[UpperEnd] = FALSE;
		nCheck[LowerEnd] = FALSE;
		nCheck[LeftEnd] = FALSE;
		nCheck[RightEnd] = FALSE;
	}

	for (int i = 0; i < TERRAIN_MAX; i++)
	{
		if (nCheck[i] == TRUE)
		{
			RandomRange.push_back(TERRAIN(i));
		}
	}

	if (RandomRange.size() < 1) return None;
	nRandom = GetRandom(0, RandomRange.size() - 1);	
	m_nMapTotal++;

	return RandomRange[nRandom];
}

TERRAIN CRandomMap::GetRandomStartTerrain(int nPosX, int nPosY, int nFormatX, int nFormatY)
{
	int nRandom;
	vector<TERRAIN> RandomRange;
	
	if (!(nPosX == 0 && nPosY != nFormatY) && !(nPosX == nFormatX && nPosY == nFormatY) &&
		!(nPosY == 0 && nPosX != nFormatX))
		RandomRange.push_back(UpperLeft);

	if (nPosX == 0 || nPosX == nFormatX)
		RandomRange.push_back(HorizontalLine);
	
	if (!(nPosX == 0 && nPosY == nFormatY) && !(nPosX == nFormatX && nPosY == nFormatY) &&
		!(nPosY == 0 && (nPosX != 0 && nPosX != nFormatX)))
		RandomRange.push_back(TLine);
	
	if (!(nPosX == 0 && nPosY == nFormatY) && !(nPosX == nFormatX && nPosY != nFormatY) &&
		!(nPosY == 0 && nPosX != 0))
		RandomRange.push_back(UpperRight);

	if (nPosY == 0 || nPosY == nFormatY)
		RandomRange.push_back(StraightLine);
	
	if (!(nPosX == nFormatX && nPosY == 0) && !(nPosX == nFormatX && nPosY == nFormatY) &&
		!(nPosX == 0 && (nPosY != 0 && nPosY != nFormatY)))
		RandomRange.push_back(LeftTLine);
	
	if (!(nPosX == 0 && nPosY == 0) && !(nPosX == 0 && nPosY == nFormatY) &&
		!(nPosX == nFormatX && nPosY == 0) && !(nPosX == nFormatX && nPosY == nFormatY))
		RandomRange.push_back(Cross);
	
	if (!(nPosX == 0 && nPosY == 0) && !(nPosX == 0 && nPosY == nFormatY) &&
		!(nPosX == nFormatX && (nPosY != 0 && nPosY != nFormatY)))
		RandomRange.push_back(RightTLine);
	
	if (!(nPosX == nFormatX && nPosY == 0) && !(nPosX == 0 && nPosY != 0) &&
		!(nPosY == nFormatY && nPosX != nFormatX))
		RandomRange.push_back(LowerLeft);
	
	if (!(nPosX == 0 && nPosY == 0) && !(nPosX == nFormatX && nPosY == 0) &&
		!(nPosY == nFormatY && (nPosX != 0 && nPosX != nFormatX)))
		RandomRange.push_back(InvertedTLeft);
	
	if (!(nPosX == 0 && nPosY == 0) && !(nPosX == nFormatX && nPosY != 0) &&
		!(nPosY == nFormatY && nPosX != 0))
		RandomRange.push_back(LowerRight);

	if (RandomRange.size() < 1) return None;
	nRandom = GetRandom(0, RandomRange.size() - 1);	
	m_nMapTotal++;

	return RandomRange[nRandom];
}

void CRandomMap::GetStartPoint(int& nPosX, int& nPosY)
{
	nPosX = m_nStartX;
	nPosY = m_nStartY;
}

BOOL CRandomMap::CheckUpperLeftTreeain(TERRAIN nTreeain, TERRAIN nUpperLeftTreeain)
{
	BOOL bReturn = TRUE;
	
	return bReturn;
}

BOOL CRandomMap::CheckLeftTreeain(TERRAIN nTreeain, TERRAIN nLeftTreeain)
{
	BOOL bReturn = TRUE;
	
	if (m_bLeftLink[nTreeain] != m_bRightLink[nLeftTreeain] && nLeftTreeain != TERRAIN_MAX)
		bReturn = FALSE;

	return bReturn;
}

BOOL CRandomMap::CheckLowerLeftTreeain(TERRAIN nTreeain, TERRAIN nLowerLeftTreeain)
{
	BOOL bReturn = TRUE;
	
	return bReturn;
}

BOOL CRandomMap::CheckUpperTreeain(TERRAIN nTreeain, TERRAIN nUpperTreeain)
{
	BOOL bReturn = TRUE;
	
	if (m_bUpperLink[nTreeain] != m_bLowerLink[nUpperTreeain] && nUpperTreeain != TERRAIN_MAX)
		bReturn = FALSE;

	return bReturn;
}

BOOL CRandomMap::CheckLowerTreeain(TERRAIN nTreeain, TERRAIN nLowerTreeain)
{
	BOOL bReturn = TRUE;
	
	if (m_bLowerLink[nTreeain] != m_bUpperLink[nLowerTreeain] && nLowerTreeain != TERRAIN_MAX)
		bReturn = FALSE;
		
	return bReturn;
}

BOOL CRandomMap::CheckUpperRightTreeain(TERRAIN nTreeain, TERRAIN nUpperRightTreeain)
{
	BOOL bReturn = TRUE;
	
	return bReturn;
}

BOOL CRandomMap::CheckRightTreeain(TERRAIN nTreeain, TERRAIN nRightTreeain)
{
	BOOL bReturn = TRUE;

	if (m_bRightLink[nTreeain] != m_bLeftLink[nRightTreeain] && nRightTreeain != TERRAIN_MAX)
		bReturn = FALSE;
	
	return bReturn;
}

BOOL CRandomMap::CheckLowerRightTreeain(TERRAIN nTreeain, TERRAIN nLowerRightTreeain)
{
	BOOL bReturn = TRUE;
	
	return bReturn;
}

BOOL CRandomMap::CheckOnlyEndTreeain(BOOL nCheck[TERRAIN_MAX])
{
	BOOL bReturn = TRUE;

	for (int i = 0; i < TERRAIN_MAX; i++)
	{
		if (i != UpperEnd && i != LowerEnd && i != LeftEnd && i != RightEnd && nCheck[i] == TRUE)
			bReturn = FALSE;
	}
	
	return bReturn;	
}

void CRandomMap::RandomMonster(int nNumber)
{
	for (int i = 0; i < nNumber; i++)
	{
		int nMonster = GetRandom(0, MONSTER_MAX - 1);
		BOOL bCreate = TRUE;
		int nMapX, nMapY;
		do
		{
			bCreate = TRUE;
			nMapX = GetRandom(0, m_nMapX - 1);
			nMapY = GetRandom(0, m_nMapY - 1);
			if (vecDungeonMap[nMapX][nMapY] == None || vecMonsterMap[nMapX][nMapY] != NoMonster
				|| (nMapX > m_nStartX - 3 && nMapX < m_nStartX + 3 && nMapY > m_nStartY - 3 && nMapY < m_nStartY + 3))
			{
				bCreate = FALSE;
			}
		} while(bCreate == FALSE);
		
		vecMonsterMap[nMapX][nMapY] = MONSTER(nMonster);
	}
}

void CRandomMap::RandomTreasure(int nNumber)
{
	for (int i = 0; i < nNumber; i++)
	{
		int nTreasure = GetRandom(0, TREASURE_MAX - 1);
		BOOL bCreate = TRUE;
		int nMapX, nMapY;
		do
		{
			bCreate = TRUE;
			nMapX = GetRandom(0, m_nMapX - 1);
			nMapY = GetRandom(0, m_nMapY - 1);
			if (vecDungeonMap[nMapX][nMapY] == None || vecTreasureMap[nMapX][nMapY] != NoTreasure
				|| (nMapX > m_nStartX - 3 && nMapX < m_nStartX + 3 && nMapY > m_nStartY - 3 && nMapY < m_nStartY + 3))
			{
				bCreate = FALSE;
			}
		} while(bCreate == FALSE);
		
		vecTreasureMap[nMapX][nMapY] = TREASURE(nTreasure);
	}
}