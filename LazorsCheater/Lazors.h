#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Point.h"
#include "Laser.h"
#include "Source.h"

using namespace std;

const int max_map = 15;
const int max_box = 7;

class Lazors
{
public:
	Lazors(void);				//无参构造
	Lazors(string);				//带参构造

	void Clear(void);			//清空所有参数

	bool IsFileExist(void);		//判断文件是否存在
	void Load(void);			//读取关卡配置文件

	bool Solve(void);			//求解

private: 
	void LoadOption(void);		//读取配置文件

	void ResetMap(void);		//重置棋盘为初始状态
	void ResetBox(void);		//重置方块为初试状态

	int GetAvailable(void);		//获取可用方块数量
	void Recursion(int, int, Point*, int*);	//递归求解

	void FindAnother(int x, int y, int &otherX, int &otherY);//找到另一个黑洞
	bool Check(bool IsRecordMaxRayTracing = false);//检查是否符合要求
	void RayTracing(Laser, int, bool);//光线跟踪
	bool IsRayOut(int, int);	//光线是否位于区域外

	void SyncMap(int, Point*, int*);
	void PrintMap(void);

	void TestRayTracing(void);	//光线跟踪测试

private:
	string filename;

	int n, m;
	int map_origin[max_map][max_map],//0 - 不可用; 1 - 可用; 2 - 反射; 3 - 折射; 4 - 吸收; 5 - 钻石; 6 - 黑洞
		map[max_map][max_map];
	
	int box_origin[max_box],//每种方块的数量: 1 - 可用; 2 - 反射; 3 - 折射; 4 - 吸收; 5 - 钻石; 6 - 黑洞
		box[max_box];

	Source source[max_map][max_map];//该点光源情况

	int n_laser;
	Laser l[max_map];
	
	int n_bulb;
	Point p[max_map];

	int threshold;				//最大跟踪步数阈值
	
	int t0, t1;					//程序耗时
	int times_answer;			//砖块枚举次数
	int totalStep_rayTracing;	//跟踪总步数
	
	/*正确解时*/
	int maxStep_rayTracing;		//最大跟踪步数
	int maxFloor_rayTracing;	//跟踪函数最大递归层数
	int times_rayTracing;		//跟踪函数执行次数
	
	bool isFindAnswer;//是否找到解
};