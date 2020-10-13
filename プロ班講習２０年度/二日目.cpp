//4/5の講習でやったところです
//自機と敵と背景の表示・移動
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
const int pWid = 1440, pHei = 810; //幅・高さ

bool WINMODE = false; //WINDOU

void GameMain(void); // ゲームメイン処理
void Wait(DWORD); // ウェイト
void FPSCount(DWORD*); // FPS計測

void Opening();
void MainRoutine();
void Ending();

BOOL KeyFlag[256];
const DWORD FPS = 60; // FPS設定
DWORD fps; // FPS計測値
BOOL EndFlag = FALSE; // 終了フラグ

HDC CreateEmptyBmp(HDC hdc, LPCSTR f_name_of_bmp);
HDC hDC_Back;
HDC h_op, h_main, h_end;
HDC h_jiki, h_teki;
HDC jikimask, tekimask;
int j_x = 0, j_y = 0;
int e_x = 0, e_y = 0;
struct JIKI {
	int x = 0, y = 0;
};
JIKI jiki;

struct TEKI {
	int x = 0, y = 0;
};
TEKI teki[100];//teki[0]~teki[99]まで使えるteki[100]は使えない
int back_x = 0, back_y = 0;
enum {
	OPENING,
	MAIN,
	ENDING
}GameFlag;
int a[100];
//======================================
// オープニング
//======================================
void Opening(void) {
	BitBlt(hDC_Back, 0, 0, 1440, 810, h_op, 0, 0, SRCCOPY);
	for (int i = 0; i < 100; i += 1) {
		teki[i].y = i * 50;
	}
	if (KeyFlag['Z']) GameFlag = MAIN;
}

//==============================================================================================
// メイン
//==============================================================================================
void MainRoutine(void) {
	if (back_x < -2880)back_x = 0;
	BitBlt(hDC_Back, back_x, back_y, 2880, 810, h_main, 0, 0, SRCCOPY);
	BitBlt(hDC_Back, 2880 + back_x, back_y, 2880, 810, h_main, 0, 0, SRCCOPY);
	back_x -= 5;
	SpriteBlt(hDC_Back, jiki.x, jiki.y, 50, 50, h_jiki, 0, 0, jikimask);
	for (int i = 0; i < 100; i += 1) {
		SpriteBlt(hDC_Back, teki[i].x, teki[i].y, 50, 50, h_teki, 0, 0, tekimask);
		teki[i].x++;
		if (i > 5)teki[i].y++;
	}
	if (KeyFlag[VK_RIGHT]) 	jiki.x += 1;
	if (KeyFlag[VK_LEFT]) 	jiki.x -= 1;
	if (KeyFlag[VK_UP]) 	jiki.y -= 1;
	if (KeyFlag[VK_DOWN]) 	jiki.y += 1;
	//	teki[0].y++;
}

//==============================================================================================
// エンディング
//==============================================================================================
void Ending(void) {
	BitBlt(hDC_Back, 0, 0, 1440, 810, h_end, 0, 0, SRCCOPY);
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
	hDC_Back = ::CreateEmptyBMP(hDC_Wnd, 1440, 810);



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
	h_op = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/op.bmp"));
	h_main = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/main.bmp"));
	h_end = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/end.bmp"));
	h_jiki = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/jiki.bmp"));
	h_teki = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/teki.bmp"));
	jikimask = CreateMask(h_jiki, RGB(255, 0, 255));
	tekimask = CreateMask(h_teki, RGB(255, 0, 255));
	//メインループ

	GameFlag = OPENING;

	const int Wid = ::GetDeviceCaps(hDC_Wnd, HORZRES), Hei = ::GetDeviceCaps(hDC_Wnd, VERTRES);

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
		case MAIN: MainRoutine(); break;
		case ENDING: Ending(); break;
		}
		//WINDOU

		if (WINMODE == 1) {
			StretchBlt(hDC_Wnd, (Wid - Hei / 3 * 4) / 2, 0, Hei / 3 * 4, Hei, hDC_Back, 0, 0, 1440, 810, SRCCOPY);
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
DeleteDC(h_op), DeleteDC(h_main), DeleteDC(h_end);
	DeleteDC(h_jiki), DeleteDC(jikimask);
	DeleteDC(h_teki), DeleteDC(tekimask);

	timeEndPeriod(Caps.wPeriodMin); // 後処理
}
