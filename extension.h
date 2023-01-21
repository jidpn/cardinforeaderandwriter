#pragma once

#include "resource.h"
#include "windows.h"
#include <iostream>
extern HMENU menu;
class Extension
{
public: int getBmp() {
	return bmp;
}
public: int getDiv() {
	return div;
}
public: int getPdm() {
	return pdm;
}
public: int getPgm() {
	return pgm;
}
public: int getPpm() {
	return ppm;
}
public: int getPnm() {
	return pnm;
}
public: int getSr() {
	return sr;
}
public: int getRas() {
	return ras;
}
public: int getPng() {
	return png;
}
public: int getTiff() {
	return tiff;
}
public: int getTif() {
	return tif;
}
public: int getJp2() {
	return jp2;
}
public: int getJpeg() {
	return jpeg;
}
public: int getJpg() {
	return jpg;
}
public: int getJpe() {
	return jpe;
}
private: int bmp;
private: int div;
private: int pdm;
private: int pgm;
private: int ppm;
private: int pnm;
private: int sr;
private: int ras;
private: int png;
private: int tiff;
private: int tif;
private: int jp2;
private: int jpeg;
private: int jpe;
private: int jpg;
public: int checkExtension(std::string ext) {
	if (ext == "bmp") {
		return bmp;
	}
	else if (ext == "div") {
		return div;
	}
	else if (ext == "pdm") {
		return pdm;
	}
	else if (ext == "pgm") {
		return pgm;
	}
	else if (ext == "ppm") {
		return ppm;
	}
	else if (ext == "pnm") {
		return pnm;
	}
	else if (ext == "sr") {
		return sr;
	}
	else if (ext == "ras") {
		return ras;
	}
	else if (ext == "png") {
		return png;
	}
	else if (ext == "tiff") {
		return tiff;
	}
	else if (ext == "tif") {
		return tif;
	}
	else if (ext == "jp2") {
		return jp2;
	}
	else if (ext == "jpeg") {
		return jp2;
	}
	else if (ext == "jpg") {
		return jpg;
	}
	else if (ext == "jpe") {
		return jpe;
	}
	return 0;
}
public: void setExtension(std::string ext,int opt) {
	if (ext == "bmp") {
		setBmp(opt);
	}
	else if (ext == "div") {
		setDiv(opt);
	}
	else if (ext == "pdm") {
		setPdm(opt);
	}
	else if (ext == "pgm") {
		setPgm(opt);
	}
	else if (ext == "ppm") {
		setPpm(opt);
	}
	else if (ext == "pnm") {
		setPnm(opt);
	}
	else if (ext == "sr") {
		setSr(opt);
	}
	else if (ext == "ras") {
		setRas(opt);
	}
	else if (ext == "png") {
		setPng(opt);
	}
	else if (ext == "tiff") {
		setTiff(opt);
	}
	else if (ext == "tif") {
		setTif(opt);
	}
	else if (ext == "jp2") {
		setJp2(opt);
	}
	else if (ext == "jpeg") {
		setJpeg(opt);
	}
	else if (ext == "jpg") {
		setJpg(opt);
	}
	else if (ext == "jpe") {
		setJpe(opt);
	}
}
public:void toggleBmp() {
	if (bmp) {
		setBmp(0);
	}
	else {
		setBmp(1);

	}
}
public: void setBmp(int opt) {
	  bmp = opt;
	  if (opt) {
		CheckMenuItem(menu, ID_BMP, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_BMP, MFS_UNCHECKED);
	  }
}
public:void toggleDiv() {
	setDiv(!div);
}
public: void setDiv(int opt) {
	  div = opt;
	  if (opt) {
		CheckMenuItem(menu, ID_DIV, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_DIV, MFS_UNCHECKED);
	  }
}
public:void togglePdm() {
	setPdm(!pdm);
}
public: void setPdm(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_PDM, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_PDM, MFS_UNCHECKED);
	  }
	  pdm = opt;
}
public:void togglePgm() {
	setPgm(!pgm);
}
public: void setPgm(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_PGM, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_PGM, MFS_UNCHECKED);
	  }
	  pgm = opt;
}
public:void togglePpm() {
	setPpm(!ppm);
}
public: void setPpm(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_PPM, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_PPM, MFS_UNCHECKED);
	  }
	  ppm = opt;
}
public:void togglePnm() {
	setPnm(!pnm);
}
public: void setPnm(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_PNM, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_PNM, MFS_UNCHECKED);
	  }
	  pnm = opt;
}
public:void toggleSr() {
	setSr(!sr);
}
public: void setSr(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_SR, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_SR, MFS_UNCHECKED);
	  }
	  sr = opt;
}
public:void toggleRas() {
	setRas(!ras);
}
public: void setRas(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_RAS, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_RAS, MFS_UNCHECKED);
	  }
	  ras = opt;
}
public:void togglePng() {
	setPng(!png);
}
public: void setPng(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_PNG, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_PNG, MFS_UNCHECKED);
	  }
	  png = opt;
}
public:void toggleTiff() {
	setTiff(!tiff);
}
public: void setTiff(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_TIFF, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_TIFF, MFS_UNCHECKED);
	  }
	  tiff = opt;
}
public:void toggleTif() {
	setTif(!tif);
}
public: void setTif(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_TIF, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_TIF, MFS_UNCHECKED);
	  }
	  tif = opt;
}
public:void toggleJp2() {
	setJp2(!jp2);
}
public: void setJp2(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_JP2, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_JP2, MFS_UNCHECKED);
	  }
	  jp2 = opt;
}
public:void toggleJpeg() {
	setJpeg(!jpeg);
}
public: void setJpeg(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_JPEG, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_JPEG, MFS_UNCHECKED);
	  }
	  jpeg = opt;
}
public:void toggleJpg() {
	setJpg(!jpg);
}
public: void setJpg(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_JPG, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_JPG, MFS_UNCHECKED);
	  }
	  jpg = opt;
}
public:void toggleJpe() {
	setJpe(!jpe);
}
public: void setJpe(int opt) {
	  if (opt) {
		CheckMenuItem(menu, ID_JPE, MFS_CHECKED);
	  }
	  else {
		CheckMenuItem(menu, ID_JPE, MFS_UNCHECKED);
	  }
	jpe = opt;
}
public: std::string getAllExtensionString() {
	std::string return_string = "";
	if (bmp) {
		return_string += "bmp\n";
	}
	if (div) {
		return_string += "div\n";
	}
	if (pdm) {
		return_string += "pdm\n";
	}
	if (pgm) {
		return_string += "pgm\n";
	}
	if (ppm) {
		return_string += "ppm\n";
	}
	if (pnm) {
		return_string += "pnm\n";
	}
	if (sr) {
		return_string += "sr\n";
	}
	if (ras) {
		return_string += "ras\n";
	}
	if (png) {
		return_string += "png\n";
	}
	if (tiff) {
		return_string += "tiff\n";
	}
	if (tif) {
		return_string += "tif\n";
	}
	if (jp2) {
		return_string += "jp2\n";
	}
	if (jpeg) {
		return_string += "jpeg\n";
	}
	if (jpe) {
		return_string += "jpe\n";
	}
	if (jpg) {
		return_string += "jpg\n";
	}
	return return_string;
}

public:Extension() {
	setBmp(true);
	setDiv(true);
	setPdm(true);
	setPgm(true);
	setPpm(true);
	setPnm(true);
	setSr(true);
	setRas(true);
	setPng(true);
	setTiff(true);
	setTif(true);
	setJp2(true);
	setJpeg(true);
	setJpg(true);
	setJpe(true);

}
};
