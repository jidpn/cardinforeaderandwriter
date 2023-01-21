#pragma once
#include "CardRect.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <tchar.h>
#include "windows.h"
#include <algorithm>
#include <vector>
#define CARD_RECT_ARRAY_SIZE 10
#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))
char* setvalue(char* p, char* field, int size);
extern HWND hWndListBox;
extern HWND hWndListBox2;
extern HWND hWndListBox3;
extern HWND hWndListBox4;
extern HWND hWndListBox5;
extern char bitmap_change_flag;
std::string WStringToString(
	std::wstring oWString,int mode
);
class CardRectArray{
public: int width;
public: int height;
public: CardRect data[CARD_RECT_ARRAY_SIZE];
public:int num;
public:CardRectArray() {
	int i;
	CardRect tmp_data = CardRect();
	for (i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		data[i] = tmp_data;
		data[i].cf.lpLogFont = &data[i].lf;
	}
	num = 0;
	return;
}
public: void reset() {
	int i;
	CardRect tmp_data = CardRect();
	for (i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		data[i] = tmp_data;
		data[i].cf.lpLogFont = &data[i].lf;
	}
	num = 0;
	return;
}
public:CardRectArray(std::string initial_language) {
	int i;
	CardRect tmp_data = CardRect(initial_language);
	for (i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		data[i] = tmp_data;
		data[i].cf.lpLogFont = &data[i].lf;
	}
	num = 0;
	return;
}
public: int Add(CardRect card_rect) {
	if (num >= CARD_RECT_ARRAY_SIZE) {
		return -1;
	}
	card_rect.cur_num = num;
	card_rect.priority = 100;
	data[num] = card_rect;
	data[num].cf.lpLogFont = &data[num].lf;
	num++;
	bitmap_change_flag = true;
	return num - 1;
	  }
public: CardRect SaveData(HWND hWnd,int tmp_num) {
	TCHAR buffer[1024];
	HWND filedit;
	filedit = GetDlgItem(hWnd, 101);
	GetWindowText(filedit, buffer, 1024);
	_tcscpy_s(data[tmp_num].data_label,50,buffer);
	filedit = GetDlgItem(hWnd, 102);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].start_x = _ttoi(buffer);
	filedit = GetDlgItem(hWnd, 103);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].start_y = _ttoi(buffer);
	filedit = GetDlgItem(hWnd, 104);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].end_x = _ttoi(buffer);
	filedit = GetDlgItem(hWnd, 105);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].end_y = _ttoi(buffer);
	filedit = GetDlgItem(hWnd, 107);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].priority = _ttoi(buffer);
	filedit = GetDlgItem(hWnd, 108);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].word_list = WStringToString(buffer,1);
	filedit = GetDlgItem(hWnd, 109);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].language = WStringToString(buffer,0);
	filedit = GetDlgItem(hWnd, 110);
	GetWindowText(filedit, buffer, 1024);
	data[tmp_num].white_or_black = SendMessage(hWndListBox, LB_GETCURSEL, 0, 0);
	CardRect return_val = data[tmp_num];
	SortRect();
	return_val.cf.lpLogFont = &return_val.lf;
	bitmap_change_flag = true;
	return return_val;
}
public: int SaveAll(std::string file_name) {
	std::ofstream ofs(file_name);
	if (!ofs) {
		return 0 ;
	}
	char tmp_data_label[256];
	for (int i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		ofs  << "\"" << data[i].cur_num << "\",\"";
		int   ret = WideCharToMultiByte(
			CP_ACP,
			0,
			data[i].data_label,
			-1,
			tmp_data_label,
			200 + 1,
			NULL,
			NULL);
		ofs << tmp_data_label << "\",\"";
		ofs << data[i].margin_x << "\",\"";
		ofs << data[i].margin_y << "\",\"";
		ofs << data[i].start_x << "\",\"";
		ofs << data[i].start_y << "\",\"";
		ofs << data[i].end_x << "\",\"";
		ofs << data[i].end_y << "\",\"";
		ofs << data[i].word_list << "\",\"";
		ofs << data[i].language << "\",\"";
		ofs << data[i].threshold << "\",\"";
		ofs << data[i].gray << "\",\"";
		ofs << data[i].white_or_black << "\",\"";
		ofs << data[i].reverse << "\",\"";
		ofs << data[i].priority << "\",\"";
		ofs << data[i].vertical_align << "\",\"";
		ofs << data[i].horizontal_align << "\",\"";
		ofs << data[i].font_height <<"\",\"";
		ofs << data[i].bkColor[0] << "-" << data[i].bkColor[1] << "-" <<data[i].bkColor[2] << "\",\"";
		ofs << data[i].figure << "\",\"";
		ofs << data[i].opaque << "\",\"";
		ofs << data[i].margin_symbol << "\",\"";
		ofs << data[i].fontColor[0] << "-" << data[i].fontColor[1] << "-" <<data[i].fontColor[2] << "\",\"";
		ofs << data[i].outColor[0] << "-" << data[i].outColor[1] << "-" <<data[i].outColor[2] << "\",\"";
		ofs << data[i].font_width << "\",\"";
		ofs << data[i].outerColor[0] << "-" << data[i].outerColor[1] << "-" <<data[i].outerColor[2] << "\",\"";
		ofs << data[i].outer_opaque << "\",\"";
		ofs << data[i].outer_width << "\",\"";
		ofs << std::endl;
	}
	ofs.close();
	ofs.open(file_name+"symbol");
	for (int i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		for (int j = 0; j < 100; j++) {
			if (data[i].symbol[j].empty()) {
				ofs << ",";
			}
			else {
				ofs << "\"" << data[i].symbol[j] << "\",";
			}
			if (data[i].symbol_name[j].empty()) {
				ofs << ",";
			}
			else {
				ofs << "\"" << data[i].symbol_name[j] << "\",";

			}
			ofs << "\"";
			ofs << (int)data[i].none_color[j][0] << "|";
			ofs << (int)data[i].none_color[j][1] << "|";
			ofs << (int)data[i].none_color[j][2] << "\",\"";
			ofs << data[i].pixel_diff[j] << "\"";
			ofs << std::endl;
		}
	}
	ofs.close();
	ofs.open(file_name+"font");
	for (int i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		ofs.write((char*)&data[i].lf, sizeof(data[i].lf));
	}
	ofs.close();
	return 1;
}
public: void LoadAll(std::string file_name) {
	std::string line;
	FILE* fp;
	fopen_s(&fp,file_name.c_str(), "r");
	char buf[256];
	char field[256];
	char tmp[100][256];
	char* p;
	num = 0;
	
	int i = 0;
	if (fp) {
		while (fgets(buf, 255, fp)) {

			std::istringstream i_stream(line);
			int j = 0;
			for (p = buf; *p;) {
				p = setvalue(p, field, sizeof(field));
				sprintf_s(tmp[j], 256, "%s", field);
				field[0] = NULL;
				j++;
				// csvファイルに書き込む
			}
			data[i].cur_num = atoi(tmp[0]);
			if (data[i].cur_num != -1) {
				num++;
			}
			MultiByteToWideChar(CP_ACP, 0, tmp[1], strlen(tmp[1]), (LPWSTR)data[i].data_label,128 );
			data[i].margin_x = atoi(tmp[2]);
			data[i].margin_y = atoi(tmp[3]);
			data[i].start_x = atoi(tmp[4]);
			data[i].start_y = atoi(tmp[5]);
			data[i].end_x = atoi(tmp[6]);
			data[i].end_y = atoi(tmp[7]);
			std::string str(tmp[8], strlen(tmp[8]));
			data[i].word_list = str;
			std::string str2(tmp[9], strlen(tmp[9]));
			data[i].language = str2;
			data[i].threshold = atoi(tmp[10]);
			data[i].gray = atoi(tmp[11]);
			data[i].white_or_black = atoi(tmp[12]);
			data[i].reverse = atoi(tmp[13]);
			data[i].priority = atoi(tmp[14]);
			data[i].vertical_align = atoi(tmp[15]);
			data[i].horizontal_align = atoi(tmp[16]);
			data[i].font_height = atoi(tmp[17]);
            std::stringstream ss{ tmp[18] };
            std::string buf;
			int k = 0;
			while (std::getline(ss, buf, '-')) {
				data[i].bkColor[k] = atoi(buf.c_str());
				k++;
			}
			data[i].figure = atoi(tmp[19]);
			data[i].opaque = atoi(tmp[20]);
			data[i].margin_symbol = atoi(tmp[21]);
			k = 0;
            std::stringstream ss2{ tmp[22] };
			while (std::getline(ss2, buf, '-')) {
				data[i].fontColor[k] = atoi(buf.c_str());
				k++;
			}
			k = 0;
            std::stringstream ss3{ tmp[23] };
			while (std::getline(ss3, buf, '-')) {
				data[i].outColor[k] = atoi(buf.c_str());
				k++;
			}
			data[i].font_width = atoi(tmp[24]);
			k = 0;
            std::stringstream ss4{ tmp[25] };
			while (std::getline(ss4, buf, '-')) {
				data[i].outerColor[k] = atoi(buf.c_str());
				k++;
			}
			data[i].outer_opaque = atoi(tmp[26]);
			data[i].outer_width = atoi(tmp[27]);
			i++;
			if (i == CARD_RECT_ARRAY_SIZE) {
				break;
			}
		}
	}
	if (fp) {
		fclose(fp);
	}
	std::string file_name_symbol = file_name+"symbol";
	fopen_s(&fp,file_name_symbol.c_str(), "r");
	if (fp){
		int j = 0;
		int i = 0;
		while (fgets(buf, 255, fp)) {

			std::istringstream i_stream(line);

			int l = 0;
			for (p = buf; *p;) {
				p = setvalue(p, field, sizeof(field));
				sprintf_s(tmp[l], 256, "%s", field);
				field[0] = NULL;
				l++;
				// csvファイルに書き込む
			}
			data[i].symbol[j]  = tmp[0];
			data[i].symbol_name[j]  = tmp[1];

            std::stringstream ss{ tmp[2] };
            std::string buf;
			int k = 0;
			while (std::getline(ss, buf, '|')) {
				data[i].none_color[j][k] = atoi(buf.c_str());
				k++;
			}
			data[i].pixel_diff[j] = atoi(tmp[3]);
			j++;
			if (j == 100) {
				i++;
				j = 0;

			}
		}
		
	}
	if (fp) {
		fclose(fp);
	}
	std::ifstream ifs(file_name+"font");
	if (ifs) {
		for (int i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
			ifs.read((char*)&data[i].lf, sizeof(data[i].lf));
			data[i].cf.lpLogFont = &data[i].lf;
			data[i].cf.lStructSize = sizeof(CHOOSEFONT);
			data[i].cf.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;

		}
		ifs.close();
	}
	else {
		for (int i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
			data[i].cf.lpLogFont = &data[i].lf;
			data[i].cf.lStructSize = sizeof(CHOOSEFONT);
			data[i].cf.Flags = CF_EFFECTS | CF_SCREENFONTS;
		}

	}
	bitmap_change_flag = true;
}
public: void SortRect() {
	std::stable_sort(data, data+SIZE_OF_ARRAY(data), compCardRect);
	for (int i = 0; i < CARD_RECT_ARRAY_SIZE; i++) {
		data[i].cf.lpLogFont = &data[i].lf;
	}
}
public: void Remove(int tmp_num) {
	int i;
	if (tmp_num < 0) {
		return;
	}
	for (i = tmp_num; i < num; i++) {
		data[i] = data[i + 1];
		data[i].cur_num = i;
		data[i].cf.lpLogFont = &data[i].lf;
	}
	num--;
	data[num].cur_num = -1;
	bitmap_change_flag = true;
	  }
};
