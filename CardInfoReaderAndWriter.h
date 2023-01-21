#pragma once

#include "resource.h"
#include <vector>
char* substring(const char* target, size_t begin, size_t length, char* result, size_t result_size);
std::vector<std::string> splitString(const std::string& str,const std::string token = "\n");
cv::Mat makealphachannel(cv::Mat src, cv::Vec3b none_color);
char OPTION_FILE_NAME[255];
using namespace cv;
void initOption();
void saveOption();
HWND CreateChildWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent);
HWND CreateAreaChooseWindow(HINSTANCE hInst, int nShowCmd, HMENU hMenuChildId, HWND hParent);
HWND CreateReadOptionWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName,HMENU hMenuChildId, HWND hParent);
HWND CreateWriteOptionWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName,HMENU hMenuChildId, HWND hParent);
void CreateAreaChooseWindowClass(HINSTANCE hInst, LPCTSTR szClassName, WNDCLASSW& wc);
LRESULT CALLBACK ChildWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ChooseAreaWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ReadOptionWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WriteOptionWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir, const std::wstring Filter = _TEXT("全てのファイル(*.*)\0*.*\0"), const std::wstring DefExt = _TEXT("TXT"));
BOOL GetFileNameReadMeta(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir );
BOOL GetFileNameCsv(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir);
BOOL GetFileNameAll(HWND hWnd, TCHAR* fname, int sz, const TCHAR* initDir);
std::string TrimBR(std::string);
std::string WStringToString
(
    std::wstring oWString,int mode=0);
struct reading_symbol {
    cv::Mat symbols[50];
    int Rect_data_num= 0;
    int valid_pixels[50] ;
} reading_symbol;
struct Mat2MemHDC {
    BITMAPINFO bmi;
    HBITMAP hbmp;
    BYTE* pBits;
    HDC memHDC;

    Mat2MemHDC() {
        pBits = 0;
    }
    ~Mat2MemHDC() {
        if (pBits) {
            DeleteObject(hbmp);
            DeleteDC(memHDC);
        }
    }
    //      Mat型の画像とメモリデバイスコンテキストに変換する
    bool operator()(HDC hdc, Mat img) {
        //      横幅を4の倍数に合わせる
        int sx = img.cols;
        if (sx % 4) {
            sx = (sx + 4) & 0xfffffffc;
        }
        if (pBits == NULL || (pBits && (sx != bmi.bmiHeader.biWidth || img.rows != bmi.bmiHeader.biHeight))) {
            DeleteObject(hbmp);
            DeleteDC(memHDC);
            bmi.bmiHeader.biSize = sizeof(bmi);
            bmi.bmiHeader.biWidth = sx;
            bmi.bmiHeader.biHeight = -img.rows;     //      上下反転
            bmi.bmiHeader.biBitCount = img.channels() * 8;

            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biSizeImage = 0;
            bmi.bmiHeader.biXPelsPerMeter = 0;
            bmi.bmiHeader.biYPelsPerMeter = 0;
            bmi.bmiHeader.biClrUsed = 0;
            bmi.bmiHeader.biClrImportant = 0;

            hbmp = CreateDIBSection(NULL, &bmi, 0, (void**)&pBits, NULL, 0);
            if (pBits == NULL) {     //      メモリ不足
                return false;
            }
            memHDC = CreateCompatibleDC(hdc);
            SelectObject(memHDC, hbmp);
        }

        //              imgの横幅が4の倍数でない場合があるので、1ラインずつコピーする
        for (int y = 0; y < height(); y++) {
            memcpy(pBits + y * width() * img.channels(), img.data + y * img.cols * img.channels(), img.cols * img.channels());
            //      右横の余白を白にする
            BYTE* p = pBits + (y * width() + img.cols) * img.channels();
            for (int x = img.cols; x < width(); x++) {
                for (int n = 0; n < img.channels(); n++) {
                    *p++ = 0xff;
                }
            }
        }
        return true;
    }
    int width(void) {
        return bmi.bmiHeader.biWidth;
    }
    int height(void) {
        return -bmi.bmiHeader.biHeight;
    }
    void BitBlt(HDC hdc, int dx, int dy, DWORD rop) {
        ::BitBlt(hdc, 0, 0, width(), height(), memHDC, dx, dy, rop);
    }
};

//      ビットマップをMat型に変換するクラス

struct MemHDC2Mat {
    Mat* mat;
    int width, height;
    MemHDC2Mat() {
        mat = 0;
    }
    void operator()(HBITMAP hbmp) {
        BITMAP bmp;
        GetObject(hbmp, sizeof(bmp), &bmp);
        if (mat == 0 || (mat && (bmp.bmHeight != height || bmp.bmWidth != width))) {
            delete mat;
            mat = new Mat(bmp.bmHeight, bmp.bmWidth, CV_8UC(bmp.bmBitsPixel / 8));
        }
        width = bmp.bmWidth;
        height = bmp.bmHeight;
        size_t sz = bmp.bmWidth * bmp.bmHeight * (bmp.bmBitsPixel / 8);
        memcpy(mat->data, bmp.bmBits, sz);
    }
    ~MemHDC2Mat() {
        if (mat)
            delete mat;
    }
};

HWND CreateAreaChooseWindow(HINSTANCE hInst, int nShowCmd, HMENU hMenuChildId, HWND hParent);

std::string checkSymbol(cv::Mat img, CardRect data, int read_flag);
CString Utf8ToCString(const std::string& utf8);
Mat2MemHDC mat2memHdc;
Mat2MemHDC mat2memHdcW;
MemHDC2Mat memHDC2Mat;
int __stdcall BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
std::string UTF8toSjis(std::string srcUTF8);
std::string ConvertCRLF
(
    std::string strString     // 変換対象の文字列
    , std::string strCRLF       // 改行コードを変換したい文字列("¥n"など)
);
int getValidPixels(cv::Mat mat, cv::Vec3b none_color);
LRESULT CALLBACK SymbolOptionWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
HWND CreateChooseSymbolWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent);
LRESULT CALLBACK ChooseSymbolWindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
HWND CreateSymbolOptionWindow(HINSTANCE hInst, int nShowCmd, LPCTSTR ClassName, HMENU hMenuChildId, HWND hParent);
void paste(cv::Mat dst, cv::Mat src, int x, int y);
