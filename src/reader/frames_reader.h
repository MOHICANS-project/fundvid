/*
 * frames_reader.h
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_READER_FRAMES_READER_H_
#define SRC_READER_FRAMES_READER_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

/**
 * @class FramesReader
 * Interface for reading frame sequences.
 */
class FramesReader {
public:
    FramesReader(){}
    virtual ~FramesReader(){}
	/**
	 * Get the next frame in the sequence.
	 * @param frame structure where the frame content is written
	 * @return true if the frame is correctly read, false if the end of the sequence is reached
	 */
	virtual bool getNextFrame(cv::Mat & frame)=0;

	/**
	 * Get the sequence number of the last frame which has been read.
	 * @return the frame's sequence number
	 */
	virtual int getFrameIndex()=0;
};

#endif /* SRC_READER_FRAMES_READER_H_ */
