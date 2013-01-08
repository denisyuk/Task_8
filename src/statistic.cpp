#include "..\hdr\Statistic.h"

CStatistic::CStatistic():CBoard()
{}

CStatistic::~CStatistic()
{}

bool CStatistic::SetCell(IN BYTE x, IN BYTE y, IN BYTE aObj)
{
	if(aObj != M_MISSED && aObj != M_BROKENSHIP)
	{
		return false;
	}
	
	BYTE tX = x - 1;	//update to array
	BYTE tY = y - 1;

	pCells[tY * 10 + tX] |= aObj;

	return true;
}

bool CStatistic::IsPrevShot(IN BYTE x, IN BYTE y)
{
	BYTE tX = x - 1;
	BYTE tY = y - 1;

	if(pCells[tY * 10 + tX] == M_MISSED || pCells[tY * 10 + tX] == M_BROKENSHIP || pCells[tY * 10 + tX] == M_WHOLEBROKEN)
	{
		return false;
	}
	return true;
}

void CStatistic::SetWholeBrokenShip(IN BYTE x, IN BYTE y)
{
	this->IsWholeBroken(x, y);
}