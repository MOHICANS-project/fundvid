/*
 * imagesreader.h
 *
 *  Created on: 26 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SOURCE_DIRECTORY__SRC_READER_IMAGESREADER_H_
#define SOURCE_DIRECTORY__SRC_READER_IMAGESREADER_H_

#include <string>
#include <fstream>

#include "frames_reader.h"


/**
 * @class ImagesReader
 * A class for reading an image sequence stored inside a folder.
 */

class ImagesReader : public FramesReader{

	std::string folder;
	std::string base;
	std::ifstream log_file;
	std::string extension;

	int image_index;


public:
	/**
	 * Constructor for ImagesReader class.
	 * @param _folder the directory containing the sequence of images
	 * @param _base the base name of the file eg. foo1.png
	 * @param _extension the extension of the image files (look at the OpenCV documentation for the supported ones)
	 * @param _log_path the path of the log file containing the list of frame numbers of the image sequence
	 */
	ImagesReader(std::string _folder, std::string _base,std::string _extension, std::string log_path):folder(_folder),base(_base)
																								,extension(_extension){log_file.open(log_path.c_str());image_index=-1;};
    virtual ~ImagesReader(){log_file.close();}

	bool getNextFrame(cv::Mat & out);

	/**
	 * Method for re-starting the image sequence reading from the beginning.
	 */
	void reinitializeStream();

	int getFrameIndex(){
		return image_index;
	}
};

#endif /* SOURCE_DIRECTORY__SRC_READER_IMAGESREADER_H_ */
