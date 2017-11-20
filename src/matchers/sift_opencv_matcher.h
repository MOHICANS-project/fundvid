/*
 * sift_opencv_matcher.h
 *
 *  Created on: 29 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_MATCHERS_SIFT_OPENCV_MATCHER_H_
#define SRC_MATCHERS_SIFT_OPENCV_MATCHER_H_

#include "abstract_matcher.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

namespace FVMatcher {

/**
 * @class SIFTOpenCVMatcher
 * SIFT matcher on an image sequence using the OpenCV SIFT implementation.
 */
class SIFTOpenCVMatcher : public AbstractMatcher{

	cv::Mat & image0;
	cv::Mat & image1;
	int depth;
	bool sym;
	bool apply_th;
	double th;


public:
	/**
	 * SIFTOpenCVMatcher constructor.
	 * @param _image0 image from camera 0
	 * @param _image1 image from camera 1
	 * @param _depth  depth of the knn matching stage
	 * @param _sym set to "true" if symmetric matching has to be performed
	 * @param _apply_th set to "true" if SIFT threshold has to be applied
	 * @param _th SIFT threshold
	 */
	SIFTOpenCVMatcher(cv::Mat &_image0, cv::Mat &_image1, int _depth = 2, bool _sym = false, bool _apply_th = true,
					  double _th = 0.8) :
														image0(_image0),image1(_image1),depth(_depth>=2?_depth:2),sym(_sym),apply_th(_apply_th),th(_th>0&&_th<1?_th:0.8){};

	~SIFTOpenCVMatcher() override = default;

	std::vector<FVMatcher::FVMatch> computeMatches() override;
};

} /* namespace FVMatcher */

#endif /* SRC_MATCHERS_SIFT_OPENCV_MATCHER_H_ */
