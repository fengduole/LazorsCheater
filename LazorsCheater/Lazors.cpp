#include "Lazors.h"


Lazors::Lazors(void)
{
	Clear();
	LoadOption();
}


Lazors::Lazors(string fileName)
{
	Clear();
	LoadOption();

	filename = fileName;
	Load();
}


void Lazors::Clear(void)
{
	n = m = 0;
	n_laser = n_bulb = 0;

	threshold = 60;

	times_answer = 0;
	totalStep_rayTracing = 0;
	maxStep_rayTracing = 0;
	maxFloor_rayTracing = 0;
	times_rayTracing = 0;

	isFindAnswer = false;

	memset(map_origin, 0, sizeof(map_origin));
	memset(box_origin, 0, sizeof(box_origin));
	memset(source, 0, sizeof(source));
}


bool Lazors::IsFileExist(void)
{
	fstream File;

    File.open(filename, ios::in);
     
	if (!File)
	{
		File.close();
		return false;
	}
	else
	{
		File.close();
		return true;
	}
}


void Lazors::Load(void)
{
	if (!IsFileExist())
		return;

	ifstream inFile(filename);

	inFile >> n >> m;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			inFile >> map_origin[i][j];

			if (map_origin[i][j] == 1)
				box_origin[1]++;
		}
	}
	ResetMap();

	for (int i = 2; i < max_box; i++)
	{
		inFile >> box_origin[i];
		box_origin[0] += box_origin[i];
	}
	box_origin[1] -= box_origin[0];
	ResetBox();

	inFile >> n_laser;
	for (int i = 1; i <= n_laser; i++)
	{
		inFile >> l[i].x >> l[i].y >> l[i].dx >> l[i].dy;
	}

	inFile >> n_bulb;
	for (int i = 1; i <= n_bulb; i++)
	{
		inFile >> p[i].x >> p[i].y;
	}

	inFile.close();
}


bool Lazors::Solve(void)
{
	int n_available = GetAvailable();
	Point pos_available[max_map * max_map];
	int select_available[max_map * max_map];
	int k = 0;

	memset(select_available, 0, sizeof(select_available));

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (map_origin[i][j] == 1)
			{
				k++;
				pos_available[k].x = i;
				pos_available[k].y = j;
			}
		}
	}

	t0 = GetTickCount();

//	TestRayTracing();

	Recursion(1, n_available, pos_available, select_available);

	return isFindAnswer;
}

//-----------------私有函数-----------------

void Lazors::LoadOption(void)
{
	ifstream inFile("Option.ini");

	inFile >> threshold;

	inFile.close();
}


void Lazors::ResetMap(void)
{
	memset(map, 0, sizeof(map));
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			map[i][j] = map_origin[i][j];
		}
	}	
}


void Lazors::ResetBox(void)
{
	for (int i = 0; i < max_box; i++)
	{
		box[i] = box_origin[i];
	}
}


int Lazors::GetAvailable(void)
{
	int sum = 0;

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (map_origin[i][j] == 1)
				sum++;
		}
	}	

	return sum;
}


void Lazors::Recursion(int x, int n_available, Point pos[], int select[])
{
	if (isFindAnswer)
		return;

	if (x > n_available)
	{
		SyncMap(n_available, pos, select);

		if (Check())
		{
			t1 = GetTickCount();
			
			Check(true);
			PrintMap();
			isFindAnswer = true;
			
			cout << endl;

			cout << "Time:  " << t1 - t0 << "ms" << endl;
			cout << "Try:   " << times_answer << endl;
			cout << "Step:  " << totalStep_rayTracing << endl << endl;
			cout << "Max:   " << maxStep_rayTracing << endl;
			cout << "Times: " << times_rayTracing << endl;
			cout << "Floor: " << maxFloor_rayTracing << endl << endl;

			system("pause");
		}
		else 
		{
			times_answer++;
		}

		return;
	}

	for (int j = max_box - 1; j >= 1; j--)
	{
		if (box[j])
		{
			box[j]--;
			select[x] = j;

			Recursion(x + 1, n_available, pos, select);

			box[j]++;
			select[x] = 0;
		}
	}
}


void Lazors::FindAnother(int x, int y, int &otherX, int &otherY)
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (map[i][j] == 6)
			{
				if (i != x || j != y)
				{
					otherX = i;
					otherY = j;

					return;
				}
			}
		}
	}
}


bool Lazors::Check(bool IsRecordMaxRayTracing)
{
	memset(source, 0, sizeof(source));
	
	for (int i = 1; i <= n_laser; i++)
	{
		RayTracing(l[i], 1, IsRecordMaxRayTracing);
	}

	for (int i = 1; i <= n_bulb; i++)
	{
		if (!source[p[i].x][p[i].y].isLight)
			return false;
	}

	return true;
}


void Lazors::RayTracing(Laser laser, int floor = 1, bool IsRecordMaxRayTracing = false)
{
	int x, y;
	int step = 0;

	if (IsRecordMaxRayTracing)
	{
		times_rayTracing++;
		if (floor > maxFloor_rayTracing)
			maxFloor_rayTracing = floor;
	}

	while (step < threshold)
	{
		step++;
		totalStep_rayTracing++;

		if(IsRecordMaxRayTracing)
		{	
			if (step > maxStep_rayTracing)
				maxStep_rayTracing = step;
		}

		if (!IsRayOut(laser.x, laser.y))
			return;

		if (source[laser.x][laser.y].isLight)
		{	
			if (!source[laser.x][laser.y].laser[laser.dx + 1][laser.dy + 1])
				source[laser.x][laser.y].laser[laser.dx + 1][laser.dy + 1] = true;
			else
				return;
		}
		else
		{
			source[laser.x][laser.y].isLight = true;
			source[laser.x][laser.y].laser[laser.dx + 1][laser.dy + 1] = true;
		}

		if (laser.x % 2)//位于横边
		{
			if (laser.dy > 0)
				x = laser.y / 2 + 1;
			else
				x = laser.y / 2;
			y = laser.x / 2 + 1;

			if (map[x][y] == 0 || map[x][y] == 1)//什么都没放
			{
				laser.x += laser.dx;
				laser.y += laser.dy;
			}
			else if (map[x][y] == 2)//反射
			{
				laser.dy *= -1;
			}
			else if (map[x][y] == 3)//折射
			{
				Laser newLaser;

				newLaser = laser;
				newLaser.x += newLaser.dx;
				newLaser.y += newLaser.dy;

				laser.dy *= -1;	

				RayTracing(newLaser, floor + 1, IsRecordMaxRayTracing);
			}
			else if (map[x][y] == 4)//吸收
			{
				return;
			}
			else if (map[x][y] == 5)//钻石
			{
				laser.y += 2 * laser.dy;
			}
			else if (map[x][y] == 6)//黑洞
			{
				int otherX, otherY;

				FindAnother(x, y, otherX, otherY);

				laser.x = otherY * 2 - 1;

				if (laser.dy == 1)//向下入射	
					laser.y = otherX * 2;
				else if (laser.dy == -1)//向上入射
					laser.y = (otherX - 1) * 2;
			}
		}
		else//位于竖边
		{
			x = laser.y / 2 + 1;
			if (laser.dx > 0)
				y = laser.x / 2 + 1;
			else 
				y = laser.x / 2;

			if (map[x][y] == 0 || map[x][y] == 1)//什么都没放的情况
			{
				laser.x += laser.dx;
				laser.y += laser.dy;
			}
			else if (map[x][y] == 2)//反射
			{
				laser.dx *= -1;
			}
			else if (map[x][y] == 3)//折射
			{
				Laser newLaser;

				newLaser = laser;
				newLaser.x += newLaser.dx;
				newLaser.y += newLaser.dy;

				laser.dx *= -1;

				RayTracing(newLaser, floor + 1, IsRecordMaxRayTracing);
			}
			else if (map[x][y] == 4)//吸收
			{
				return;
			}
			else if (map[x][y] == 5)//钻石
			{
				laser.x += 2 * laser.dx;
			}
			else if (map[x][y] == 6)//黑洞
			{
				int otherX, otherY;

				FindAnother(x, y, otherX, otherY);

				if (laser.dx == 1)//向右入射	
					laser.x = otherY * 2;
				else if (laser.dx == -1)//向左入射
					laser.x = (otherY - 1) * 2;

				laser.y = otherX * 2 - 1;
			}
		}
	}
}


bool Lazors::IsRayOut(int x, int y)
{
	if ((0 <= x && x <= m * 2) && (0 <= y && y <= n * 2))
		return true;

	return false;
}


void Lazors::SyncMap(int n_available, Point pos[], int select[])
{
	ResetMap();
	for (int i = 1; i <= n_available; i++)
	{
		map[pos[i].x][pos[i].y] = select[i];
	}
}


void Lazors::PrintMap(void)
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (map[i][j] == 0)
				cout << "  ";
			else if (map[i][j] == 1)
				cout << ". ";
			else
				cout << map[i][j] << " ";
		}
		cout << endl;
	}
}


void Lazors::TestRayTracing(void)
{
	//map[1][3] = 3;
	//map[3][3] = 3;
	//map[4][2] = 3;
	//map[1][2] = 5;
	//map[4][4] = 3;
	//map[5][1] = 3;
	//map[6][2] = 3;

	map[1][4] = 3;
	map[3][2] = 3;
	map[3][3] = 3;
	map[4][2] = 5;
	map[5][4] = 3;
	map[5][1] = 3;
	map[6][3] = 3;

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			cout << map[i][j] << " ";
		}
		cout << endl;
	}

	system("pause");

	bool x = Check(true);

	cout << x << endl;
	//RayTracing(l[1]);

	system("pause");
}