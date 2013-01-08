#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <stdlib.h>
#include <time.h>

using namespace std;

typedef unsigned char BYTE;

#ifdef IN
#undef IN
#endif
#define IN const

//ships
const BYTE SINGLE_SHIP	=	0x1;	//single ship
const BYTE DOUBLE_SHIP	=	0x2;	//double ship
const BYTE TRIPLE_SHIP	=	0x3;	//triple ship
const BYTE FFOLD_SHIP	=	0x4;	//Four fold ship

const BYTE BOARD_HEIGTH	=	0xA;	//count of vertical cells
const BYTE BOARD_WIDTH	=	0xA;	//count of horizontal cells

const bool HORIZ_DIRECT	=	false;
const bool VERT_DIRECT	=	true;

const BYTE M_EMPTY		=	0x0;	//empty cell				0000 0000
const BYTE M_SHIP		=	0x1;	//ship is situated in cell	0000 0001
const BYTE M_BROKENSHIP	=	0x2;	//ship is broken in cell	0000 0010
const BYTE M_MISSED		=	0x3;	//failed shot (statistic)	0000 0011
const BYTE M_WHOLEBROKEN=	0x4;	//whole broken ship			0000 0100

const BYTE M_ALLSHIP_COUNT	=0x9D;	//1001 1101	(4 3 2 1  - see CCount struct)

//message
const BYTE MSG_READY		=0x1;	//opponent is ready
const BYTE MSG_SHOT			=0x2;	//shot
const BYTE MSG_MISSED		=0x3;	//fail shot
const BYTE MSG_DIRECTHIT	=0x4;	//direct hit
const BYTE MSG_WHOLESHIP	=0x5;	//whole ship is broken
const BYTE MSG_LOSE			=0x6;	//opponent is lose

#define MAX_IP_SIZE		16	//count of symbols in IP address

//random
#define MAX_RAND_VAL	10
#define MIN_RAND_VAL	1

//timer
#define ONE_SEC		1000

union CCount
{
	struct 
	{
		BYTE vFf	: 1;	//count of four fold ship	MAX 1
		BYTE vTr	: 2;	//count of triple ship		max 2
		BYTE vDb	: 2;	//count of double ship		max 3
		BYTE vSi	: 3;	//count of single ship		max 4
	};
	BYTE vCount;
};

#endif /*DEFINES_H*/