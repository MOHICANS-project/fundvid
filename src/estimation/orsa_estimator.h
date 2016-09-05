/*
 * orsa_utils.h
 *
 *  Created on: 30 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_ESTIMATION_ORSA_ESTIMATOR_H_
#define SRC_ESTIMATION_ORSA_ESTIMATOR_H_

#include "src/third_party/orsa-fundamental_1.0/src/extras/libNumerics/numerics.h"
#include "src/third_party/orsa-fundamental_1.0/src/libOrsa/match.hpp"

#include "abstract_estimator.h"

/**
 * @class ORSAEStimator
 * Facade class for interfacing with the third-party Moisan's ORSA library.
 *
 */

class ORSAEStimator : public AbstractEstimator{

	double precision;
	int max_iterations;
	int height;
	int width;
public:
	/**
	 * ORSAEStimator constructor.
	 * @param _precision the precision of the ORSA algorithm
	 * @param _max_iterations maximum number of iterations of the ORSA algorithm
	 * @param _height height of the images from which the keypoint have been extracted
	 * @param _width width of the images from which the keypoint have been extracted
	 */
	ORSAEStimator(double _precision, int _max_iterations, int _height,int _width):precision(_precision),max_iterations(_max_iterations),height(_height),width(_width){};
	virtual ~ORSAEStimator(){};

	void estimateF(const std::vector<cv::Point2f> & p0,const std::vector<cv::Point2f> & p1,cv::Mat & F, std::vector<bool> & inl_mask);
};



#endif /* SRC_ESTIMATION_ORSA_ESTIMATOR_H_ */
