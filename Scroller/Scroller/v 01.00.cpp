// v 01.00.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "v 01.00.h"
#include <fstream>
#include <string>

#include "PlayerShip.h"
#include "StarField.h"
#include "Enemy.h"
#include "Bullet.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst = NULL;							// current instance
HWND hMainWnd = NULL;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL InitD3D(HWND hWnd, int iWindowWidth, int iWindowHeight);

CShaderPtr m_pPixelShader, m_pVertexShader;
std::vector<CEnemy> vecEnemies;
std::vector<CBullet> vecBulletPlayer;
std::vector<CBullet> vecBulletEnemy;

void FireEnemy(D3DXVECTOR2 pos, D3DXVECTOR2 speed)
{
	vecBulletEnemy.push_back(CBullet(pos, speed, L"Data/bullet-enemy.dds"));
}

string GetFileAsString(const string& strFileName)
{
	string content;
	std::ifstream in(strFileName.c_str(), std::ios::binary);
	std::istreambuf_iterator<char> begin(in), end;
	while(begin != end)
		content += *begin++;
	return content;
}

CStarField* g_pStarField = NULL;
CPlayerShip* g_pPlayer = NULL;

void ProcessUserInput(float fDeltaTime)
{
	int dx=0;
	int dy=0;
	if (GetAsyncKeyState(VK_LEFT)&32768)
		dx--; // движемся влево
	if (GetAsyncKeyState(VK_RIGHT)&32768)
		dx++; // движемся вправо
	if (GetAsyncKeyState(VK_UP)&32768)
		dy--;// движемся вверх
	if (GetAsyncKeyState(VK_DOWN)&32768)
		dy++;// движемся вниз

#define SHIP_SPEED 200
	g_pPlayer->Scroll(dx*SHIP_SPEED*fDeltaTime, dy*SHIP_SPEED*fDeltaTime);

	static float fFireTimeOut=.0f;
	fFireTimeOut -= fDeltaTime;
	if (GetAsyncKeyState(VK_SPACE)&32768 && fFireTimeOut<=0)
	{
		// стреляем
		fFireTimeOut = .3f;
		CBullet bullet(g_pPlayer->Pos()-D3DXVECTOR2(0, 30), 
			D3DXVECTOR2(0, -300));
		vecBulletPlayer.push_back(bullet);
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_V0100, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	RECT rc;
	GetClientRect(hMainWnd, &rc);
	int iWndWidth = rc.right-rc.left;
	int iWndHeight = rc.bottom-rc.top;
	if (!InitD3D(hMainWnd, iWndWidth, iWndHeight))
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_V0100));

	// Main message loop:
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while( msg.message!=WM_QUIT ) // если не сообщение о выходе
	{
		// проверяем, есть ли сообщения в очереди
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			// сообщения есть, обрабатываем их
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else // иначе занимаемся рендером
		{
			if (g_pPlayer->IsAlive())
			{
				if (GetAsyncKeyState(VK_RETURN)&32768)
					g_pPlayer->InvDecHealth();

				if (!g_pPlayer->IsDecHealth())
				{

					ProcessUserInput(.02f);
					g_pStarField->Update(.02f);

					// апдейт летящих снарядов
					for (size_t i=0; i<vecBulletPlayer.size(); i++)
						vecBulletPlayer[i].Update(.02f);

					// снаряды врагов
					for (size_t i=0; i<vecBulletEnemy.size(); i++)
					{
						vecBulletEnemy[i].Update(.02f);
						// попал в игрока?
						if (vecBulletEnemy[i].TestCollision(*g_pPlayer))
						{
							// наносим дамаг и убираем из массива
							g_pPlayer->DoDamage(vecBulletEnemy[i].Damage());
							g_pPlayer->InvDecHealth();
							vecBulletEnemy[i] = * vecBulletEnemy.rbegin();
							vecBulletEnemy.pop_back();
						}
					}	

					for (size_t i=0; i<vecEnemies.size(); i++)
					{
						vecEnemies[i].Update(.02f);	

						// проверяем столкновение с игроком
						if (vecEnemies[i].TestCollision(*g_pPlayer))
						{
							g_pPlayer->DoDamage(vecBulletEnemy[i].Damage());
							g_pPlayer->InvDecHealth();
							vecEnemies[i].DoDamage(100); // наносим урон
							g_pPlayer->AddScore(10);
						}

						// проверяем столкновение со снарядами игрока
						for (size_t j=0; j<vecBulletPlayer.size(); j++)
							if (vecBulletPlayer[j].TestCollision(vecEnemies[i]))
							{
								vecEnemies[i].DoDamage(vecBulletPlayer[j].Damage());
								vecBulletPlayer[j] = * vecBulletPlayer.rbegin();
								vecBulletPlayer.pop_back();
								g_pPlayer->AddScore(50);
							}

						// проверяем на вышел ли за экран
						if (vecEnemies[i].Pos().y>400)
							vecEnemies[i].DoDamage(100000); // уничтожаем	

						if (!vecEnemies[i].IsAlive()) // если объект умер
						{
							// копируем последний в текущий
							vecEnemies[i] = *vecEnemies.rbegin();
							vecEnemies.pop_back(); // удаляем последний
						}
					}
				}
	
				if ( (rand()&127)==127)
					vecEnemies.push_back(CEnemy());	

				Graphics& gr = Graphics::get();
				if (!FAILED(gr.StartRender(0x00000000)))
				{
					// настраиваем рендер-стейты
					gr.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					gr.SetRenderState(D3DRS_LIGHTING, FALSE);
					gr.SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
					gr.SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
					gr.SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					gr.SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
					gr.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

					D3DXMATRIX mat, matViewProj, matProj, matView;
					D3DXMatrixIdentity(&mat);	

					// матрицы вида и проекции
					D3DXMatrixTranslation(&matView, 0, 0, 0);
					D3DXMatrixOrthoLH(&matProj, (float)iWndWidth, (float)-iWndHeight, 0.0f, 10.0f);

					// получаем матрицу вида/проекции
					D3DXMatrixMultiply(&matViewProj, &matView, &matProj);	

					// устанавливаем шейдеры
					gr.SetPixelShader(m_pPixelShader);
					gr.SetVertexShader(m_pVertexShader);	

					// передаём константы в шейдеры
					m_pVertexShader->SetMatrix("mWorld", &mat);
					m_pVertexShader->SetMatrix("mViewProjection", &matViewProj);

					// настраиваем фильтрацию текстуры
					gr.SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
					gr.SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
					gr.SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
					
					// включаем альфа-блендинг
					gr.SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
					gr.SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
					gr.SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA );
	
					// рисуем звёзды
					g_pStarField->Draw();	

					// рисуем игрока
					g_pPlayer->Draw();
	
					// рисуем врагов
					for (size_t i=0; i<vecEnemies.size(); i++)
						vecEnemies[i].Draw();	
	
					// рисуем снаряды
					for (size_t i=0; i<vecBulletPlayer.size(); i++)
						vecBulletPlayer[i].Draw();
					for (size_t i=0; i<vecBulletEnemy.size(); i++)
						vecBulletEnemy[i].Draw();

					//выводим счет и жизни
					for (int i = g_pPlayer->GetHealth(); i > 0 ; i -= 50)
						g_pPlayer->Draw(360-i/3,-265);
					wchar_t wstr[10] = {};
					_itow_s(g_pPlayer->GetScore(),wstr,10);
					gr.DrawScore(wstr,10,10,800,800,D3DCOLOR_ARGB(250,250,250,50));
	
					// закончили рендер
					gr.EndRender();
				}
				else
					gr.Reset();
			}
			else
				if (MessageBox(NULL,L"GameOver!\nDo you want to start a new game?",L"Scroller",MB_YESNO|MB_ICONQUESTION) == IDNO)
					exit(0);
				else
				{
					g_pPlayer->NewGame();
					vecEnemies.clear();
					vecBulletEnemy.clear();
					vecBulletPlayer.clear();
					g_pStarField ->Release();
				}
		}
	}
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_V0100));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hMainWnd = CreateWindow(szWindowClass, L"Scroller", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hMainWnd)
   {
      return FALSE;
   }

   ShowWindow(hMainWnd, nCmdShow);
   UpdateWindow(hMainWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ERASEBKGND:
		return TRUE;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

BOOL InitD3D(HWND hWnd, int iWindowWidth, int iWindowHeight)
{

	LPD3DXBUFFER pErrors = NULL;
	LPD3DXBUFFER pShaderBuff = NULL;

	// Инициализируем графику
	if (!Graphics::get().Init(hWnd, iWindowWidth, iWindowHeight))
		return FALSE;

	// загружаем вертексный шейдер
	m_pVertexShader = Graphics::get().LoadShader("vs.hlsl", "main", "vs_3_0");
	
	// загружаем пиксельный шейдер
	m_pPixelShader  = Graphics::get().LoadShader("ps.hlsl", "pixel_shader_main", "ps_3_0");

	g_pPlayer = new CPlayerShip();
	g_pStarField = new CStarField();
	return TRUE;
}

