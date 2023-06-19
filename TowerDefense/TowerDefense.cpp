// TowerDefense.cpp : Определяет точку входа для приложения.
//

#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "TowerDefense.h"
#include "stdio.h"
#include "time.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TOWERDEFENSE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOWERDEFENSE));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOWERDEFENSE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TOWERDEFENSE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Глобальные переменные

    // Вкл/Выкл
int StartNewWave = 0; // 0 выкл 1 вкл

    // Размеры объектов
int sizeX = 15; // По оси X
int sizeY = 15; // По оси Y

int LiveRdCb = 1; // Кол-во жизней противника
int LiveBlackCb = 10; // Прочность ворот
int NumMoney = 20; // Кол-во монет
int NumOpponents = 5; // Кол-во противников
int NumWave = 1; // Номер волны
int SumOpponents = 0; // Счетчик отрисованных противников на карте
int SumDeadOpponents = 0; // Счетчик мертвых противников
int Speed = 100; // Скорость передвижения противников

// Карта

#define NUM_PW_Y 25 // Количество массивов (строк)
#define NUM_PW_X 25 // Количество значений в одном массиве (столбцов)

int PaintWorld[NUM_PW_Y][NUM_PW_X] =
{
    {7,2,2,2,2, 2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,2,2,2, 2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,3,3,3, 3,3,3,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,3,2,2, 2,2,3,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,3,2,2, 2,2,3,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},

    {2,2,3,2,2, 2,2,3,2,2,  3,3,3,3,3,  3,3,3,3,2,  2,2,2,2,2},
    {2,2,3,2,2, 2,2,3,2,2,  3,2,2,2,2,  2,2,2,3,2,  2,2,2,2,2},
    {2,2,3,2,2, 2,2,3,2,2,  3,2,2,2,2,  2,2,2,3,2,  2,2,2,2,2},
    {2,2,3,2,2, 2,2,3,2,2,  3,2,2,2,2,  2,2,2,3,2,  2,2,2,2,2},
    {6,3,3,2,2, 2,3,3,2,2,  3,2,2,2,2,  4,2,2,3,2,  2,2,2,2,2},

    {2,2,2,2,2, 2,3,2,2,2,  3,2,2,2,4,  4,4,2,3,3,  3,3,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,2,2,2,4,  5,4,2,2,2,  2,2,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,2,2,2,2,  3,2,2,2,2,  2,2,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,3,2,2,2,  3,2,2,2,2,  2,2,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  2,3,2,2,2,  3,2,2,2,2,  2,2,3,2,2},

    {2,2,2,2,2, 2,3,2,2,2,  3,3,2,2,2,  3,2,2,2,2,  2,2,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,2,2,2,2,  3,2,2,2,2,  2,2,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,3,2,2,2,  3,2,2,2,2,  2,2,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  2,3,2,2,2,  3,3,3,3,3,  3,3,3,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,3,2,2,2,  2,2,2,2,2,  2,2,2,2,2},

    {2,2,2,2,2, 2,3,2,2,2,  3,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,2,2,2, 2,3,2,2,2,  3,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,2,2,2, 2,3,3,3,3,  3,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,2,2,2, 2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2},
    {2,2,2,2,2, 2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2,  2,2,2,2,2}
};

// 1 - башни,
// 2 - пустая клетка (трава);
// 3 - дорога;
// 4 - замок;
// 5 - ворота в замок;
// 6 - спавн врагов;
// 7 - игрок;
// 11,12,13,14,16,17,18,19,20 - противники;

// Отрисовка объектов

void DrawObject(HDC hdc) {
    HBRUSH hBrushRdCb = CreateSolidBrush(RGB(255, 0, 0)); // Противники [11,12,13,14] красный
    HBRUSH hBrushBlCb = CreateSolidBrush(RGB(0, 0, 255)); // Башни [1] синий
    HBRUSH hBrushGrCb = CreateSolidBrush(RGB(0, 255, 0)); // Пустое пространство [2] зеленый
    HBRUSH hBrushYlCb = CreateSolidBrush(RGB(255, 255, 100)); // Дорога [3] желтый
    HBRUSH hBrushTqBlCb = CreateSolidBrush(RGB(0, 255, 255)); // Стены замка [4] бирюзовый
    HBRUSH hBrushBlackCb = CreateSolidBrush(RGB(0, 0, 0)); // Ворота [5] черный
    HBRUSH hBrushPpCb = CreateSolidBrush(RGB(255, 0, 255)); // Спавн врагов [6] пурпурный
    HBRUSH hBrushGrayCb = CreateSolidBrush(RGB(150, 150, 150)); // Игрок [7] серый
    HBRUSH hBrushDarkOrange = CreateSolidBrush(RGB(255, 115, 30)); // Зона атаки башни [15] темно-оранжевый
    HBRUSH hBrushRdCb16 = CreateSolidBrush(RGB(220, 0, 0)); // Осталось 4 жизни
    HBRUSH hBrushRdCb17 = CreateSolidBrush(RGB(200, 0, 0)); // Осталось 3 жизни
    HBRUSH hBrushRdCb18 = CreateSolidBrush(RGB(180, 0, 0)); // Осталось 2 жизни
    HBRUSH hBrushRdCb19 = CreateSolidBrush(RGB(160, 0, 0)); // Осталась одна жизнь
    HBRUSH hBrushRdCb20 = CreateSolidBrush(RGB(90, 0, 0)); // Мертв

    int y = 0; // Номер массива

    while (y < NUM_PW_Y) {
        int x = 0; // Номер строки в массиве
        while(x < NUM_PW_X) {
            RECT rectangle = { x * sizeX, y * sizeY, (x + 1) * sizeX, (y + 1) * sizeY }; // Отрисовка прямоугольника с цветом...
            if (PaintWorld[y][x] == 1) {
                FillRect(hdc, &rectangle, hBrushBlCb); // Если 1 то синий
            }
            else if (PaintWorld[y][x] == 2) {
                FillRect(hdc, &rectangle, hBrushGrCb); // Если 2 то зеленый
            }
            else if (PaintWorld[y][x] == 3) {
                FillRect(hdc, &rectangle, hBrushYlCb); // Если 3 то желтый
            }
            else if (PaintWorld[y][x] == 4) {
                FillRect(hdc, &rectangle, hBrushTqBlCb); // Если 4 то бирюзовый
            }
            else if (PaintWorld[y][x] == 5) {
                FillRect(hdc, &rectangle, hBrushBlackCb); // Если 5 то черный
            }
            else if (PaintWorld[y][x] == 6) {
                FillRect(hdc, &rectangle, hBrushPpCb); // Если 6 то пурпурный
            }
            else if (PaintWorld[y][x] == 7) {
                FillRect(hdc, &rectangle, hBrushGrayCb); // Если 7 то серый
            }
            else if (PaintWorld[y][x] == -7) {
                FillRect(hdc, &rectangle, hBrushGrayCb); // Если -7 то серый
            }
            else if (PaintWorld[y][x] == 11) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если 11 то красный
            }
            else if (PaintWorld[y][x] == -11) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если -11 то красный
            }
            else if (PaintWorld[y][x] == 12) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если 12 то красный
            }
            else if (PaintWorld[y][x] == -12) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если -12 то красный
            }
            else if (PaintWorld[y][x] == 13) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если 13 то красный
            }
            else if (PaintWorld[y][x] == -13) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если -13 то красный
            }
            else if (PaintWorld[y][x] == 14) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если 14 то красный
            }
            else if (PaintWorld[y][x] == -14) {
                FillRect(hdc, &rectangle, hBrushRdCb); // Если -14 то красный
            }
            else if (PaintWorld[y][x] == 15) {
                FillRect(hdc, &rectangle, hBrushDarkOrange); // Если 15 то темно-оранжевый
            }
            else if (PaintWorld[y][x] == 1116) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == -1116) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == 1216) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == -1216) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == 1316) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == -1316) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == 1416) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == -1416) {
                FillRect(hdc, &rectangle, hBrushRdCb16); // Осталась 4 жизни
            }
            else if (PaintWorld[y][x] == 1117) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == -1117) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == 1217) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == -1217) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == 1317) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == -1317) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == 1417) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == -1417) {
                FillRect(hdc, &rectangle, hBrushRdCb17); // Осталась 3 жизни
            }
            else if (PaintWorld[y][x] == 1118) {
                FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == -1118) {
            FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == 1218) {
                FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == -1218) {
            FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == 1318) {
                FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == -1318) {
            FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == 1418) {
                FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == -1418) {
            FillRect(hdc, &rectangle, hBrushRdCb18); // Осталась 2 жизни
            }
            else if (PaintWorld[y][x] == 1119) {
                FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == -1119) {
            FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == 1219) {
                FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == -1219) {
            FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == 1319) {
                FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == -1319) {
            FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == 1419) {
                FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == -1419) {
            FillRect(hdc, &rectangle, hBrushRdCb19); // Осталась одна жизнь
            }
            else if (PaintWorld[y][x] == 20) {
            FillRect(hdc, &rectangle, hBrushRdCb20); // Мертв
            }
            else {
               
            }
            x++;
        }
        y++;
    }

    // Информационное окно

    HFONT font_txt; // Формат текста окна
    font_txt = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Courier New");

    SelectObject(hdc, font_txt);
    SetTextColor(hdc, RGB(0, 0, 0)); // Цвет текста в окне

    if (SumOpponents == SumDeadOpponents) {
        TCHAR ClickEnter[] = _T("Для старта нажмите Enter");
        TextOut(hdc, 400, sizeY + 150, (LPCWSTR)ClickEnter, _tcslen(ClickEnter));
    }

    // Содержание окна

    TCHAR  string1[] = _T("Прочность замка:"); // TCHAR - тип переменной (символы), _T - ???
    TCHAR  string2[] = _T("Монеты:");
    TCHAR  string3[] = _T("Волна:");
    TCHAR  string4[] = _T("Врагов:");
    TCHAR  string5[] = _T("Жизни врагов:");

    // Расположение содержимого (текста)

    TextOut(hdc, 400, sizeY, (LPCWSTR)string1, _tcslen(string1)); // TextOut(дескриптор, x, y, строка символов, кол-во символов)
    TextOut(hdc, 400, sizeY + 30, (LPCWSTR)string2, _tcslen(string2));
    TextOut(hdc, 400, sizeY + 280, (LPCWSTR)string3, _tcslen(string3));
    TextOut(hdc, 400, sizeY + 310, (LPCWSTR)string4, _tcslen(string4));
    TextOut(hdc, 400, sizeY + 340, (LPCWSTR)string5, _tcslen(string5));

    // Вывод переменных на экран

        // Прочность замка
    char S_LiveBlackCb[5];
    TCHAR  TS_LiveBlackCb[5];
    sprintf(S_LiveBlackCb, "%d", LiveBlackCb);
    OemToChar(S_LiveBlackCb, TS_LiveBlackCb);
    TextOut(hdc, 650, sizeY, (LPCWSTR)TS_LiveBlackCb, _tcslen(TS_LiveBlackCb));

        // Монеты
    char S_NumMoney[5];
    TCHAR  TS_NumMoney[5];
    sprintf(S_NumMoney, "%d", NumMoney);
    OemToChar(S_NumMoney, TS_NumMoney);
    TextOut(hdc, 650, sizeY + 30, (LPCWSTR)TS_NumMoney, _tcslen(TS_NumMoney));

        // Волна
    char S_NumWave[5];
    TCHAR  TS_NumWave[5];
    sprintf(S_NumWave, "%d", NumWave);
    OemToChar(S_NumWave, TS_NumWave);
    TextOut(hdc, 650, sizeY + 280, (LPCWSTR)TS_NumWave, _tcslen(TS_NumWave));

        // Врагов
    char S_NumOpponents[5];
    TCHAR  TS_NumOpponents[5];
    sprintf(S_NumOpponents, "%d", NumOpponents);
    OemToChar(S_NumOpponents, TS_NumOpponents);
    TextOut(hdc, 650, sizeY + 310, (LPCWSTR)TS_NumOpponents, _tcslen(TS_NumOpponents));

        // Жизни врагов
    char S_LiveRdCb[5];
    TCHAR  TS_LiveRdCb[5];
    sprintf(S_LiveRdCb, "%d", LiveRdCb);
    OemToChar(S_LiveRdCb, TS_LiveRdCb);
    TextOut(hdc, 650, sizeY + 340, (LPCWSTR)TS_LiveRdCb, _tcslen(TS_LiveRdCb));

    DeleteObject(hBrushRdCb);
    DeleteObject(hBrushBlCb);
    DeleteObject(hBrushGrCb);
    DeleteObject(hBrushYlCb);
    DeleteObject(hBrushTqBlCb);
    DeleteObject(hBrushBlackCb);
    DeleteObject(hBrushPpCb);
    DeleteObject(hBrushGrayCb);
    DeleteObject(hBrushDarkOrange);
    DeleteObject(font_txt);
}

// Отрисовываем противников на карте и считаем сколько нарисовали (SumOpponents)
// А также проверяем, прошел ли ранее отрисованный противник триггер (триггер находится на PaintWorld[6][2])
// Если противник наступил на триггер, то рисуем следующего.
// Рисуем до тех пор, пока противников не станет столько, сколько возможно на данном уровне (NumOpponents)

void DrawOpponents() {
    int y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 6) {
                if (SumOpponents == 0) {
                    if (PaintWorld[y][x + 1] == 3) {
                        PaintWorld[y][x + 1] = -12;
                        SumOpponents++;
                    }
                }
            }
            x++;
        }
        y++;
    }
}

void DrawNewOpponents() {
    if (SumOpponents < NumOpponents && PaintWorld[6][2] == 11) {
        PaintWorld[9][1] = -12;
        SumOpponents++;
    }
}

// Перемещение противников

// Противники запоминают откуда они пришли и не могут двигаться обратно.
// То есть они должны двигаться только вперед по дороге.
// Если 11 то я пришел снизу
// Если 12 то я пришел слева
// Если 13 то я пришел сверху
// Если 14 то я пришел справа

void moveOpponents() {
    int y = 1;
    while (y < NUM_PW_Y) {
        int x = 1;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 11) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -12;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y][x + 1] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y][x + 1] = -1219;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y][x + 1] = -1218;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y][x + 1] = -1217;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y][x + 1] = -1216;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -11;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y - 1][x] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y - 1][x] = -1119;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y - 1][x] = -1118;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y - 1][x] = -1117;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y - 1][x] = -1116;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -14;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y][x - 1] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y][x - 1] = -1419;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y][x - 1] = -1418;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y][x - 1] = -1417;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y][x - 1] = -1416;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 12) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -12;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y][x + 1] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y][x + 1] = -1219;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y][x + 1] = -1218;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y][x + 1] = -1217;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y][x + 1] = -1216;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -13;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y + 1][x] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y + 1][x] = -1319;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y + 1][x] = -1318;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y + 1][x] = -1317;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y + 1][x] = -1316;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -11;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y - 1][x] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y - 1][x] = -1119;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y - 1][x] = -1118;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y - 1][x] = -1117;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y - 1][x] = -1116;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 13) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -12;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y][x + 1] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y][x + 1] = -1219;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y][x + 1] = -1218;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y][x + 1] = -1217;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y][x + 1] = -1216;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -13;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y + 1][x] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y + 1][x] = -1319;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y + 1][x] = -1318;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y + 1][x] = -1317;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y + 1][x] = -1316;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -14;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y][x - 1] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y][x - 1] = -1419;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y][x - 1] = -1418;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y][x - 1] = -1417;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y][x - 1] = -1416;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 14) {
                if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -14;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y][x - 1] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y][x - 1] = -1419;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y][x - 1] = -1418;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y][x - 1] = -1417;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y][x - 1] = -1416;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -13;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y + 1][x] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y + 1][x] = -1319;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y + 1][x] = -1318;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y + 1][x] = -1317;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y + 1][x] = -1316;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -11;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    if (LiveRdCb == 1) {
                        PaintWorld[y - 1][x] = 20;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 2) {
                        PaintWorld[y - 1][x] = -1119;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 3) {
                        PaintWorld[y - 1][x] = -1118;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 4) {
                        PaintWorld[y - 1][x] = -1117;
                        PaintWorld[y][x] = 15;
                    }
                    else if (LiveRdCb == 5) {
                        PaintWorld[y - 1][x] = -1116;
                        PaintWorld[y][x] = 15;
                    }
                    else {

                    }
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1119) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1219;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1119;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1419;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = 20;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1118) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1218;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1219;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1118;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1119;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1418;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1419;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1117) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1217;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1218;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1117;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1118;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1417;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1418;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1116) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1216;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1217;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1116;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1117;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1416;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1417;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1219) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1219;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1319;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1119;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = 20;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1218) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1218;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1219;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1318;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1319;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1118;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1119;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1217) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1217;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1218;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1317;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1318;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1117;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1118;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1216) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1216;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1217;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1316;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1317;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1116;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1117;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1319) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1219;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1319;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1419;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = 20;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1318) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1218;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1219;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1318;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1319;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1418;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1419;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1317) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1217;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1218;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1317;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1318;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1417;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1418;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1316) {
                if (PaintWorld[y][x + 1] == 3) {
                    PaintWorld[y][x + 1] = -1216;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x + 1] == 15) {
                    PaintWorld[y][x + 1] = -1217;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1316;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1317;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1416;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1417;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1419) {
                if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1419;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1319;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = 20;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1119;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = 20;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1418) {
                if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1418;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1419;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1318;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1319;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1118;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1119;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1417) {
                if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1417;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1418;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1317;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1318;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1117;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1118;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            else if (PaintWorld[y][x] == 1416) {
                if (PaintWorld[y][x - 1] == 3) {
                    PaintWorld[y][x - 1] = -1416;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y][x - 1] == 15) {
                    PaintWorld[y][x - 1] = -1417;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y + 1][x] == 3) {
                    PaintWorld[y + 1][x] = -1316;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y + 1][x] == 15) {
                    PaintWorld[y + 1][x] = -1317;
                    PaintWorld[y][x] = 15;
                }
                else if (PaintWorld[y - 1][x] == 3) {
                    PaintWorld[y - 1][x] = -1116;
                    PaintWorld[y][x] = 3;
                }
                else if (PaintWorld[y - 1][x] == 15) {
                    PaintWorld[y - 1][x] = -1117;
                    PaintWorld[y][x] = 15;
                }
                else {

                }
            }
            x++;
        }
        y++;
    }
    y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == -11) {
                PaintWorld[y][x] = 11;
            }
            else if (PaintWorld[y][x] == -12) {
                PaintWorld[y][x] = 12;
            }
            else if (PaintWorld[y][x] == -13) {
                PaintWorld[y][x] = 13;
            }
            else if (PaintWorld[y][x] == -14) {
                PaintWorld[y][x] = 14;
            }
            else if (PaintWorld[y][x] == -1119) {
                PaintWorld[y][x] = 1119;
            }
            else if (PaintWorld[y][x] == -1118) {
                PaintWorld[y][x] = 1118;
            }
            else if (PaintWorld[y][x] == -1117) {
                PaintWorld[y][x] = 1117;
            }
            else if (PaintWorld[y][x] == -1116) {
                PaintWorld[y][x] = 1116;
            }
            else if (PaintWorld[y][x] == -1219) {
                PaintWorld[y][x] = 1219;
            }
            else if (PaintWorld[y][x] == -1218) {
                PaintWorld[y][x] = 1218;
            }
            else if (PaintWorld[y][x] == -1217) {
                PaintWorld[y][x] = 1217;
            }
            else if (PaintWorld[y][x] == -1216) {
                PaintWorld[y][x] = 1216;
            }
            else if (PaintWorld[y][x] == -1319) {
                PaintWorld[y][x] = 1319;
            }
            else if (PaintWorld[y][x] == -1318) {
                PaintWorld[y][x] = 1318;
            }
            else if (PaintWorld[y][x] == -1317) {
                PaintWorld[y][x] = 1317;
            }
            else if (PaintWorld[y][x] == -1316) {
                PaintWorld[y][x] = 1316;
            }
            else if (PaintWorld[y][x] == -1419) {
                PaintWorld[y][x] = 1419;
            }
            else if (PaintWorld[y][x] == -1418) {
                PaintWorld[y][x] = 1418;
            }
            else if (PaintWorld[y][x] == -1417) {
                PaintWorld[y][x] = 1417;
            }
            else if (PaintWorld[y][x] == -1416) {
                PaintWorld[y][x] = 1416;
            }
            else {

            }
            x++;
        }
        y++;
    }
    y = 1;
    while (y < NUM_PW_Y) {
        int x = 1;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 15) {
                if (PaintWorld[y - 1][x] != 1) {
                    if (PaintWorld[y - 1][x + 1] != 1) {
                        if (PaintWorld[y][x + 1] != 1) {
                            if (PaintWorld[y + 1][x + 1] != 1) {
                                if (PaintWorld[y + 1][x] != 1) {
                                    if (PaintWorld[y + 1][x - 1] != 1) {
                                        if (PaintWorld[y][x - 1] != 1) {
                                            if (PaintWorld[y - 1][x - 1] != 1) {
                                                PaintWorld[y][x] = 3;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (PaintWorld[y][x] == 3) {
                if (PaintWorld[y - 1][x] == 1 || PaintWorld[y - 1][x + 1] == 1 || PaintWorld[y][x + 1] == 1 ||
                    PaintWorld[y + 1][x + 1] == 1 || PaintWorld[y + 1][x] == 1 || PaintWorld[y + 1][x - 1] == 1 ||
                    PaintWorld[y][x - 1] == 1 || PaintWorld[y - 1][x - 1] == 1) {
                    PaintWorld[y][x] = 15;
                }
            }
            x++;
        }
        y++;
    }
}

// Нападение на замок

void attackCastle() {
    int y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 5) {
                if (PaintWorld[y + 1][x] == 11 || PaintWorld[y + 1][x] == 1119 || PaintWorld[y + 1][x] == 1118 ||
                    PaintWorld[y + 1][x] == 1117 || PaintWorld[y + 1][x] == 1116) {
                    PaintWorld[y + 1][x] = 3;
                    SumDeadOpponents++;
                    LiveBlackCb--;
                }
            }
            x++;
        }
        y++;
    }
}

// Все ли противники мертвы? Если да то запускай следующую волну.

void DeadOpponents() {
    int y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[x][y] == 20) {
                PaintWorld[x][y] = 15;
                SumDeadOpponents++;
                NumMoney++;
            }
            x++;
        }
        y++;
    }
    if (SumDeadOpponents == SumOpponents) {
        NumWave++;
        NumOpponents += 2;
        if (LiveRdCb < 5 && NumWave % 2 == 1) {
            LiveRdCb++;
        }
        if (NumWave > 2 && NumWave < 5) {
            Speed /= 2;
        }
        if (NumWave > 4 && NumWave < 7) {
            Speed /= 2;
        }
        if (NumWave > 6) {
            Speed = 50;
        }
        SumDeadOpponents = 0;
        SumOpponents = 0;
        StartNewWave = 0;
    }
}

// Запуск новой волны

void StartGame() {
    if (StartNewWave == 0) {
        StartNewWave = 1;
    }
    else if (StartNewWave == 1) {
        StartNewWave = 0;
    }
}

// Управление игроком

// Логика:
// 1. Цикл ищет по всем значениям массива PaintWorld игрока [7].
// 2. Если он его находит, то проверяет можно ли ему переместиться на соседнюю клетку [2].
// 3. Если можно то он его перемещает и продолжает поиск игрока [7].
// 4. Если нет, то цикл продолжит поиск игрока в пределах PaintWorld, не найдет его и остановится.

// Вверх

void moveUp() {
    int y = 1;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 7) {
                if (PaintWorld[y - 1][x] == 2) {
                    PaintWorld[y - 1][x] = -7;
                    PaintWorld[y][x] = 2;
                }
            }
            x++;
        }
        y++;
    }
    y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == -7) {
                PaintWorld[y][x] = 7;
            }
            x++;
        }
        y++;
    }
}

// Вниз

void moveDown() {
    int y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 7) {
                if (PaintWorld[y + 1][x] == 2) {
                    PaintWorld[y + 1][x] = -7;
                    PaintWorld[y][x] = 2;
                }
            }
            x++;
        }
        y++;
    }
    y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == -7) {
                PaintWorld[y][x] = 7;
            }
            x++;
        }
        y++;
    }
}

// Вправо

void moveRight() {
    int y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 7) {
                if (PaintWorld[y][x + 1] == 2) {
                    PaintWorld[y][x + 1] = -7;
                    PaintWorld[y][x] = 2;
                }
            }
            x++;
        }
        y++;
    }
    y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == -7) {
                PaintWorld[y][x] = 7;
            }
            x++;
        }
        y++;
    }
}

// Влево

void moveLeft() {
    int y = 0;
    while (y < NUM_PW_Y) {
        int x = 1;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 7) {
                if (PaintWorld[y][x - 1] == 2) {
                    PaintWorld[y][x - 1] = -7;
                    PaintWorld[y][x] = 2;
                }
            }
            x++;
        }
        y++;
    }
    y = 0;
    while (y < NUM_PW_Y) {
        int x = 0;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == -7) {
                PaintWorld[y][x] = 7;
            }
            x++;
        }
        y++;
    }
}

    // Если игрок находится возле дороги ,при нажатии на пробел, построй башню и определи зону атаки башни

void BuildTower() {
    int BuildNewTower = 0;
    int y = 1;
    while (y < NUM_PW_Y) {
        int x = 1;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == 7) {
                if (PaintWorld[y - 1][x] == 3 || PaintWorld[y][x + 1] == 3 || PaintWorld[y + 1][x] == 3 || PaintWorld[y][x - 1] == 3) {
                    BuildNewTower = 1;
                }
                if (PaintWorld[y - 1][x] == 2 && BuildNewTower == 1 && NumMoney >= 10) {
                    PaintWorld[y - 1][x] = -7;
                    PaintWorld[y][x] = 1;
                    if (PaintWorld[y - 1][x + 1] == 3) {
                        PaintWorld[y - 1][x + 1] = 15;
                    }
                    if (PaintWorld[y][x + 1] == 3) {
                        PaintWorld[y][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x + 1] == 3) {
                        PaintWorld[y + 1][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x] == 3) {
                        PaintWorld[y + 1][x] = 15;
                    }
                    if (PaintWorld[y + 1][x - 1] == 3) {
                        PaintWorld[y + 1][x - 1] = 15;
                    }
                    if (PaintWorld[y][x - 1] == 3) {
                        PaintWorld[y][x - 1] = 15;
                    }
                    if (PaintWorld[y - 1][x - 1] == 3) {
                        PaintWorld[y - 1][x - 1] = 15;
                    }
                    NumMoney -= 10;
                }
                else if (PaintWorld[y][x + 1] == 2 && BuildNewTower == 1 && NumMoney >= 10) {
                    PaintWorld[y][x + 1] = -7;
                    PaintWorld[y][x] = 1;
                    if (PaintWorld[y - 1][x + 1] == 3) {
                        PaintWorld[y - 1][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x + 1] == 3) {
                        PaintWorld[y + 1][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x] == 3) {
                        PaintWorld[y + 1][x] = 15;
                    }
                    if (PaintWorld[y + 1][x - 1] == 3) {
                        PaintWorld[y + 1][x - 1] = 15;
                    }
                    if (PaintWorld[y][x - 1] == 3) {
                        PaintWorld[y][x - 1] = 15;
                    }
                    if (PaintWorld[y - 1][x - 1] == 3) {
                        PaintWorld[y - 1][x - 1] = 15;
                    }
                    if (PaintWorld[y - 1][x] == 3) {
                        PaintWorld[y - 1][x] = 15;
                    }
                    NumMoney -= 10;
                }
                else if (PaintWorld[y + 1][x] == 2 && BuildNewTower == 1 && NumMoney >= 10) {
                    PaintWorld[y + 1][x] = -7;
                    PaintWorld[y][x] = 1;
                    if (PaintWorld[y - 1][x + 1] == 3) {
                        PaintWorld[y - 1][x + 1] = 15;
                    }
                    if (PaintWorld[y][x + 1] == 3) {
                        PaintWorld[y][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x + 1] == 3) {
                        PaintWorld[y + 1][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x - 1] == 3) {
                        PaintWorld[y + 1][x - 1] = 15;
                    }
                    if (PaintWorld[y][x - 1] == 3) {
                        PaintWorld[y][x - 1] = 15;
                    }
                    if (PaintWorld[y + 1][x - 1] == 3) {
                        PaintWorld[y + 1][x - 1] = 15;
                    }
                    if (PaintWorld[y - 1][x] == 3) {
                        PaintWorld[y - 1][x] = 15;
                    }
                    NumMoney -= 10;
                }
                else if (PaintWorld[y][x - 1] == 2 && BuildNewTower == 1 && NumMoney >= 10) {
                    PaintWorld[y][x - 1] = -7;
                    PaintWorld[y][x] = 1;
                    if (PaintWorld[y - 1][x] == 3) {
                        PaintWorld[y - 1][x] = 15;
                    }
                    if (PaintWorld[y - 1][x + 1] == 3) {
                        PaintWorld[y - 1][x + 1] = 15;
                    }
                    if (PaintWorld[y][x + 1] == 3) {
                        PaintWorld[y][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x + 1] == 3) {
                        PaintWorld[y + 1][x + 1] = 15;
                    }
                    if (PaintWorld[y + 1][x] == 3) {
                        PaintWorld[y + 1][x] = 15;
                    }
                    if (PaintWorld[y + 1][x - 1] == 3) {
                        PaintWorld[y + 1][x - 1] = 15;
                    }
                    if (PaintWorld[y - 1][x - 1] == 3) {
                        PaintWorld[y - 1][x - 1] = 15;
                    }
                    NumMoney -= 10;
                }
                else {

                }
            }
            x++;
        }
        y++;
    }
    y = 1;
    while (y < NUM_PW_Y) {
        int x = 1;
        while (x < NUM_PW_X) {
            if (PaintWorld[y][x] == -7) {
                PaintWorld[y][x] = 7;
            }
            x++;
        }
        y++;
    }
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
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

    case WM_CREATE:         // Запускаем таймер
        srand(time(NULL));
        SetTimer(hWnd, 1, Speed, NULL);
        break;

    case WM_TIMER:              // Ходим противниками по таймеру
        if (StartNewWave == 1) {
            DrawOpponents();        // Рисуем первого противника
            DrawNewOpponents();     // Рисуем новых противников (второго, третьего...)
            DeadOpponents();        // Все ли противники мертвы
            moveOpponents();        // Перемещение противников
            attackCastle();         // Атаковали ли противники замок
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_KEYDOWN:        // События при нажатии на клавиши
        switch (wParam)
        {
        case VK_UP:
            moveUp();           // Игрок ходит вверх
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            moveRight();        // Игрок ходит вправо
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_DOWN:
            moveDown();         // Игрок ходит вниз
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            moveLeft();         // Игрок ходит влево
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RETURN:
            StartGame(); // Старт
            break;
        case VK_SPACE:
            BuildTower(); // Строим башни
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            DrawObject(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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