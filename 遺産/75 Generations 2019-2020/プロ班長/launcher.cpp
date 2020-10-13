#include "DxLib.h"
#define dis_x  1920
#define dis_y dis_x*9/16
char key[256], buf[128],setumei[256];
int tmax;
WIN32_FIND_DATA Fd[128];

int gpUpdateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i < 256; i++) {
		tmpKey[i] != 0 ? key[i]++ : key[i] = 0;    // 押されたら加算or0
	}
	return 0;
}
int n = 0;
int img, upup, img2;
int font, font2;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	SetOutApplicationLogValidFlag(FALSE), SetGraphMode(dis_x, dis_y, 16), ChangeWindowMode(true), SetMainWindowText("launcher 2019");
	// SetAlwaysRunFlag(TRUE);
	if (DxLib_Init() == -1) return -1;
	font = CreateFontToHandle(NULL, dis_y/20, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	font2 = CreateFontToHandle(NULL, dis_y / 10, 3, DX_FONTTYPE_ANTIALIASING_EDGE);
	SetDrawScreen(DX_SCREEN_BACK);//, SetDrawMode(DX_DRAWMODE_BILINEAR);
	HANDLE hFind;
	HWND m_hWnd=0;
	char ch[512];
	const char* target = "*";
	const char* ext;
	int fileCount = 0;
	int file;
	hFind = FindFirstFile(target, &Fd[fileCount]);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(hFind, &Fd[fileCount])) {
			sprintf_s(buf, "%s", Fd[fileCount].cFileName);
			ext = strrchr(buf, '.');
			if (ext == NULL)fileCount++;
			tmax = fileCount;
		}
		FindClose(hFind);
		fileCount = 0;
	}
	
	int y=0;
	SetFontSize(dis_y/20);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !gpUpdateKey()) {//ループ
		if (fileCount++ == 0) {
			upup = 0;
			sprintf_s(buf, "%s/Back.jpg", Fd[n].cFileName);
			img = LoadGraph(buf);
			if (img == -1)
				sprintf_s(buf, "%s/Back.png", Fd[n].cFileName);
			img = LoadGraph(buf);
			sprintf_s(buf, "%s/Banner.jpg", Fd[n].cFileName);
			img2 = LoadGraph(buf);
			if (img2 == -1)
				sprintf_s(buf, "%s/Banner.png", Fd[n].cFileName);
			img2 = LoadGraph(buf);
			sprintf_s(buf, "%s/Setsumei.txt", Fd[n].cFileName);
			file = FileRead_open(buf);
			while (FileRead_eof(file) == 0) {
				FileRead_gets(ch, 512, file);

			}
			FileRead_close(file);
			sprintf_s(buf, "%s/game.exe", Fd[n].cFileName);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if (150 > upup)upup += 2;
		SetDrawBright(upup, upup, upup);
		DrawExtendGraph(0, 0, dis_x, dis_y, img, TRUE);
		SetDrawBright(255, 255, 255);
		//DrawFormatString(0, y, GetColor(255, 255, 255), "%s", ch);
		DrawExtendGraph(dis_x / 2, dis_y / 2 - dis_y / 4, dis_x, dis_y - dis_y / 4, img, TRUE);
		DrawExtendGraph(dis_x / 2, 0, dis_x, dis_y / 4, img2, TRUE);
		//DrawExtendGraph(dis_x/2, dis_y/6, dis_x/10*9, dis_y/6*3, img, TRUE);
		for (int i = 0; i < tmax; i++) {
			if (i == n) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				DrawBox(dis_x / 10, dis_y / 4 + dis_y / 15 * (i + 1) - n * dis_y / 15, dis_x / 2, dis_y / 10 * 3 + dis_y / 15 * (i + 1) - n * dis_y / 15, GetColor(100, 100, 100), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawFormatString(dis_x / 10, dis_y / 4 + dis_y / 15 * (i + 1) - n * dis_y / 15, GetColor(255, 255, 255), Fd[i].cFileName);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				DrawBox(dis_x / 19, dis_y / 4 + dis_y / 15 * (i + 1) - n * dis_y / 15, dis_x / 2, dis_y / 20 * 6 + dis_y / 15 * (i + 1) - n * dis_y / 15, GetColor(100, 100, 100), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				DrawString(dis_x / 19, dis_y / 4 + dis_y / 15 * (i + 1) - n * dis_y / 15, Fd[i].cFileName, GetColor(255, 255, 255));
			}
		}
		DrawBox(dis_x / 20 * 14, dis_y / 20 * 17, dis_x / 40 * 39, dis_y / 40 * 39, GetColor(255, 0, 0), TRUE);
		DrawFormatStringToHandle(dis_x / 20 * 15, dis_y / 20 * 17, GetColor(255, 255, 255), font2,"Aでプレイ");
		//DrawString(dis_x/19, 30 + 30 * n, "■", GetColor(255, 255, 255));
		if (key[KEY_INPUT_DOWN] == 1 && n < tmax - 1)  n++, fileCount = 0;
		if (key[KEY_INPUT_UP] == 1 && n > 0)n--, fileCount = 0;
		if (key[KEY_INPUT_Z] == 1) {
			WinExec(buf, SW_SHOW);
		}
	}
	
	DxLib_End(); // DXライブラリ終了処理
	return 0;
}
