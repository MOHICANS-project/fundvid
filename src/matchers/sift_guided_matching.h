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
#include <src/sigma_functions/sigma_function.h>
#include <src/density_estimator/density_estimator.h>
#include <memory>


namespace FVMatcher {

/**
 * @class SIFTGuidedMatching
 * Implementation of the proposed guided SIFT matching strategy.
 */

    class SIFTGuidedMatching : public AbstractMatcher {

        std::shared_ptr<FUncertainty> fu;
        std::shared_ptr<FUncertainty> fu_inv;
        cv::Mat image0;
        cv::Mat image1;
        std::vector<cv::Point2f> inliers0;
        std::vector<cv::Point2f> inliers1;
        std::vector<bool> is_core0;
        std::vector<bool> is_core1;

        std::shared_ptr<DensityEstimator> densityEstimator;
        std::shared_ptr<SigmaFunction> sigmaFunction;

        int depth;
        double th;
        double k_squared;

        void filter_band_matches(const std::vector<cv::KeyPoint> &keys0, const std::vector<cv::KeyPoint> &keys1,
                                 const std::vector<std::vector<cv::DMatch> > &initmatches,
                                 std::vector<std::vector<cv::DMatch> > &bandmatches, bool cam);

        void filter_ratio_matches(const std::vector<std::vector<cv::DMatch> > &bandmatches,
                                  std::vector<cv::DMatch> &out_matches);

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
         * @param densityEstimator estimate inliers local density
         * @param sigmaFunction provides a value for sigma as a function of the local density
         * @param _depth  depth of the knn matching stage
         * @param _th SIFT threshold
         * @param _k_squared k^2 parameter epipolar line covariance
         */
        SIFTGuidedMatching(std::shared_ptr<FUncertainty> &_fu, std::shared_ptr<FUncertainty> &_fu_inv, cv::Mat &_image0,
                           cv::Mat &_image1,
                           std::vector<cv::Point2f> _inliers0, std::vector<cv::Point2f> _inliers1,
                           std::vector<bool> _is_core0, std::vector<bool> _is_core1,
                           std::shared_ptr<DensityEstimator> &densityEstimator,
                           std::shared_ptr<SigmaFunction> &sigmaFunction, int _depth, double _th,
                           double _k_squared) :
                fu(_fu), fu_inv(_fu_inv), image0(_image0), image1(_image1),
                inliers0(std::move(_inliers0)), inliers1(std::move(_inliers1)), is_core0(std::move(_is_core0)),
                is_core1(std::move(_is_core1)), densityEstimator(densityEstimator),
                sigmaFunction(sigmaFunction), depth(_depth), th(_th), k_squared(_k_squared) {};

        ~SIFTGuidedMatching() override = default;;

        std::vector<FVMatcher::FVMatch> computeMatches() override;

    };

} /* namespace FVMatcher */

#endif /* SRC_MATCHERS_SIFT_GUIDED_MATCHING_H_ */
