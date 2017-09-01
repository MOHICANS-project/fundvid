/*
 * imagesreader.h
 *
 *  Created on: 8 sept 2016
 *      Author: Emi
 */

#ifndef SOURCE_DIRECTORY__SRC_READER_BOOSTREADER_H_
#define SOURCE_DIRECTORY__SRC_READER_BOOSTREADER_H_

#include <string>
#include <vector>
#include <fstream>

#include <boost/filesystem.hpp>

#include "frames_reader.h"
#include "alphanum.hpp"


/**
 * @class BoostReader
 * A class for reading automatically an image sequence stored inside a folder using Boost.
 * The images are read in natural order of their names. (i.e. im1.png < im2.png < im10.png etc)
 */

class BoostReader : public FramesReader{

	std::string folder;
	std::vector<std::string> imageNames;
    std::vector<int> frameIndices;
    std::string extension;
	std::string base;
    int firstImageIndex;
	//std::ifstream log_file;


    int dt;
    int frame_index;
	int image_index;

     int getImageIndex(std::string imgname);

public:
	/**
	 * Constructor for BoostReader class.
	 * @param _folder the directory containing the sequence of images
	 * @param _extension the extension of the image files (look at the OpenCV documentation for the supported ones); only files with the specified extension will be considered
	 * @param _base base name of an image
     * @param _dt step size for reading frame images (1 is default)
	 */
    BoostReader(std::string _folder, std::string _extension,std::string _base, std::string _first_imgname, int _dt);
    virtual ~BoostReader(){}

	bool getNextFrame(cv::Mat & out);

	int getFrameIndex(){
        return frame_index;
	}

};

#endif /* SOURCE_DIRECTORY__SRC_READER_BOOSTREADER_H_ */
