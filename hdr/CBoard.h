#ifndef BOARD_H
#define BOARD_H

#include "defines.h"

class CBoard
{
public:
	__declspec(dllexport) CBoard();
	__declspec(dllexport) virtual ~CBoard();

	__declspec(dllexport) bool SetShip(IN BYTE x, IN BYTE y, IN bool dir, IN BYTE size);	//situate ship on field
	__declspec(dllexport) bool Shot(IN BYTE x, IN BYTE y);								//shot
	__declspec(dllexport) BYTE IsWholeBroken(IN BYTE x, IN BYTE y);						//return size of whole broken ship
	
	__declspec(dllexport) BYTE  GetCountOfShip();											//return CCount struct (BYTE)
	__declspec(dllexport) void Show();
	__declspec(dllexport) void SetRandomPosOfShip();
	__declspec(dllexport) void Clear();
	__declspec(dllexport) bool CheckShip(IN BYTE size);	//checks CCount (checks each count of ship)
protected:
	BYTE *pCells;	//board
	CCount vCount;	//count of ships
private:
	BYTE GetRandValue();
	void CalcShip(IN BYTE aSize);	//calculation of a count of ships
	bool ScanRegion(IN BYTE x, IN BYTE y, IN bool dir, IN BYTE size);	//true - can situate ship here, false - can not
};

#endif /*BOARD_H*/