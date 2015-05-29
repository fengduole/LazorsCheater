/**************************************
*                                     *
*    iOS游戏——Lazors 解密程序       *
* 		  适用游戏版本 2.30			  *
*                                     *
**************************************/

#include <iostream>

#include "Lazors.h"

using namespace std;

int main(void)
{
	string fileName;
	bool isFindAnswer;

	cin >> fileName;
	//fileName = "Friendly1";
	//fileName = "Glassware10";
	//fileName = "grande10";
	//fileName = "Wormhole6";

	Lazors maze(fileName + ".txt");
	
	isFindAnswer = maze.Solve();

	if (!isFindAnswer)
	{
		cout << "Can not find the answer." << endl;

		system("pause");
	}

	//system("pause");
	return 0;
}