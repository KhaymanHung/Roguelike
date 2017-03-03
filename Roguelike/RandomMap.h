#pragma once
#include <map>
#include <vector>

using namespace std;

enum TERRAIN
{
	None = 0,
	UpperLeft = 1,			//¢z
	HorizontalLine = 2,		//¢w
	TLine = 3,				//¢s
	UpperRight = 4,			//¢{
	StraightLine = 5,		//¢x
	LeftTLine = 6,			//¢u
	Cross = 7,				//¢q
	RightTLine = 8,			//¢t
	LowerLeft = 9,			//¢|
	InvertedTLeft = 10,		//¢r
	LowerRight = 11,		//¢}
	UpperEnd = 12,
	LowerEnd = 13,
	LeftEnd = 14,
	RightEnd = 15,
	TERRAIN_MAX
};

enum MONSTER
{
	NoMonster = 0,
	LV1Monster = 1,
	MONSTER_MAX
};

enum TREASURE
{
	NoTreasure = 0,
	Gold = 1,
	Equipment = 2,
	Item = 3,
	TREASURE_MAX
};

enum WEAPON
{
	NoWeapon = 0,
	WEAPON_MAX
};

enum ARMOUR
{
	NoArmour = 0,
	ARMOUR_MAX
};

struct Attribute
{
	int Level;
	int Experience;
	int Life;
	int Attack;	
	int Defense;
	int Money;
	WEAPON Weapon;
	ARMOUR Armour;
};

class CRandomMap
{
public:
	CRandomMap(void);
	~CRandomMap(void);
	
	void CreateMap(int nFormatX, int nFormatY);
	void GetStartPoint(int& nPosX, int& nPosY);
	vector<vector<TERRAIN> > GetDungeonMap();
	vector<vector<MONSTER> > GetMonsterMap();
	vector<vector<TREASURE> > GetTreasureMap();

	int GetRandom(int nMin, int nMax);
	float GetRandom(float nMin, float nMax);
	
	BOOL CheckLeftLink(TERRAIN nTerrain);
	BOOL CheckRightLink(TERRAIN nTerrain);
	BOOL CheckUpperLink(TERRAIN nTerrain);
	BOOL CheckLowerLink(TERRAIN nTerrain);

private:
	vector<vector<TERRAIN> > vecDungeonMap;
	vector<vector<MONSTER> > vecMonsterMap;
	vector<vector<TREASURE> > vecTreasureMap;

	int m_nStartX;
	int m_nStartY;
	int m_nMapX;
	int m_nMapY;
	int m_nMapTotal;
	BOOL m_bLeftLink[TERRAIN_MAX];
	BOOL m_bRightLink[TERRAIN_MAX];
	BOOL m_bUpperLink[TERRAIN_MAX];
	BOOL m_bLowerLink[TERRAIN_MAX];

	void GetNextRandomPosition(vector<vector<TERRAIN> > pMap, int& nPosX, int& nPosY);
	TERRAIN GetRandomTerrain(vector<vector<TERRAIN> > pMap, int nPosX, int nPosY, int nFormatX, int nFormatY);
	TERRAIN GetRandomStartTerrain(int nPosX, int nPosY, int nFormatX, int nFormatY);
	void RandomMonster(int nNumber);
	void RandomTreasure(int nNumber);
	
	void SetLeftLink();
	void SetRightLink();
	void SetUpperLink();
	void SetLowerLink();
	
	BOOL CheckUpperLeftTreeain(TERRAIN nTreeain, TERRAIN nUpperLeftTreeain);
	BOOL CheckLeftTreeain(TERRAIN nTreeain, TERRAIN nLeftTreeain);
	BOOL CheckLowerLeftTreeain(TERRAIN nTreeain, TERRAIN nLowerLeftTreeain);
	BOOL CheckUpperTreeain(TERRAIN nTreeain, TERRAIN nUpperTreeain);
	BOOL CheckLowerTreeain(TERRAIN nTreeain, TERRAIN nLowerTreeain);
	BOOL CheckUpperRightTreeain(TERRAIN nTreeain, TERRAIN nUpperRightTreeain);
	BOOL CheckRightTreeain(TERRAIN nTreeain, TERRAIN nRightTreeain);
	BOOL CheckLowerRightTreeain(TERRAIN nTreeain, TERRAIN nLowerRightTreeain);
	BOOL CheckOnlyEndTreeain(BOOL nCheck[TERRAIN_MAX]);
};

