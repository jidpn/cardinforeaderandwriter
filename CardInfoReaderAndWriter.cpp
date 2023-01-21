// CardInfoReaderAndWriter.cpp : アプリケーションのエントリ ポイントを定義します。
//r
#include <sstream>
#include <cmath>
#include <vector>
#include <direct.h>
#include <atlstr.h>
#include <iostream>
#include <fstream>
#include <string>
#include "framework.h"
#include "windows.h"
#include "WindowsX.h"
#include <commdlg.h>
#include <tchar.h>
#include <commctrl.h>
#include <objbase.h>
#include <objidl.h>
#include <gdiplus.h>
#include <strsafe.h>
#include "CardRect.h"
#include "CardRectArray.h"
#include "winuser.h"
#include <cstdlib>
#include <opencv2/text.hpp>
#include <opencv2/opencv.hpp>
#include "CardInfoReaderAndWriter.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <shlobj.h>
#include "extension.h"
#include "option.h"
#include "language.h"
#define GetRValue(rgb) ((BYTE)(rgb))
#define GetGValue(rgb) ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb) ((BYTE)((rgb)>>16))
#define SetRValue(rgb) ((BYTE)(rgb))
#define SetGValue(rgb) ((int)(((BYTE)(rgb)) << 8))
#define SetBValue(rgb) ((int)(((BYTE)rgb) << 16))
#define ID_BUTTON   1000
#define ID_BUTTON_READ_OPTION   1010
#define ID_BUTTON_READ_WINDOW   1020
#define ID_BUTTON_WRITE_WINDOW   1030
#define ID_BUTTON_WRITE_OPTION   1040
#define ID_BUTTON_WRITE_COLOR_CHOOSE 1045
#define ID_BUTTON_WRITE_OUTER_COLOR_CHOOSE 1051
#define ID_BUTTON_WRITE_FONT_COLOR 1048
#define ID_BUTTON_OUT_COLOR_CHOOSE 1049
#define ID_BUTTON_SYMBOL_WINDOW   1050
#define ID_BUTTON_SYMBOL_SAVE   1060
#define ID_BUTTON_SYMBOL_COLOR_CHOOSE   1070
#define ID_BUTTON_SYMBOL_FILE 1080
#define ID_BUTTON_FONT   1500
#define ID_BUTTON_REMOVE   2000
#define ID_BUTTON_WORD   3000
#define ID_BUTTON_ALPHABET   4000
#define ID_BUTTON_SYMBOL   4100
#define ID_BUTTON_JPN_SYMBOL   4200
#define ID_BUTTON_TEST   4500
#define ID_BUTTON_WRITE_TEST   4510
#define ID_BUTTON_CHOOSE_SYMBOL   5000
#define SYMBOL_CHILD_WINDOW 5010
#define CHILD_ID_A  5000

#pragma comment (lib, "comctl32")
#define MAX_LOADSTRING 100
#ifndef UNICODE  
typedef std::string String;
#else
typedef std::wstring String;
#endif
using namespace cv;
using namespace Gdiplus;
HWND hWndListBox; 
HWND hWndListBox2; 
HWND hWndListBox3; 
HWND hWndListBox4; 
HWND hWndListBox5; 
HWND hWndListBox6; 
HWND hWndListBox7; 
cv::Mat template_img ;
cv::Mat template_img_resized ;
char bitmap_change_flag = FALSE;
char template_img_flag = FALSE;
char template_img_flag2 = FALSE;
int current_data_num;
int current_data_symbol;
// グローバル変数:
Extension extension;
HINSTANCE hInst;                                // 現在のインターフェイス
Option option;
int changing_data = 0;
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
CardRectArray card_data;
int window_width = 100;
int window_height = 100;
int dialog_mode = 0;
int dialog_mode2 = 0;
int dialog_mode3 = 0;
HMENU menu;
CardRect current_data;
// このコード モジュールに含まれる関数の宣言を転送します:
char mouse_down_flag = 0;
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
HBITMAP HBitmap;
HDC BitmapDC;

void CreateChooseSymbolWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ChooseSymbolWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName = (LPCWSTR)szClassName;
}
void CreateAreaChooseWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ChooseAreaWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName = (LPCWSTR)szClassName;
}
void CreateWriteOptionWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WriteOptionWindowProc; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName = (LPCWSTR)szClassName;
}
void CreateReadOptionWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ReadOptionWindowProc; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName = (LPCWSTR)szClassName;
}
void CreateSymbolOptionWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = SymbolOptionWindowProc; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName = (LPCWSTR)szClassName;
}
void CreateWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc)
{
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = ChildWindowProc; // プロシージャ名
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); //グレーにする
    wc.lpszMenuName = NULL;  // 未サポート
    wc.lpszClassName = (LPCWSTR)szClassName;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    setlocale(LC_ALL, "Japanese");
    initOption();
    if (card_data.num == 0) {
        card_data = CardRectArray(option.getLanguage());
    }
    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CARDINFOREADERANDWRITER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CARDINFOREADERANDWRITER));

    MSG msg;

    initOption();
    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
            card_data.data[0].cf.hwndOwner = NULL;
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CARDINFOREADERANDWRITER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CARDINFOREADERANDWRITER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

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

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
void changeEach(HMENU tmp_menu,int position, int flag, TCHAR* string ) {
    MENUITEMINFOW lpmii = { sizeof(lpmii) };
    GetMenuItemInfoW(
        tmp_menu,
        position,
        flag,
        &lpmii
    );
    lpmii.fMask = MIIM_STRING;
    lpmii.dwTypeData = string;
    SetMenuItemInfoW(
        tmp_menu,
        position,
        flag,
        &lpmii
    );
}
    
void changeMenuChinese() {
    HMENU local_menu,local_child_menu;
    changeEach(menu,0, 1, MENU_FILE_C);
    changeEach(menu,1, 1, MENU_REGION_C);
    changeEach(menu,2, 1, MENU_OPTION_C);
    changeEach(menu,3, 1, MENU_HELP_C);
    local_menu = GetSubMenu(menu, 0);
    changeEach(local_menu, 0, 1, MENU_READ_TEMPLATE_FILE_C);
    changeEach(local_menu, 1, 1, MENU_BASE_DIR_C);
    changeEach(local_menu, 2, 1, MENU_READ_DIR_C);
    changeEach(local_menu, 3, 1, MENU_READ_CARD_DATA_C);
    changeEach(local_menu, 4, 1, MENU_TEST_WRITE_DATA_C);
    changeEach(local_menu, 5, 1, MENU_WRITE_DATA_C);
    changeEach(local_menu, 6, 1, MENU_READ_META_DATA_C);
    changeEach(local_menu, 7, 1, MENU_WRITE_META_DATA_C);
    changeEach(local_menu, 8, 1, MENU_OVER_WRITE_META_DATA_C);
    changeEach(local_menu, 9, 1, MENU_EXIT_C);
    local_menu = GetSubMenu(menu, 1);
    changeEach(local_menu, 0, 1, MENU_CHOOSE_C);
    changeEach(local_menu, 1, 1, MENU_RESET_C);
    local_menu = GetSubMenu(menu, 2);
    changeEach(local_menu, 0, 1, MENU_MAGNIFICATION_C);
    changeEach(local_menu, 1, 1, MENU_LEAVE_INTERMEDIATE_C);
    changeEach(local_menu, 2, 1, MENU_EXTENSION_C);
    changeEach(local_menu, 3, 1, MENU_LANGUAGE_C);
    local_child_menu = GetSubMenu(local_menu, 3);
    changeEach(local_child_menu, 0, 1, MENU_JAPANESE_C);
    changeEach(local_child_menu, 1, 1, MENU_ENGLISH_C);
    changeEach(local_child_menu, 2, 1, MENU_CHINESE_C);
    local_menu = GetSubMenu(menu, 3);
    changeEach(local_menu, 0, 1, MENU_VERSION_C);
    //DestroyMenu(menu);



}
void changeMenuJapanese() {
    HMENU local_menu,local_child_menu;
    changeEach(menu,0, 1, MENU_FILE);
    changeEach(menu,1, 1, MENU_REGION);
    changeEach(menu,2, 1, MENU_OPTION);
    changeEach(menu,3, 1, MENU_HELP);
    local_menu = GetSubMenu(menu, 0);
    changeEach(local_menu, 0, 1, MENU_READ_TEMPLATE_FILE);
    changeEach(local_menu, 1, 1, MENU_BASE_DIR);
    changeEach(local_menu, 2, 1, MENU_READ_DIR);
    changeEach(local_menu, 3, 1, MENU_READ_CARD_DATA);
    changeEach(local_menu, 4, 1, MENU_TEST_WRITE_DATA);
    changeEach(local_menu, 5, 1, MENU_WRITE_DATA);
    changeEach(local_menu, 6, 1, MENU_READ_META_DATA);
    changeEach(local_menu, 7, 1, MENU_WRITE_META_DATA);
    changeEach(local_menu, 8, 1, MENU_OVER_WRITE_META_DATA);
    changeEach(local_menu, 9, 1, MENU_EXIT);
    local_menu = GetSubMenu(menu, 1);
    changeEach(local_menu, 0, 1, MENU_CHOOSE);
    changeEach(local_menu, 1, 1, MENU_RESET);
    local_menu = GetSubMenu(menu, 2);
    changeEach(local_menu, 0, 1, MENU_MAGNIFICATION);
    changeEach(local_menu, 1, 1, MENU_LEAVE_INTERMEDIATE);
    changeEach(local_menu, 2, 1, MENU_EXTENSION);
    changeEach(local_menu, 3, 1, MENU_LANGUAGE);
    local_child_menu = GetSubMenu(local_menu, 3);
    changeEach(local_child_menu, 0, 1, MENU_JAPANESE);
    changeEach(local_child_menu, 1, 1, MENU_ENGLISH);
    changeEach(local_child_menu, 2, 1, MENU_CHINESE);
    local_menu = GetSubMenu(menu, 3);
    changeEach(local_menu, 0, 1, MENU_VERSION);
   // DestroyMenu(menu);
}
void changeMenuEnglish() {
    HMENU local_menu,local_child_menu;
    changeEach(menu,0, 1, MENU_FILE_E);
    changeEach(menu,1, 1, MENU_REGION_E);
    changeEach(menu,2, 1, MENU_OPTION_E);
    changeEach(menu,3, 1, MENU_HELP_E);
    local_menu = GetSubMenu(menu, 0);
    changeEach(local_menu, 0, 1, MENU_READ_TEMPLATE_FILE_E);
    changeEach(local_menu, 1, 1, MENU_BASE_DIR_E);
    changeEach(local_menu, 2, 1, MENU_READ_DIR_E);
    changeEach(local_menu, 3, 1, MENU_READ_CARD_DATA_E);
    changeEach(local_menu, 4, 1, MENU_TEST_WRITE_DATA_E);
    changeEach(local_menu, 5, 1, MENU_WRITE_DATA_E);
    changeEach(local_menu, 6, 1, MENU_READ_META_DATA_E);
    changeEach(local_menu, 7, 1, MENU_WRITE_META_DATA_E);
    changeEach(local_menu, 8, 1, MENU_OVER_WRITE_META_DATA_E);
    changeEach(local_menu, 9, 1, MENU_EXIT_E);
    local_menu = GetSubMenu(menu, 1);
    changeEach(local_menu, 0, 1, MENU_CHOOSE_E);
    changeEach(local_menu, 1, 1, MENU_RESET_E);
    local_menu = GetSubMenu(menu, 2);
    changeEach(local_menu, 0, 1, MENU_MAGNIFICATION_E);
    changeEach(local_menu, 1, 1, MENU_LEAVE_INTERMEDIATE_E);
    changeEach(local_menu, 2, 1, MENU_EXTENSION_E);
    changeEach(local_menu, 3, 1, MENU_LANGUAGE_E);
    local_child_menu = GetSubMenu(local_menu, 3);
    changeEach(local_child_menu, 0, 1, MENU_JAPANESE_E);
    changeEach(local_child_menu, 1, 1, MENU_ENGLISH_E);
    changeEach(local_child_menu, 2, 1, MENU_CHINESE_E);
    local_menu = GetSubMenu(menu, 3);
    changeEach(local_menu, 0, 1, MENU_VERSION_E);
    //DestroyMenu(menu);


}
void changeMenuLanguage(std::string language) {
    if (language == "jpn") {
        changeMenuJapanese();
        setlocale(LC_ALL, "Japanese");
    }
    else if (language == "jpn") {
        setlocale(LC_ALL, "Chinese-simplified");
        changeMenuChinese();

    }
    else {
        changeMenuEnglish();
        setlocale(LC_ALL, "English");

    }
}
HWND hChildWnd_A;
HWND hChildWnd_B;
HWND hChildWnd_C;
HWND hChildWnd_D;
HWND hChildWnd_E;
HWND hChildWnd_F;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap;
    HPEN hpen;
    HPEN hpen2;
    char tmp_multi[256];
    int num;
    int ret;
    int width = 0;
    int height = 0;
    PAINTSTRUCT ps;
    HDC hdc;
    HDC hdc2;
    cv::Mat gray;
    TCHAR data_name[10];
    TCHAR writing_tmp[256];
    WNDCLASSW wc_A;
    static tesseract::TessBaseAPI* ocr = NULL;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    BROWSEINFO bInfo;
    LPITEMIDLIST pIDList;
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    char result_file[255];
    char result_file2[255];
    struct stat stat_tmp;
    std::string out_text;
    std::ofstream ofs;
	std::string line;
    TCHAR inputPathTmp[MAX_PATH];
    char char_input_path[MAX_PATH];
    char full_file_name[MAX_PATH];
    CardRect cur_data;
    TEXTMETRIC tm;
    const std::string TestData = "AAA\nBBB\nCCC\n";
    std::wstring initial_file;
    std::vector<std::string> writing_strings;
    int strings_size;
    int read_flag = false;
    static std::wstring wfile_name = TEXT("");
	FILE* fp;

    switch (message)
    {
    case WM_CREATE:
        if (ocr) {
            ocr->End();
        }
        ocr = new tesseract::TessBaseAPI();
        menu = GetMenu(hWnd);
        changeMenuLanguage(option.language);
        if (template_img_flag == true) {
            bitmap_change_flag = TRUE;
            hdc = GetDC(hWnd);
            mat2memHdc(hdc, template_img_resized);
            SetWindowPos(hWnd, 0, 0, 0, template_img_resized.cols + 30, template_img_resized.rows + 60, 0);
            window_width = template_img_resized.cols;
            window_height = template_img_resized.rows;
            ReleaseDC(hWnd, hdc);
            InvalidateRect(hWnd, 0, TRUE);
        }
        break;
    case WM_LBUTTONDOWN:
        if (template_img_flag == false) {
            break;
        }
        if (dialog_mode == 1)
            break;
        current_data = CardRect(GET_X_LPARAM(lParam) / option.resize, GET_Y_LPARAM(lParam) / option.resize,option.getLanguage());
        mouse_down_flag = 1;
        // Msg was handled, return zero.
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_MOUSEMOVE:
        if (template_img_flag == false) {
            break;
        }
        if (dialog_mode == 1)
            break;
        if (mouse_down_flag == 1) {
            current_data.update(GET_X_LPARAM(lParam) / option.resize, GET_Y_LPARAM(lParam) / option.resize);
        }
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_LBUTTONUP:
        if (template_img_flag == false) {
            break;
        }
        if (dialog_mode == 1)
            break;
        if (current_data.start_x == GET_X_LPARAM(lParam) / option.resize || current_data.start_y == GET_Y_LPARAM(lParam) / option.resize) {
            mouse_down_flag = 0;
            return 0;
        }
        current_data.finalize(GET_X_LPARAM(lParam) / option.resize, GET_Y_LPARAM(lParam) / option.resize);
        num = card_data.Add(current_data);
        wsprintf(data_name, TEXT("%d"), num);
        mouse_down_flag = 0;
        dialog_mode = 1;
        // 子ウィンドウＡの生成
        CreateWindowClass(hInst, data_name, wc_A);
        RegisterClassW(&wc_A);
        changing_data = num;
        hChildWnd_A = CreateChildWindow(hInst, SW_SHOW, data_name, (HMENU)(100 * (card_data.num - 1)), hWnd);

        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case LANG_JPN:
            option.language = "jpn";
            menu = GetMenu(hWnd);
            changeMenuJapanese();
            DrawMenuBar(hWnd);
            setlocale(LC_ALL, "Japanese");
            break;
        case ID_CHINESE:
            option.language = "chi_sim";
            menu = GetMenu(hWnd);
            setlocale(LC_ALL, "Chinese-simplified");
            changeMenuChinese();

            DrawMenuBar(hWnd);
            break;
        case LANG_ENG:
            option.language = "eng";
            menu = GetMenu(hWnd);
            changeMenuEnglish();
            setlocale(LC_ALL, "English");
            DrawMenuBar(hWnd);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDABORT:
            TCHAR szFile[MAX_PATH];
            if (wfile_name == TEXT("")) {
                initial_file = _TEXT("C:\\");
            }
            else {
                initial_file = wfile_name;

            }
            if (GetFileNameAll(0, szFile, sizeof(szFile) / sizeof(TCHAR), initial_file.c_str())) {
                wfile_name = szFile;
                std::string file_name = WStringToString(wfile_name, 0);
                option.template_file_name = file_name;
                if (template_img_flag == TRUE) {
                    template_img.release();
                }
                template_img_flag = TRUE;
                bitmap_change_flag = TRUE;
                template_img = cv::imread(file_name);
                resize(template_img, template_img_resized, cv::Size(), option.resize, option.resize);
                hdc = GetDC(hWnd);
                mat2memHdc(hdc, template_img_resized);
                SetWindowPos(hWnd, 0, 0, 0, template_img_resized.cols + 30, template_img_resized.rows + 60, 0);
                window_width = template_img_resized.cols;
                window_height = template_img_resized.rows;
                ReleaseDC(hWnd, hdc);
                InvalidateRect(hWnd, 0, TRUE);
            }
            break;
        case WRITE_CARD:
        {
            cv::Mat template_img2;
            if (_tcslen(option.inputPath) == 0) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("読み込みディレクトリを指定してください。"), TEXT("読み込みディレクトリを指定してください。"), MB_OK);
                }
                else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("选择目录"), TEXT("选择目录。"), MB_OK);
                }
                else {
                    MessageBox(hWnd, TEXT("Choose read directory"), TEXT("Choose read directory"), MB_OK);
                }
                return false;
            }                char multi[256];
            ret = WideCharToMultiByte(
                CP_ACP,
                0,
                option.inputPath,
                -1,
                tmp_multi,
                200 + 1,
                NULL,
                NULL);
            strcpy_s(result_file2, 255, tmp_multi);
            strcat_s(result_file2, 255, "\\out");
            if (stat(result_file2, &stat_tmp) != 0) {
                _mkdir(result_file2);
            }
            if (GetFileNameCsv(0, szFile, sizeof(szFile) / sizeof(TCHAR), initial_file.c_str())) {
                std::wstring wfile_name = szFile;
                std::string file_name = WStringToString(wfile_name, 0);
                if (fopen_s(&fp, file_name.c_str(), "r")) {
                    if (option.language == "jpn") {
                        MessageBox(hWnd, TEXT("領域データが読み込めませんでした"), TEXT("領域データが読み込めませんでした。"), MB_OK);
                    }
                    else if (option.language == "chi_sim") {
                        MessageBox(hWnd, TEXT("不可能读"), TEXT("不可能读"), MB_OK);
                    }
                    else {
                        MessageBox(hWnd, TEXT("Could not read region data"), TEXT("Could not read region data"), MB_OK);
                    }
                    return 0;
                }
            }
            else {
                return 0;
            }
            char buf[256];
            char field[256];
            char tmp[50][256];
            char* p;
            fgets(buf, 256, fp);
            while (fgets(buf, 256, fp)) {
                std::istringstream i_stream(line);
                int j = 0;
                for (p = buf; *p;) {
                    p = setvalue(p, field, sizeof(field));
                    sprintf_s(tmp[j], 256, "%s", field);
                    j++;
                }
                snprintf(multi, 256, "%s\\%s", tmp_multi, tmp[0]);
                template_img2 = cv::imread(multi);
                if (template_img2.cols == 0 || template_img2.rows == 0) {
                    continue;
                }
                HDC hdc = GetDC(hWnd);
                mat2memHdc(hdc, template_img2);
                Graphics graphics(mat2memHdc.memHDC);
                cur_data.cf.hwndOwner = hWnd;
                for (int i = 0; i < card_data.num; i++) {
                    cur_data = card_data.data[i];
                    if (cur_data.white_or_black == 2 || cur_data.white_or_black == 3) {
                        continue;
                    }
                    //    Graphics graphics(mat2memHdc.memHDC);
                    SolidBrush    pen(Color(cur_data.opaque, cur_data.bkColor[0], cur_data.bkColor[1], cur_data.bkColor[2]));
                    if (cur_data.figure == 0) {
                        Gdiplus::Pen outer_pen(Color(cur_data.outer_opaque, cur_data.outerColor[0], cur_data.outerColor[1], cur_data.outerColor[2]), cur_data.outer_width);
                        graphics.DrawRectangle(&outer_pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y - cur_data.start_y);
                        graphics.FillRectangle(&pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y - cur_data.start_y);
                    }
                    else {
                        Gdiplus::Pen outer_pen(Color(cur_data.outer_opaque, cur_data.outerColor[0], cur_data.outerColor[1], cur_data.outerColor[2]), cur_data.outer_width);
                        graphics.DrawEllipse(&outer_pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y - cur_data.start_y);
                        graphics.FillEllipse(&pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y - cur_data.start_y);
                    }
                    HFONT hFont = CreateFontIndirect(&cur_data.lf);
                    HPEN font_pen = CreatePen(PS_SOLID, cur_data.font_width, SetRValue(cur_data.outColor[0]) + SetGValue(cur_data.outColor[1]) + SetBValue(cur_data.outColor[0]));
                    SelectObject(mat2memHdc.memHDC, hFont);
                    SelectObject(mat2memHdc.memHDC, font_pen);
                    SetTextColor(mat2memHdc.memHDC, cur_data.cf.rgbColors);
                    SetBkMode(mat2memHdc.memHDC, TRANSPARENT);
                    writing_strings = splitString(tmp[i + 1]);
                    strings_size = writing_strings.size();
                    int font_height;
                    if (cur_data.font_height == -1) {
                        GetTextMetrics(mat2memHdc.memHDC, &tm);
                        font_height = tm.tmHeight;
                    }
                    else {
                        font_height = cur_data.font_height;

                    }
                    BeginPath(mat2memHdc.memHDC);
                    for (int j = 0; j < strings_size; j++) {
                        MultiByteToWideChar(CP_UTF8, 0, writing_strings.at(j).c_str(), 256, ((LPWSTR)writing_tmp), 256);
                        if (cur_data.horizontal_align == 0) {
                            if (cur_data.vertical_align == 0) {
                                SetTextAlign(mat2memHdc.memHDC, TA_LEFT | TA_TOP);
                                TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                            }
                            else if (cur_data.vertical_align == 1) {
                                SetTextAlign(mat2memHdc.memHDC, TA_LEFT);
                                TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, (cur_data.start_y + cur_data.end_y + cur_data.margin_y + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                            }
                            else if (cur_data.vertical_align == 2) {
                                SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM) | TA_LEFT);
                                TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                            }
                        }
                        else if (cur_data.horizontal_align == 1) {
                            if (cur_data.vertical_align == 0) {
                                SetTextAlign(mat2memHdc.memHDC, TA_TOP);
                                TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                            }
                            else if (cur_data.vertical_align == 1) {
                                SetTextAlign(mat2memHdc.memHDC, (TA_CENTER));
                                TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, (cur_data.start_y + cur_data.end_y + cur_data.margin_y - font_height + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                            }
                            else if (cur_data.vertical_align == 2) {
                                SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM));
                                TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                            }
                        }
                        else if (cur_data.horizontal_align == 2) {
                            if (cur_data.vertical_align == 0) {
                                SetTextAlign(mat2memHdc.memHDC, TA_RIGHT | TA_TOP);
                                TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                            }
                            else if (cur_data.vertical_align == 1) {
                                SetTextAlign(mat2memHdc.memHDC, TA_RIGHT);
                                TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, (cur_data.start_y + cur_data.end_y + +cur_data.margin_y + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                            }
                            else if (cur_data.vertical_align == 2) {
                                SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM) | TA_RIGHT);
                                TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                            }
                        }
                    }
                    EndPath(mat2memHdc.memHDC);
                    SolidBrush    pen2(Color(255, cur_data.fontColor[0], cur_data.fontColor[1], cur_data.fontColor[2]));
                    SelectObject(mat2memHdc.memHDC, &pen2);
                    StrokeAndFillPath(mat2memHdc.memHDC);
                }
                memHDC2Mat(mat2memHdc.hbmp);
                for (int i = 0; i < card_data.num; i++) {
                    int repeat = 0;
                    int x = 0;
                    int y = 0;
                    int y_flag = false;
                    cur_data = card_data.data[i];
                    cv::Mat writing_img;
                    writing_strings = splitString(tmp[i + 1]);
                    strings_size = writing_strings.size();
                    for (int j = 0; j < strings_size; j++) {
                        for (int k = 0, start_k = 0; k < strlen(writing_strings.at(j).c_str()); k++) {
                            for (int l = 0; l < SYMBOL_NUM; l++) {
                                if (cur_data.symbol[l] == "") {
                                    continue;
                                }
                                if (cur_data.white_or_black == 2 && k == 0) {
                                    continue;
                                }
                                char dst[50];
                                substring(writing_strings.at(j).c_str(), start_k, k - start_k, dst, sizeof(dst));
                                if (strcmp(dst, cur_data.symbol_name[l].c_str()) == 0) {
                                    y_flag = true;
                                    start_k = k - 1;
                                    writing_img = imread(cur_data.symbol[l]);
                                    writing_img = makealphachannel(writing_img, cur_data.none_color[l]);
                                    if (cur_data.white_or_black == 2) {
                                        paste(*memHDC2Mat.mat, writing_img, cur_data.start_x + x, cur_data.start_y + y);
                                        x += writing_img.cols + cur_data.margin_symbol;
                                    }
                                    else {
                                        char dst2[50];
                                        repeat = atoi(substring(writing_strings.at(j).c_str(), start_k + 1, 1, dst2, sizeof(dst2)));
                                        for (int i = 0; i < repeat; i++) {
                                            paste(*memHDC2Mat.mat, writing_img, cur_data.start_x + x, cur_data.start_y + y);
                                            x += writing_img.cols + cur_data.margin_symbol;
                                        }
                                        start_k += 2;
                                        k++;
                                    }
                                }
                            }
                        }
                        if (y_flag) {
                            y += writing_img.rows;
                            x = 0;
                        }
                    }
                    continue;
                }
                snprintf(multi, 256, "%s\\out\\%s", tmp_multi, tmp[0]);
                cv::imwrite(multi, *memHDC2Mat.mat);
                ReleaseDC(hWnd, hdc);

            }
            if (fp) {
                fclose(fp);
            }

            if (option.language == "jpn") {
                MessageBox(hWnd, TEXT("データ書き込みが終わりました。"), TEXT("データ書き込みが終わりました"), MB_OK);
            }
            else if (option.language == "chi_sim") {
                MessageBox(hWnd, TEXT("完了"), TEXT("完了"), MB_OK);
            }
            else {
                MessageBox(hWnd, TEXT("Finished "), TEXT("Finished"), MB_OK);
            }
            break;
        }
        case WRITE_TEST:
            if (template_img_flag == FALSE) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("テンプレートファイルを指定してください"), TEXT("テンプレートファイルを指定してください。"), MB_OK);
                }
                else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("选择模板文件."), TEXT("选择模板文件.`"), MB_OK);
                }
                else {
                    MessageBox(hWnd, TEXT("Choose template file"), TEXT("Choose template file"), MB_OK);
                }
            }
            hdc2 = GetDC(hWnd);

            mat2memHdc(hdc2, template_img);

            for (int i = 0; i < card_data.num; i++) {
                cur_data = card_data.data[i];
                cur_data.cf.hwndOwner = hWnd;

                HFONT hFont = CreateFontIndirect(&cur_data.lf);
                SelectObject(mat2memHdc.memHDC, hFont);
                SetTextColor(mat2memHdc.memHDC, cur_data.cf.rgbColors);
                SetBkMode(mat2memHdc.memHDC, TRANSPARENT);
                writing_strings = splitString(TestData);
                strings_size = writing_strings.size();
                int font_height;
                if (cur_data.font_height == -1) {
                    GetTextMetrics(mat2memHdc.memHDC, &tm);
                    font_height = tm.tmHeight;
                }
                else {
                    font_height = cur_data.font_height;

                }
                for (int j = 0; j < strings_size; j++) {
                    MultiByteToWideChar(CP_UTF8, 0, writing_strings.at(j).c_str(), sizeof(writing_strings.at(j).c_str()), ((LPWSTR)writing_tmp), 256);
                    if (cur_data.horizontal_align == 0) {
                        if (cur_data.vertical_align == 0) {
                            SetTextAlign(mat2memHdc.memHDC, TA_LEFT | TA_TOP);
                            TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 1) {
                            SetTextAlign(mat2memHdc.memHDC, TA_LEFT);
                            TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, (cur_data.start_y + cur_data.end_y + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 2) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM) | TA_LEFT);
                            TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                    }
                    else if (cur_data.horizontal_align == 1) {
                        if (cur_data.vertical_align == 0) {
                            SetTextAlign(mat2memHdc.memHDC, TA_TOP);
                            TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 1) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_CENTER));
                            TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, (cur_data.start_y + cur_data.end_y + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 2) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM));
                            TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                    }
                    else if (cur_data.horizontal_align == 2) {
                        if (cur_data.vertical_align == 0) {
                            SetTextAlign(mat2memHdc.memHDC, TA_RIGHT | TA_TOP);
                            TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 1) {
                            SetTextAlign(mat2memHdc.memHDC, TA_RIGHT);
                            TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, (cur_data.start_y + cur_data.end_y + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 2) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM) | TA_RIGHT);
                            TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                    }
                }
                SelectObject(mat2memHdc.memHDC, GetStockObject(SYSTEM_FONT));
                DeleteObject(hFont);

            }
            memHDC2Mat(mat2memHdc.hbmp);
            imshow("OpenCV namedWindow", *memHDC2Mat.mat);
            break;
        case READ:
            read_flag = false;
            if (_tcslen(option.inputPath) == 0) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("読み込みディレクトリを指定してください。"), TEXT("読み込みディレクトリを指定してください。"), MB_OK);
                }
                else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("选择读目录"), TEXT("选择读目录"), MB_OK);

                }
                else {
                    MessageBox(hWnd, TEXT("Choose read directory"), TEXT("Choose read directory"), MB_OK);
                }
                return false;
            }
            char base_dir[255];
            char intermediate_file[255];
            char tmp_data_label[256];
            ret = WideCharToMultiByte(
                CP_ACP,
                0,
                option.base_dir,
                -1,
                base_dir,
                200 + 1,
                NULL,
                NULL);
            _tcscpy_s(inputPathTmp, MAX_PATH, option.inputPath);
            StringCchCat(inputPathTmp, MAX_PATH, TEXT("\\*"));
            hFind = FindFirstFileEx(inputPathTmp, FindExInfoStandard, &ffd, FindExSearchNameMatch, NULL, 0);
            ret = WideCharToMultiByte(
                CP_ACP,
                0,
                option.inputPath,
                -1,
                result_file,
                200 + 1,
                NULL,
                NULL);
            strcpy_s(result_file2, 255, result_file);
            strcpy_s(char_input_path, 255, result_file);
            strcat_s(result_file2, 255, "\\intermediate");
            if (stat(result_file2, &stat_tmp) != 0) {
                if (option.getIntermediate()) {
                    if (!_mkdir(result_file2)) {
                        if (option.language == "jpn") {
                            MessageBox(hWnd, TEXT("中間フォルダを作成するのに失敗しました"), TEXT("中間フォルダを作成するのに失敗しました。"), MB_OK);
                        }
                        else if (option.language == "chi_sim") {
                            MessageBox(hWnd, TEXT("失败做中文件·"), TEXT("失败做中文件"), MB_OK);
                        }
                        else {
                            MessageBox(hWnd, TEXT("Failed to create intermediate file"), TEXT("Failed to create intermediate file"), MB_OK);
                        }
                        return false;

                    }
                }
            }

            strcat_s(result_file, 255, "\\result.csv");
            ofs.open(result_file);
            if (!ofs) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("CSVファイルが書き込みできません。"), TEXT("。"), MB_OK);
                    return 0;
                }
                else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("不能开格式文件."), TEXT("不能开格式文件."), MB_OK);
                    return 0;

                }
                else {
                    MessageBox(hWnd, TEXT("Could not open csv file"), TEXT("Could not open csv file"), MB_OK);
                    return 0;
                }

            }
            EnableWindow(hWnd, FALSE);
            SetCursor(LoadCursor(0, IDC_WAIT));
            ofs << "ファイル名" << ",";
            for (int i = 0; i < card_data.num; i++) {
                CardRect tmp = card_data.data[i];
                ret = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    card_data.data[i].data_label,
                    -1,
                    tmp_data_label,
                    200 + 1,
                    NULL,
                    NULL);
                ofs << tmp_data_label;
                if (i < card_data.num - 1) {
                    ofs << ",";
                }
            }
            ofs << std::endl;
            do
            {
                cv::Mat template_img2;
                char fileName[255];
                ret = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    ffd.cFileName,
                    -1,
                    fileName,
                    200 + 1,
                    NULL,
                    NULL);
                ffd = ffd;
                const char* ext = strrchr(fileName, '.');

                if (ext && strlen(ext) > 0) {
                    const char* ext_for_check = &(ext[1]);
                    if (!extension.checkExtension(ext_for_check)) {
                        continue;
                    }
                }
                else {
                    continue;
                }
                if (template_img_flag2 == TRUE) {

                    template_img2.release();
                }
                template_img_flag2 = TRUE;
                strcpy_s(full_file_name, MAX_PATH, char_input_path);
                strcat_s(full_file_name, MAX_PATH, "\\");
                strcat_s(full_file_name, MAX_PATH, fileName);
                template_img2 = cv::imread(full_file_name);
                if (template_img2.cols == 0) {
                    continue;
                }
                if (template_img2.rows == 0) {
                    continue;
                }
                if (width == 0) {
                    width = template_img2.cols;
                }
                if (height == 0) {
                    height = template_img2.rows;
                }
                /*
                if (template_img.cols != width || template_img.rows != height) {
                    resize(template_img, template_img, cv::Size(), (double)width / template_img.cols, (double)height / template_img.rows);
                }
                */
                cv::cvtColor(template_img2, gray, COLOR_RGB2GRAY);
                /*
                HDC hdc = GetDC(hWnd);
                mat2memHdc(hdc, template_img);
                SetWindowPos(hWnd, 0, 0, 0, template_img.cols, template_img.rows, 0);
                window_width = template_img.cols;
                window_height = template_img.rows;
                ReleaseDC(hWnd, hdc);
                InvalidateRect(hWnd, 0, TRUE);
                */


                ofs << "\"" << fileName << "\",";
                for (int i = 0; i < card_data.num; i++) {
                    if (card_data.data[i].end_x > window_width) {
                        continue;
                    }
                    if (card_data.data[i].end_x > template_img2.cols) {
                        continue;
                    }
                    if (card_data.data[i].end_y > window_height) {
                        continue;
                    }
                    if (card_data.data[i].end_y > template_img2.rows) {
                        continue;
                    }
                    if (card_data.data[i].white_or_black == 2 || card_data.data[i].white_or_black == 3) {
                        std::string result = checkSymbol(template_img, card_data.data[i], false);
                        if (result.size() > 0) {
                            ofs << "\"" << result << "\"";
                        }
                        else {
                            ofs << result;
                        }
                        if (i < card_data.num - 1) {
                            ofs << ",";
                        }
                        continue;
                    }
                    if (ocr) {
                        ocr->End();
                    }
                    ret = WideCharToMultiByte(
                        CP_ACP,
                        0,
                        card_data.data[i].data_label,
                        -1,
                        tmp_data_label,
                        200 + 1,
                        NULL,
                        NULL);
                    if (ocr->Init(base_dir, card_data.data[i].language.c_str(), tesseract::OEM_LSTM_ONLY)) {
                        if (option.language == "jpn") {
                            MessageBox(hWnd, TEXT("ocrの読み込みに失敗しました"), TEXT("OCRの読み込みに失敗しました。"), MB_OK);
                        }
                        else if (option.language == "chi_sim") {
                            MessageBox(hWnd, TEXT("失败开文件"), TEXT("失败开文件"), MB_OK);
                        }
                        else {
                            MessageBox(hWnd, TEXT("Failed open ocr"), TEXT("Failed open ocr"), MB_OK);

                        }
                        ofs.close();
                        return false;
                    }
                    CardRect tmp = card_data.data[i];
                    cv::Mat dst;
                    cv::Mat binary;
                    cv::Mat reverse;
                    if (card_data.data[i].gray == true) {
                        dst = cv::Mat(gray, cv::Rect(tmp.start_x, tmp.start_y, tmp.end_x - tmp.start_x, tmp.end_y - tmp.start_y));
                    }
                    else {
                        dst = cv::Mat(template_img2, cv::Rect(tmp.start_x, tmp.start_y, tmp.end_x - tmp.start_x, tmp.end_y - tmp.start_y));
                    }

                    if (card_data.data[i].white_or_black == WHITE_LIST) {
                        ocr->SetVariable("tessedit_char_whitelist", card_data.data[i].word_list.c_str());
                    }
                    else {
                        ocr->SetVariable("tessedit_char_blacklist", card_data.data[i].word_list.c_str());
                    }
                    if (card_data.data[i].gray == true) {
                        if (card_data.data[i].threshold >= 0 && card_data.data[i].threshold <= 255) {

                            cv::threshold(dst, binary, card_data.data[i].threshold, 255, cv::THRESH_BINARY);
                            if (card_data.data[i].reverse) {
                                bitwise_not(binary, reverse);
                                ocr->SetImage(reverse.data, reverse.cols, reverse.rows, 1, static_cast<int>(reverse.step));
                                imshow("reverse", reverse);
                                if (option.getIntermediate()) {
                                    strcpy_s(intermediate_file, 255, result_file2);
                                    strcat_s(intermediate_file, 255, "\\");
                                    strcat_s(intermediate_file, 255, fileName);
                                    cv::imwrite(intermediate_file, reverse);
                                }
                            }
                            else {
                                ocr->SetImage(binary.data, binary.cols, binary.rows, 1, static_cast<int>(binary.step));
                                imshow("binary", binary);

                                if (option.getIntermediate()) {
                                    strcpy_s(intermediate_file, 255, result_file2);
                                    strcat_s(intermediate_file, 255, "\\");
                                    strcat_s(intermediate_file, 255, tmp_data_label);
                                    strcat_s(intermediate_file, 255, "_");
                                    strcat_s(intermediate_file, 255, fileName);
                                    cv::imwrite(intermediate_file, binary);
                                }
                            }
                        }

                        else {
                            ocr->SetImage(dst.data, dst.cols, dst.rows, 1, static_cast<int>(dst.step));
                            imshow("gray", dst);
                            if (option.getIntermediate()) {
                                strcpy_s(intermediate_file, 255, result_file2);
                                strcat_s(intermediate_file, 255, "\\");
                                strcat_s(intermediate_file, 255, tmp_data_label);
                                strcat_s(intermediate_file, 255, "_");
                                strcat_s(intermediate_file, 255, fileName);
                                cv::imwrite(intermediate_file, gray);
                            }
                        }
                    }
                    else {
                        imshow("template_img2", dst);
                        ocr->SetImage(dst.data, dst.cols, dst.rows, 3, static_cast<int>(dst.step));
                        if (option.getIntermediate()) {
                            strcpy_s(intermediate_file, 255, result_file2);
                            strcat_s(intermediate_file, 255, "\\");
                            strcat_s(intermediate_file, 255, tmp_data_label);
                            strcat_s(intermediate_file, 255, "_");
                            strcat_s(intermediate_file, 255, fileName);
                            cv::imwrite(intermediate_file, template_img2);
                        }

                    }
                    out_text = ocr->GetUTF8Text();
                    out_text = UTF8toSjis(out_text);
                    if (out_text.size() > 0) {
                        ofs << "\"" << out_text << "\"";
                    }
                    else {
                        ofs << "" << out_text << "";

                    }

                    if (i < card_data.num - 1) {
                        ofs << ",";
                    }
                    dst.release();
                    if (card_data.data[i].threshold >= 0 && card_data.data[i].threshold <= 255) {
                        binary.release();
                        if (card_data.data[i].reverse) {
                            reverse.release();
                        }
                    }

                }
                gray.release();
                ofs << std::endl;
            } while (FindNextFile(hFind, &ffd) != 0);
            if (ocr) {
                ocr->End();
            }
            ofs.close();
            bitmap_change_flag = true;
            EnableWindow(hWnd, TRUE);
            if (option.language == "jpn") {
                MessageBox(hWnd, TEXT("データ読み込みが終わりました。"), TEXT("データ読み込みが終わりました。"), MB_OK);
            }
            else if (option.language == "chi_sim") {
                MessageBox(hWnd, TEXT("完了读消息"), TEXT("完了读消息"), MB_OK);
            }
            else {
                MessageBox(hWnd, TEXT("Finished reading data"), TEXT("Finished reading data"), MB_OK);

            }
                break;
            case ID_BASEDIR:
                memset(&bInfo, 0, sizeof(bInfo));
                bInfo.hwndOwner = hWnd; // ダイアログの親ウインドウのハンドル 
                bInfo.pidlRoot = NULL; // ルートフォルダをデスクトップフォルダとする 
                bInfo.pszDisplayName = option.base_dir; //フォルダ名を受け取るバッファへのポインタ 
                bInfo.lpszTitle = TEXT("フォルダの選択"); // ツリービューの上部に表示される文字列 
                bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_VALIDATE | BIF_NEWDIALOGSTYLE; // 表示されるフォルダの種類を示すフラグ 
                bInfo.lpfn = BrowseCallbackProc; // BrowseCallbackProc関数のポインタ 
                bInfo.lParam = (LPARAM)option.base_dir;
                pIDList = SHBrowseForFolder(&bInfo);
                if (pIDList == NULL) {
                    option.inputPath[0] = _TEXT('\0');
                    return false; //何も選択されなかった場合 
                }
                else {
                    if (!SHGetPathFromIDList(pIDList, option.base_dir))
                        return false;//変換に失敗 
                    CoTaskMemFree(pIDList);// pIDListのメモリを開放 
                    return true;
                }
                break;

            case META_WRITE:
                if (option.metaPath != TEXT("")) {
                    if (GetFileNameReadMeta(0, szFile, sizeof(szFile) / sizeof(TCHAR), option.metaPath.c_str())) {
                        std::wstring wfile_name = szFile;
                        std::string file_name = WStringToString(wfile_name, 0);
                        card_data.SaveAll(file_name);
                    }
                }
                else  if (GetFileNameReadMeta(0, szFile, sizeof(szFile) / sizeof(TCHAR), _TEXT("C:\\"))) 
                {

                    std::wstring wfile_name = szFile;
                    std::string file_name = WStringToString(wfile_name,0);
                    card_data.SaveAll(file_name);

                }
                break;
            case RESET_META:
                option.metaPath = TEXT("");
                card_data.reset();
                bitmap_change_flag = true;
                InvalidateRect(hWnd, 0, TRUE);
                break;
            case META_READ:
                if (option.metaPath != TEXT("")) {
                    if (GetFileNameReadMeta(0, szFile, sizeof(szFile) / sizeof(TCHAR),option.metaPath.c_str() )) 
                    {
                        option.metaPath = szFile;
                        std::string file_name = WStringToString(option.metaPath, 0);
                        card_data.LoadAll(file_name);

                    }
                }
                else {
                    if (GetFileNameReadMeta(0, szFile, sizeof(szFile) / sizeof(TCHAR), _TEXT("C:\\"))) 
                    {
                    option.metaPath = szFile;
                    std::string file_name = WStringToString(option.metaPath,0);
                    card_data.LoadAll(file_name);

                    }
                }
                InvalidateRect(hWnd, 0, TRUE);
                break;
            case OVER_WRITE_META_DATA:
                if (option.metaPath != TEXT("")) {
                    std::string file_name = WStringToString(option.metaPath, 0);
                    int ret = card_data.SaveAll(file_name);
                    if (!ret) {
                        MessageBox(hWnd, TEXT("領域情報の保存に失敗しました。"), TEXT("領域情報の保存に失敗しました。"), MB_OK);
                        return false;
                    }
                }
                else {
                    if (GetFileNameReadMeta(0, szFile, sizeof(szFile) / sizeof(TCHAR), _TEXT("C:\\"))) {
                        option.metaPath = szFile;
                        std::string file_name = WStringToString(option.metaPath,0);
                        int ret = card_data.SaveAll(file_name);
                        if (!ret) {
                            if (option.language == "jpn") {
                                MessageBox(hWnd, TEXT("領域情報の保存に失敗しました。"), TEXT("領域情報の保存に失敗しました。"), MB_OK);
                            }
                            else if (option.language == "chi_sim") {
                                MessageBox(hWnd, TEXT("失败保存消息"), TEXT("失败保存消息"), MB_OK);
                            }
                            else {
                                MessageBox(hWnd, TEXT("Failed to save regional data"), TEXT("Failed to save regional data"), MB_OK);
                            }
                            return false;
                        }
                    }

                }

                break;



            case READ_DIRECTORY:

                memset(&bInfo, 0, sizeof(bInfo));
                bInfo.hwndOwner = hWnd; // ダイアログの親ウインドウのハンドル 
                bInfo.pidlRoot = NULL; // ルートフォルダをデスクトップフォルダとする 
                bInfo.pszDisplayName = option.inputPath; //フォルダ名を受け取るバッファへのポインタ 
                bInfo.lpszTitle = TEXT("フォルダの選択"); // ツリービューの上部に表示される文字列 
                bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_VALIDATE | BIF_NEWDIALOGSTYLE; // 表示されるフォルダの種類を示すフラグ 
                bInfo.lpfn = BrowseCallbackProc; // BrowseCallbackProc関数のポインタ 
                bInfo.lParam = (LPARAM)option.inputPath;
                pIDList = SHBrowseForFolder(&bInfo);
                if (pIDList == NULL) {
                    option.inputPath[0] = _TEXT('\0');
                    return false; //何も選択されなかった場合 
                }
                else {
                    if (!SHGetPathFromIDList(pIDList, option.inputPath))
                        return false;//変換に失敗 
                    CoTaskMemFree(pIDList);// pIDListのメモリを開放 
                    return true;
                }
            case AREA_CHOOSE:
                if (card_data.num == 0) {
                    if (option.language == "jpn") {
                        MessageBox(hWnd, TEXT("領域が選択されていません。"), TEXT("領域が選択されていません。"), MB_OK);
                        return true;
                    }
                    else if (option.language == "chi_sim") {
                        MessageBox(hWnd, TEXT("地区没被选择"), TEXT("地区没被选择"), MB_OK);
                        return true;

                    }
                    else {
                        MessageBox(hWnd, TEXT("Region is not selected."), TEXT("Region is not selected。"), MB_OK);
                        return true;

                    }
                }
                CreateAreaChooseWindowClass(hInst, TEXT("SELECT_AREA"), wc_A);
                RegisterClassW(&wc_A);
                hChildWnd_B = CreateAreaChooseWindow(hInst, SW_SHOW, (HMENU)(8*100+1), hWnd);
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_INTERMEDIATE:
                option.toggleIntermediate();
                break;
            case RESIZE_100:
                option.resize = 1.0;
                if (template_img_flag == true) {
                    resize(template_img, template_img_resized, cv::Size(), option.resize, option.resize);
                    hdc = GetDC(hWnd);
                    mat2memHdc(hdc, template_img_resized);
                    SetWindowPos(hWnd, 0, 0, 0, template_img_resized.cols + 30, template_img_resized.rows + 60, 0);
                    window_width = template_img_resized.cols;
                    window_height = template_img_resized.rows;
                    ReleaseDC(hWnd, hdc);
                    InvalidateRect(hWnd, 0, TRUE);
                    bitmap_change_flag = TRUE;
                }
                break;
            case RESIZE_50:
                option.resize = 0.5;
                if (template_img_flag == true) {
                    resize(template_img, template_img_resized, cv::Size(), option.resize, option.resize);
                    hdc = GetDC(hWnd);
                    mat2memHdc(hdc, template_img_resized);
                    SetWindowPos(hWnd, 0, 0, 0, template_img_resized.cols + 30, template_img_resized.rows + 60, 0);
                    window_width = template_img_resized.cols;
                    window_height = template_img_resized.rows;
                    ReleaseDC(hWnd, hdc);
                    InvalidateRect(hWnd, 0, TRUE);
                    bitmap_change_flag = TRUE;
                }
                break;
            case RESIZE_200:
                option.resize = 2.0;
                if (template_img_flag == true) {
                    resize(template_img, template_img_resized, cv::Size(), option.resize, option.resize);
                    hdc = GetDC(hWnd);
                    mat2memHdc(hdc, template_img_resized);
                    SetWindowPos(hWnd, 0, 0, 0, template_img_resized.cols + 30, template_img_resized.rows + 60, 0);
                    window_width = template_img_resized.cols;
                    window_height = template_img_resized.rows;
                    ReleaseDC(hWnd, hdc);
                    InvalidateRect(hWnd, 0, TRUE);
                    bitmap_change_flag = TRUE;
                }
                break;
        case ID_HALFAUTO:
            option.toggleHalfAuto();
            break;
                

        case ID_BMP:
            extension.toggleBmp();
            break;
        case ID_DIV:
            extension.toggleDiv();
            break;
        case ID_PDM:
            extension.togglePdm();
            break;
        case ID_PGM:
            extension.togglePgm();
            break;
        case ID_PPM:
            extension.togglePpm();
            break;
        case ID_PNM:
            extension.togglePnm();
            break;
        case ID_SR:
            extension.toggleSr();
            break;
        case ID_RAS:
            extension.toggleRas();
            break;
        case ID_PNG:
            extension.togglePng();
            break;
        case ID_TIFF:
            extension.toggleTiff();
            break;
        case ID_TIF:
            extension.toggleTif();
            break;
        case ID_JP2:
            extension.toggleJp2();
            break;
        case ID_JPEG:
            extension.toggleJpeg();
            break;
        case ID_JPG:
            extension.toggleJpg();
            break;
        case ID_JPE:
            extension.toggleJpe();
            break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        if (bitmap_change_flag == false) {
            // TODO: HDC を使用する描画コードをここに追加してください...
            hdc = BeginPaint(hWnd, &ps);
            if (template_img_flag) {
                BitBlt(hdc, 0, 0, template_img_resized.cols, template_img_resized.rows, BitmapDC, 0, 0, SRCCOPY);//100＊100を転送
            }
            for (int i = 0; i < card_data.num; i++) {
                if (i == changing_data) {
                    hpen = CreatePen(PS_DASH, 1, RGB(0x00, 0x7f, 0xff));
                    SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    SelectObject(hdc, hpen);
                    Rectangle(hdc, card_data.data[i].start_x * option.resize, card_data.data[i].start_y * option.resize, card_data.data[i].end_x * option.resize, card_data.data[i].end_y * option.resize);
                }
            }
            hpen = CreatePen(PS_DASH, 1, RGB(0x00, 0x7f, 0xff));
            SelectObject(hdc, hpen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, current_data.start_x * option.resize, current_data.start_y * option.resize, current_data.end_x * option.resize, current_data.end_y * option.resize);
            EndPaint(hWnd, &ps);
            return 0;
        }
        if (!template_img_flag) {
            return 0;
        }
        hdc = GetDC(hWnd);
        if (BitmapDC) {
            DeleteDC(BitmapDC);
            DeleteObject(HBitmap);
            BitmapDC = NULL;
        }
        BitmapDC = CreateCompatibleDC(hdc);
        HBitmap = CreateCompatibleBitmap(hdc, template_img_resized.cols, template_img_resized.rows);//ビットマップを100＊100で作成
        SelectObject(BitmapDC, HBitmap);
        ReleaseDC(hWnd, hdc);


        mat2memHdc.BitBlt(BitmapDC, 0, 0, SRCCOPY);
        hpen = CreatePen(PS_DASH, 1, RGB(0x00, 0x7f, 0xff));
        SelectObject(BitmapDC, hpen);
        SelectObject(BitmapDC, GetStockObject(NULL_BRUSH));
        Rectangle(BitmapDC, current_data.start_x * option.resize, current_data.start_y * option.resize, current_data.end_x * option.resize, current_data.end_y * option.resize);
        hpen2 = CreatePen(PS_DASH, 1, RGB(0x7f, 0x00, 0xff));
        SelectObject(BitmapDC, hpen2);
        for (int i = 0; i < card_data.num; i++) {
            if (i == changing_data) {
                hpen = CreatePen(PS_DASH, 1, RGB(0x00, 0x7f, 0xff));
                SelectObject(BitmapDC, hpen);
                Rectangle(BitmapDC, card_data.data[i].start_x * option.resize, card_data.data[i].start_y * option.resize, card_data.data[i].end_x * option.resize, card_data.data[i].end_y * option.resize);
            }
            else {
                hpen2 = CreatePen(PS_DASH, 1, RGB(0x7f, 0x00, 0xff));
                SelectObject(BitmapDC, hpen2);
                Rectangle(BitmapDC, card_data.data[i].start_x * option.resize, card_data.data[i].start_y * option.resize, card_data.data[i].end_x * option.resize, card_data.data[i].end_y * option.resize);

            }
        }
        bitmap_change_flag = false;
        InvalidateRect(hWnd, NULL, 1);
    }
        break;

    case WM_DESTROY:
        if (ocr) {
            ocr->End();
        }
        saveOption();
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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


BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir, const std::wstring Filter, const std::wstring DefExt) {
    OPENFILENAME o;
    fname[0] = _T('\0');
    ZeroMemory(&o, sizeof(o));
    o.lStructSize = sizeof(o);              //      構造体サイズ
    o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
    o.lpstrInitialDir = initDir;    //      初期フォルダー
    o.lpstrFile = fname;                    //      取得したファイル名を保存するバッファ
    o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
    o.lpstrFilter =  Filter.c_str();
    o.lpstrDefExt = DefExt.c_str();
    o.lpstrTitle = _TEXT("");
    o.nFilterIndex = 1;
    return GetOpenFileName(&o);
}
BOOL GetFileNameReadMeta(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir) {
    OPENFILENAME o;
    fname[0] = _T('\0');
    ZeroMemory(&o, sizeof(o));
    o.lStructSize = sizeof(o);              //      構造体サイズ
    o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
    o.lpstrInitialDir = initDir;    //      初期フォルダー
    o.lpstrFile = fname;                    //      取得したファイル名を保存するバッファ
    o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
    o.lpstrFilter =  TEXT("領域情報ファイル(*.med)\0*.med\0\0");
    o.lpstrDefExt = TEXT("med");
    o.lpstrTitle = _TEXT("");
    o.nFilterIndex = 1;
    return GetOpenFileName(&o);
}

BOOL GetFileNameAll(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir) {
    OPENFILENAME o;
    fname[0] = _T('\0');
    ZeroMemory(&o, sizeof(o));
    o.lStructSize = sizeof(o);              //      構造体サイズ
    o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
    o.lpstrInitialDir = initDir;    //      初期フォルダー
    o.lpstrFile = fname;                    //      取得したファイル名を保存するバッファ
    o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
    o.lpstrFilter =   TEXT("全てのファイル(*.*)\0*.*\0");
    o.lpstrDefExt = TEXT("");
    o.lpstrTitle = _TEXT("");
    o.nFilterIndex = 1;
    return GetOpenFileName(&o);
}

BOOL GetFileNameCsv(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir) {
    OPENFILENAME o;
    fname[0] = _T('\0');
    ZeroMemory(&o, sizeof(o));
    o.lStructSize = sizeof(o);              //      構造体サイズ
    o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
    o.lpstrInitialDir = initDir;    //      初期フォルダー
    o.lpstrFile = fname;                    //      取得したファイル名を保存するバッファ
    o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
    o.lpstrFilter =  TEXT("csvファイル(*.csv)\0*.csv\0\0");
    o.lpstrDefExt = TEXT("csv");
    o.lpstrTitle = _TEXT("");
    o.nFilterIndex = 1;
    return GetOpenFileName(&o);
}
std::wstring StringToWString(const std::string& str)
{
    std::wstring wstr;
    size_t size;
    wstr.resize(str.length());
    mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
    return wstr;
}

std::string WStringToString
(
    std::wstring oWString, int mode  
)
{
    // wstring → SJIS
    int iBufferSize;
    if (mode == 0) {
        iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
            , -1, (char*)NULL, 0, NULL, NULL);
    }
    else {
        iBufferSize = WideCharToMultiByte(CP_UTF8, 0, oWString.c_str()
            , -1, (char*)NULL, 0, NULL, NULL);
    }

    // バッファの取得
    char* cpMultiByte = new char[iBufferSize];

    // wstring → SJIS
    if (mode == 0) {
    WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
        , iBufferSize, NULL, NULL);
    }
    else {
    WideCharToMultiByte(CP_UTF8, 0, oWString.c_str(), -1, cpMultiByte
        , iBufferSize, NULL, NULL);
    }

    // stringの生成
    std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    // バッファの破棄
    delete[] cpMultiByte;

    // 変換結果を返す
    return(oRet);
}
//      Mat型をメモリデバイスコンテキストに変換するクラス

LRESULT CALLBACK ChooseSymbolWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    HWND hParent = GetParent(hWnd);
    PAINTSTRUCT ps;
    int i;
    int j;
    HDC hdc;
    WNDCLASSW wc_A;
    TCHAR tmp_data_name[256];
    TCHAR data_name[3];
    HWND cardButton[100];
    switch (msg) {
    case WM_CREATE:
        BringWindowToTop(
            hWnd
        );
        dialog_mode3 = 1;
        for (i = 0; i < 100; i++) {
            if (card_data.data[current_data_num].symbol[i] == "") {
                break;
            }
            std::wstring tmp_wstring = StringToWString(card_data.data[current_data_num].symbol_name[i]);
            wsprintf(tmp_data_name, TEXT("%s"), tmp_wstring.c_str());
            cardButton[i] = CreateWindowEx(0, TEXT("BUTTON"), tmp_data_name,
                WS_CHILD | WS_VISIBLE | BS_FLAT, 0, i * 30, 100, 30,
                hWnd, (HMENU)ID_BUTTON_CHOOSE_SYMBOL + i + 1, hInst, NULL);
        }
        if (i < 99) {
            if (option.language == "jpn") {
                cardButton[i] = CreateWindowEx(0, TEXT("BUTTON"), TEXT("新規"),
                    WS_CHILD | WS_VISIBLE | BS_FLAT, 0, i * 30, 100, 30,
                    hWnd, (HMENU)ID_BUTTON_CHOOSE_SYMBOL + i + 1, hInst, NULL);
            }else if (option.language == "chi_sim") {
                cardButton[i] = CreateWindowEx(0, TEXT("BUTTON"), TEXT("新"),
                    WS_CHILD | WS_VISIBLE | BS_FLAT, 0, i * 30, 100, 30,
                    hWnd, (HMENU)ID_BUTTON_CHOOSE_SYMBOL + i + 1, hInst, NULL);
            }
            else {
                cardButton[i] = CreateWindowEx(0, TEXT("BUTTON"), TEXT("New"),
                    WS_CHILD | WS_VISIBLE | BS_FLAT, 0, i * 30, 100, 30,
                    hWnd, (HMENU)ID_BUTTON_CHOOSE_SYMBOL + i + 1, hInst, NULL);

            }

        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        EndPaint(hWnd, &ps);     // 描画処理を開始します。
        break;
    case WM_COMMAND:
    {
        wp = LOWORD(wp);
        j = (int(wp) - ID_BUTTON_CHOOSE_SYMBOL) / 4 - 1;
        current_data_symbol = j;
        int label = j + 30;
        wsprintf(data_name, TEXT("%d"), label);
        CreateSymbolOptionWindowClass(hInst, data_name, wc_A);
        //           CreateWindowClass(hInst, data_name, wc_A);
        RegisterClassW(&wc_A);
        hChildWnd_F = CreateSymbolOptionWindow(hInst, SW_SHOW, data_name, (HMENU)(200*(j+1)), hParent);
        dialog_mode3 = 0;
        DestroyWindow(hWnd);
    }
        break;
    case WM_CLOSE:
        dialog_mode3 = 0;
        DestroyWindow(hWnd);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}
LRESULT CALLBACK ChooseAreaWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    HWND hParent = GetParent(hWnd);
    PAINTSTRUCT ps;
    int i;
    int j;
    HDC hdc;
    WNDCLASSW wc_A;
    TCHAR tmp_data_name[256];
    TCHAR data_name[3];
    HWND cardButton[CARD_RECT_ARRAY_SIZE];
    switch (msg) {
    case WM_CREATE:
        dialog_mode =1;
        for (i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
            if (card_data.data[i].cur_num == -1) {
                break;
            }
            wsprintf(tmp_data_name, TEXT("%s"), card_data.data[i].data_label);
            cardButton[i] = CreateWindowEx(0, TEXT("BUTTON"), tmp_data_name,
                WS_CHILD | WS_VISIBLE | BS_FLAT, 0, i * 30, 100, 30,
                hWnd, (HMENU)ID_BUTTON + i + 1, hInst, NULL);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        EndPaint(hWnd, &ps);     // 描画処理を開始します。
        break;
    case WM_COMMAND:
        wp = LOWORD(wp);
        j = (int(wp) - ID_BUTTON)/4-1;
        changing_data = j;
        dialog_mode = 1;
        wsprintf(data_name, TEXT("%d"), j);
       
        CreateWindowClass(hInst, data_name, wc_A);
        RegisterClassW(&wc_A);
        hChildWnd_A = CreateChildWindow(hInst, SW_SHOW, data_name, (HMENU)(100 * (j - 1)), hParent);
        DestroyWindow(hWnd);
        break;
    case WM_CLOSE:
        dialog_mode = 0;
        DestroyWindow(hWnd);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}
LRESULT CALLBACK WriteOptionWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    HWND hParent = GetParent(hWnd);
    CHOOSECOLOR cc;
    TCHAR writing_tmp[256];
    PAINTSTRUCT ps;
    HWND hButton_font;
    HWND hButton_save;
    HWND hButton_color;
    HDC hdc;
    static COLORREF bkColor;
    TCHAR A[16];
    LPCTSTR lptStr1 = TEXT("幅マージン");
    LPCTSTR lptStr2 = TEXT("縦マージン");
    LPCTSTR lptStr3 = TEXT("水平位置");
    LPCTSTR lptStr4 = TEXT("垂直位置");
    LPCTSTR lptStr5 = TEXT("フォントの高さ");
    LPCTSTR lptStr6 = TEXT("図形");
    LPCTSTR lptStr7 = TEXT("透明度");
    LPCTSTR lptStr11 = TEXT("外枠幅");
    LPCTSTR lptStr8 = TEXT("シンボルマージン");
    LPCTSTR lptStr9 = TEXT("フォントカラー");
    LPCTSTR lptStr10 = TEXT("フォント幅");
    LPCTSTR lptStr_c1 = TEXT("页边的空白x");
    LPCTSTR lptStr_c2 = TEXT("页边的空白y");
    LPCTSTR lptStr_c3 = TEXT("水平位");
    LPCTSTR lptStr_c4 = TEXT("垂直位");
    LPCTSTR lptStr_c5 = TEXT("字体高度");
    LPCTSTR lptStr_c6 = TEXT("图形");
    LPCTSTR lptStr_c7 = TEXT("透明度");
    LPCTSTR lptStr_c11 = TEXT("外框");
    LPCTSTR lptStr_c8 = TEXT("符号页边的空白");
    LPCTSTR lptStr_c9 = TEXT("符号的颜色");
    LPCTSTR lptStr_c10 = TEXT("文字宽度");
    LPCTSTR lptStr_e1 = TEXT("margin x");
    LPCTSTR lptStr_e2 = TEXT("margin y");
    LPCTSTR lptStr_e3 = TEXT("horizontal alignment");
    LPCTSTR lptStr_e4 = TEXT("vertical alignment");
    LPCTSTR lptStr_e5 = TEXT("font height");
    LPCTSTR lptStr_e6 = TEXT("figure");
    LPCTSTR lptStr_e7 = TEXT("alpha");
    LPCTSTR lptStr_e11 = TEXT("outer_width");
    LPCTSTR lptStr_e8 = TEXT("margin symbol");
    LPCTSTR lptStr_e9 = TEXT("font color");
    LPCTSTR lptStr_e10 = TEXT("font width");
    WCHAR lpClassName[10] ;
    HWND filedit;
    HWND hButton;
    HWND hButton_outercolor;
    TCHAR buffer[256];
    static int data_num;
    HWND word_list ;
    static HWND hEdit[20];
    LPTSTR resultText;
    TCHAR tmp_data_name[256];
    static CardRect cur_data;
    int x, y;
    int k;
    TCHAR TextAlignPosition[3][10] =
    {
       TEXT("左寄り"),TEXT("中央"),TEXT("右寄り")
    };
    TCHAR TextAlignPosition_E[3][10] =
    {
       TEXT("Left"),TEXT("Center"),TEXT("Right")
    };
    TCHAR TextAlignPosition_C[3][10] =
    {
       TEXT("左"),TEXT("中央"),TEXT("右")
    };
    TCHAR VerticalAlignPosition[3][10] =
    {
       TEXT("上"),TEXT("中央"),TEXT("下")
    };
    TCHAR VerticalAlignPosition_C[3][10] =
    {
       TEXT("上"),TEXT("中央"),TEXT("下")
    };
    TCHAR VerticalAlignPosition_E[3][10] =
    {
       TEXT("Top"),TEXT("Center"),TEXT("Bottom")
    };
    TCHAR FIGURE[2][10] =
    {
       TEXT("長方形"),TEXT("楕円")
    };
    TCHAR FIGURE_C[2][10] =
    {
       TEXT("矩形"),TEXT("椭圆")
    };
    TCHAR FIGURE_E[2][10] =
    {
       TEXT("Rectangle"),TEXT("Ellipse")
    };
    switch (msg) {
    case WM_CLOSE:
        dialog_mode2 = 0;
        DestroyWindow(hWnd);
        return true;
        break;
    case WM_LBUTTONDOWN:
        x = GET_X_LPARAM(lp);
        y = GET_Y_LPARAM(lp);
        if (x >= 0 && x <= 180 && y >= 00 && y <= 30) {
            SetFocus(hEdit[0]);
        }
        if (x >= 0 && x <= 180 && y >= 30 && y <= 60) {
            SetFocus(hEdit[1]);
        }
        if (x >= 0 && x <= 180 && y >= 150 && y <= 180) {
            SetFocus(hEdit[2]);
        }
        if (x >= 0 && x <= 180 && y >= 180 && y <= 210) {
            SetFocus(hEdit[3]);
        }
        if (x >= 0 && x <= 50 && y >= 210 && y <= 240) {
            SetFocus(hEdit[4]);
        }
        if (x >= 50 && x <= 100 && y >= 210 && y <= 240) {
            SetFocus(hEdit[10]);
        }
        if (x >= 0 && x <= 180 && y >= 240 && y <= 270) {
            SetFocus(hEdit[11]);
        }
        if (x >= 0 && x <= 180 && y >= 270 && y <= 300) {
            SetFocus(hEdit[9]);
        }
        if (x >= 0 && x <= 180 && y >= 300 && y <= 330) {
            SetFocus(hEdit[5]);
        }
        if (x >= 0 && x <= 180 && y >= 390 && y <= 420) {
            SetFocus(hEdit[8]);
        }
        // Msg was handled, return zero.
        // Msg was handled, return zero.
        break;
    case WM_CREATE:
        BringWindowToTop(
            hWnd
        );
        GetClassName(
            hWnd,
            lpClassName,
            2
        );
        data_num = current_data_num;
        cur_data = card_data.data[data_num];
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.margin_x);
        hEdit[0] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 0, 100, 30, hWnd, (HMENU)110,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.margin_y);
        hEdit[1] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110,30, 100, 30, hWnd, (HMENU)111,
            hInst, NULL
        );
        hWndListBox3 = CreateWindow(WC_LISTBOX, TEXT(""),
            WS_CHILD | WS_VISIBLE | LBS_STANDARD,
            110, 60, 180, 30, hWnd, (HMENU)198, hInst,
            NULL);
        memset(&A, 0, sizeof(A));
        for (k = 0; k <= 2; k ++)
        {
            if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)TextAlignPosition[k]);
            }else if (option.language == "chi_sim") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)TextAlignPosition_C[k]);
            }
            else {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)TextAlignPosition_E[k]);

            }

            // Add string to combobox.
            SendMessage(hWndListBox3, (UINT)LB_INSERTSTRING, (WPARAM)k, (LPARAM)A);
        }
            SendMessage(hWndListBox3, LB_SETCURSEL, cur_data.horizontal_align, 0);
        hWndListBox4 = CreateWindow(WC_LISTBOX, TEXT(""),
            WS_CHILD | WS_VISIBLE | LBS_STANDARD,
            110, 90, 180, 30, hWnd, (HMENU)197, hInst,
            NULL);
        memset(&A, 0, sizeof(A));
        for (k = 0; k <= 2; k ++)
        {
            if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)VerticalAlignPosition[k]);
            }else if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)VerticalAlignPosition_C[k]);
            }
            else {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)VerticalAlignPosition_E[k]);

            }
            // Add string to combobox.
            SendMessage(hWndListBox4, (UINT)LB_INSERTSTRING, (WPARAM)k, (LPARAM)A);
        }
            SendMessage(hWndListBox4, LB_SETCURSEL, cur_data.vertical_align, 0);
        hWndListBox7 = CreateWindow(WC_LISTBOX, TEXT(""),
            WS_CHILD | WS_VISIBLE | LBS_STANDARD,
            110, 120, 180, 30, hWnd, (HMENU)199, hInst,
            NULL);
        memset(&A, 0, sizeof(A));
        for (k = 0; k <= 1; k ++)
        {
            if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)FIGURE[k]);
            }else if (option.language == "chi_sim") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)FIGURE_C[k]);
            }
            else {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)FIGURE_E[k]);

            }

            // Add string to combobox.
            SendMessage(hWndListBox7, (UINT)LB_INSERTSTRING, (WPARAM)k, (LPARAM)A);
        }
            SendMessage(hWndListBox7, LB_SETCURSEL, cur_data.figure, 0);
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.font_height);
        hEdit[2] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110,150, 100, 30, hWnd, (HMENU)112,
            hInst, NULL
        );
        if (option.language == "jpn") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("背景色"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 180, 70, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_COLOR_CHOOSE, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("背景色"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 180, 70, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_COLOR_CHOOSE, hInst, NULL);
        }
        else {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Background Color"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 180, 70, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_COLOR_CHOOSE, hInst, NULL);
        }
        wsprintf(tmp_data_name, TEXT("%d,%d,%d"), (int)cur_data.bkColor[0], (int)cur_data.bkColor[1], (int)cur_data.bkColor[2]);
        hEdit[3] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 180, 100, 30, hWnd, (HMENU)117,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), (int)cur_data.opaque);
        hEdit[4] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 210, 50, 30, hWnd, (HMENU)118,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), (int)cur_data.outer_opaque);
        hEdit[10] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            160, 210, 50, 30, hWnd, (HMENU)124,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), (int)cur_data.outer_width);
        hEdit[11] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 240, 100, 30, hWnd, (HMENU)125,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d,%d,%d"), (int)cur_data.outerColor[0], (int)cur_data.outerColor[1], (int)cur_data.outerColor[2]);
        hEdit[9] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 270, 100, 30, hWnd, (HMENU)123,
            hInst, NULL
        );
        if (option.language == "jpn") {
            hButton_outercolor = CreateWindowEx(0, TEXT("BUTTON"), TEXT("外枠"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 270, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_OUTER_COLOR_CHOOSE, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton_outercolor = CreateWindowEx(0, TEXT("BUTTON"), TEXT("外框"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 270, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_OUTER_COLOR_CHOOSE, hInst, NULL);
        }
        else {
            hButton_outercolor = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Outer Color"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 270, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_OUTER_COLOR_CHOOSE, hInst, NULL);
        }
        wsprintf(tmp_data_name, TEXT("%d"), (int)cur_data.margin_symbol);
        hEdit[5] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 300, 100, 30, hWnd, (HMENU)119,
            hInst, NULL
        );
        if (option.language == "jpn") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("文字色"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 330, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_FONT_COLOR, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("文字色"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 330, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_FONT_COLOR, hInst, NULL);
        }
        else {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Font Color"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 330, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_FONT_COLOR, hInst, NULL);
        }
        wsprintf(tmp_data_name, TEXT("%d,%d,%d"), (int)cur_data.fontColor[0], (int)cur_data.fontColor[1], (int)cur_data.fontColor[2]);
        hEdit[6] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 330, 100, 30, hWnd, (HMENU)120,
            hInst, NULL
        );
        if (option.language == "jpn") {
            hButton_font = CreateWindowEx(0, TEXT("BUTTON"), TEXT("フォント"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 390, 100, 30,
                hWnd, (HMENU)ID_BUTTON_FONT, hInst, NULL);
            hButton_save = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 390, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_OPTION, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton_font = CreateWindowEx(0, TEXT("BUTTON"), TEXT("字体"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 390, 100, 30,
                hWnd, (HMENU)ID_BUTTON_FONT, hInst, NULL);
            hButton_save = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80,390, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_OPTION, hInst, NULL);
        }
        else {
            hButton_font = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Font"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 390, 100, 30,
                hWnd, (HMENU)ID_BUTTON_FONT, hInst, NULL);
            hButton_save = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Save"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 390, 100, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_OPTION, hInst, NULL);
        }
        if (option.language == "jpn") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("文字枠色"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 360, 100, 30,
                hWnd, (HMENU)ID_BUTTON_OUT_COLOR_CHOOSE, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("外色"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 360, 100, 30,
                hWnd, (HMENU)ID_BUTTON_OUT_COLOR_CHOOSE, hInst, NULL);
        }
        else {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Out Color"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 360, 100, 30,
                hWnd, (HMENU)ID_BUTTON_OUT_COLOR_CHOOSE, hInst, NULL);
        }
        wsprintf(tmp_data_name, TEXT("%d,%d,%d"), (int)cur_data.outColor[0], (int)cur_data.outColor[1], (int)cur_data.outColor[2]);
        hEdit[7] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 360, 100, 30, hWnd, (HMENU)121,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), (int)cur_data.font_width);
        hEdit[8] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 390, 100, 30, hWnd, (HMENU)122,
            hInst, NULL
        );
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr1, lstrlen(lptStr1));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
            TextOutW(hdc, 0, 90, lptStr4, lstrlen(lptStr4));
            TextOutW(hdc, 0, 120, lptStr6, lstrlen(lptStr6));
            TextOutW(hdc, 0, 150, lptStr5, lstrlen(lptStr5));
            TextOutW(hdc, 0, 210, lptStr7, lstrlen(lptStr7));
            TextOutW(hdc, 0, 240, lptStr11, lstrlen(lptStr11));
            TextOutW(hdc, 0, 300, lptStr8, lstrlen(lptStr8));
            TextOutW(hdc, 0, 330, lptStr9, lstrlen(lptStr9));
            TextOutW(hdc, 0, 390, lptStr10, lstrlen(lptStr10));
        }else if (option.language == "chi_sim") {
            TextOutW(hdc, 0, 0, lptStr_c1, lstrlen(lptStr_c1));
            TextOutW(hdc, 0, 30, lptStr_c2, lstrlen(lptStr_c2));
            TextOutW(hdc, 0, 60, lptStr_c3, lstrlen(lptStr_c3));
            TextOutW(hdc, 0, 90, lptStr_c4, lstrlen(lptStr_c4));
            TextOutW(hdc, 0, 120, lptStr_c6, lstrlen(lptStr_c6));
            TextOutW(hdc, 0, 150, lptStr_c5, lstrlen(lptStr_c5));
            TextOutW(hdc, 0, 210, lptStr_c7, lstrlen(lptStr_c7));
            TextOutW(hdc, 0, 240, lptStr_c11, lstrlen(lptStr_c11));
            TextOutW(hdc, 0, 300, lptStr_c8, lstrlen(lptStr_c8));
            TextOutW(hdc, 0, 330, lptStr_c9, lstrlen(lptStr_c9));
            TextOutW(hdc, 0, 390, lptStr_c10, lstrlen(lptStr_c10));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr_e1, lstrlen(lptStr_e1));
            TextOutW(hdc, 0, 30, lptStr_e2, lstrlen(lptStr_e2));
            TextOutW(hdc, 0, 60, lptStr_e3, lstrlen(lptStr_e3));
            TextOutW(hdc, 0, 90, lptStr_e4, lstrlen(lptStr_e4));
            TextOutW(hdc, 0, 120, lptStr_e6, lstrlen(lptStr_e6));
            TextOutW(hdc, 0, 150, lptStr_e5, lstrlen(lptStr_e5));
            TextOutW(hdc, 0, 210, lptStr_e7, lstrlen(lptStr_e7));
            TextOutW(hdc, 0, 240, lptStr_e11, lstrlen(lptStr_e11));
            TextOutW(hdc, 0, 300, lptStr_e8, lstrlen(lptStr_e8));
            TextOutW(hdc, 0, 330, lptStr_e9, lstrlen(lptStr_e9));
            TextOutW(hdc, 0, 390, lptStr_e10, lstrlen(lptStr_e10));
        }
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。

        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr1, lstrlen(lptStr1));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
            TextOutW(hdc, 0, 90, lptStr4, lstrlen(lptStr4));
            TextOutW(hdc, 0, 120, lptStr6, lstrlen(lptStr6));
            TextOutW(hdc, 0, 150, lptStr5, lstrlen(lptStr5));
            TextOutW(hdc, 0, 210, lptStr7, lstrlen(lptStr7));
            TextOutW(hdc, 0, 240, lptStr11, lstrlen(lptStr11));
            TextOutW(hdc, 0, 270, lptStr8, lstrlen(lptStr8));
            TextOutW(hdc, 0, 300, lptStr9, lstrlen(lptStr9));
            TextOutW(hdc, 0, 360, lptStr10, lstrlen(lptStr10));
        }
        else if(option.language == "chi_sim") {
            TextOutW(hdc, 0, 0, lptStr_c1, lstrlen(lptStr_c1));
            TextOutW(hdc, 0, 30, lptStr_c2, lstrlen(lptStr_c2));
            TextOutW(hdc, 0, 60, lptStr_c3, lstrlen(lptStr_c3));
            TextOutW(hdc, 0, 90, lptStr_c4, lstrlen(lptStr_c4));
            TextOutW(hdc, 0, 120, lptStr_c6, lstrlen(lptStr_c6));
            TextOutW(hdc, 0, 150, lptStr_c5, lstrlen(lptStr_c5));
            TextOutW(hdc, 0, 210, lptStr_c7, lstrlen(lptStr_c7));
            TextOutW(hdc, 0, 240, lptStr_c11, lstrlen(lptStr_c11));
            TextOutW(hdc, 0, 270, lptStr_c8, lstrlen(lptStr_c8));
            TextOutW(hdc, 0, 300, lptStr_c9, lstrlen(lptStr_c9));
            TextOutW(hdc, 0, 360, lptStr_c10, lstrlen(lptStr_c10));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr_e1, lstrlen(lptStr_e1));
            TextOutW(hdc, 0, 30, lptStr_e2, lstrlen(lptStr_e2));
            TextOutW(hdc, 0, 60, lptStr_e3, lstrlen(lptStr_e3));
            TextOutW(hdc, 0, 90, lptStr_e4, lstrlen(lptStr_e4));
            TextOutW(hdc, 0, 120, lptStr_e6, lstrlen(lptStr_e6));
            TextOutW(hdc, 0, 150, lptStr_e5, lstrlen(lptStr_e5));
            TextOutW(hdc, 0, 210, lptStr_e7, lstrlen(lptStr_e7));
            TextOutW(hdc, 0, 240, lptStr_e11, lstrlen(lptStr_e11));
            TextOutW(hdc, 0, 270, lptStr_e8, lstrlen(lptStr_e8));
            TextOutW(hdc, 0, 300, lptStr_e9, lstrlen(lptStr_e9));
            TextOutW(hdc, 0, 360, lptStr_e10, lstrlen(lptStr_e10));
        }


        EndPaint(hWnd, &ps);

        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON_WRITE_FONT_COLOR:
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hWnd;
            cc.rgbResult = 0;
            cc.lpCustColors = &bkColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&cc)) {
                std::wstring tmp_color_string;
                bkColor = cc.rgbResult;
                tmp_color_string = std::to_wstring(GetRValue(cc.rgbResult)) + TEXT(",")+ std::to_wstring(GetGValue(cc.rgbResult))+ TEXT(",")+std::to_wstring(GetBValue(cc.rgbResult));
                card_data.data[data_num].fontColor[0] = GetRValue(cc.rgbResult);
                card_data.data[data_num].fontColor[1] = GetGValue(cc.rgbResult);
                card_data.data[data_num].fontColor[2] = GetBValue(cc.rgbResult);
                filedit = GetDlgItem(hWnd, 120);
                SetWindowText(filedit, tmp_color_string.c_str());

            }
            break;
        case ID_BUTTON_WRITE_OUTER_COLOR_CHOOSE:
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hWnd;
            cc.rgbResult = 0;
            cc.lpCustColors = &bkColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&cc)) {
                std::wstring tmp_color_string;
                bkColor = cc.rgbResult;
                tmp_color_string = std::to_wstring(GetRValue(cc.rgbResult)) + TEXT(",")+ std::to_wstring(GetGValue(cc.rgbResult))+ TEXT(",")+std::to_wstring(GetBValue(cc.rgbResult));
                card_data.data[data_num].outerColor[0] = GetRValue(cc.rgbResult);
                card_data.data[data_num].outerColor[1] = GetGValue(cc.rgbResult);
                card_data.data[data_num].outerColor[2] = GetBValue(cc.rgbResult);
                filedit = GetDlgItem(hWnd, 123);
                SetWindowText(filedit, tmp_color_string.c_str());

            }
            break;
        case ID_BUTTON_OUT_COLOR_CHOOSE:
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hWnd;
            cc.rgbResult = 0;
            cc.lpCustColors = &bkColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&cc)) {
                std::wstring tmp_color_string;
                bkColor = cc.rgbResult;
                tmp_color_string = std::to_wstring(GetRValue(cc.rgbResult)) + TEXT(",")+ std::to_wstring(GetGValue(cc.rgbResult))+ TEXT(",")+std::to_wstring(GetBValue(cc.rgbResult));
                card_data.data[data_num].outColor[0] = GetRValue(cc.rgbResult);
                card_data.data[data_num].outColor[1] = GetGValue(cc.rgbResult);
                card_data.data[data_num].outColor[2] = GetBValue(cc.rgbResult);
                filedit = GetDlgItem(hWnd, 121);
                SetWindowText(filedit, tmp_color_string.c_str());

            }
        case ID_BUTTON_WRITE_COLOR_CHOOSE:
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hWnd;
            cc.rgbResult = 0;
            cc.lpCustColors = &bkColor;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&cc)) {
                std::wstring tmp_color_string;
                bkColor = cc.rgbResult;
                tmp_color_string = std::to_wstring(GetRValue(cc.rgbResult)) + TEXT(",")+ std::to_wstring(GetGValue(cc.rgbResult))+ TEXT(",")+std::to_wstring(GetBValue(cc.rgbResult));
                card_data.data[data_num].bkColor[0] = GetRValue(cc.rgbResult);
                card_data.data[data_num].bkColor[1] = GetGValue(cc.rgbResult);
                card_data.data[data_num].bkColor[2] = GetBValue(cc.rgbResult);
                filedit = GetDlgItem(hWnd, 117);
                SetWindowText(filedit, tmp_color_string.c_str());

            }
            break;
        case ID_BUTTON_WRITE_OPTION:
	        filedit = GetDlgItem(hWnd, 110);
	        GetWindowText(filedit, buffer, 256);
        	card_data.data[data_num].margin_x = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 111);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].margin_y = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 112);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].font_height = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 118);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].opaque = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 124);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].outer_opaque = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 125);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].outer_width = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 119);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].margin_symbol = _ttoi(buffer);
	        filedit = GetDlgItem(hWnd, 122);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].font_width = _ttoi(buffer);
	        card_data.data[data_num].horizontal_align = SendMessage(hWndListBox3, LB_GETCURSEL, 0, 0);
        	card_data.data[data_num].vertical_align = SendMessage(hWndListBox4, LB_GETCURSEL, 0, 0);
        	card_data.data[data_num].figure = SendMessage(hWndListBox7, LB_GETCURSEL, 0, 0);
            dialog_mode2 = 0;
            DestroyWindow(hWnd);
            break;
        case ID_BUTTON_FONT:
            card_data.data[data_num].cf.hwndOwner = NULL;
            if (!ChooseFont(&card_data.data[data_num].cf)) return 0;
            card_data.data[data_num].cf.Flags |= CF_INITTOLOGFONTSTRUCT;
            cur_data = card_data.data[data_num];
            return 0;
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY :
        //PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}
LRESULT CALLBACK SymbolOptionWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    CHOOSECOLOR cc;
    HWND hParent = GetParent(hWnd);
    PAINTSTRUCT ps;
    HDC hdc;
    COLORREF bkColor;
    TCHAR A[16];
    LPCTSTR lptStr1 = TEXT("ファイル");
    LPCTSTR lptStr2 = TEXT("シンボル");
    LPCTSTR lptStr3 = TEXT("背景色");
    LPCTSTR lptStr4 = TEXT("閾値");
    LPCTSTR lptStr1_c = TEXT("文件");
    LPCTSTR lptStr2_c = TEXT("符号");
    LPCTSTR lptStr3_c = TEXT("背景色");
    LPCTSTR lptStr4_c = TEXT("界限");
    LPCTSTR lptStr1_e = TEXT("File Name");
    LPCTSTR lptStr2_e = TEXT("Symbol");
    LPCTSTR lptStr3_e = TEXT("Background");
    LPCTSTR lptStr4_e = TEXT("Threshold");
    WCHAR lpClassName[10] ;
    static COLORREF local_none_color;
    HWND filedit;
    HWND hButton;
    HWND hButton_color;
    TCHAR buffer[256];
    static int data_num;
    static CardRect cur_data;
    static HWND hEdit[10];
    LPTSTR resultText;
    TCHAR tmp_data_name[256];
    int x, y;
    int k;
    size_t size;        // 文字列のサイズを格納する
    switch (msg) {
    case WM_CLOSE:
        dialog_mode2 = 0;
        DestroyWindow(hWnd);
        return true;
        break;
    case WM_LBUTTONDOWN:
        x = GET_X_LPARAM(lp);
        y = GET_Y_LPARAM(lp);
        if (x >= 0 && x <= 180 && y >= 00 && y <= 30) {
            SetFocus(hEdit[0]);
        }
        if (x >= 0 && x <= 180 && y >= 30 && y <= 60) {
            SetFocus(hEdit[1]);
        }
        if (x >= 0 && x <= 180 && y >= 60 && y <= 90) {
            SetFocus(hEdit[2]);
        }
        if (x >= 0 && x <= 180 && y >= 90 && y <= 120) {
            SetFocus(hEdit[3]);
        }
        // Msg was handled, return zero.
        // Msg was handled, return zero.
        break;
    case WM_CREATE:
    {
        BringWindowToTop(
            hWnd
        );
        GetClassName(
            hWnd,
            lpClassName,
            2
        );

        data_num = current_data_num;
        if (data_num < 0) {
            data_num = 0;
        }
        cur_data = card_data.data[data_num];



        std::wstring tmp_string = StringToWString(cur_data.symbol[current_data_symbol]);
        wsprintf(tmp_data_name, TEXT("%s"), tmp_string.c_str());
        hEdit[0] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            100, 0, 400, 30, hWnd, (HMENU)113,
            hInst, NULL
        );
        tmp_string = StringToWString(cur_data.symbol_name[current_data_symbol]);
        wsprintf(tmp_data_name, TEXT("%s"), tmp_string.c_str());
        hEdit[1] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            100, 30, 100, 30, hWnd, (HMENU)114,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d,%d,%d"), (int)cur_data.none_color[current_data_symbol][0], (int)cur_data.none_color[current_data_symbol][1], (int)cur_data.none_color[current_data_symbol][2]);
        hEdit[2] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            100, 60, 100, 30, hWnd, (HMENU)115,
            hInst, NULL
        );
        local_none_color = SetRValue(cur_data.none_color[current_data_symbol][0]) + SetGValue(cur_data.none_color[current_data_symbol][1]) + SetBValue(cur_data.none_color[current_data_symbol][2]);
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.pixel_diff[current_data_symbol]);
        hEdit[3] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            100, 90, 100, 30, hWnd, (HMENU)116,
            hInst, NULL
        );
        if (option.language == "jpn") {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 100, 200, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_SAVE, hInst, NULL);
        }
        else if (option.language == "chi_sim") {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 100, 200, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_SAVE, hInst, NULL);
        }
        else {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Save"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 100, 200, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_SAVE, hInst, NULL);
        }
        if (option.language == "jpn") {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("ファイル"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 510, 0, 100, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_FILE, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("字体"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 510, 0, 100, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_FILE, hInst, NULL);
        }
        else {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("File Name"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 510, 0, 100, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_FILE, hInst, NULL);
        }
        if (option.language == "jpn") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("色選択"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 210, 60, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_COLOR_CHOOSE, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("颜色选择"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 210, 60, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_COLOR_CHOOSE, hInst, NULL);
        }
        else {
            hButton_color = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Choose Color"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 210, 60, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_COLOR_CHOOSE, hInst, NULL);
        }
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr1, lstrlen(lptStr1));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
            TextOutW(hdc, 0, 80, lptStr4, lstrlen(lptStr4));
        }
        else if(option.language == "chi_sim"){
            TextOutW(hdc, 0, 0, lptStr1_c, lstrlen(lptStr1_c));
            TextOutW(hdc, 0, 30, lptStr2_c, lstrlen(lptStr2_c));
            TextOutW(hdc, 0, 60, lptStr3_c, lstrlen(lptStr3_c));
            TextOutW(hdc, 0, 90, lptStr4_c, lstrlen(lptStr4_c));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr1_e, lstrlen(lptStr1_e));
            TextOutW(hdc, 0, 30, lptStr2_e, lstrlen(lptStr2_e));
            TextOutW(hdc, 0, 60, lptStr3_e, lstrlen(lptStr3_e));
            TextOutW(hdc, 0, 90, lptStr4_e, lstrlen(lptStr4_e));
        }
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);
        InvalidateRect(hWnd, 0, TRUE);
        break;
    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr1, lstrlen(lptStr1));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
            TextOutW(hdc, 0, 90, lptStr4, lstrlen(lptStr4));
        } else if(option.language == "chi_sim"){
            TextOutW(hdc, 0, 0, lptStr1_c, lstrlen(lptStr1_c));
            TextOutW(hdc, 0, 30, lptStr2_c, lstrlen(lptStr2_c));
            TextOutW(hdc, 0, 60, lptStr3_c, lstrlen(lptStr3_c));
            TextOutW(hdc, 0, 90, lptStr4_c, lstrlen(lptStr4_c));
        }

        else {
            TextOutW(hdc, 0, 0, lptStr1_e, lstrlen(lptStr1_e));
            TextOutW(hdc, 0, 30, lptStr2_e, lstrlen(lptStr2_e));
            TextOutW(hdc, 0, 60, lptStr3_e, lstrlen(lptStr3_e));
            TextOutW(hdc, 0, 90, lptStr4_e, lstrlen(lptStr4_e));
        }



        EndPaint(hWnd, &ps);

        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON_SYMBOL_COLOR_CHOOSE:
            cc.lStructSize = sizeof(CHOOSECOLOR);
            cc.hwndOwner = hWnd;
            cc.rgbResult = 0;
            cc.lpCustColors = &local_none_color;
            cc.Flags = CC_FULLOPEN | CC_RGBINIT;
            if (ChooseColor(&cc)) {
                std::wstring tmp_color_string;
                local_none_color = cc.rgbResult;
                tmp_color_string = std::to_wstring(GetRValue(cc.rgbResult)) + TEXT(",")+ std::to_wstring(GetGValue(cc.rgbResult))+ TEXT(",")+std::to_wstring(GetBValue(cc.rgbResult));
                filedit = GetDlgItem(hWnd, 115);
                SetWindowText(filedit, tmp_color_string.c_str());

            }
            break;
        case ID_BUTTON_SYMBOL_SAVE:
        {
            filedit = GetDlgItem(hWnd, 113);
            GetWindowText(filedit, buffer, 256);
            card_data.data[data_num].symbol[current_data_symbol] = WStringToString(buffer);
            filedit = GetDlgItem(hWnd, 114);
            GetWindowText(filedit, buffer, 256);
            card_data.data[data_num].symbol_name[current_data_symbol] = WStringToString(buffer);
            filedit = GetDlgItem(hWnd, 115);
            GetWindowText(filedit, buffer, 256);
            std::string s = WStringToString(buffer);
            std::stringstream ss{ s };
            std::string buf;
            int i = 0;
            card_data.data[data_num].none_color[current_data_symbol][0] = 0;
            card_data.data[data_num].none_color[current_data_symbol][1] = 0;
            card_data.data[data_num].none_color[current_data_symbol][2] = 0;
            while (std::getline(ss, buf, ',')) {
                card_data.data[data_num].none_color[current_data_symbol][i] = atoi(buf.c_str());
                i++;
            }
            filedit = GetDlgItem(hWnd, 116);
            GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].pixel_diff[current_data_symbol] = _ttoi(buffer);
            DestroyWindow(hWnd);
            break;
        }
        case ID_BUTTON_SYMBOL_FILE:
        {
            TCHAR szFile[MAX_PATH];
            std::wstring wfile_name;
            std::wstring tmp_file_name;
            tmp_file_name = StringToWString(card_data.data[data_num].symbol[current_data_symbol]);
            if (GetFileNameAll(0, szFile, sizeof(szFile) / sizeof(TCHAR), TEXT(""))) {
                wfile_name = szFile;
                std::string file_name = WStringToString(wfile_name, 0);
                card_data.data[data_num].symbol[current_data_symbol] = file_name;
                filedit = GetDlgItem(hWnd, 113);
                SetWindowText(filedit, wfile_name.c_str());
            }
        }
        default:
            break;
        }
        break;
    case WM_DESTROY :
        //PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}
LRESULT CALLBACK ReadOptionWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    HWND hParent = GetParent(hWnd);
    PAINTSTRUCT ps;
    HWND hButton_test;
    HDC hdc;
    COLORREF bkColor;
    TCHAR A[16];
    LPCTSTR lptStr1 = TEXT("グレースケール");
    LPCTSTR lptStr2 = TEXT("しきい値");
    LPCTSTR lptStr3 = TEXT("反転");
    LPCTSTR lptStr_c1 = TEXT("灰度尺");
    LPCTSTR lptStr_c2 = TEXT("界限");
    LPCTSTR lptStr_c3 = TEXT("反转");
    LPCTSTR lptStr_e1 = TEXT("Gray scale");
    LPCTSTR lptStr_e2 = TEXT("Thores hold");
    LPCTSTR lptStr_e3 = TEXT("Reverse");
    WCHAR lpClassName[10] ;
    HWND filedit;
    HWND hButton;
    TCHAR buffer[256];
    static int data_num;
    HWND word_list ;
    static HWND hEdit[10];
    LPTSTR resultText;
    TCHAR tmp_data_name[256];
    static CardRect cur_data;
    int x, y;
    int k;
    TCHAR GrayKind[2][10] =
    {
       TEXT("利用しない"),TEXT("利用する")
    };
    TCHAR GrayKind_C[2][10] =
    {
       TEXT("不应用"),TEXT("应用")
    };
    TCHAR GrayKind_E[2][10] =
    {
       TEXT("No use"),TEXT("Use")
    };
    TCHAR ReverseBlackAndWhite[2][20] =
    {
       TEXT("反転しない"),TEXT("反転する")
    };
    TCHAR ReverseBlackAndWhite_C[2][20] =
    {
       TEXT("不反转"),TEXT("反转")
    };
    TCHAR ReverseBlackAndWhite_E[2][10] =
    {
       TEXT("NO use"),TEXT("Use")
    };
    size_t size;        // 文字列のサイズを格納する
    switch (msg) {
    case WM_CLOSE:
        dialog_mode2 = 0;
        DestroyWindow(hWnd);
        return true;
        break;
    case WM_LBUTTONDOWN:
        x = GET_X_LPARAM(lp);
        y = GET_Y_LPARAM(lp);
        if (x >= 0 && x <= 180 && y >= 30 && y <= 60) {
            SetFocus(hEdit[0]);
        }
        // Msg was handled, return zero.
        // Msg was handled, return zero.
        break;
    case WM_CREATE:
        BringWindowToTop(
            hWnd
        );
        GetClassName(
            hWnd,
            lpClassName,
            2
        );
        data_num = current_data_num ;
        if (data_num < 0) {
            data_num = 0;
        }
        cur_data = card_data.data[data_num];
        hWndListBox2 = CreateWindow(WC_LISTBOX, TEXT(""),
            WS_CHILD | WS_VISIBLE | LBS_STANDARD,
            110, 0, 100, 30, hWnd, (HMENU)99, hInst,
            NULL);


        memset(&A, 0, sizeof(A));
        for (k = 0; k <= 1; k += 1)
        {
            if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)GrayKind[k]);
            } else if(option.language == "chi_sim"){
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)GrayKind_C[k]);
            }
            else {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)GrayKind_E[k]);
            }

            // Add string to combobox.
            SendMessage(hWndListBox2, (UINT)LB_INSERTSTRING, (WPARAM)k, (LPARAM)A);
        }
        SendMessage(hWndListBox2, LB_SETCURSEL, cur_data.gray, 0);


        hWndListBox5 = CreateWindow(WC_LISTBOX, TEXT(""),
            WS_CHILD | WS_VISIBLE | LBS_STANDARD,
            110, 60, 100, 90, hWnd, (HMENU)196, hInst,
            NULL);
        memset(&A, 0, sizeof(A));
        for (k = 0; k <= 1; k ++)
        {
            if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)ReverseBlackAndWhite[k]);
            }
            else if (option.language == "chi_sim") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)ReverseBlackAndWhite_C[k]);
            }
            else {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)ReverseBlackAndWhite_E[k]);

            }


            // Add string to combobox.
            SendMessage(hWndListBox5, (UINT)LB_INSERTSTRING, (WPARAM)k, (LPARAM)A);
        }
            SendMessage(hWndListBox5, LB_SETCURSEL, cur_data.reverse, 0);
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.threshold);
        hEdit[0] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 30, 100, 30, hWnd, (HMENU)106,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.priority);
        if (option.language == "jpn") {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 100, 200, 70, 30,
                hWnd, (HMENU)ID_BUTTON_READ_OPTION, hInst, NULL);
        }else if (option.language == "chi_sim") {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 100, 200, 70, 30,
                hWnd, (HMENU)ID_BUTTON_READ_OPTION, hInst, NULL);
        }
        else {
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Save"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 100, 200, 70, 30,
                hWnd, (HMENU)ID_BUTTON_READ_OPTION, hInst, NULL);
        }
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr1, lstrlen(lptStr1));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
        }
        else if(option.language == "chi_sim"){
            TextOutW(hdc, 0, 0, lptStr_c1, lstrlen(lptStr_c1));
            TextOutW(hdc, 0, 30, lptStr_c2, lstrlen(lptStr_c2));
            TextOutW(hdc, 0, 60, lptStr_c3, lstrlen(lptStr_c3));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr_e1, lstrlen(lptStr_e1));
            TextOutW(hdc, 0, 30, lptStr_e2, lstrlen(lptStr_e2));
            TextOutW(hdc, 0, 60, lptStr_e3, lstrlen(lptStr_e3));
        }
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr1, lstrlen(lptStr1));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
        }
        else if(option.language == "chi_sim"){
            TextOutW(hdc, 0, 0, lptStr_c1, lstrlen(lptStr_c1));
            TextOutW(hdc, 0, 30, lptStr_c2, lstrlen(lptStr_c2));
            TextOutW(hdc, 0, 60, lptStr_c3, lstrlen(lptStr_c3));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr_e1, lstrlen(lptStr_e1));
            TextOutW(hdc, 0, 30, lptStr_e2, lstrlen(lptStr_e2));
            TextOutW(hdc, 0, 60, lptStr_e3, lstrlen(lptStr_e3));
        }



        EndPaint(hWnd, &ps);

        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON_READ_OPTION:
	        card_data.data[data_num].gray = SendMessage(hWndListBox2, LB_GETCURSEL, 0, 0);
	        card_data.data[data_num].reverse = SendMessage(hWndListBox5, LB_GETCURSEL, 0, 0);
	        filedit = GetDlgItem(hWnd, 106);
	        GetWindowText(filedit, buffer, 256);
	        card_data.data[data_num].threshold = _ttoi(buffer);
            dialog_mode2 = 0;
            DestroyWindow(hWnd);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY :
        //PostQuitMessage(0);
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}
LRESULT CALLBACK ChildWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    TEXTMETRIC tm;
    std::vector<std::string> writing_strings;
    int strings_size;
    HWND hParent = GetParent(hWnd);
    const std::string TestData = "AAA\nBBB\nCCC\n";
    PAINTSTRUCT ps;
    HWND hButton;
    HWND hButton_font;
    HWND hButton_remove;
    HWND hButton_alphabet;
    HWND hButton_number;
    HWND hButton_symbol;
    HWND hButton_jpn_symbol;
    HWND hButton_test;
    HWND hButton_readoption;
    HWND hButton_symboloption;
    HWND hButton_writeoption;
    HDC hdc;
    HDC hdc2;
    COLORREF bkColor;
    TCHAR A[16];
    static HWND hEdit[30];
    static int data_num;
    int tmp_num = 0;
    int  k = 0;
    int ret;
    TCHAR writing_tmp[256];
    TCHAR tmp_data_name[256] = TEXT("");
    LPCTSTR lptStr = TEXT("領域名");
    LPCTSTR lptStr2 = TEXT("領域left");
    LPCTSTR lptStr3 = TEXT("領域top");
    LPCTSTR lptStr4 = TEXT("領域right");
    LPCTSTR lptStr5 = TEXT("領域bottom");
    LPCTSTR lptStr6 = TEXT("順番");
    LPCTSTR lptStr7 = TEXT("文字可能性");
    LPCTSTR lptStr8 = TEXT("文字列リスト");
    LPCTSTR lptStr9 = TEXT("言語");
    LPCTSTR lptStr_c = TEXT("地区");
    LPCTSTR lptStr2_c = TEXT("地区左");
    LPCTSTR lptStr3_c = TEXT("地区上");
    LPCTSTR lptStr4_c = TEXT("地区右");
    LPCTSTR lptStr5_c = TEXT("地区下");
    LPCTSTR lptStr6_c = TEXT("顺序");
    LPCTSTR lptStr7_c = TEXT("允许文字");
    LPCTSTR lptStr8_c = TEXT("文字目錄·");
    LPCTSTR lptStr9_c = TEXT("语");
    LPCTSTR lptStr_e = TEXT("Region");
    LPCTSTR lptStr2_e = TEXT("Region left");
    LPCTSTR lptStr3_e = TEXT("Region top");
    LPCTSTR lptStr4_e = TEXT("Region right");
    LPCTSTR lptStr5_e = TEXT("Region bottom");
    LPCTSTR lptStr6_e = TEXT("Order");
    LPCTSTR lptStr7_e = TEXT("Allowed word");
    LPCTSTR lptStr8_e = TEXT("Word list");
    LPCTSTR lptStr9_e = TEXT("Language");
    WNDCLASSW wc_A;
    /*
    LPCTSTR lptStr10 = TEXT("書き出し水平位置");
    LPCTSTR lptStr14 = TEXT("書き出し垂直位置");
    LPCTSTR lptStr15 = TEXT("書き出しxマージン");
    LPCTSTR lptStr16 = TEXT("書き出しyマージン");
    */
    WCHAR lpClassName[30] ;
    HWND filedit;
    TCHAR buffer[256];
    TCHAR data_name[3];
    HWND word_list ;
    LPTSTR resultText;
    static CardRect cur_data;
    int x, y;
    TCHAR Kind[4][10] =
    {
        TEXT("ホワイトリスト"),TEXT("ブラックリスト"),TEXT("シンボル"),TEXT("シンボル数値")
    };
    TCHAR Kind_C[4][10] =
    {
        TEXT("白名单"),TEXT("黑名单"),TEXT("符号"),TEXT("符号数")
    };
    TCHAR Kind_E[4][20] =
    {
        TEXT("White list"),TEXT("Black list"),TEXT("Symbol"),TEXT("Symbol Number")
    };
    size_t size;        // 文字列のサイズを格納する
    switch (msg) {
    case WM_CLOSE:
        if (_tcslen(cur_data.data_label) == 0) {
            if (option.language == "jpn") {
                MessageBox(hWnd, _TEXT("領域名を入力してください"), _TEXT("領域名を入力してください"), MB_OK);
            }else if (option.language == "chi_sim") {
                MessageBox(hWnd, _TEXT("请输入数据地区名字"), _TEXT("領域名を入力してください"), MB_OK);
            }
            else {
                MessageBox(hWnd, _TEXT("Please enter region name"), _TEXT("Please enter region name"), MB_OK);
            }
            return 0;
        }
        dialog_mode2 = 0;
        DestroyWindow(hWnd);
        return true;
        break;
    case WM_LBUTTONDOWN:
        x = GET_X_LPARAM(lp);
        y = GET_Y_LPARAM(lp);
        if (x >= 0 && x <= 180 && y >= 0 && y <= 30) {
            SetFocus(hEdit[0]);
        }
        else if (x >= 0 && x <= 180 && y >= 30 && y <= 60) {
            SetFocus(hEdit[1]);
        }
        else if (x >= 0 && x <= 180 && y >= 60 && y <= 90) {
            SetFocus(hEdit[2]);
        }
        else if (x >= 0 && x <= 180 && y >= 90 && y <= 120) {
            SetFocus(hEdit[3]);
        }
        else if (x >= 0 && x <= 180 && y >= 120 && y <= 150) {
            SetFocus(hEdit[4]);
        }
        else if (x >= 0 && x <= 180 && y >= 150 && y <= 180) {
            SetFocus(hEdit[5]);
        }
        else if (x >= 0 && x <= 180 && y >= 180 && y <= 250) {
            SetFocus(hEdit[6]);
        }
        else if (x >= 0 && x <= 180 && y >= 250 && y <= 280) {
            SetFocus(hEdit[7]);
        } 
        else if (x >= 0 && x <= 180 && y >= 340 && y <= 370) {
            SetFocus(hEdit[8]);
        }
        else if (x >= 0 && x <= 180 && y >= 520 && y <= 550) {
            SetFocus(hEdit[9]);
        }
        else if (x >= 0 && x <= 180 && y >= 550 && y <= 580) {
            SetFocus(hEdit[10]);
        }
        // Msg was handled, return zero.
        // Msg was handled, return zero.
        break;
    case WM_CREATE:
        GetClassName(
            hWnd,
            lpClassName,
            2
        );
        data_num = _ttoi(lpClassName);
        current_data_num = data_num;
        cur_data = card_data.data[data_num];
        hWndListBox = CreateWindow(WC_LISTBOX, TEXT(""),
            WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_CLIPSIBLINGS,
            110, 180, 180, 30, hWnd, (HMENU)99, hInst,
            NULL);


        memset(&A, 0, sizeof(A));
        for (k = 0; k <= 3; k += 1)
        {
            if (option.language == "jpn") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Kind[k]);
            }else if (option.language == "chi_sim") {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Kind_C[k]);
            }
            else {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Kind_E[k]);
                
            }

            // Add string to combobox.
            SendMessage(hWndListBox, (UINT)LB_INSERTSTRING, (WPARAM)k, (LPARAM)A);
        }
        SendMessage(hWndListBox, LB_SETCURSEL, cur_data.white_or_black, 0);
        hEdit[0] = CreateWindowEx(0,
            TEXT("EDIT"), (cur_data.data_label),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 0, 180, 30, hWnd, (HMENU)101,
            hInst, NULL
        );
        SendMessage(hEdit[0], EM_SETLIMITTEXT, (WPARAM)63, 0);
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.start_x);
        hEdit[1] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 30, 180, 30, hWnd, (HMENU)102,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.start_y);

        hEdit[2] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 60, 180, 30, hWnd, (HMENU)103,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.end_x);
        hEdit[3] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 90, 180, 30, hWnd, (HMENU)104,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.end_y);
        hEdit[4] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 120, 180, 30, hWnd, (HMENU)105,
            hInst, NULL
        );
        wsprintf(tmp_data_name, TEXT("%d"), cur_data.priority);
        hEdit[5] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 150, 180, 30, hWnd, (HMENU)107,
            hInst, NULL
        );
        MultiByteToWideChar(CP_UTF8, 0, cur_data.word_list.c_str(), 256, ((LPWSTR)tmp_data_name), 256);
        hEdit[6] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT |  ES_MULTILINE | WS_CLIPSIBLINGS,
            110, 200, 180, 70, hWnd, (HMENU)108,
            hInst, NULL
        );
        MultiByteToWideChar(CP_ACP, 0, cur_data.language.c_str(), sizeof(cur_data.language.c_str()), ((LPWSTR)tmp_data_name), 256);
        hEdit[7] = CreateWindowEx(0,
            TEXT("EDIT"), tmp_data_name,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | WS_CLIPSIBLINGS,
            110, 270, 180, 30, hWnd, (HMENU)109,
            hInst, NULL
        );
        if (option.language == "jpn") {
            hButton_readoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("読み取りオプション"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 300, 200, 30,
                hWnd, (HMENU)ID_BUTTON_READ_WINDOW, hInst, NULL);
            hButton_symboloption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("シンボルオプション"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 200, 300, 400, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_WINDOW, hInst, NULL);
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("保存"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON, hInst, NULL);
            hButton_remove = CreateWindowEx(0, TEXT("BUTTON"), TEXT("削除"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON_REMOVE, hInst, NULL);
            hButton_alphabet = CreateWindowEx(0, TEXT("BUTTON"), TEXT("英語"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_ALPHABET, hInst, NULL);
            hButton_number = CreateWindowEx(0, TEXT("BUTTON"), TEXT("数字"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_WORD, hInst, NULL);
            hButton_symbol = CreateWindowEx(0, TEXT("BUTTON"), TEXT("記号"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 160, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL, hInst, NULL);
            hButton_jpn_symbol = CreateWindowEx(0, TEXT("BUTTON"), TEXT("全角記号"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 160, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_JPN_SYMBOL, hInst, NULL);
            hButton_test = CreateWindowEx(0, TEXT("BUTTON"), TEXT("テスト"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 240, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON_TEST, hInst, NULL);
            hButton_writeoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("書き込みオプション"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 420, 200, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_WINDOW, hInst, NULL);
            hButton_writeoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("書き込みテスト"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 450, 200, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_TEST, hInst, NULL);
        }
        else if(option.language == "chi_sim"){
            hButton_readoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("读选项"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 300, 200, 30,
                hWnd, (HMENU)ID_BUTTON_READ_WINDOW, hInst, NULL);
            hButton_symboloption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("符号选项"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 200, 300, 400, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_WINDOW, hInst, NULL);
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Save"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON, hInst, NULL);
            hButton_remove = CreateWindowEx(0, TEXT("BUTTON"), TEXT("删除"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON_REMOVE, hInst, NULL);
            hButton_alphabet = CreateWindowEx(0, TEXT("BUTTON"), TEXT("字母"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_ALPHABET, hInst, NULL);
            hButton_number = CreateWindowEx(0, TEXT("BUTTON"), TEXT("数"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_WORD, hInst, NULL);
            hButton_symbol = CreateWindowEx(0, TEXT("BUTTON"), TEXT("符号"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 160, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL, hInst, NULL);
            hButton_test = CreateWindowEx(0, TEXT("BUTTON"), TEXT("试"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 160, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON_TEST, hInst, NULL);
            hButton_writeoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("写选项"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 420, 200, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_WINDOW, hInst, NULL);
            hButton_writeoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("写试"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 450, 200, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_TEST, hInst, NULL);
        }
        else {
            hButton_readoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Read Option"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 300, 200, 30,
                hWnd, (HMENU)ID_BUTTON_READ_WINDOW, hInst, NULL);
            hButton_symboloption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Symbol Option"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 200, 300, 400, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL_WINDOW, hInst, NULL);
            hButton = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Save"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON, hInst, NULL);
            hButton_remove = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Delete"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON_REMOVE, hInst, NULL);
            hButton_alphabet = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Alphabet"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_ALPHABET, hInst, NULL);
            hButton_number = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Number"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 80, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_WORD, hInst, NULL);
            hButton_symbol = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Symbol"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 160, 340, 70, 30,
                hWnd, (HMENU)ID_BUTTON_SYMBOL, hInst, NULL);
            hButton_test = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Test"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 160, 380, 70, 30,
                hWnd, (HMENU)ID_BUTTON_TEST, hInst, NULL);
            hButton_writeoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Write Option"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 420, 200, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_WINDOW, hInst, NULL);
            hButton_writeoption = CreateWindowEx(0, TEXT("BUTTON"), TEXT("Write Test"),
                WS_CHILD | WS_VISIBLE | BS_FLAT | WS_CLIPSIBLINGS, 0, 450, 200, 30,
                hWnd, (HMENU)ID_BUTTON_WRITE_TEST, hInst, NULL);
        }

        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。
        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr, lstrlen(lptStr));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
            TextOutW(hdc, 0, 90, lptStr4, lstrlen(lptStr4));
            TextOutW(hdc, 0, 120, lptStr5, lstrlen(lptStr5));
            TextOutW(hdc, 0, 150, lptStr6, lstrlen(lptStr6));
            TextOutW(hdc, 0, 180, lptStr7, lstrlen(lptStr7));
            TextOutW(hdc, 0, 210, lptStr8, lstrlen(lptStr8));
            TextOutW(hdc, 0, 240, lptStr9, lstrlen(lptStr9));
        } else if(option.language == "chi_sim"){
            TextOutW(hdc, 0, 0, lptStr_c, lstrlen(lptStr_c));
            TextOutW(hdc, 0, 30, lptStr2_c, lstrlen(lptStr2_c));
            TextOutW(hdc, 0, 60, lptStr3_c, lstrlen(lptStr3_c));
            TextOutW(hdc, 0, 90, lptStr4_c, lstrlen(lptStr4_c));
            TextOutW(hdc, 0, 120, lptStr5_c, lstrlen(lptStr5_c));
            TextOutW(hdc, 0, 150, lptStr6_c, lstrlen(lptStr6_c));
            TextOutW(hdc, 0, 180, lptStr7_c, lstrlen(lptStr7_c));
            TextOutW(hdc, 0, 210, lptStr8_c, lstrlen(lptStr8_c));
            TextOutW(hdc, 0, 240, lptStr9_c, lstrlen(lptStr9_c));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr_e, lstrlen(lptStr_e));
            TextOutW(hdc, 0, 30, lptStr2_e, lstrlen(lptStr2_e));
            TextOutW(hdc, 0, 60, lptStr3_e, lstrlen(lptStr3_e));
            TextOutW(hdc, 0, 90, lptStr4_e, lstrlen(lptStr4_e));
            TextOutW(hdc, 0, 120, lptStr5_e, lstrlen(lptStr5_e));
            TextOutW(hdc, 0, 150, lptStr6_e, lstrlen(lptStr6_e));
            TextOutW(hdc, 0, 180, lptStr7_e, lstrlen(lptStr7_e));
            TextOutW(hdc, 0, 210, lptStr8_e, lstrlen(lptStr8_e));
            TextOutW(hdc, 0, 240, lptStr9_e, lstrlen(lptStr9_e));
        }
        /*
        TextOutW(hdc, 0, 270, lptStr10, lstrlen(lptStr10));
        TextOutW(hdc, 0, 340, lptStr11, lstrlen(lptStr11));
        TextOutW(hdc, 0, 370, lptStr12, lstrlen(lptStr12));
        TextOutW(hdc, 0, 490, lptStr13, lstrlen(lptStr13));
        TextOutW(hdc, 0, 520, lptStr14, lstrlen(lptStr14));
        TextOutW(hdc, 0, 550, lptStr15, lstrlen(lptStr15));
        TextOutW(hdc, 0, 580, lptStr16, lstrlen(lptStr16));
        */
        EndPaint(hWnd, &ps);
        ReleaseDC(hWnd, hdc);
        InvalidateRect(hWnd, 0, TRUE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);     // 描画処理を開始します。

        if (option.language == "jpn") {
            TextOutW(hdc, 0, 0, lptStr, lstrlen(lptStr));
            TextOutW(hdc, 0, 30, lptStr2, lstrlen(lptStr2));
            TextOutW(hdc, 0, 60, lptStr3, lstrlen(lptStr3));
            TextOutW(hdc, 0, 90, lptStr4, lstrlen(lptStr4));
            TextOutW(hdc, 0, 120, lptStr5, lstrlen(lptStr5));
            TextOutW(hdc, 0, 150, lptStr6, lstrlen(lptStr6));
            TextOutW(hdc, 0, 180, lptStr7, lstrlen(lptStr7));
            TextOutW(hdc, 0, 210, lptStr8, lstrlen(lptStr8));
            TextOutW(hdc, 0, 240, lptStr9, lstrlen(lptStr9));
        }
        else if(option.language == "chi_sim"){
            TextOutW(hdc, 0, 0, lptStr_c, lstrlen(lptStr_c));
            TextOutW(hdc, 0, 30, lptStr2_c, lstrlen(lptStr2_c));
            TextOutW(hdc, 0, 60, lptStr3_c, lstrlen(lptStr3_c));
            TextOutW(hdc, 0, 90, lptStr4_c, lstrlen(lptStr4_c));
            TextOutW(hdc, 0, 120, lptStr5_c, lstrlen(lptStr5_c));
            TextOutW(hdc, 0, 150, lptStr6_c, lstrlen(lptStr6_c));
            TextOutW(hdc, 0, 180, lptStr7_c, lstrlen(lptStr7_c));
            TextOutW(hdc, 0, 210, lptStr8_c, lstrlen(lptStr8_c));
            TextOutW(hdc, 0, 240, lptStr9_c, lstrlen(lptStr9_c));
        }
        else {
            TextOutW(hdc, 0, 0, lptStr_e, lstrlen(lptStr_e));
            TextOutW(hdc, 0, 30, lptStr2_e, lstrlen(lptStr2_e));
            TextOutW(hdc, 0, 60, lptStr3_e, lstrlen(lptStr3_e));
            TextOutW(hdc, 0, 90, lptStr4_e, lstrlen(lptStr4_e));
            TextOutW(hdc, 0, 120, lptStr5_e, lstrlen(lptStr5_e));
            TextOutW(hdc, 0, 150, lptStr6_e, lstrlen(lptStr6_e));
            TextOutW(hdc, 0, 180, lptStr7_e, lstrlen(lptStr7_e));
            TextOutW(hdc, 0, 210, lptStr8_e, lstrlen(lptStr8_e));
            TextOutW(hdc, 0, 240, lptStr9_e, lstrlen(lptStr9_e));
        }


        EndPaint(hWnd, &ps);

        break;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case ID_BUTTON_READ_WINDOW:
            if (dialog_mode2 == 1) {
                return false;
            }
            tmp_num = data_num + 10;
            wsprintf(data_name, TEXT("%d"), tmp_num);
            CreateReadOptionWindowClass(hInst, data_name, wc_A);
 //           CreateWindowClass(hInst, data_name, wc_A);
            RegisterClassW(&wc_A);
            hChildWnd_C = CreateReadOptionWindow(hInst, SW_SHOW, data_name,(HMENU)tmp_num, hWnd);
            break;
        case ID_BUTTON_SYMBOL_WINDOW:
            if (dialog_mode3 == 1) {
                return false;
            }
            tmp_num = data_num + 10;
            wsprintf(data_name, TEXT("%d"), tmp_num);
            CreateChooseSymbolWindowClass(hInst, data_name, wc_A);
 //           CreateWindowClass(hInst, data_name, wc_A);
            RegisterClassW(&wc_A);
            hChildWnd_E = CreateChooseSymbolWindow(hInst, SW_SHOW,data_name,(HMENU)tmp_num, hWnd);
            break;
        case ID_BUTTON_WRITE_WINDOW:
            if (dialog_mode2 == 1) {
                return false;
            }
            tmp_num = data_num + 20;
            wsprintf(data_name, TEXT("%d"), tmp_num);
            CreateWriteOptionWindowClass(hInst, data_name, wc_A);
 //           CreateWindowClass(hInst, data_name, wc_A);
            RegisterClassW(&wc_A);
            hChildWnd_D = CreateWriteOptionWindow(hInst, SW_SHOW, data_name,(HMENU)tmp_num, hWnd);
            break;
        case ID_BUTTON_WRITE_TEST:
        {
            if (template_img_flag == FALSE) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("テンプレートファイルを指定してください"), TEXT("テンプレートファイルを指定してください。"), MB_OK);
                }else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("选择模板文件"), TEXT("选择模板文件"), MB_OK);
                }
                else {
                    MessageBox(hWnd, TEXT("Choose template file"), TEXT("Choose template file。"), MB_OK);
                }

            }
            hdc2 = GetDC(hParent);

            mat2memHdc(hdc2, template_img);
            Graphics graphics(mat2memHdc.memHDC);

            for (int i = data_num; i <= data_num; i++) {
                cur_data = card_data.data[i];
                cur_data.cf.hwndOwner = hParent;
                SolidBrush    pen(Color(cur_data.opaque, cur_data.bkColor[0], cur_data.bkColor[1], cur_data.bkColor[2]));
                if (cur_data.figure == 0) {
                    Gdiplus::Pen outer_pen(Color(cur_data.outer_opaque,cur_data.outerColor[0],cur_data.outerColor[1],cur_data.outerColor[2]),cur_data.outer_width);
                    graphics.DrawRectangle(&outer_pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y-cur_data.start_y);
                    graphics.FillRectangle(&pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y-cur_data.start_y);
                }
                else {
                    Gdiplus::Pen outer_pen(Color(cur_data.outer_opaque,cur_data.outerColor[0],cur_data.outerColor[1],cur_data.outerColor[2]),cur_data.outer_width);
                    graphics.DrawEllipse(&outer_pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y-cur_data.start_y);
                    graphics.FillEllipse(&pen, cur_data.start_x, cur_data.start_y, cur_data.end_x - cur_data.start_x, cur_data.end_y- cur_data.start_y);
                }

                HFONT hFont = CreateFontIndirect(&cur_data.lf);
                SelectObject(mat2memHdc.memHDC, hFont);
                SetTextColor(mat2memHdc.memHDC, cur_data.cf.rgbColors);
                SetBkMode(mat2memHdc.memHDC, TRANSPARENT);

                writing_strings = splitString(TestData);
                strings_size = writing_strings.size();
                int font_height;
                if (cur_data.font_height == -1) {
                    GetTextMetrics(mat2memHdc.memHDC, &tm);
                    font_height = tm.tmHeight;
                }
                else {
                    font_height = cur_data.font_height;
                }
                BeginPath(mat2memHdc.memHDC);
                for (int j = 0; j < strings_size; j++) {
                    if (cur_data.white_or_black == 2 || cur_data.white_or_black == 3) {
                            continue;
                    }
                    MultiByteToWideChar(CP_UTF8, 0, writing_strings.at(j).c_str(), 256, ((LPWSTR)writing_tmp), 256);

                    if (cur_data.horizontal_align == 0) {
                        if (cur_data.vertical_align == 0) {
                            SetTextAlign(mat2memHdc.memHDC, TA_LEFT | TA_TOP);
                            TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 1) {
                            SetTextAlign(mat2memHdc.memHDC, TA_LEFT);
                            TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, (cur_data.start_y + cur_data.end_y + (j - strings_size / 2) * font_height) / 2, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 2) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM) | TA_LEFT);
                            TextOutW(mat2memHdc.memHDC, cur_data.start_x + cur_data.margin_x, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                    }
                    else if (cur_data.horizontal_align == 1) {
                        if (cur_data.vertical_align == 0) {
                            SetTextAlign(mat2memHdc.memHDC, TA_TOP);
                            TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 1) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_CENTER));
                            TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, (cur_data.start_y + cur_data.end_y  + cur_data.margin_y +(j - strings_size / 2) * font_height) / 2, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 2) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM));
                            TextOutW(mat2memHdc.memHDC, (cur_data.start_x + cur_data.end_x) / 2, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                    }
                    else if (cur_data.horizontal_align == 2) {
                        if (cur_data.vertical_align == 0) {
                            SetTextAlign(mat2memHdc.memHDC, TA_RIGHT | TA_TOP);
                            TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, cur_data.start_y + cur_data.margin_y + j * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 1) {
                            SetTextAlign(mat2memHdc.memHDC, TA_RIGHT);
                            TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, (cur_data.start_y + cur_data.end_y + (j - strings_size / 2) * font_height * 2) / 2, writing_tmp, lstrlen(writing_tmp));
                        }
                        else if (cur_data.vertical_align == 2) {
                            SetTextAlign(mat2memHdc.memHDC, (TA_BOTTOM) | TA_RIGHT);
                            TextOutW(mat2memHdc.memHDC, cur_data.end_x - cur_data.margin_x, cur_data.end_y - cur_data.margin_y - (strings_size - j - 1) * font_height, writing_tmp, lstrlen(writing_tmp));
                        }
                    }
                }
                EndPath(mat2memHdc.memHDC);
                SolidBrush    pen2(Color(255, cur_data.fontColor[0], cur_data.fontColor[1], cur_data.fontColor[2]));
                SelectObject(mat2memHdc.memHDC, &pen2 );
                StrokeAndFillPath(mat2memHdc.memHDC);
                SelectObject(mat2memHdc.memHDC, GetStockObject(SYSTEM_FONT));
                DeleteObject(hFont);

            }
            memHDC2Mat(mat2memHdc.hbmp);
            for (int i = data_num; i <= data_num; i++) {
                strings_size = writing_strings.size();
                for (int j = 0; j < strings_size; j++) {
                    reading_symbol.symbols[0] = imread(cur_data.symbol[0]);
                    paste(*memHDC2Mat.mat, reading_symbol.symbols[0], cur_data.start_x, cur_data.start_y);
                }
            }
            imshow("OpenCV namedWindow", *memHDC2Mat.mat);
            mat2memHdc(hdc2, template_img_resized);
            break;
        }
        case ID_BUTTON:
            filedit = GetDlgItem(hWnd, 101);
            GetWindowText(filedit, buffer, 256);
            if (buffer[0] == _T('\0')) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, _TEXT("領域名を入力してください"), _TEXT("領域名を入力してください"), MB_OK);
                    return FALSE;
                }else if (option.language == "chi_sim") {
                    MessageBox(hWnd, _TEXT("登录地区名字"), _TEXT("登录地区名字"), MB_OK);
                    return FALSE;
                }
                else {
                    MessageBox(hWnd, _TEXT("Enter region name"), _TEXT("Enter region name"), MB_OK);
                    return FALSE;
                }

            }
            else {
                cur_data = card_data.SaveData(hWnd, data_num);            
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("保存しました"), TEXT("保存しました。"), MB_OK);
                }else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("保存了"), TEXT("保存了"), MB_OK);
                }
                else {
                    MessageBox(hWnd, TEXT("Saved"), TEXT("Saved"), MB_OK);
                }
                bitmap_change_flag = true;
                InvalidateRect(hParent, 0, TRUE);
                //DestroyWindow(hWnd);
                return TRUE;
            }

            break;
        case ID_BUTTON_REMOVE:
            card_data.Remove(data_num);
            dialog_mode = 0;
            DestroyWindow(hWnd);
            return TRUE;
            break;
        case ID_BUTTON_WORD:
            word_list = GetDlgItem(hWnd, 108);
            GetDlgItemText(hWnd, 108, LPTSTR(buffer), 256);
            lstrcat(buffer, TEXT("0123456789"));
            SetWindowText(word_list, buffer);

            return TRUE;
            break;
        case ID_BUTTON_ALPHABET:
            word_list = GetDlgItem(hWnd, 108);
            GetDlgItemText(hWnd, 108, LPTSTR(buffer), 256);
            lstrcat(buffer, TEXT("abcdefghijklmnopqrstuvwxyz"));
            SetWindowText(word_list, buffer);

            return TRUE;
            break;
        case ID_BUTTON_SYMBOL:
            word_list = GetDlgItem(hWnd, 108);
            GetDlgItemText(hWnd, 108, LPTSTR(buffer), 256);
            lstrcat(buffer, TEXT("!#$ % &'()*+-./:;<=>?@[\]^_`{|}~"));
            SetWindowText(word_list, buffer);

            return TRUE;
            break;
        case ID_BUTTON_JPN_SYMBOL:
            word_list = GetDlgItem(hWnd, 108);
            GetDlgItemText(hWnd, 108, LPTSTR(buffer), 256);
            lstrcat(buffer, TEXT("、。，．・：；？！゛゜´｀¨＾￣＿ヽヾゝゞ〃仝々〆〇ー―‐／＼～∥｜…‥‘’“”（）〔〕［］｛｝〈〉《》「」『』【】＋－±×"));
            SetWindowText(word_list, buffer);

            return TRUE;
            break;
        case ID_BUTTON_TEST:
            cv::Mat gray;
            if (cur_data.cur_num == -1) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("保存してください"), TEXT("保存してください。"), MB_OK);
                }else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("请保存"), TEXT("请保存"), MB_OK);
                }
                else {
                    MessageBox(hWnd, TEXT("Please save"), TEXT("Please save"), MB_OK);
                }

            }
            if (template_img_flag == FALSE) {
                if (option.language == "jpn") {
                    MessageBox(hWnd, TEXT("テンプレートファイルを指定してください"), TEXT("テンプレートファイルを指定してください。"), MB_OK);
                }else if (option.language == "chi_sim") {
                    MessageBox(hWnd, TEXT("请选择模板文件"), TEXT("请选择模板文件"), MB_OK);
                }
                else {
                    MessageBox(hWnd, TEXT("Choose template file"), TEXT("Choose template file"), MB_OK);
                }
            }
            else {
                cur_data = card_data.data[data_num];
                cv::destroyAllWindows();
                if (cur_data.white_or_black == 2 or cur_data.white_or_black == 3) {
                    std::string result = checkSymbol(template_img, cur_data,false);
                    size_t char_length = 0;
                    wchar_t* wcstring = new wchar_t[2000];
                    mbstowcs_s(&char_length,wcstring, 20000,result.c_str(), _TRUNCATE);
                    MessageBox(hWnd, wcstring, wcstring, MB_OK);
                    return true;
                }
                cv::cvtColor(template_img, gray, COLOR_RGB2GRAY);
                tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
                char base_dir[255];
                ret = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    option.base_dir,
                    -1,
                    base_dir,
                    200 + 1,
                    NULL,
                    NULL);
                ocr->Init(base_dir, cur_data.language.c_str(), tesseract::OEM_LSTM_ONLY);
                CardRect tmp = cur_data;
                cv::Mat dst;
                cv::Mat binary;
                cv::Mat reverse;
                if (cur_data.gray == true) {
                    dst = cv::Mat(gray, cv::Rect(tmp.start_x, tmp.start_y, tmp.end_x - tmp.start_x, tmp.end_y - tmp.start_y));
                }
                else {
                    dst = cv::Mat(template_img, cv::Rect(tmp.start_x, tmp.start_y, tmp.end_x - tmp.start_x, tmp.end_y - tmp.start_y));
                }
                if (cur_data.white_or_black == WHITE_LIST) {
                    ocr->SetVariable("tessedit_char_whitelist", cur_data.word_list.c_str());
                }
                else {
                    ocr->SetVariable("tessedit_char_blacklist", cur_data.word_list.c_str());

                }
                if (cur_data.gray == true) {
                    if (cur_data.threshold >= 0 && cur_data.threshold <= 255) {
                        cv::threshold(dst, binary, cur_data.threshold, 255, cv::THRESH_BINARY);
                        if (cur_data.reverse) {
                            bitwise_not(binary, reverse);
                            ocr->SetImage(reverse.data, reverse.cols, reverse.rows, 1, static_cast<int>(reverse.step));
                            imshow("reverse", reverse);
                        }
                        else {
                            ocr->SetImage(binary.data, binary.cols, binary.rows, 1, static_cast<int>(binary.step));
                            imshow("binary", binary);
                        }
                        
                    }
                    else {
                        ocr->SetImage(dst.data, dst.cols, dst.rows, 1, static_cast<int>(dst.step));
                        imshow("gray", dst);
                    }
                }
                else {
                    imshow("template_img", dst);
                    ocr->SetImage(dst.data, dst.cols, dst.rows, 3, static_cast<int>(dst.step));
                        imshow("template", dst);

                }

                std::string out_text = ocr->GetUTF8Text();
                wchar_t* wcstring = new wchar_t[2000];
                size_t convertedChars = 0;
                out_text = UTF8toSjis(out_text);
                size_t char_length = 0;
                mbstowcs_s(&char_length,wcstring, 20000,out_text.c_str(), _TRUNCATE);
 //               out_text = ConvertCRLF(out_text, "");
//                wsprintf(tmp_data_name, TEXT("%s"), out_text.c_str());
                MessageBox(hWnd, wcstring, wcstring, MB_OK);
                dst.release();
                if (cur_data.threshold >= 0 && cur_data.threshold <= 255 && cur_data.gray == true) {
                    if (cur_data.reverse == true) {
                        reverse.release();
                    }
                    binary.release();
                }
            }
            gray.release();
            return TRUE;
            break;
        }
        break;
    case WM_DESTROY :
        dialog_mode = 0;
        break;
    default:
        return (DefWindowProc(hWnd, msg, wp, lp));
    }
    return 0;
}
HWND CreateWriteOptionWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent) {
    HWND hWnd;
    hWnd = CreateWindow(
        ClassName,
        NULL,
        WS_CHILD |WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_CLIPSIBLINGS,
        0,                      //親ウィンドウと同じサイズ
        100,
        300,
        500,
        hParent,        //親ウィンドウのハンドル
        (HMENU)hMenuChildId,    // 子ウィンドウを判別するためのID
        hInst,          //インスタンスハンドル
        NULL);
    if (option.language == "jpn") {
        SetWindowText(hWnd, TEXT("書き込みオプション選択"));
    }else if (option.language == "chi_sim") {
        SetWindowText(hWnd, TEXT("选择写选项"));
    }
    else {
        SetWindowText(hWnd, TEXT("Choose Write Option"));
        
    }
    if (!hWnd)
        return FALSE;
    dialog_mode2 = 1;

    return hWnd;
}
HWND CreateSymbolOptionWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent) {
    HWND hWnd;
    hWnd = CreateWindow(
        ClassName,
        NULL,
        WS_CHILD |WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_CLIPSIBLINGS,
        0,                      //親ウィンドウと同じサイズ
        100,
        700,
        300,
        hParent,        //親ウィンドウのハンドル
        (HMENU)hMenuChildId,    // 子ウィンドウを判別するためのID
        hInst,          //インスタンスハンドル
        NULL);
    if (option.language == "jpn") {
        SetWindowText(hWnd, TEXT("シンボルオプション選択"));
    }else if (option.language == "chi_sim") {
        SetWindowText(hWnd, TEXT("选择符号选项"));
    }
    else {
        SetWindowText(hWnd, TEXT("Choose symbol option"));

    }
    if (!hWnd)
        return FALSE;
    dialog_mode2 = 1;

    return hWnd;
}
HWND CreateReadOptionWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent) {
    HWND hWnd;
    hWnd = CreateWindow(
        ClassName,
        NULL,
        WS_CHILD | WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_CLIPSIBLINGS,
        0,                      //親ウィンドウと同じサイズ
        100,
        300,
        300,
        hParent,        //親ウィンドウのハンドル
        (HMENU)hMenuChildId,    // 子ウィンドウを判別するためのID
        hInst,          //インスタンスハンドル
        NULL);
    if (option.language == "jpn") {
        SetWindowText(hWnd, TEXT("読み取りオプション選択"));
    }else if (option.language == "chi_sim") {
        SetWindowText(hWnd, TEXT("选择读选项"));
    }
    else {
        SetWindowText(hWnd, TEXT("Choose Read Option"));
    }
    if (!hWnd)
        return FALSE;
    dialog_mode2 = 1;

    return hWnd;
}
HWND CreateChooseSymbolWindow(HINSTANCE hInst, int nShowCmd,LPCTSTR ClassName,  HMENU hMenuChildId, HWND hParent) {
    HWND hWnd;
    hWnd = CreateWindow(
        ClassName,
        NULL,
        WS_CHILD | WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_CLIPSIBLINGS,
        0,                      //親ウィンドウと同じサイズ
        100,
        200,
        300,
        hParent,        //親ウィンドウのハンドル
        (HMENU)hMenuChildId,    // 子ウィンドウを判別するためのID
        hInst,          //インスタンスハンドル
        NULL);
    if (option.language == "jpn") {
        SetWindowText(hWnd, TEXT("シンボル選択"));
    }else if (option.language == "chi_sim") {
        SetWindowText(hWnd, TEXT("选择符号"));
    }
    else {
        SetWindowText(hWnd, TEXT("Choose Symbol"));
    }
    if (!hWnd)
        return FALSE;

    return hWnd;
}
HWND CreateAreaChooseWindow(HINSTANCE hInst, int nShowCmd, HMENU hMenuChildId, HWND hParent) {
    HWND hWnd;
    hWnd = CreateWindow(
        TEXT("SELECT_AREA"),
        NULL,
        WS_CHILD |WS_CAPTION | WS_VISIBLE | WS_SYSMENU,
        0,                      //親ウィンドウと同じサイズ
        0,
        200,
        300,
        hParent,        //親ウィンドウのハンドル
        (HMENU)hMenuChildId,    // 子ウィンドウを判別するためのID
        hInst,          //インスタンスハンドル
        NULL);
    if (option.language == "jpn") {
        SetWindowText(hWnd, TEXT("領域選択"));
    }else if (option.language == "chi_sim") {
        SetWindowText(hWnd, TEXT("选择地区"));
    }
    else {
        SetWindowText(hWnd, TEXT("Choose Area"));
    }
    if (!hWnd)
        return FALSE;
    dialog_mode = 1;

    return hWnd;
}
// 子ウィンドウの生成
HWND CreateChildWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent)
{
    HWND hWnd;

    // 親ウィンドウのサイズを取得

    hWnd = CreateWindow(
        ClassName,
        NULL,
        WS_CHILD |WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_CLIPSIBLINGS,
        0,                      //親ウィンドウと同じサイズ
        0,
        600,
        600,
        hParent,        //親ウィンドウのハンドル
        (HMENU)hMenuChildId,    // 子ウィンドウを判別するためのID
        hInst,          //インスタンスハンドル
        NULL);
    if (option.language == "jpn") {
        SetWindowText(hWnd, TEXT("領域情報"));
    }else if (option.language == "chi_sim") {
        SetWindowText(hWnd, TEXT("地区情报"));
    }
    else
    {
        SetWindowText(hWnd, TEXT("Area Info"));
    }
        


    if (!hWnd)
        return FALSE;
    dialog_mode = 1;
    dialog_mode2 = 0;
    return hWnd; 
}
CString Utf8ToCString(const std::string& utf8)
{

    INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    WCHAR* utf16 = new WCHAR[bufsize];

    ::MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, utf16, bufsize);

    CString ret(utf16, ::wcslen(utf16));
    delete[] utf16;

    return ret;
}
int __stdcall BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
    TCHAR dir[MAX_PATH];
    ITEMIDLIST* lpid;
    HWND hEdit;

    switch (uMsg) {
    case BFFM_INITIALIZED:  //      ダイアログボックス初期化時
        SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);     //      コモンダイアログの初期ディレクトリ
        break;
    case BFFM_VALIDATEFAILED:       //      無効なフォルダー名が入力された
        if (option.language == "jpn") {
            MessageBox(hWnd, (TCHAR*)lParam, _TEXT("無効なフォルダー名が入力されました"), MB_OK);
        }else if (option.language == "chi_sim") {
            MessageBox(hWnd, (TCHAR*)lParam, _TEXT("无效的文件夹"), MB_OK);
        }
        else {
            MessageBox(hWnd, (TCHAR*)lParam, _TEXT("Invalid folder name"), MB_OK);
        }
        hEdit = FindWindowEx(hWnd, NULL, _TEXT("EDIT"), NULL);     //      エディットボックスのハンドルを取得する
        SetWindowText(hEdit, _TEXT(""));
        return 1;       //      ダイアログボックスを閉じない
        break;
    case BFFM_IUNKNOWN:
        break;
    case BFFM_SELCHANGED:   //      選択フォルダーが変化した場合
        lpid = (ITEMIDLIST*)lParam;
        SHGetPathFromIDList(lpid, dir);
        hEdit = FindWindowEx(hWnd, NULL, _TEXT("EDIT"), NULL);     //      エディットボックスのハンドルを取得する
        SetWindowText(hEdit, dir);
        break;
    }
    return 0;
}
void initOption() {
    TCHAR tmpMetaPath[200];
    std::ifstream option_file(OPTION_FILE_NAME);
    if (!option_file) {
        return;
    }
    std::string buffer;
    getline(option_file, buffer);
    option.setLanguage(buffer);
    getline(option_file, buffer);
    option.resize = atof(buffer.c_str());
    getline(option_file, buffer);
    option.template_file_name= buffer;
    std::string file_name = option.template_file_name;
    template_img = cv::imread(file_name);
    if (!template_img.empty()) {
          resize(template_img, template_img_resized, cv::Size(), option.resize, option.resize);
          template_img_flag = TRUE;
    }
    getline(option_file, buffer);
    MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        buffer.c_str(),
        200,
        option.base_dir,
        200 + 1);
    getline(option_file, buffer);
    MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        buffer.c_str(),
        200,
        option.inputPath,
        200 + 1);
    getline(option_file, buffer);
    MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        buffer.c_str(),
        200,
        tmpMetaPath,
        200 + 1);
    option.metaPath = TEXT("");
    option.metaPath += tmpMetaPath;
    file_name = WStringToString(option.metaPath, 0);
    card_data.LoadAll(file_name);
    while (getline(option_file, buffer)) {
        if (buffer == "intermediate") {
            option.setIntermediate(true);
        }else if (buffer == "bmp") {
            extension.setBmp(true);
        }
        else if (buffer == "div") {
            extension.setDiv(true);
        }
        else if (buffer == "pdm") {
            extension.setPdm(true);
        }
        else if (buffer == "pgm") {
            extension.setPgm(true);
        }
        else if (buffer == "ppm") {
            extension.setPpm(true);
        }
        else if (buffer == "pnm") {
            extension.setPnm(true);
        }
        else if (buffer == "sr") {
            extension.setSr(true);
        }
        else if (buffer == "ras") {
            extension.setRas(true);
        }
        else if (buffer == "png") {
            extension.setPng(true);
        }
        else if (buffer == "tiff") {
            extension.setTiff(true);
        }
        else if (buffer == "tif") {
            extension.setTif(true);
        }
        else if (buffer == "jp2") {
            extension.setJp2(true);
        }
        else if (buffer == "jpeg") {
            extension.setJpeg(true);
        }
        else if (buffer == "jpg") {
            extension.setJpg(true);
        }
        else if (buffer == "jpe") {
            extension.setJpe(true);
        }

    }
    option_file.close();

}
void saveOption() {
    std::ofstream option_file(OPTION_FILE_NAME);
    option_file << option.getAllOptionString() << extension.getAllExtensionString();
    option_file.close();
}
std::string UTF8toSjis(std::string srcUTF8) {
    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //UTF8からUnicodeへ変換
    MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

    //ShiftJISへ変換後の文字列長を得る
    int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけShiftJIS文字列のバッファを確保
    char* bufShiftJis = new char[lengthSJis];

    //UnicodeからShiftJISへ変換
    WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

    std::string strSJis(bufShiftJis);

    delete bufUnicode;
    delete bufShiftJis;

    return strSJis;
}
std::string ConvertCRLF
(
    std::string strString     // 変換対象の文字列
    , std::string strCRLF       // 改行コードを変換したい文字列("¥n"など)
)
{
    std::string strRet;

    std::string::iterator ite = strString.begin();
    std::string::iterator iteEnd = strString.end();

    if (0 < strString.size()) {

        unsigned char bNextChar = *ite++;

        while (1) {
            if ('\r' == bNextChar) {

                // 改行確定
                strRet += strCRLF;

                // EOF判定
                if (ite == iteEnd) {
                    break;
                }

                // 1文字取得
                bNextChar = *ite++;

                if ('\n' == bNextChar) {

                    // EOF判定
                    if (ite == iteEnd) {
                        break;
                    }

                    // 1文字取得
                    bNextChar = *ite++;
                }
            }
            else if ('\n' == bNextChar) {

                // 改行確定
                strRet += strCRLF;

                // EOF判定
                if (ite == iteEnd) {
                    break;
                }

                // 1文字取得
                bNextChar = *ite++;

                if ('¥r' == bNextChar) {

                    // EOF判定
                    if (ite == iteEnd) {
                        break;
                    }

                    // 1文字取得
                    bNextChar = *ite++;
                }
            }
            else {

                // 改行以外
                strRet += bNextChar;

                // EOF判定
                if (ite == iteEnd) {
                    break;
                }

                // 1文字取得
                bNextChar = *ite++;
            }
        };
    }
    return(strRet);
}
std::vector<std::string> splitString(const std::string& str,const std::string split_value )
{
    std::vector<std::string> tokens;

    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, split_value.c_str()[0])) {
        tokens.push_back(token);
    }

    return tokens;
}
char* setvalue(char* p, char* field, int size)
	  {
		  if (*p == '"')
		  {
			  /* ここでのインクリメントは
			   * 最後のインクリメントではポインタが＋２になる(評価された場合のみ)
			   * *p != '"'が0になった場合のみ *(++p) == '"'が実行される
			   * これはダブルクォート連続の時の対策
			   */
			  while (*(++p) && *p != '\n' && (*p != '"' || *(++p) == '"'))
			  {
				  if (--size > 0)
				  {
					  *(field++) = *p;
				  }
			  }
		  }
		  // ここの部分は""で囲まれていない部分が通るようになっている
		  for (; *p && *p != ',' && *p != '\n'; p++)
		  {
			  if (--size > 0)
			  {
				  *(field++) = *p;
			  }
		  }
		  *field = '\0';
		  return *p ? (p + 1) : p;
	  }
 std::string checkSymbol(cv::Mat img, CardRect data,int read_flag) {
    std::string ret;
    std::string prev_string = "\0";
    int count = 0;
    int x, y;
    int min_width = INT_MAX;
    int rows,cols;
    TCHAR tbase_dir[256];
    CHAR base_dir[256];
    CHAR tmp_file[256];
    GetCurrentDirectory(255, tbase_dir);
    int tmp = WideCharToMultiByte(
                    CP_ACP,
                    0,
                    tbase_dir,
                    -1,
                    base_dir,
                    200 + 1,
                    NULL,
                    NULL);
    int symbol_number_flag = data.white_or_black == 2 ? false : true;
    if ( read_flag == false) {
        for (int i = 0; i < 50; i++) {
            if (data.symbol[i].empty()) {
                continue;
            }
            else {
                if (reading_symbol.symbols[i].cols > 0) {
                    reading_symbol.symbols[i].release();
                }
                strcpy_s(tmp_file,255, data.symbol[i].c_str());
                reading_symbol.symbols[i] = imread(data.symbol[i]);
                if(reading_symbol.symbols[i].cols == 0 || reading_symbol.symbols[i].rows == 0 ){
                    continue;
                }
                min_width = min_width < reading_symbol.symbols[i].cols ? min_width : reading_symbol.symbols[i].cols;
                reading_symbol.valid_pixels[i] = getValidPixels(reading_symbol.symbols[i], data.none_color[i]);
            }
        }
    }
    for (x = data.start_x; x < data.end_x; x++) {
        for (y = data.start_y; y < data.end_y; y++) {
            for (int i = 0; i < 50; i++) {
                rows = reading_symbol.symbols[i].rows;
                cols = reading_symbol.symbols[i].cols;
                if (rows == 0 || cols == 0)
                    continue;
                if (y + rows >= data.end_y) {
                    continue;
                }
                if (y + rows >= img.rows) {
                    continue;
                }
                if (x + cols >= img.cols) {
                    continue;
                }
                if (x + cols >= data.end_x) {
                    continue;
                }
                int tmp_y = 0;
                double diff = 0;
                for (int p_y = y; p_y < y + rows; p_y++, tmp_y++)
                {
                    int tmp_x = 0;
                    for (int p_x = x; p_x < x + cols; p_x++, tmp_x++)
                    {
                        cv::Vec3b bgr = reading_symbol.symbols[i].at<cv::Vec3b>(tmp_y, tmp_x);
                        if (abs((int)bgr[2] - (int)data.none_color[i][0]) + abs((int)bgr[1] - (int)data.none_color[i][1]) + abs((int)bgr[0] - (int)data.none_color[i][2]) < 0.1) {
                            continue;
                        }
                        cv::Vec3b bgr2 = img.at<cv::Vec3b>(p_y, p_x);
                        diff += abs((int)bgr[0] - (int)bgr2[0]) + abs((int)bgr[1] - (int)bgr2[1]) + abs((int)bgr[2] - (int)bgr2[2]);
                        /*
                        TCHAR debug_string[50];
                        wsprintf(debug_string, TEXT("i%d,tmp_x%d,tmp_y%d,p_x%d,p_y%d,diff%d,bgr(%d,%d,%d),bgr2(%d,%d,%d)\n"),i,tmp_x,tmp_y,p_x,p_y,diff,bgr[0],bgr[1],bgr[2],bgr2[0],bgr2[1],bgr2[2] );
                        OutputDebugString(debug_string);
                        */
                        if (diff > data.pixel_diff[i] * reading_symbol.valid_pixels[i]) {
                            break;
                        }
                    }
                    if (diff > data.pixel_diff[i] * reading_symbol.valid_pixels[i]) {
                        break;
                    }
                }
                if (diff > data.pixel_diff[i] * reading_symbol.valid_pixels[i]) {
                    continue;
                }
                else {
                    if (symbol_number_flag == TRUE) {
                        if (prev_string == data.symbol_name[i]) {
                            count++;
                        }
                        else {
                            if (count > 0) {
                                ret += prev_string;
                                ret += std::to_string(count);
                                count = 0;
                            }
                            prev_string = data.symbol_name[i];
                            count++;

                        }
                    }
                    else {
                        ret += data.symbol_name[i];
                    }
                    x += min_width - 1;
                }
            }
        }
    }
    if (count > 0) {
          ret += prev_string;
          ret += std::to_string(count);
          count = 0;
    }
    return ret;
}
int getValidPixels(cv::Mat symbol, Vec3b none_color) {
    int ret = 0;
    for (int x = 0; x < symbol.cols; x++) {
        for (int y = 0; y < symbol.rows; y++) {
            cv::Vec3b bgr = symbol.at<cv::Vec3b>(y, x);
            if (abs(bgr[0] - none_color[0]) + abs(bgr[1] - none_color[1]) && abs(bgr[2] - none_color[2])  == 0) {
                continue;
            }
            ret++;
        }
    }
    return ret;
}
// 画像を画像に貼り付ける関数
void paste(cv::Mat dst, cv::Mat src, int xx, int yy, int width, int height) {
    cv::Mat resized_img;
   
    for (int y = 0; y < height ; y++) {
        for (int x = 0; x < width ; x++) {
            cv::Vec4b tmp = src.at<cv::Vec4b>(cv::Point(x, y));
            TCHAR debug[50];
            wsprintf(debug, TEXT("%d,%d,%d,%d,x %d,y %d\n"), tmp[0], tmp[1], tmp[2], tmp[3], x, y);
            OutputDebugStringW(debug);
            if (tmp[3] == 0) {
                continue;
            }
            else {
                if (x > 0 && x < dst.cols  + xx && y > 0 && y < dst.rows + yy) {
                    dst.at<cv::Vec3b>(cv::Point(x + xx, y+yy))
                      = cv::Vec3b(tmp[0], tmp[1],tmp[2]);
                }
            }

        }
    }

}

// 画像を画像に貼り付ける関数（サイズ指定を省略したバージョン）
void paste(cv::Mat dst, cv::Mat src, int x, int y) {
    paste(dst, src, x, y, src.cols, src.rows);
}
char* substring(const char* target, size_t begin, size_t length, char* result, size_t result_size)
{
    assert(target != NULL);
    assert(result != NULL);
    assert(length + 1 <= result_size);

    if (begin < strlen(target)) {

        // target[begin] を起点に、length の文字数分だけコピー
        strncpy_s(result, result_size,target + begin, length);

        // 末尾のヌル文字を付加
        // strncpy() によってすでに付加されている可能性はある。
        // また、result[length] よりも手前にすでにヌル文字があるかもしれない。
        result[length] = '\0';
    }
    else {
        // begin が target の末尾以降の位置にあるときは、
        // 空文字列を返す
        result[0] = '\0';
    }

    return result;
}
cv::Mat makealphachannel(cv::Mat src, cv::Vec3b none_color) {
    cv::Mat ret(src.size(), CV_8UC4);
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            cv::Vec3b p = src.at<cv::Vec3b>(cv::Point(x, y));
            if (p[0] == none_color[0] && p[1] == none_color[1] && p[2] == none_color[2]) {
                 ret.at<cv::Vec4b>(cv::Point(x, y))
                      = cv::Vec4b(p[0], p[1], p[2], 0);
            }
            else {
                 ret.at<cv::Vec4b>(cv::Point(x, y))
                      = cv::Vec4b(p[0], p[1], p[2], 255);

            }
        }
    }
    return ret;
}
