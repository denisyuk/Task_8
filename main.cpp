#include "hdr\GameEngine.h"
#include "hdr\Thread.h"

void main(void)
{
	CGEngine* pGE = new CGEngine();	//Game engine
	CThread* pTh = new CThread();	//thread
	bool bExit = false;

	do
	{

		pGE->Wellcome();

		bool bChoice = pGE->ChoiceGame();
		if(bChoice)
		{
			if(!pTh->Acception())
			{
				continue;
			}
		}
		else
		{
			if(!pTh->Connection())
			{
				continue;
			}
		}
		pGE->SetSocket(pTh->GetOppSocket());
		pTh->CreateReceiveThread(pGE);
		pGE->SetShips();
		pGE->Battle();

		pTh->ClearSockets();

		cout<<"\nDo you want exit?\n";
		bExit = pGE->AnswerYN();
	}while(!bExit);


	
	delete pGE;
system("pause");
}