/*
 * lm_fundmat_optimizer.h
 *
 *  Created on: 30 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_ESTIMATION_LM_FUNDMAT_OPTIMIZER_H_
#define SRC_ESTIMATION_LM_FUNDMAT_OPTIMIZER_H_

#include "fundmat_optimizer.h"
#include <eigen3/Eigen/Dense>

/**
 * @class LMFundMatOptimizer
 * Uses Levenberg-Marquardt algorithm to optimize a fundamental matrix as a 7-parameters vector.
 */
class LMFundMatOptimizer : public FundMatOptimizer{

	bool derivative;
	Eigen::Matrix<double,9,9> covarF;
public:
	/**
	 * LMFundMatOptimizer constructor.
	 * @param _derivative if true analytical jacobian is computed, otherwise the numerical one is used
	 */
	LMFundMatOptimizer(bool _derivative):derivative(_derivative){};
	virtual ~LMFundMatOptimizer(){};
	void optFundMat(double F[3][3], double **p0, double **p1,int n);


	void getCovarF(double Cov[9][9]);
};

#endif /* SRC_ESTIMATION_LM_FUNDMAT_OPTIMIZER_H_ */
