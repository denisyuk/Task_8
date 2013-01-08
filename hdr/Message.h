#ifndef MESSAGE_H
#define MESSAGE_H

#include "defines.h"

class CMessage
{
public:
	CMessage();
	~CMessage();

	void SetKey(IN BYTE k);
	void SetShot(IN BYTE x, IN BYTE y);

	BYTE GetKey(void);
	BYTE GetX(void);
	BYTE GetY(void);

	void Clear(void);
private:
	BYTE	vKey;
	BYTE	vX;
	BYTE	vY;
};

#endif /*MESSAGE_H*/