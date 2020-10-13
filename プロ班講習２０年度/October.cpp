#include <windows.h>
#include <mmsystem.h>
#include<math.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib") // VC++の場合、これでもリンク可能

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // ウィンドウプロシージャ
void SpriteBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, HDC hdcMask);
bool hit(int obj1_x, int obj1_y, int obj2_x, int obj2_y, int obj1_size, int obj2_size);

HINSTANCE hInst; // インスタンス
HWND hWnd; // ウィンドウハンドル
HDC hDC_Wnd; // ウィンドウのデバイスコンテキスト
const int pWid = 640, pHei = 480; //幅・高さ

bool WINMODE = false; //WINDOU

void GameMain(void); // ゲームメイン処理
void Wait(DWORD); // ウェイト
void FPSCount(DWORD*); // FPS計測

void Opening();
void Setumei();
void MainRoutine();
void Ending();

BOOL KeyFlag[256];
const DWORD FPS = 60; // FPS設定
DWORD fps; // FPS計測値
BOOL EndFlag = FALSE; // 終了フラグ

HDC CreateEmptyBmp(HDC hdc, LPCSTR f_name_of_bmp);
HDC hDC_Back;
HDC iOP;
HDC iOPl;
HDC haikei;
enum {
	OPENING,
	SETUMEI,
	MAIN,
	ENDING
}GameFlag;

int x = 100;
int y = 100;
int hx = 0;
int isleft = 0;
double G = 0;
double Accele = 0;
int jumpcount = 0;
int jumpnum=0;
int fly = 0;
//======================================
// オープニング
//======================================
void Opening(void) {
	BitBlt(hDC_Back, 0, 0, 640, 480, haikei, hx, 0, SRCCOPY);//画像を表示
	BitBlt(hDC_Back, 0, 0, 640, 480, haikei, hx-1280, 0, SRCCOPY);//画像を表示
	if (hx > 1280)hx = 0;
	if(isleft==1)
	BitBlt(hDC_Back, 0, 0, 640, 480, iOPl, -x, -y, SRCCOPY);//画像を表示
	else
	BitBlt(hDC_Back, 0, 0, 640, 480, iOP, -x, -y, SRCCOPY);//画像を表示

	if (KeyFlag[VK_RIGHT] != 0) {
		if (Accele < 5)Accele += 0.1;
		x += Accele, isleft = 0, hx += 3;
	}
	
	if (KeyFlag[VK_LEFT] != 0) {
		if (Accele < 5)Accele += 0.1;
		x -= Accele, isleft = 1, hx -= 3;
	}
	if (KeyFlag[VK_LEFT] == 0&& KeyFlag[VK_RIGHT] == 0)  Accele = 0;
	if (KeyFlag[VK_UP] != 0 && y >= 350) {
	
		y -= 3, G = -5;
		if (Accele >= 3)G -= 3, fly = 6
 ;
		if (Accele >= 4 && fly>=5)G -= 2;
		if (Accele >= 5 && fly>=8)G -=1;
	}
	if (fly > 0 && y >= 350) {
		fly-=1;
	}

if(y < 350)y+=G,G+=0.1;
if(y>350)y=350;
if (x > 510)x=510;
if (y < 300)y += G, G += 0.2;
if (x < 30)x = 30;

}

//=======================================================
//説明
//=======================================================
void Setumei(void) {

}

//==============================================================================================
// メイン
//==============================================================================================
void MainRoutine(void) {

}

//==============================================================================================
// エンディング
//==============================================================================================
void Ending(void) {

}

bool hit(int obj1_x, int obj1_y, int obj2_x, int obj2_y, int obj1_size, int obj2_size) {
	int obj1_half = obj1_size / 2;
	int obj2_half = obj2_size / 2;
	double z = sqrt(((obj1_x + obj1_half) - (obj2_x + obj2_half)) * ((obj1_x + obj1_half) - (obj2_x + obj2_half)) + ((obj1_y + obj1_half) - (obj2_y + obj2_half)) * ((obj1_y + obj1_half) - (obj2_y + obj2_half)));
	if (z <= obj1_half + obj2_half) {
		return true;
	}
	else {
		return false;
	}
};
//--------------------------------------------
//ウェイト
//--------------------------------------------
void Wait(DWORD wait_time) {
	MSG msg;
	DWORD start_time = timeGetTime();
	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (wait_time > 0) Sleep(1);
	} while (timeGetTime() < wait_time + start_time);
}

//============================
// FPS の計測
//============================
void FPSCount(DWORD* fps) {
	static DWORD before_time = timeGetTime();
	DWORD        now_time = timeGetTime();
	static DWORD fps_ctr = 0;

	if (now_time - before_time >= 1000) {

		before_time = now_time;
		*fps = fps_ctr;
		fps_ctr = 0;
	}

	fps_ctr++;
}

//==============================================================================================
// 空のビットマップ作成
//==============================================================================================
HDC CreateEmptyBMP(HDC hdc, int width, int height) {
	HBITMAP hbmp;
	HDC hdc_work;

	hbmp = CreateCompatibleBitmap(hdc, width, height);
	hdc_work = CreateCompatibleDC(hdc);
	SelectObject(hdc_work, hbmp);
	PatBlt(hdc_work, 0, 0, width, height, WHITENESS);
	DeleteObject(hbmp);
	return hdc_work;
}
//==============================================================================================
// Windows メイン処理
//==============================================================================================
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("Windows");
	if (!RegisterClass(&wc)) return 0;
	hWnd = CreateWindow(
		wc.lpszClassName,
		TEXT("Windows"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		pWid,
		pHei,
		NULL,
		NULL,
		hInstance,
		NULL);

	RECT rc, rw;
	GetWindowRect(hWnd, &rw);
	GetClientRect(hWnd, &rc);

	if (WINMODE == FALSE)
	{
		while (ShowCursor(TRUE) < 0)
		{

		}
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		SetWindowPos(hWnd, NULL, 100, 100, (rw.right - rw.left) - (rc.right - rc.left) + pWid, (rw.bottom - rw.top) - (rc.bottom - rc.top) + pHei, SWP_SHOWWINDOW);
	}
	else
	{
		while (ShowCursor(FALSE) >= 0)
		{

		}

		SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
		SetWindowPos(hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CXFULLSCREEN), NULL);
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hInst = hInstance;
	hDC_Wnd = GetDC(hWnd);
	hDC_Back = ::CreateEmptyBMP(hDC_Wnd, 640, 480);



	GameMain();

	MSG msg;


	ReleaseDC(hWnd, hDC_Wnd);


	return 0;


	return msg.wParam;
}
//==============================================
// ビットマップ読み込み（ファイルから）
//==============================================
HDC LoadBMPfromFile(HDC hdc, LPCTSTR f_name_of_bmp) {
	HBITMAP hbmp;
	HDC hdc_work;

	hbmp = (HBITMAP)LoadImage(0, f_name_of_bmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hdc_work = CreateCompatibleDC(hdc);
	SelectObject(hdc_work, hbmp);
	DeleteObject(hbmp);
	return hdc_work;
}
//==============================================================================================
// マスク作成（マスク：透過する部分 = 白、キャラクタ部分 = 黒）（元絵の加工含む）
//==============================================================================================
HDC CreateMask(HDC hdc, COLORREF transparent_color) {
	HBITMAP hbmp_mono;
	HDC hdc_mono, hdc_color_white, hdc_color_black;
	int width = GetDeviceCaps(hdc, HORZRES);
	int height = GetDeviceCaps(hdc, VERTRES);

	COLORREF default_bkcolor = SetBkColor(hdc, transparent_color); // 透過色の設定

																   // モノクロビットマップでマスク作成
	hbmp_mono = CreateBitmap(width, height, 1, 1, 0);
	hdc_mono = CreateCompatibleDC(hdc);
	SelectObject(hdc_mono, hbmp_mono);
	BitBlt(hdc_mono, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
	DeleteObject(hbmp_mono);

	SetBkColor(hdc, default_bkcolor); // 背景色の設定を戻す

									  // カラービットマップに変換
	hdc_color_white = CreateEmptyBMP(hdc, width, height);
	BitBlt(hdc_color_white, 0, 0, width, height, hdc_mono, 0, 0, SRCCOPY);
	DeleteDC(hdc_mono);

	// 元絵の透過色部分を黒にする
	hdc_color_black = CreateEmptyBMP(hdc, width, height);
	BitBlt(hdc_color_black, 0, 0, width, height, hdc_color_white, 0, 0, NOTSRCCOPY);
	BitBlt(hdc, 0, 0, width, height, hdc_color_black, 0, 0, SRCAND);
	DeleteDC(hdc_color_black);

	return hdc_color_white;
}

//==============================================================================================
// スプライト
//==============================================================================================
void SpriteBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, HDC hdcMask) {
	BitBlt(hdc, x, y, cx, cy, hdcMask, x1, y1, SRCAND);
	BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, SRCPAINT);
}

//==============================================================================================
// ウィンドウプロシージャ
//==============================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_DESTROY:                                    // ウィンドウが破棄されたときの処理
														// 終了メッセージ
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		KeyFlag[wParam] = TRUE;
		return 0;

	case WM_KEYUP:
		KeyFlag[wParam] = FALSE;
		return 0;

	default:                                            // デフォルト処理
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
//==============================================================================================
// ゲームメイン処理
//==============================================================================================
void GameMain(void) {

	// 設定

	MSG msg;

	DWORD StartTime, EndTime, PassTime;

	TIMECAPS Caps;

	timeGetDevCaps(&Caps, sizeof(TIMECAPS)); // 性能取得
	timeBeginPeriod(Caps.wPeriodMin); // 設定

									  // バックバッファ作成
	hDC_Back = CreateEmptyBMP(hDC_Wnd, pWid, pHei);

	// ビットマップを読み込む
	//メインループ

	GameFlag = OPENING;

	const int Wid = ::GetDeviceCaps(hDC_Wnd, HORZRES), Hei = ::GetDeviceCaps(hDC_Wnd, VERTRES);
	iOP = LoadBMPfromFile(hDC_Wnd, TEXT("op.bmp"));
	iOPl = LoadBMPfromFile(hDC_Wnd, TEXT("opl.bmp"));
	haikei = LoadBMPfromFile(hDC_Wnd, TEXT("haikei.bmp"));
	while (!EndFlag) {
		StartTime = timeGetTime();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { //メッセージが来たら
			if (msg.message != WM_QUIT) { // 終了メッセージでなければ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				break; // 終了メッセージが来たら脱出。
			}
		}

		// 分岐
		switch (GameFlag) {
		case OPENING: Opening(); break;
		case SETUMEI: Setumei(); break;
		case MAIN: MainRoutine(); break;
		case ENDING: Ending(); break;
		}
		//WINDOU

		if (WINMODE == 1) {
			StretchBlt(hDC_Wnd, (Wid - Hei / 3 * 4) / 2, 0, Hei / 3 * 4, Hei, hDC_Back, 0, 0, 640, 480, SRCCOPY);
		}
		else {
			BitBlt(hDC_Wnd, 0, 0, pWid, pHei, hDC_Back, 0, 0, SRCCOPY);
		}

		EndTime = timeGetTime();
		PassTime = EndTime - StartTime;
		(1000 / FPS > PassTime) ? Wait(1000 / FPS - PassTime) : Wait(0);
		FPSCount(&fps);

		if (KeyFlag[VK_ESCAPE]) {
			EndFlag = 1;
		}
	}
	timeEndPeriod(Caps.wPeriodMin); // 後処理
	DeleteDC(iOP);
	DeleteDC(iOPl);
}
