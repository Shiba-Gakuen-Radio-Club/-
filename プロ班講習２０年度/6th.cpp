//4/9の講習で解説しました
//キャラクターのアニメーション・ジャンプ処理・ブロック
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
void Shooting();
void Action();
void Ending();

BOOL KeyFlag[256];
const DWORD FPS = 60; // FPS設定
DWORD fps; // FPS計測値
BOOL EndFlag = FALSE; // 終了フラグ
HFONT hfont;
HDC CreateEmptyBmp(HDC hdc, LPCSTR f_name_of_bmp);
HDC hDC_Back;
HDC h_op, h_main, h_end, h_select;
HDC h_jikir1, h_jikir2, h_jikil1, h_jikil2;
HDC h_jiki, h_teki, h_tama;
HDC jikimask, tekimask, tamamask;
HDC h_item1, h_item2;
HDC jikir1mask, jikir2mask, jikil1mask, jikil2mask;
HDC item1mask, item2mask;
HDC h_block, blockmask;
int keypress = 0;
struct JIKI {
	int x = 0, y = 0;
	int trigger = 0;
	int hp = 3;
	int vx = 0, vy = 0;
	int speed = 1;
	int tri = 20;
	int muteki;
	int mode = 1;//自機の向き
	int land = 0;//地面にあたっているか
};

JIKI jiki;

int score = 0;
const int TEKI_MAX = 100;
const int TAMA_MAX = 100;
const int ETAMA_MAX = 100;
struct TEKI {
	int x = 1500, y = 0;
	int trigger = 0;
	int hp;
};
TEKI teki[TEKI_MAX];//teki[0]~teki[99]まで使えるteki[100]は使えない
TEKI boss;
struct TAMA {
	int x = 0, y = 0;
	int flag = 0;
	int vx = 0, vy = 0;
};
struct ITEM {
	int x = 0, y = 0;
	int flag = 0;
};

ITEM item[4];
TAMA tama[TAMA_MAX];//teki[0]~teki[99]まで使えるteki[100]は使えない
TAMA etama[ETAMA_MAX];
int back_x = 0, back_y = 0;

struct BLOCK {
	int x = 0, y = 0;
	int flag = 0;
};
const int B_MAX = 10;
BLOCK block[B_MAX];
enum {
	OPENING,
	MAIN,
	SHOOTING,
	ACTION,
	ENDING
}GameFlag;
int a[100];
void ShotTAMA(TEKI* teki, TAMA* tama, int x, int y);
void MoveTAMA(TAMA* tama);
//======================================
// オープニング
//======================================
void Opening(void) {
	BitBlt(hDC_Back, 0, 0, 1440, 810, h_op, 0, 0, SRCCOPY);

	if (KeyFlag['Z'] && keypress < 1) GameFlag = MAIN;

}

//==============================================================================================
// メイン
//==============================================================================================
void MainRoutine(void) {
	BitBlt(hDC_Back, 0, 0, 1440, 810, h_select, 0, 0, SRCCOPY);
	if (KeyFlag['C']) GameFlag = SHOOTING;
	if (KeyFlag['X']) GameFlag = ACTION;
}
int count = 0, num = 0, m_score[1000];
int G = 0;
//==============================================================================================
// アクション
//==============================================================================================
void Action(void) {
	if (count == 0) {
		jiki.x = 200;
		jiki.y = 0;
		count++;
		jiki.speed;
		for (int i = 0; i < B_MAX-1; i++)block[i].flag = 1, block[i].x = i*100, block[i].y = 810 - 100;
		block[9].x = 80;
		block[9].y = 200;
		block[9].flag = 1;
	}
	if (back_x < -2880)back_x = 0;
	BitBlt(hDC_Back, back_x, back_y, 2880, 810, h_main, 0, 0, SRCCOPY);
	BitBlt(hDC_Back, 2880 + back_x, back_y, 2880, 810, h_main, 0, 0, SRCCOPY);
	back_x -= 1;
	if(jiki.x%20<10&&jiki.mode==1)
	SpriteBlt(hDC_Back, jiki.x, jiki.y, 100, 200, h_jikir1, 0, 0, jikir1mask);
	else if(jiki.mode==1)
	SpriteBlt(hDC_Back, jiki.x, jiki.y, 100, 200, h_jikir2, 0, 0, jikir2mask);
	if (jiki.x % 20 < 10 && jiki.mode == 0)
		SpriteBlt(hDC_Back, jiki.x, jiki.y, 100, 200, h_jikil1, 0, 0, jikil1mask);
	else if (jiki.mode == 0)
		SpriteBlt(hDC_Back, jiki.x, jiki.y, 100, 200, h_jikil2, 0, 0, jikil2mask);
	for (int i = 0; i < B_MAX; i++) {
		if (block[i].flag == 1)SpriteBlt(hDC_Back, block[i].x, block[i].y, 100, 100, h_block, 0, 0, blockmask);
		if (block[i].y+100>jiki.y&&block[i].y < jiki.y + 200 &&block[i].x+100>jiki.x&& block[i].x<jiki.x)jiki.land = 1,jiki.y=block[i].y-201;
	if (jiki.x + 100 > block[i].x && jiki.y + 200 > block[i].y && jiki.y + 200 < block[i].y + 100)jiki.x = block[i].x;
//if (jiki.x < block[i].x +100&& jiki.y + 200 > block[i].y && jiki.y + 200 < block[i].y + 100)jiki.x = block[i].x;
	}
	if (jiki.y > 900)GameFlag = ENDING;

	if (KeyFlag[VK_RIGHT]) 	jiki.x += jiki.speed,jiki.mode=1;
	if (KeyFlag[VK_LEFT]) 	jiki.x -= jiki.speed,jiki.mode=0;
	if (KeyFlag[VK_UP]&&jiki.land==1) G -= 30,jiki.land=0;
	G++, jiki.y += G / 7;
	if (jiki.land==1)G=0;
}
//==============================================================================================
//シューティング
//==============================================================================================
int defeatflag = 0;
void Shooting(void) {
	if (count == 0) {
		for (int i = 0; i < 100; i += 1) {
			teki[i].y = sin(i) * 50 + 810 / 2;
			teki[i].x = cos(i) * 50 + 1440 / 2;
		}
		item[0].x = 1500;
		item[0].y = 300;
		item[0].flag = 1;

		item[1].x = 1800;
		item[1].y = 150;
		item[1].flag = 2;

		item[2].x = 2100;
		item[2].y = 400;
		item[2].flag = 3;

		item[3].x = 2400;
		item[3].y = 550;
		item[3].flag = 4;

		jiki.vx = 5;
		boss.hp = 5;

		jiki.y = 810 / 2;
		jiki.x = 100;
		count++;
	}

	if (back_x < -2880)back_x = 0;
	BitBlt(hDC_Back, back_x, back_y, 2880, 810, h_main, 0, 0, SRCCOPY);
	BitBlt(hDC_Back, 2880 + back_x, back_y, 2880, 810, h_main, 0, 0, SRCCOPY);
	back_x -= 1;
	SpriteBlt(hDC_Back, jiki.x, jiki.y, 50, 50, h_jiki, 0, 0, jikimask);
	//スコアと体力の表示
	char string[100];
	wsprintfA(string, "HP = %d", jiki.hp);
	TextOutA(hDC_Back, 0, 0, string, strlen(string));
	wsprintfA(string, "SCORE = %d", score);
	TextOutA(hDC_Back, 1300, 0, string, strlen(string));


	for (int i = 0; i < TAMA_MAX; i += 1) {
		if (tama[i].flag == 1) {
			SpriteBlt(hDC_Back, tama[i].x, tama[i].y, 10, 10, h_tama, 0, 0, tamamask);
			tama[i].vx = jiki.vx;
			for (int t = 0; t < TEKI_MAX; t++) {
				if (hit(tama[i].x, tama[i].y, teki[t].x, teki[t].y, 10, 50)) {
					teki[t].y = 10000;
					tama[i].flag = 0;
					score += 100;
				}
			}
			if (hit(tama[i].x, tama[i].y, boss.x, boss.y, 10, 50)) {
				boss.hp -= 5;
				tama[i].flag = 0;
				score += 100;
			}

			MoveTAMA(&tama[i]);
		}
		//Zキーが押されてtriggerが１０以上でtamaが存在しなければ発射
		if (KeyFlag['Z'] && jiki.trigger > jiki.tri && tama[i].flag == 0) {
			tama[i].x = jiki.x;
			tama[i].y = jiki.y;
			jiki.trigger = 0;
			tama[i].flag = 1;

		}
	}

	//item
	for (int i = 0; i < 4; i++) {
		if (item[i].flag > 0) {
			if (i < 3)	SpriteBlt(hDC_Back, item[i].x, item[i].y, 25, 25, h_item1, 0, 0, item1mask);
			if (i == 3)	SpriteBlt(hDC_Back, item[i].x, item[i].y, 25, 25, h_item2, 0, 0, item2mask);
		}
		if (hit(item[i].x, item[i].y, jiki.x, jiki.y, 25, 50) && item[i].flag == 1) item[i].flag = 0, jiki.vx = 10, score += 10;
		if (hit(item[i].x, item[i].y, jiki.x, jiki.y, 25, 50) && item[i].flag == 2) item[i].flag = 0, jiki.speed = 3, score += 10;
		if (hit(item[i].x, item[i].y, jiki.x, jiki.y, 25, 50) && item[i].flag == 3) item[i].flag = 0, jiki.tri = 5, score += 10;
		if (hit(item[i].x, item[i].y, jiki.x, jiki.y, 25, 50) && item[i].flag == 4) item[i].flag = 0, jiki.speed = -3, score -= 10;
		item[i].x -= 3;
	}
	jiki.muteki--;

	//etama
	for (int i = 0; i < ETAMA_MAX; i++) {
		if (etama[i].flag == 1) {
			SpriteBlt(hDC_Back, etama[i].x, etama[i].y, 10, 10, h_tama, 0, 0, tamamask);
			if (hit(etama[i].x, etama[i].y, jiki.x, jiki.y, 10, 50) && jiki.muteki < 0) {
				jiki.hp -= 1;
				jiki.muteki = 100;
				etama[i].flag = 0;
			}
		}
		MoveTAMA(&etama[i]);
	}
	//teki
	for (int t = 0; t < TEKI_MAX; t += 1) {
		SpriteBlt(hDC_Back, teki[t].x, teki[t].y, 50, 50, h_teki, 0, 0, tekimask);
		teki[t].x--;
		teki[t].trigger += 1;
		if (t > 5)teki[t].y++;

		for (int i = 0; i < ETAMA_MAX; i++) {
			if (teki[t].trigger > 10) {
				ShotTAMA(&teki[t], &etama[i], sin(i) * 4, cos(i) * 3);
			}
		}
	}
	SpriteBlt(hDC_Back, boss.x, boss.y, 50, 50, h_teki, 0, 0, tekimask);
	if (boss.x > 1200)
		boss.x -= 5;
	boss.trigger += 1;
	for (int i = 0; i < ETAMA_MAX; i++) {
		if (boss.trigger > 0) {
			ShotTAMA(&boss, &etama[i], sin(i) * 4, cos(i) * 3);
		}
	}
	jiki.trigger += 1;
	if (KeyFlag[VK_RIGHT]) 	jiki.x += jiki.speed;
	if (KeyFlag[VK_LEFT]) 	jiki.x -= jiki.speed;
	if (KeyFlag[VK_UP]) 	jiki.y -= jiki.speed;
	if (KeyFlag[VK_DOWN]) 	jiki.y += jiki.speed;
	//	teki[0].y++;
	if (jiki.hp < 0)defeatflag++;
	if (defeatflag > 100)
		GameFlag = ENDING;
	if (boss.hp == 0)defeatflag++;

}

//==============================================================================================
// エンディング
//==============================================================================================
void Ending(void) {
	BitBlt(hDC_Back, 0, 0, 1440, 810, h_end, 0, 0, SRCCOPY);
	if (count == 1) {
		hfont = CreateFont(100, 100, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
		FILE* fp;
		fopen_s(&fp, "score.ini", "a");
		fprintf_s(fp, "%d\n", score);
		fclose(fp);
		fopen_s(&fp, "score.ini", "r");
		for (int i = 0; fscanf_s(fp, "%d", &i) != EOF; num++) {
			m_score[num] = i;
		}
		fclose(fp);
		count++;
	}
	for (int i = 0; i < num; i++) {
		for (int j = i + 1, tmp; j < num; j++) {
			if (m_score[i] < m_score[j])
				tmp = m_score[i], m_score[i] = m_score[j], m_score[j] = tmp;
		}
	}
	char string[100];
	wsprintfA(string, "あなたのすこあは%d", score);
	TextOutA(hDC_Back, 1440 / 2, 100, string, strlen(string));
	for (int i = 0; i < num; i++) {
		wsprintfA(string, "%d : %d", i + 1, m_score[i]);
		TextOutA(hDC_Back, 1440 / 2, i * 50 + 200, string, strlen(string));
	}
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
void ShotTAMA(TEKI* teki, TAMA* tama, int x, int y) {
	if (tama->flag == 0) {
		tama->x = teki->x;
		tama->y = teki->y;
		tama->vx = x, tama->vy = y;
		tama->flag = 1;
		teki->trigger = 0;
	}
}
void MoveTAMA(TAMA* tama) {
	tama->x += tama->vx;//vx=1ならばtama.x+=1 vx=-1ならばtama.x+=-1(tama.x-=1)
	tama->y += tama->vy;
	if (tama->x < 0 || tama->x>1440 || tama->y < 0 || tama->y>900) {
		tama->flag = 0;
	}
}
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
	h_select = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/select.bmp"));
	jikimask = CreateMask(h_jiki, RGB(255, 0, 255));
	h_tama = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/tama.bmp"));
	tamamask = CreateMask(h_tama, RGB(255, 0, 255));
	h_item1 = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/item1.bmp"));
	item1mask = CreateMask(h_item1, RGB(255, 0, 255));
	h_item2 = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/item2.bmp"));
	item2mask = CreateMask(h_item2, RGB(255, 0, 255));
	tekimask = CreateMask(h_teki, RGB(255, 0, 255));
	h_jikir1 = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/jikir1.bmp"));
	jikir1mask = CreateMask(h_jikir1, RGB(255, 0, 255));
	h_jikir2 = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/jikir2.bmp"));
	jikir2mask = CreateMask(h_jikir2, RGB(255, 0, 255));
	h_jikil2 = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/jikil2.bmp"));
	jikil2mask = CreateMask(h_jikil2, RGB(255, 0, 255));
	h_jikil1 = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/jikil1.bmp"));
	jikil1mask = CreateMask(h_jikil1, RGB(255, 0, 255));
	h_block = LoadBMPfromFile(hDC_Wnd, TEXT("Pictures/block.bmp"));
	blockmask = CreateMask(h_block, RGB(255, 0, 255));
	//メインループ

	GameFlag = OPENING;

	const int Wid = ::GetDeviceCaps(hDC_Wnd, HORZRES), Hei = ::GetDeviceCaps(hDC_Wnd, VERTRES);
	//------------------

	SetBkMode(hDC_Back, TRANSPARENT); // 背景透過
	SetTextColor(hDC_Back, 0xFFFFFF); // 文字色：白
	hfont = CreateFont(25, 0, 0, 0, 0, FALSE, FALSE, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
	SelectObject(hDC_Back, hfont);
	DeleteObject(hfont);
	//------------
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
		if (KeyFlag['Z'])keypress++;
		else keypress--;
		// 分岐
		switch (GameFlag) {
		case OPENING: Opening(); break;
		case MAIN: MainRoutine(); break;
		case SHOOTING: Shooting(); break;
		case ACTION:Action(); break;
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
	DeleteDC(h_op), DeleteDC(h_main), DeleteDC(h_end), DeleteDC(h_select);
	DeleteDC(h_jiki), DeleteDC(jikimask);
	DeleteDC(h_teki), DeleteDC(tekimask);
	DeleteDC(h_tama), DeleteDC(tamamask);
	DeleteDC(h_item1), DeleteDC(item1mask);
	DeleteDC(h_item2), DeleteDC(item2mask);
	timeEndPeriod(Caps.wPeriodMin); // 後処理
}




