#include "..\hdr\CBoard.h"

CBoard::CBoard()
{
	pCells = new BYTE[BOARD_HEIGTH * BOARD_WIDTH];
	Clear();
}

CBoard::~CBoard()
{
	delete[] pCells;
}

bool CBoard::SetShip(IN BYTE x, IN BYTE y, IN bool dir, IN BYTE size)
{
	bool bRetVal = ScanRegion(x, y, dir, size);
	if(!bRetVal)
	{
		return false;
	}
	//check Count-Value of all ships
	bRetVal = CheckShip(size);
	if(!bRetVal)
	{
		return false;
	}
/////////////////////////////////////////////
//check cells
	BYTE i = 0;
	BYTE j = 0;

	BYTE tX = x - 1;	//update to array
	BYTE tY = y - 1;

	BYTE hDir = (dir == HORIZ_DIRECT ? size:1) + tX;	//tmp horizontal direction value
	BYTE vDir = (dir == VERT_DIRECT ? size:1) + tY;	//tmp vertical direction value

	for(i = tY; i < vDir; i++)
	{
		for(j = tX; j < hDir; j++)
		{
			if(i < 0 || i > 9 || j < 0 || j > 9)
			{
				return false;
			}
			if(pCells[(i*10) + j] != NULL)
			{
				return false;
			}
		}
	}
//situate ship on board
	for(i = tY; i < vDir; i++)
	{
		for(j = tX; j < hDir; j++)
		{
			pCells[(i*10) + j] |= M_SHIP;
		}
	}

	CalcShip(size); //calculate count of ship

	return true;
}

bool CBoard::Shot(IN BYTE x, IN BYTE y)
{
	BYTE tX = x - 1;	//update to array
	BYTE tY = y - 1;

	if(pCells[tY * 10 + tX] == M_EMPTY)
	{
		pCells[tY * 10 + tX] |= M_MISSED;
		return false;
	}
	else
	{
		(pCells[tY * 10 + tX] &= M_EMPTY) |= M_BROKENSHIP;
		return true;
	}
}

BYTE CBoard::IsWholeBroken(IN BYTE x, IN BYTE y)
{
	BYTE tX = x - 1;
	BYTE tY = y - 1;

	if(pCells[tY * 10 + tX] == M_EMPTY)
	{
		return NULL;
	}

	BYTE Up = 0;
	BYTE Down = 0;
	BYTE Left = 0;
	BYTE Right = 0;

	while(tY >= 0 && tY <= 9)
	{
		if((pCells[tY * 10 + tX] == M_SHIP))
		{
			return NULL;
		}
		else
		{
			if(pCells[tY * 10 + tX] == M_BROKENSHIP)
			{
				Up++;
			}
			else
			{
				break;
			}

			if(tY == 0)
			{
				break;
			}
			else
			{
				tY = tY - 1;
			}
		}
	}

	tX = x - 1;
	tY = y - 1;

	while(tY >= 0 && tY <= 9)
	{
		if((pCells[tY * 10 + tX] == M_SHIP))
		{
			return NULL;
		}
		else
		{
			if(pCells[tY * 10 + tX] == M_BROKENSHIP)
			{
				Down++;
			}
			else
			{
				break;
			}
			
			if(tY == 9)
			{
				break;
			}
			else
			{
				tY = tY + 1;
			}
		}
	}

	tX = x - 1;
	tY = y - 1;


	while(tX >= 0 && tX <= 9)
	{
		if((pCells[tY * 10 + tX] == M_SHIP))
		{
			return NULL;
		}
		else
		{
			if(pCells[tY * 10 + tX] == M_BROKENSHIP)
			{
				Left++;
			}
			else
			{
				break;
			}

			if(tX == 0)
			{
				break;
			}
			else
			{
				tX = tX - 1;
			}
		}
	}

	tX = x - 1;
	tY = y - 1;


	while(tX >= 0 && tX <= 9)
	{
		if((pCells[tY * 10 + tX] == M_SHIP))
		{
			return NULL;
		}
		else
		{
			if(pCells[tY * 10 + tX] == M_BROKENSHIP)
			{
				Right++;
			}
			else
			{
				break;
			}
		
			if(tX == 9)
			{
				break;
			}
			else
			{
				tX = tX + 1;
			}
		}
	}

	tX = x - 1;
	tY = y - 1;

	////////////////////////
	BYTE RetVal = 0;

	while(--Up > 0)
	{
		RetVal++;
		(pCells[(tY - Up) * 10 + tX] &= M_EMPTY) |= M_WHOLEBROKEN;
	}
	while(--Down > 0)
	{
		RetVal++;
		(pCells[(tY + Down) * 10 + tX] &= M_EMPTY) |= M_WHOLEBROKEN;
	}
	while(--Left > 0)
	{
		RetVal++;
		(pCells[tY * 10 + (tX - Left)] &= M_EMPTY) |= M_WHOLEBROKEN;
	}
	while(--Right > 0)
	{
		RetVal++;
		(pCells[tY * 10 + (tX + Right)] &= M_EMPTY) |= M_WHOLEBROKEN;
	}

	(pCells[(tY * 10) + tX] &= M_EMPTY) |= M_WHOLEBROKEN;

	return ++RetVal;
}

BYTE CBoard::GetCountOfShip()
{
	return vCount.vCount;
}

void CBoard::Show()
{
	cout<<"\n    1 2 3 4 5 6 7 8 9 10\n\n";
	for(BYTE i = 0; i < BOARD_WIDTH; i++)
	{
		cout<<setw(2)<<(i+1)<<"  ";
		for(BYTE j = 0; j < BOARD_HEIGTH; j++)
		{
			switch(pCells[(i*10) + j])
			{
			case M_EMPTY:	//empty cell
			{
				cout<<": ";
			}break;

			case M_SHIP:	//ship in cell
			{
				cout<<"S ";
			}break;

			case M_BROKENSHIP:	//broken ship
			{
				cout<<"X ";
			}break;

			case M_MISSED:	//failed shot
			{
				cout<<"* ";
			}break;

			case M_WHOLEBROKEN:	//failed shot
			{
				cout<<"W ";
			}break;

			default:
			{
				cout<<"] ";
			}
			}
		}
		cout<<endl;
	}
}

void CBoard::SetRandomPosOfShip()
{
	bool tDir = true;	//temporary variable "direction" of ship
	bool bRetVal = false;
	BYTE xPos = 0;
	BYTE yPos = 0;
	//four fold ship
	do
	{
		((this->GetRandValue()%2) == 0) ? tDir = true : tDir = false;	//random direction	
		xPos = GetRandValue();
		yPos = GetRandValue();
		bRetVal = SetShip(xPos, yPos, tDir, FFOLD_SHIP);
	}
	while(!bRetVal);

	while(vCount.vTr)	//triple ship
	{
		do
		{
			((this->GetRandValue()%2) == 0) ? tDir = true : tDir = false;	//random direction	
			xPos = GetRandValue();
			yPos = GetRandValue();
			bRetVal = SetShip(xPos, yPos, tDir, TRIPLE_SHIP);
		}
		while(!bRetVal);
	}

	while(vCount.vDb)	//double ship
	{
		do
		{
			((this->GetRandValue()%2) == 0) ? tDir = true : tDir = false;	//random direction	
			xPos = GetRandValue();
			yPos = GetRandValue();
			bRetVal = SetShip(xPos, yPos, tDir, DOUBLE_SHIP);
		}
		while(!bRetVal);
	}

	while(vCount.vSi)	//single ship
	{
		do
		{
			((this->GetRandValue()%2) == 0) ? tDir = true : tDir = false;	//random direction	
			xPos = GetRandValue();
			yPos = GetRandValue();
			bRetVal = SetShip(xPos, yPos, tDir, SINGLE_SHIP);
		}
		while(!bRetVal);
	}
}


void CBoard::Clear()
{
	memset(pCells, 0, BOARD_HEIGTH * BOARD_WIDTH);

	memset(&vCount, 0, sizeof(CCount));
	vCount.vCount |= M_ALLSHIP_COUNT;
}


//private
BYTE CBoard::GetRandValue()
{
	//srand(time(NULL));
	BYTE RetVal =(BYTE)( rand() % MAX_RAND_VAL + MIN_RAND_VAL);	//random value MAX to MIN equal: rand() % (max - min + 1) + min;
	return RetVal;
}


void CBoard::CalcShip(IN BYTE aSize)
{
	switch(aSize)
	{
		case SINGLE_SHIP:
		{
			vCount.vSi -= 1;
		}break;

		case DOUBLE_SHIP:
		{
			vCount.vDb -= 1;
		}break;

		case TRIPLE_SHIP:
		{
			vCount.vTr -= 1;
		}break;

		case FFOLD_SHIP:
		{
			vCount.vFf -= 1;
		}break;

		default:
		{
			return;
		}
	}
}


bool CBoard::ScanRegion(IN BYTE x, IN BYTE y, IN bool dir, IN BYTE size)
{
	BYTE tX = x - 1;
	BYTE tY = y - 1;
	BYTE xStep = 0;
	BYTE yStep = 0;
	BYTE xBackStep = 0;
	BYTE yBackStep = 0;

	if(dir == HORIZ_DIRECT) //horizontal position
	{
////////////////////////////////////////
//xPos
		if(tX == 0)
		{
			xBackStep = 0;
			xStep = size;
		}
		else
		{
			if(tX == (9 - (size - 1)))
			{
				xBackStep = 1;
				xStep = size - 1;
			}
			else
			{
				xBackStep = 1;
				xStep = size;
			}
		}
//////////////////////////////////
//yPos
		if(tY == 0)
		{
			yBackStep = 0;
			yStep = 1;
		}
		else
		{
			if(tY == 9)
			{
				yBackStep = 1;
				yStep = 0;
			}
			else
			{
				yBackStep = 1;
				yStep = 1;
			}
		}
	}
	else //vertical position
	{
////////////////////////////////////////
//xPos
		if(tX == 0)
		{
			xBackStep = 0;
			xStep = 1;
		}
		else
		{
			if(tX == 9)
			{
				xBackStep = 1;
				xStep = 0;
			}
			else
			{
				xBackStep = 1;
				xStep = 1;
			}
		}
//////////////////////////////////
//yPos
		if(tY == 0)
		{
			yBackStep = 0;
			yStep = size;
		}
		else
		{
			if(tY == (9 - (size - 1)))
			{
				yBackStep = 1;
				yStep = size - 1;
			}
			else
			{
				yBackStep = 1;
				yStep = size;
			}
		}
	}
//////////////////////////////////
	//
	for(BYTE i = tX - xBackStep; i <= tX + xStep; i++)
	{
		for(BYTE j = tY - yBackStep; j <= tY + yStep; j++)
		{
			if(pCells[j * 10 + i] != M_EMPTY)
			{
				return false;
			}
		}
	}
	return true;
}


//check ship
bool CBoard::CheckShip(IN BYTE size)
{
	switch(size)
	{
		case SINGLE_SHIP:
		{
			if(vCount.vSi == 0)	//all single ships are situated
			{
				return false;
			}
		}break;
		case DOUBLE_SHIP:
		{
			if(vCount.vDb == 0)	//all double ships are situated
			{
				return false;
			}
		}break;
		case TRIPLE_SHIP:
		{
			if(vCount.vTr == 0)	//all triple ships are situated
			{
				return false;
			}
		}break;
		case FFOLD_SHIP:
		{
			if(vCount.vFf == 0)	//all four fold ships are situated
			{
				return false;
			}
		}break;

		default:
		{
			return false;
		}
	}
	return true;
}