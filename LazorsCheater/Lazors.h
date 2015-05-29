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
	Lazors(void);				//�޲ι���
	Lazors(string);				//���ι���

	void Clear(void);			//������в���

	bool IsFileExist(void);		//�ж��ļ��Ƿ����
	void Load(void);			//��ȡ�ؿ������ļ�

	bool Solve(void);			//���

private: 
	void LoadOption(void);		//��ȡ�����ļ�

	void ResetMap(void);		//��������Ϊ��ʼ״̬
	void ResetBox(void);		//���÷���Ϊ����״̬

	int GetAvailable(void);		//��ȡ���÷�������
	void Recursion(int, int, Point*, int*);	//�ݹ����

	void FindAnother(int x, int y, int &otherX, int &otherY);//�ҵ���һ���ڶ�
	bool Check(bool IsRecordMaxRayTracing = false);//����Ƿ����Ҫ��
	void RayTracing(Laser, int, bool);//���߸���
	bool IsRayOut(int, int);	//�����Ƿ�λ��������

	void SyncMap(int, Point*, int*);
	void PrintMap(void);

	void TestRayTracing(void);	//���߸��ٲ���

private:
	string filename;

	int n, m;
	int map_origin[max_map][max_map],//0 - ������; 1 - ����; 2 - ����; 3 - ����; 4 - ����; 5 - ��ʯ; 6 - �ڶ�
		map[max_map][max_map];
	
	int box_origin[max_box],//ÿ�ַ��������: 1 - ����; 2 - ����; 3 - ����; 4 - ����; 5 - ��ʯ; 6 - �ڶ�
		box[max_box];

	Source source[max_map][max_map];//�õ��Դ���

	int n_laser;
	Laser l[max_map];
	
	int n_bulb;
	Point p[max_map];

	int threshold;				//�����ٲ�����ֵ
	
	int t0, t1;					//�����ʱ
	int times_answer;			//ש��ö�ٴ���
	int totalStep_rayTracing;	//�����ܲ���
	
	/*��ȷ��ʱ*/
	int maxStep_rayTracing;		//�����ٲ���
	int maxFloor_rayTracing;	//���ٺ������ݹ����
	int times_rayTracing;		//���ٺ���ִ�д���
	
	bool isFindAnswer;//�Ƿ��ҵ���
};