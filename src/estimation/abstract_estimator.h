/*
 * abstract_estimator.h
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_ESTIMATION_ABSTRACT_ESTIMATOR_H_
#define SRC_ESTIMATION_ABSTRACT_ESTIMATOR_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

/**
 * @class AbstractEstimator
 * Interface for fundamental matrix estimation algorithms.
 */
class AbstractEstimator {
public:
	AbstractEstimator(){};
	virtual ~AbstractEstimator(){};

	/**
	 * Estimates fundamental matrix.
	 * @param p0 keypoints from image 0
	 * @param p1 keypoints from image 1
	 * @param F  output fundamental matrix
	 * @param inl_mask mask labeling the inliers found from the estimation
	 */
	virtual void estimateF(const std::vector<cv::Point2f> & p0,const std::vector<cv::Point2f> & p1,cv::Mat & F, std::vector<bool> & inl_mask)=0;
};

#endif /* SRC_ESTIMATION_ABSTRACT_ESTIMATOR_H_ */
