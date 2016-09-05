/*
 * fundmat_optimizer.h
 *
 *  Created on: 30 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_ESTIMATION_FUNDMAT_OPTIMIZER_H_
#define SRC_ESTIMATION_FUNDMAT_OPTIMIZER_H_



/**
 * @class FundMatOptimizer
 * Generic interface for fundamental matrix optimization.
 */
class FundMatOptimizer {

public:

	FundMatOptimizer(){};
	virtual ~FundMatOptimizer(){};
	/**
	 * Optimize the fundamental matrix given in input.
	 * @param F input fundamental matrix, it will contain the optimized version at the end
	 * @param p0 inlier points from image 0
	 * @param p1 inlier points from image 1
	 * @param n number of points
	 */
	virtual void optFundMat(double F[3][3], double **p0, double **p1,int n)=0;

	/**
	 * Get the covariance matrix representing the uncertainty of the F estimation
	 * @param the 9x9 covariance matrix to fill
	 */
	virtual void getCovarF(double Cov[9][9])=0;

};

#endif /* SRC_ESTIMATION_FUNDMAT_OPTIMIZER_H_ */
