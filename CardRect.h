#pragma once
#include <string>
#include <tchar.h>
#include <opencv2/opencv.hpp>
#include "windows.h"
#define WHITE_LIST 0
#define BLACK_LIST 1
#define SYMBOL_NUM 100
class CardRect
{
public: int cur_num;
public: std::string symbol[SYMBOL_NUM];
public: std::string symbol_name[SYMBOL_NUM];
public: cv::Vec3b none_color[SYMBOL_NUM];
public: int pixel_diff[SYMBOL_NUM];
public: TCHAR data_label[50] = TEXT("");
public: int margin_x;
public: int margin_y;
public: int start_x;
public: int start_y;
public: int margin_symbol;
public: int end_x;
public: int end_y;
public: int white_or_black;
public: std::string word_list;
public: std::string language;
public: int threshold;
public: int reverse;
public: int gray;
public: int priority;
public: LOGFONT lf;
public: CHOOSEFONT cf;
public: int vertical_align;
public: int horizontal_align;
public: int font_height;
public: int bkColor[3];
public: int fontColor[3];
public: int outColor[3];
public: int outerColor[3];
public: int font_width;
public: int figure;
public: int opaque;
public: int outer_opaque;
public: int outer_width;
private: void initSymbolData() {
	int i;
	for (i = 0; i < SYMBOL_NUM; i++) {
		symbol[i] = "";
		symbol_name[i] = "";
		none_color[i][0] = 255;
		none_color[i][1] = 255;
		none_color[i][2] = 255;
		pixel_diff[i] = 1;
	}
}
public: CardRect() {
		  start_x = 0;
		  start_y = 0;
		  margin_x = 0;
		  margin_y = 0;
		  vertical_align = 0;
		  horizontal_align = 0;
		  font_height = -1;
		  bkColor[0] = 255;
		  bkColor[1] = 255;
		  bkColor[2] = 255;
		  fontColor[0] = 255;
		  fontColor[1] = 255;
		  fontColor[2] = 255;
		  outColor[0] = 255;
		  outColor[1] = 255;
		  outColor[2] = 255;
		  outerColor[0] = 255;
		  outerColor[1] = 255;
		  outerColor[2] = 255;
		  font_width = 1;

		  end_x = 0;
		  end_y = 0;
		  cur_num = -1;
		  white_or_black= BLACK_LIST;
		  word_list = "";
		  threshold = 80;
		  priority = INT_MAX;
		  language = "jpn";
		  gray = false;
		  reverse = false;
		  cf.lStructSize = sizeof(CHOOSEFONT);
		  cf.lpLogFont = &lf;
		  cf.Flags = CF_EFFECTS | CF_SCREENFONTS ;
		  figure = 0;
		  opaque = 0;
		  outer_opaque = 0;
		  outer_width = 1;
		  margin_symbol = 0;
		  initSymbolData();

}
public:	  CardRect(std::string initial_language) {
		  start_x = 0;
		  start_y = 0;
		  margin_x = 0;
		  margin_y = 0;
		  vertical_align = 0;
		  horizontal_align = 0;
		  font_height = -1;
		  end_x = 0;
		  end_y = 0;
		  cur_num = -1;
		  white_or_black= BLACK_LIST;
		  word_list = "";
		  threshold = 80;
		  priority = INT_MAX;
		  gray = false;
		  reverse = false;
		  language = initial_language;
		  cf.lStructSize = sizeof(CHOOSEFONT);
		  cf.lpLogFont = &lf;
		  cf.Flags = CF_EFFECTS | CF_SCREENFONTS ;
		  bkColor[0] = 255;
		  bkColor[1] = 255;
		  bkColor[2] = 255;
		  fontColor[0] = 255;
		  fontColor[1] = 255;
		  fontColor[2] = 255;
		  outColor[0] = 255;
		  outColor[1] = 255;
		  outColor[2] = 255;
		  outerColor[0] = 255;
		  outerColor[1] = 255;
		  outerColor[2] = 255;
		  font_width = 1;
		  end_x = 0;
		  end_y = 0;
		  cur_num = -1;
		  white_or_black= BLACK_LIST;
		  word_list = "";
		  threshold = 80;
		  priority = INT_MAX;
		  language = "jpn";
		  gray = false;
		  reverse = false;
		  cf.lStructSize = sizeof(CHOOSEFONT);
		  cf.lpLogFont = &lf;
		  cf.Flags = CF_EFFECTS | CF_SCREENFONTS ;
		  figure = 0;
		  opaque = 0;
		  outer_opaque = 0;
		  outer_width = 1;
		  margin_symbol = 0;
		  initSymbolData();

}
public:	   CardRect(int x, int y,std::string initial_language) {
		   start_x = x;
		   start_y = y;
		  margin_x = 0;
		  margin_y = 0;
		  vertical_align = 0;
		  horizontal_align = 0;
		  font_height = -1;
		   end_x = x;
		   end_y = y;
		   cur_num = -1;
		   white_or_black = BLACK_LIST;
		  threshold = 80;
		  language = initial_language;
		  priority = INT_MAX;
		  gray = false;
		  reverse = false;
		  word_list = "";
		  cf.lStructSize = sizeof(CHOOSEFONT);
		  cf.lpLogFont = &lf;
		  cf.Flags = CF_EFFECTS | CF_SCREENFONTS ;
		  bkColor[0] = 255;
		  bkColor[1] = 255;
		  bkColor[2] = 255;
		  fontColor[0] = 255;
		  fontColor[1] = 255;
		  fontColor[2] = 255;
		  outColor[0] = 255;
		  outColor[1] = 255;
		  outColor[2] = 255;
		  outerColor[0] = 255;
		  outerColor[1] = 255;
		  outerColor[2] = 255;
		  font_width = 1;
		  figure = 0;
		  opaque = 0;
		  outer_opaque = 0;
		  outer_width = 1;
		  margin_symbol = 0;
		  initSymbolData();
		  
	   }
public: void update(int x, int y) {
	end_x = x;
	end_y = y;
}
	  public: void finalize(int x, int y) {
		  end_x = x;
		  end_y = y;
		  int tmp;
		  if (start_x > end_x) {
			  tmp = start_x;
			  start_x = end_x;
			  end_x = tmp;
		  }
		  if (start_y > end_y) {
			  tmp = start_y;
			  start_y = end_y;
			  end_y = tmp;
		  }
	  }

};
bool compCardRect(const CardRect& a, const CardRect& b);
