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
    

    POINT StartToCur[20][20]; // 시작점에서부터의 거리를 표기할 좌표
    POINT EndToCur[20][20]; // 종료점에서부터의 거리를 표기할 좌표
    POINT Center[20][20]; // 출력하게 될 칸의 중심좌표
    POINT Total[20][20]; // 거리 합계를 표기할 좌표

    int StartG[20][20], EndH[20][20], TotalF[20][20]; // 시작점에서의 거리 / 종료점에서의 거리/ 거리 합산
    bool isClose[20][20], isStart[20][20], isEnd[20][20], isVisited[20][20]; // 벽인지, 현재가 시작점/종료점인지, 방문했던 곳인지

    POINT START_INDEX;
    POINT END_INDEX;

    NODE PathNode[20][20]; // 최단거리 경로
    priority_queue<priority> Path_Check; // 방문하게 되는 우선순위 큐

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