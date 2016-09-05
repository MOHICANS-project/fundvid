/*
 * imagesreader.cpp
 *
 *  Created on: 26 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "imagesreader.h"

bool ImagesReader::getNextFrame(cv::Mat & out){
	int idx;
	if(log_file>>idx){
		std::string imgpath=folder+"/"+base+std::to_string(idx)+"."+extension;
		out=cv::imread(imgpath,0);
		image_index=idx;
		return true;
	}
	return false;

}

void ImagesReader::reinitializeStream(){
	log_file.clear();
	log_file.seekg(0);
}
