#pragma once
#include "resource.h"
#include <sstream>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <queue>
#include <tchar.h>
#pragma comment(lib,"Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

struct priority {
    POINT p;
    int G_Value;
    int F_Value;
    priority(POINT _p, int _G_Value, int _F_Value) : p(_p), G_Value(_G_Value), F_Value(_F_Value) {}
    bool operator<(const priority tg) const {
        if (F_Value > tg.F_Value)
            return true;
        else if (F_Value == tg.F_Value)
            return (G_Value > tg.G_Value);
        else
            return false;
    }
};

struct NODE
{
    NODE* parent = nullptr;
    POINT curPos = { -1,-1 };
};

class AStar
{
private:
    

    POINT StartToCur[20][20]; // ���������������� �Ÿ��� ǥ���� ��ǥ
    POINT EndToCur[20][20]; // ���������������� �Ÿ��� ǥ���� ��ǥ
    POINT Center[20][20]; // ����ϰ� �� ĭ�� �߽���ǥ
    POINT Total[20][20]; // �Ÿ� �հ踦 ǥ���� ��ǥ

    int StartG[20][20], EndH[20][20], TotalF[20][20]; // ������������ �Ÿ� / ������������ �Ÿ�/ �Ÿ� �ջ�
    bool isClose[20][20], isStart[20][20], isEnd[20][20], isVisited[20][20]; // ������, ���簡 ������/����������, �湮�ߴ� ������

    POINT START_INDEX;
    POINT END_INDEX;

    NODE PathNode[20][20]; // �ִܰŸ� ���
    priority_queue<priority> Path_Check; // �湮�ϰ� �Ǵ� �켱���� ť

public:
    void Init();

    void SetClose(int i, int j) { isClose[i][j] = true; }


    bool GetVisited(int i, int j) { return isVisited[i][j]; }

    NODE getPathNode(int i, int j) { return PathNode[i][j]; }

    priority_queue<priority> getVisited() { return Path_Check; }

    void DrawGHF_Text(HDC hdc);

    bool Set_FromStart_Position(int i, int j, priority_queue<priority>& path_Check);

    void SetStart(int i, int j)
    {
        PathNode[i][j].curPos = POINT{ 0,0 };
        isVisited[i][j] = true;
        isStart[i][j] = true;
        StartG[i][j] = 0;
        TotalF[i][j] = 0;
        START_INDEX = { i,j };
        Set_FromStart_Position(i, j, Path_Check);
    }


    void SetEnd(int i, int j)
    {
        END_INDEX = { i,j };
        isEnd[i][j] = true;
        EndH[i][j] = 0;
    }
};