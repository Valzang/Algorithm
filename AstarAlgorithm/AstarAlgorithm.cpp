// AstarAlgorithm.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#define _USE_MATH_DEFINES
#include "framework.h"
#include "Astar.h"
#include <commdlg.h>

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100


HDC hdc;
PAINTSTRUCT ps;
static RECT rcClient;

// ==============================================>>

HBITMAP hSigongImage;
BITMAP bitSigong;

HBITMAP hDoubleBufferImage;

void DrawBitmapDoubleBuffering(HDC hdc);


bool MoveDirection = true;
TCHAR sKeyState[128];
POINT ptAni = { 200, 400 };


enum BS { path_open = 0, path_close = 1, start_point = 2 , end_point = 3, visited_way = 4, chose_way = 5};
int BoxState[20][20] = { 0 };
AStar astar_data;

bool Lbuttondown = false;

int Start_and_End = 0;
POINT startPos, EndPos; // 시작 지점과 끝 지점.

bool End_Finished = false, Start_Finished = false; // 시작과 끝 지점을 찍었는 지 여부

ULONG_PTR g_GdiToken;

void Gdi_Init()
{
    GdiplusStartupInput gpsi;
    GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void Gdi_End()
{
    GdiplusShutdown(g_GdiToken);
}


// ==============================================<<

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASTARALGORITHM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTARALGORITHM));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTARALGORITHM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASTARALGORITHM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, _T("A* PathFinding"), WS_OVERLAPPEDWINDOW,
                             0, 0, 1016, 1059, nullptr, nullptr, hInstance, nullptr);


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
	{
        case WM_CREATE:
            GetClientRect(hWnd, &rcClient);
            //Gdi_Init();
            astar_data.Init();
            break;
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 메뉴 선택을 구문 분석합니다:
			switch (wmId)
			{
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            //DrawBitmapDoubleBuffering(hdc);
            for (int i = 0; i < rcClient.right; i += 50)
            {
                for (int j = 0; j < rcClient.bottom; j += 50)
                {
                    switch (BoxState[i / 50][j / 50])
                    {
                        case path_open:
                        default:
                            Rectangle(hdc, j, i, j + 50, i + 50);
                            break;
                        case path_close:
                        {
                            HBRUSH hBrush, oldBrush; // 도형의 내부 색
                            hBrush = CreateSolidBrush(RGB(100, 100, 100));
                            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                            Rectangle(hdc, j, i, j + 50, i + 50);
                            SelectObject(hdc, oldBrush);
                            DeleteObject(hBrush);
                        }
                            break;
                        case start_point:
                        case end_point:
                        {
                            HBRUSH hBrush, oldBrush; // 도형의 내부 색
                            hBrush = CreateSolidBrush(RGB(0, 100, 200));
                            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                            Rectangle(hdc, j, i, j + 50, i + 50);
                            SelectObject(hdc, oldBrush);
                            DeleteObject(hBrush);
                        }
                        break;
                        case visited_way:
                        {
                            HBRUSH hBrush, oldBrush; // 도형의 내부 색
                            hBrush = CreateSolidBrush(RGB(40, 255, 0));
                            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                            Rectangle(hdc, j, i, j + 50, i + 50);
                            SelectObject(hdc, oldBrush);
                            DeleteObject(hBrush);
                        }
                            break;
                        case chose_way:
                        {
                            HBRUSH hBrush, oldBrush; // 도형의 내부 색
                            hBrush = CreateSolidBrush(RGB(255, 80, 0));
                            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                            Rectangle(hdc, j, i, j + 50, i + 50);
                            SelectObject(hdc, oldBrush);
                            DeleteObject(hBrush);
                        }
                        break;
                    }
                }
            }

            astar_data.DrawGHF_Text(hdc);

			EndPaint(hWnd, &ps);
		}
		break;
        case WM_LBUTTONUP:
            Lbuttondown = false;
            break;

        case WM_MOUSEMOVE:
        {
            if (!Lbuttondown)
                break;
            if (End_Finished)
            {
                Start_and_End = 0;
                End_Finished = false;
                Start_Finished = false;
                astar_data.Init();
                memset(BoxState, path_open, sizeof(BoxState[0][0]) * 20 * 20);
                InvalidateRect(hWnd, NULL, TRUE);
            }
            POINT startPos;
            startPos.x = LOWORD(lParam);
            startPos.y = HIWORD(lParam);

            bool break_point = false;

            for (int i = 0; i < rcClient.right; i += 50)
            {
                for (int j = 0; j < rcClient.bottom; j += 50)
                {
                    if ((startPos.x < i + 50 && startPos.x >= i)
                        && (startPos.y < j + 50 && startPos.y >= j))
                    {
                        if (BoxState[j / 50][i / 50] == path_open)
                        {
                            BoxState[j / 50][i / 50] = path_close;
                            astar_data.SetClose(i / 50, j / 50);
                            InvalidateRect(hWnd, NULL, TRUE);
                            break_point = true;
                            break;
                        }
                    }
                }
                if (break_point)
                    break;
            }
        }
            break;

        case WM_LBUTTONDOWN:
            Lbuttondown = true;
            break;
        case WM_RBUTTONDOWN:
        {        
            
            bool before_start_Break = false;
            switch (Start_and_End)
            {
                case 0:
                    startPos.x = LOWORD(lParam);
                    startPos.y = HIWORD(lParam);
                    Start_Finished = true;
                    ++Start_and_End;
                    break;

                case 1:
                {
                    EndPos.x = LOWORD(lParam);
                    EndPos.y = HIWORD(lParam);
                    astar_data.SetEnd(EndPos.x / 50, EndPos.y / 50);
                    astar_data.SetStart(startPos.x / 50, startPos.y / 50);

                    for (int i = 0; i < 20; ++i)
                    {
                        for (int j = 0; j < 20; ++j)
                        {
                            if (astar_data.GetVisited(i, j))
                                BoxState[j][i] = visited_way;
                        }
                    }
                    BoxState[startPos.y / 50][startPos.x / 50] = start_point;
                    BoxState[EndPos.y / 50][EndPos.x / 50] = end_point;

                    POINT tempNode = { EndPos.x / 50, EndPos.y / 50 };
                    if (astar_data.getPathNode(tempNode.x, tempNode.y).parent != nullptr)
                    {       
                        while (astar_data.getPathNode(tempNode.x, tempNode.y).parent->curPos.x != 0)
                        {
                            tempNode = astar_data.getPathNode(tempNode.x, tempNode.y).parent->curPos;
                            BoxState[tempNode.y][tempNode.x] = chose_way;
                        }
                    }
                    ++Start_and_End; 
                    InvalidateRect(hWnd, NULL, TRUE);
                    End_Finished = true;
                }
                    break;

                default:
                    before_start_Break = true;
                    break;
            }
            if (before_start_Break)
                break;

            for (int i = 0; i < rcClient.right; i += 50)
            {
                for (int j = 0; j < rcClient.bottom; j += 50)
                {
                    if ((startPos.x < i + 50 && startPos.x >= i)
                        && (startPos.y < j + 50 && startPos.y >= j)
                        && BoxState[j / 50][i / 50] == path_open)
                    {
                        BoxState[j / 50][i / 50] = start_point;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    else if (End_Finished && (EndPos.x < i + 50 && EndPos.x >= i)
                             && (EndPos.y < j + 50 && EndPos.y >= j)
                             && BoxState[j / 50][i / 50] == path_open)
                    {

                        BoxState[j / 50][i / 50] = end_point;
                        InvalidateRect(hWnd, NULL, TRUE);
                        before_start_Break = true;
                        break;
                    }
                }
                if (before_start_Break)
                    break;
            }

        }
        break;
		case WM_DESTROY:
			PostQuitMessage(0);
            Gdi_End();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}




void DrawBitmapDoubleBuffering(HDC hdc)
{
    HDC hMemDC;
    HBITMAP hOldBitmap;

    // : 건드리지 않는 배경 이미지 =====================================================
    hMemDC = CreateCompatibleDC(hdc);
    if (hDoubleBufferImage == NULL)
        hDoubleBufferImage = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);

    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);

    for (int i = 0; i <= rcClient.right; i += 50)
    {
        MoveToEx(hMemDC, i, 0, NULL);
        LineTo(hMemDC, i, rcClient.right);
        MoveToEx(hMemDC, 0, i, NULL);
        LineTo(hMemDC, rcClient.bottom, i);
    }
    BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hMemDC, 0, 0, SRCCOPY);
    //SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
    
}
