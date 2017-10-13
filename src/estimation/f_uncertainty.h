/*
 * FUncertainty.h
 *
 * Utility methods for computing F covariance matrix, and Epipolar lines' covariance
 *
 *  Created on: 26 feb 2016
 *      Author: nicola
 */

#ifndef SOURCE_DIRECTORY__SRC_FUNCERTAINTY_H_
#define SOURCE_DIRECTORY__SRC_FUNCERTAINTY_H_

#include <eigen3/Eigen/Dense>
#include <vector>

/**
 * @class FUncertainty
 * Class which manages the uncertainty of the F estimation.
 */
class FUncertainty {

	Eigen::Matrix3d F;
	Eigen::Matrix<double,9,9> Cov;
	double sigma;

	double geometricError(Eigen::Vector3d ep0h,Eigen::Vector3d ep1h);

public:
	/**
	 * FUncertainty constructor.
	 * @param _F fundamental matrix
	 * @param _Cov fundamental matrix covariance
	 * @param _sigma standard deviation used in epipolar band covariance
	 */
	FUncertainty(Eigen::Matrix3d _F,Eigen::Matrix<double,9,9> _Cov,double _sigma):
			F(_F), Cov(_Cov), sigma(_sigma) {}

	virtual ~FUncertainty() {}

	/**
	 * Evaluates the covariance of the epipolar line drawn from the given point.
	 * @param x the point in the first image
	 * @param Cov_line the output covariance of the epipolar line F*x
	 */
	void getEpipolarLineCovariance(const Eigen::Vector3d & x, Eigen::Matrix<double,3,3> & Cov_line);

	/**
	 * Sigma parameter setter.
	 * @param sigma the new epipolar band standard deviation
	 */
	void setSigma(double sigma) {
		this->sigma = sigma;
	}
	/**
	 * Fundamental matrix getter.
	 * @return the fundamental matrix
	 */
	const Eigen::Matrix3d& getF() const {
		return F;
	}
};

#endif /* SOURCE_DIRECTORY__SRC_FUNCERTAINTY_H_ */
