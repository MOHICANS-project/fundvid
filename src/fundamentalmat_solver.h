/*
 * fundamentalmat_solver.h
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_FUNDAMENTALMAT_SOLVER_H_
#define SRC_FUNDAMENTALMAT_SOLVER_H_


#include "reader/frames_reader.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <src/density_estimator/density_estimator.h>
#include <src/sigma_functions/sigma_function.h>
#include <memory>

#include "matchers/abstract_matcher.h"

#include "estimation/lm_fundmat_optimizer.h"
#include "estimation/abstract_estimator.h"

/**
 * @class FundamentalMatSolver
 * Class containing the template method of the proposed fundamental matrix iterative estimation algorithm.
 */

class FundamentalMatSolver {

    std::shared_ptr<FramesReader> r1;
    std::shared_ptr<FramesReader> r2;
    std::shared_ptr<FVMatcher::AbstractMatcher> init_matcher;
    std::shared_ptr<AbstractEstimator> estimator;
    std::shared_ptr<FundMatOptimizer> optimizer;
    std::shared_ptr<DensityEstimator> densityEstimator;
    std::shared_ptr<SigmaFunction> sigmaFunction;

    double alpha;
    int depth;
    double th;
    int last_frame;

    std::vector<cv::Point2f> inliers_image0;
    std::vector<cv::Point2f> inliers_image1;

#ifdef ENABLE_DEBUG
    int experiment_number;
    std::string debug_folder;
#endif

    void refinement(const cv::vector<cv::Point2f> &inl0, const cv::vector<cv::Point2f> &inl1, Eigen::Matrix3d &eF);

    void labelCorePoints(const std::vector<cv::Point2f> &matches, std::vector<bool> &is_core);

public:
    /**
     * FundamentalMatSolver constructor.
     * @param _r1 frame reader for camera 1
     * @param _r2 frame reader for camera 2
     * @param _init_matcher matcher for initial fundamental matrix estimation
     * @param _estimator fundamental matrix estimator
     * @param _optimizer generic fundamental matrix optimizer
     * @param densityEstimator estimate inliers local density
     * @param sigmaFunction provides a value for sigma as a function of the local density
     * @param _alpha confidence level epipolar band
     * @param _depth depth of the SIFT knn matcher
     * @param _th SIFT ratio threshold
     * @param _last_frame last frame to be processed
     */
    FundamentalMatSolver(std::shared_ptr<FramesReader> &r1,
                         std::shared_ptr<FramesReader> &r2,
                         std::shared_ptr<FVMatcher::AbstractMatcher> &init_matcher,
                         std::shared_ptr<AbstractEstimator> &estimator,
                         std::shared_ptr<FundMatOptimizer> &optimizer,
                         std::shared_ptr<DensityEstimator> &densityEstimator,
                         std::shared_ptr<SigmaFunction> &sigmaFunction, double _alpha, int _depth,
                         double _th, int _last_frame) :
            r1(r1), r2(r2), init_matcher(init_matcher),
            estimator(estimator), optimizer(optimizer),
            densityEstimator(densityEstimator), sigmaFunction(sigmaFunction), alpha(_alpha),
            depth(_depth), th(_th), last_frame(_last_frame) {};

    virtual ~FundamentalMatSolver() = default;

    /**
     * Solve the fundamental matrix iterative estimation problem.
     * @return the fundamental matrix computed at last iteration
     */
    cv::Mat solve(const cv::Mat &Finit = cv::Mat::zeros(3, 3, cv::DataType<double>::type));

    /**
     * Inliers from image 0 getter.
     * @return the vector of inlier points of image 0 found at the last estimation.
     */
    const std::vector<cv::Point2f> &getInliersImage0() const {
        return inliers_image0;
    }

    /**
      * Inliers from image 1 getter.
      * @return the vector of inlier points of image 1 found at the last estimation.
      */
    const std::vector<cv::Point2f> &getInliersImage1() const {
        return inliers_image1;
    }

#ifdef ENABLE_DEBUG

    void setExperimentNumber(int _experiment_number) {
        experiment_number = _experiment_number;
    }

    void setDebugFolder(char *folder) {
        debug_folder = std::string(folder);
    }

#endif
};

#endif /* SRC_FUNDAMENTALMAT_SOLVER_H_ */
