#include "..\hdr\Message.h"

CMessage::CMessage()
{
	vKey = NULL;
	vX = NULL;
	vY = NULL;
}

CMessage::~CMessage()
{}

void CMessage::SetKey(IN BYTE k)
{
	vKey = k;
}

void CMessage::SetShot(IN BYTE x, IN BYTE y)
{
	vX = x;
	vY = y;
}

BYTE CMessage::GetKey(void)
{
	return vKey;
}

BYTE CMessage::GetX(void)
{
	return vX;
}

BYTE CMessage::GetY(void)
{
	return vY;
}

void CMessage::Clear(void)
{
	vKey = NULL;
	vX = NULL;
	vY = NULL;
}