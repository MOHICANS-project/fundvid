/*
 * sift_guided_matching.h
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_MATCHERS_SIFT_GUIDED_MATCHING_H_
#define SRC_MATCHERS_SIFT_GUIDED_MATCHING_H_

#include "abstract_matcher.h"
#include "src/estimation/f_uncertainty.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

namespace FVMatcher {

/**
 * @class SIFTGuidedMatching
 * Implementation of the proposed guided SIFT matching strategy.
 */

class SIFTGuidedMatching : public AbstractMatcher{

	FUncertainty & fu;
	FUncertainty & fu_inv;
	cv::Mat & image0;
	cv::Mat & image1;
	std::vector<cv::Point2f> & inliers0;
	std::vector<cv::Point2f> & inliers1;
	std::vector<bool> & is_core0;
	std::vector<bool> & is_core1;

	int depth;
	float th;
	double sigma_high;
	double sigma_low;
	double epsilon;
	double num_pts;
	double k_squared;

	void filter_band_matches(const std::vector<cv::KeyPoint> & keys0,const std::vector<cv::KeyPoint> & keys1,const std::vector< std::vector<cv::DMatch> > & initmatches, std::vector< std::vector<cv::DMatch> > & bandmatches, bool cam);
	void filter_ratio_matches(const std::vector< std::vector<cv::DMatch> > & bandmatches, std::vector<cv::DMatch> &  out_matches);
public:
	/**
	 * SIFTGuidedMatching constructor.
	 * @param _fu uncertainty object from camera 0 to 1
	 * @param _fu_inv uncertainty object from camera 1 to 0
	 * @param _image0 image from camera 0
	 * @param _image1 image from camera 1
	 * @param _inliers0 inlier points from image 0
	 * @param _inliers1 inlier points from image 1
	 * @param _is_core0 core points labels from image 0
	 * @param _is_core1 core points labels from image 1
	 * @param _depth  depth of the knn matching stage
	 * @param _th SIFT threshold
	 * @param _sigma_high std for highly confident epipolar bands
	 * @param _sigma_low std for lowly confident epipolar bands
	 * @param _epsilon clustering parameter (radius of the neighborhood)
	 * @param _num_pts clustering parameter (minimum number of neighbors)
	 * @param _k_squared k^2 parameter epipolar line covariance
	 */
	SIFTGuidedMatching(FUncertainty & _fu,FUncertainty & _fu_inv,cv::Mat & _image0,cv::Mat & _image1,std::vector<cv::Point2f> & _inliers0,std::vector<cv::Point2f> & _inliers1,std::vector<bool> & _is_core0,std::vector<bool> & _is_core1,int _depth,float _th,double _sigma_high, double _sigma_low, double _epsilon, double _num_pts, double _k_squared):
						fu(_fu),fu_inv(_fu_inv),image0(_image0),image1(_image1),inliers0(_inliers0),inliers1(_inliers1),is_core0(_is_core0),is_core1(_is_core1),depth(_depth),th(_th),sigma_high(_sigma_high),sigma_low(_sigma_low),epsilon(_epsilon),num_pts(_num_pts),k_squared(_k_squared){};
	virtual ~SIFTGuidedMatching(){};

	std::vector<FVMatcher::FVMatch> computeMatches();

};

} /* namespace FVMatcher */

#endif /* SRC_MATCHERS_SIFT_GUIDED_MATCHING_H_ */
