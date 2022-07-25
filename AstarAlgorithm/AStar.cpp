#include "AStar.h"

void AStar::Init()
{
    Path_Check = priority_queue<priority>();
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            Center[j][i] = { j * 50 + 25, i * 50 + 25 };
            StartToCur[j][i] = { Center[j][i].x - 23, Center[j][i].y - 23 };
            EndToCur[j][i] = { Center[j][i].x - 23, Center[j][i].y-10 };
            Total[j][i] = { Center[j][i].x - 23, Center[j][i].y+5 };
        }
    }
    memset(StartG, -1, sizeof(StartG[0][0]) * 20 * 20);
    memset(EndH, -1, sizeof(EndH[0][0]) * 20 * 20);
    memset(TotalF, -1, sizeof(TotalF[0][0]) * 20 * 20);
    memset(isClose, 0, sizeof(isClose[0][0]) * 20 * 20);
    memset(isStart, 0, sizeof(isStart[0][0]) * 20 * 20);
    memset(isEnd, 0, sizeof(isEnd[0][0]) * 20 * 20);
    memset(isVisited, 0, sizeof(isVisited[0][0]) * 20 * 20);
}

void AStar::DrawGHF_Text(HDC hdc)
{
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            if (isClose[i][j] || isStart[i][j] || isEnd[i][j] || TotalF[i][j] == -1 || EndH[i][j] == -1)
                continue;
            wchar_t Sbuffer[256];
            wsprintfW(Sbuffer, L"G=%d", StartG[i][j]);
            TextOut(hdc, StartToCur[i][j].x, StartToCur[i][j].y, Sbuffer, _tcslen(Sbuffer));

            wchar_t Ebuffer[256];
            wsprintfW(Ebuffer, L"H=%d", EndH[i][j]);
            TextOut(hdc, EndToCur[i][j].x, EndToCur[i][j].y, Ebuffer, _tcslen(Ebuffer));

            wchar_t Tbuffer[256];
            wsprintfW(Tbuffer, L"F=%d", TotalF[i][j]);
            TextOut(hdc, Total[i][j].x, Total[i][j].y, Tbuffer, _tcslen(Tbuffer));
        }
    }
}

bool AStar::Set_FromStart_Position(int i, int j, priority_queue<priority>& path_Check)
{
    for (int k = i - 1; k <= i + 1; ++k)
    {
        if (k < 0 || k >= 20) // 범위를 벗어날 때
            continue;

        for (int l = j - 1; l <= j + 1; ++l)
        {
            if (l < 0 || l >= 20 || (l == j && k == i)) // 범위를 벗어날 때 or 자기 자신일 때
                continue;
            if (isClose[k][l] || isVisited[k][l]) // 장애물이거나, 이미 들렀던 곳일 때
                continue;
            if (this->isEnd[i][j]) // 종료점을 찾았을 때
            {
                PathNode[k][l].parent = &PathNode[i][j];
                return true;
            }

            if (abs(l - j) + abs(k - i) == 1) // 상하좌우
            {
                if ((StartG[k][l] == -1) || (StartG[k][l] > StartG[i][j] + 10))
                {
                    StartG[k][l] = StartG[i][j] + 10;
                    PathNode[k][l].parent = &PathNode[i][j];
                    PathNode[k][l].curPos = { k,l };
                    int diff_x = abs(END_INDEX.x - k);
                    int diff_y = abs(END_INDEX.y - l);
                    int diff = abs(diff_x - diff_y);
                    int End_XorY = diff_x > diff_y ? diff_x - diff : diff_y - diff;
                    EndH[k][l] = diff * 10 + End_XorY * 14;
                    TotalF[k][l] = StartG[k][l] + EndH[k][l];                    
                    path_Check.push(priority(POINT{ k, l },StartG[k][l],TotalF[k][l]));
                    int teptemp = 0;
                }
            }
            else // 대각선
            {
                if ((StartG[k][l] == -1) || (StartG[k][l] > StartG[i][j] + 14))
                {
                    StartG[k][l] = StartG[i][j] + 14;
                    PathNode[k][l].parent = &PathNode[i][j];
                    PathNode[k][l].curPos = { k,l };
                    int diff_x = abs(END_INDEX.x - k);
                    int diff_y = abs(END_INDEX.y - l);
                    int diff = abs(diff_x - diff_y);
                    int End_XorY = diff_x > diff_y ? diff_x - diff : diff_y - diff;
                    EndH[k][l] = diff * 10 + End_XorY * 14;
                    TotalF[k][l] = StartG[k][l] + EndH[k][l];

                    path_Check.push(priority(POINT{ k, l }, StartG[k][l], TotalF[k][l]));
                    int teptemp = 0;
                }
            }

            
        }
    }
    // 우선순위 큐에 저장
    for (int k = 0; k < path_Check.size(); ++k)
    {     
        isVisited[path_Check.top().p.x][path_Check.top().p.y] = true;
        int x = path_Check.top().p.x;
        int y = path_Check.top().p.y;
        path_Check.pop();
        bool check = Set_FromStart_Position(x, y, path_Check);
        if (check)
            return true;
    }
}
