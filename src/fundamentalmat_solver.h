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

#include "matchers/abstract_matcher.h"

#include "estimation/lm_fundmat_optimizer.h"
#include "estimation/abstract_estimator.h"

/**
 * @class FundamentalMatSolver
 * Class containing the template method of the proposed fundamental matrix iterative estimation algorithm.
 */

class FundamentalMatSolver {

	FramesReader* r1;
	FramesReader* r2;
	FVMatcher::AbstractMatcher* init_matcher;
	AbstractEstimator* estimator;
	FundMatOptimizer* optimizer;

	double alpha;
	int num_pts;
	double epsilon;
	double sigma_high;
	double sigma_low;
	int depth;
	double th;
	int last_frame;

	void refinement(const cv::vector<cv::Point2f> & inl0,const cv::vector<cv::Point2f> & inl1,Eigen::Matrix3d & eF);
	void labelCorePoints(const std::vector<cv::Point2f> & matches,std::vector<bool> & is_core);

public:
	/**
	 * FundamentalMatSolver constructor.
	 * @param _r1 frame reader for camera 1
	 * @param _r2 frame reader for camera 2
	 * @param _init_matcher matcher for initial fundamental matrix estimation
	 * @param _estimator fundamental matrix estimator
	 * @param _optimizer generic fundamental matrix optimizer
	 * @param _alpha confidence level epipolar band
	 * @param _num_pts clustering parameter (min number of neighbors)
	 * @param _epsilon clustering parameter (radius of the neighborhood)
	 * @param _sigma_high std for highly confident epipolar bands
	 * @param _sigma_low std for lowly confident epipolar bands
	 * @param _depth depth of the SIFT knn matcher
	 * @param _th SIFT ratio threshold
	 * @param _last_frame last frame to be processed
	 */
	FundamentalMatSolver(FramesReader* _r1,FramesReader* _r2,FVMatcher::AbstractMatcher* _init_matcher,AbstractEstimator* _estimator,FundMatOptimizer* _optimizer,double _alpha, int _num_pts, double _epsilon, double _sigma_high, double _sigma_low,int _depth,double _th,int _last_frame):
								r1(_r1),r2(_r2),init_matcher(_init_matcher),estimator(_estimator),optimizer(_optimizer),alpha(_alpha),num_pts(_num_pts),epsilon(_epsilon),sigma_high(_sigma_high),sigma_low(_sigma_low),depth(_depth),th(_th),last_frame(_last_frame){};

	virtual ~FundamentalMatSolver(){
		delete r1;
		delete r2;
		delete init_matcher;
		delete estimator;
		delete optimizer;
	};

	/**
	 * Solve the fundamental matrix iterative estimation problem.
	 * @return the fundamental matrix computed at last iteration
	 */
	 cv::Mat solve();
};

#endif /* SRC_FUNDAMENTALMAT_SOLVER_H_ */
