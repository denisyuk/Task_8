#include "CBoard.h"

class CStatistic:public CBoard
{
public:
	__declspec(dllexport) CStatistic();
	__declspec(dllexport) ~CStatistic();

	__declspec(dllexport) bool SetCell(IN BYTE x, IN BYTE y, IN BYTE aObj);
	__declspec(dllexport) bool IsPrevShot(IN BYTE x, IN BYTE y);
	__declspec(dllexport) void SetWholeBrokenShip(IN BYTE x, IN BYTE y);
};