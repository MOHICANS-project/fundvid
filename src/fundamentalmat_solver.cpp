/*
 * fundamentalmat_solver.cpp
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "fundamentalmat_solver.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <regex>
#include <opencv2/nonfree/nonfree.hpp>
#include <eigen3/Eigen/Dense>
#include <opencv2/core/eigen.hpp>

#include "src/estimation/f_uncertainty.h"
#include "src/matchers/sift_guided_matching.h"

#include "src/exceptions/estimation_error_exception.h"
#include "src/exceptions/optimization_exception.h"


void rawToPoint2f(const std::vector<FVMatcher::FVMatch> & init_matches,std::vector<cv::Point2f> & out0,std::vector<cv::Point2f> & out1){
	for (size_t i = 0; i < init_matches.size(); ++i) {
		cv::Point2f p0(init_matches[i].p0.first,init_matches[i].p0.second);
		cv::Point2f p1(init_matches[i].p1.first,init_matches[i].p1.second);
		out0.push_back(p0);
		out1.push_back(p1);
	}
}

void FundamentalMatSolver::refinement(const cv::vector<cv::Point2f> & inl0,const cv::vector<cv::Point2f> & inl1,Eigen::Matrix3d & eF){
	double Farr[3][3];
	Farr[0][0]=eF(0,0);Farr[0][1]=eF(0,1);Farr[0][2]=eF(0,2);
	Farr[1][0]=eF(1,0);Farr[1][1]=eF(1,1);Farr[1][2]=eF(1,2);
	Farr[2][0]=eF(2,0);Farr[2][1]=eF(2,1);Farr[2][2]=eF(2,2);
	double** points0=new double*[inl0.size()];
	double** points1=new double*[inl1.size()];
	for (size_t i = 0; i < inl0.size(); ++i) {
		points0[i]=new double[2];
		points1[i]=new double[2];
	}
	for (size_t i = 0; i < inl0.size(); ++i) {
		points0[i][0]=inl0[i].x;
		points0[i][1]=inl0[i].y;
		points1[i][0]=inl1[i].x;
		points1[i][1]=inl1[i].y;
	}

	try{
		optimizer->optFundMat(Farr,points0,points1,inl0.size());
	}catch(OptimizationException & e){
		throw e;
	}

	for (size_t i = 0; i < inl0.size(); ++i) {
		delete[] points0[i];
		delete[] points1[i];
	}
	delete[] points0;
	delete[] points1;

	eF(0,0)=Farr[0][0];eF(0,1)=Farr[0][1];eF(0,2)=Farr[0][2];
	eF(1,0)=Farr[1][0];eF(1,1)=Farr[1][1];eF(1,2)=Farr[1][2];
	eF(2,0)=Farr[2][0];eF(2,1)=Farr[2][1];eF(2,2)=Farr[2][2];

}


void FundamentalMatSolver::labelCorePoints(const std::vector<cv::Point2f> & matches,std::vector<bool> & is_core){

	for (size_t i = 0; i < matches.size(); ++i) {
			cv::Point2f testp=matches[i];
			int count=0;
			is_core[i]=false;
			for (size_t j = 0; j < matches.size(); ++j) {
				if(i==j) continue;
				cv::Point2f checkp=matches[j];
				if(((testp.x-checkp.x)*(testp.x-checkp.x)+(testp.y-checkp.y)*(testp.y-checkp.y))<=epsilon*epsilon){
					count++;
				}
				if(count >=num_pts){
					is_core[i]=true;
					break;
				}
			}
	}

}

void permuteCov(const Eigen::Matrix<double,9,9> & inCov, const std::vector<int> permutation, Eigen::Matrix<double,9,9> & outCov){
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			outCov(i,j)=inCov(permutation[i],permutation[j]);
		}
	}
}

cv::Mat FundamentalMatSolver::solve(){

	cv::initModule_nonfree();
	srand((unsigned int)time(0));

	double k_squared=-2*log(1.0f-alpha);

	std::vector<FVMatcher::FVMatch> init_matches=init_matcher->computeMatches();
	std::vector<cv::Point2f> ptt0;
	std::vector<cv::Point2f> ptt1;
	rawToPoint2f(init_matches,ptt0,ptt1);

	std::vector<int> permutation(9);
	permutation[0]=0;permutation[1]=3;permutation[2]=6;
	permutation[3]=1;permutation[4]=4;permutation[5]=7;
	permutation[6]=2;permutation[7]=5;permutation[8]=8;

	//Initial estimation of the F matrix
	std::cout << "Estimation frame "<< r1->getFrameIndex() << " in progress..." << std::endl;
	cv::Mat F;
	F=cv::Mat::zeros(3,3,cv::DataType<double>::type);

	std::vector<bool> mask(ptt0.size());
	try{
		estimator->estimateF(ptt0,ptt1,F,mask);
	}catch(EstimationErrorException & e){
		throw e;
	}
	Eigen::Matrix3d eF;
	cv2eigen(F,eF);

	//Select only inliers as current matches set
	std::vector<cv::Point2f> currentMatches0;
	std::vector<cv::Point2f> currentMatches1;

	for (size_t i = 0; i < mask.size(); ++i) {
		if(mask[i]){
			currentMatches0.push_back(ptt0[i]);
			currentMatches1.push_back(ptt1[i]);
		}
	}

	refinement(currentMatches0,currentMatches1,eF);
	eigen2cv(eF,F);

	Eigen::Matrix<double,9,9> ecurrentCov;
	double raw_covar[9][9];
	optimizer->getCovarF(raw_covar);
	for (size_t i = 0; i < 9; ++i) {
		for (size_t j = 0; j < 9; ++j) {
			ecurrentCov(i,j)=raw_covar[i][j];
		}
	}
	cv::Mat currentCov=cv::Mat(9,9, CV_64FC1);
	eigen2cv(ecurrentCov,currentCov);

	cv::Mat image0,image1;

	inliers_image0.clear();
	inliers_image1.clear();
	for (size_t i = 0; i < currentMatches0.size(); ++i) {
		inliers_image0.push_back(currentMatches0[i]);
		inliers_image1.push_back(currentMatches1[i]);
	}


	while(r1->getNextFrame(image0) && r2->getNextFrame(image1) && (last_frame<0 || r2->getFrameIndex()<=last_frame)){

		std::cout << "Estimation frame "<< r1->getFrameIndex() << " in progress..." << std::endl;

		//Detect core points
		std::vector<bool> is_core0(currentMatches0.size());
		labelCorePoints(currentMatches0,is_core0);
		std::vector<bool> is_core1(currentMatches1.size());
		labelCorePoints(currentMatches1,is_core1);

		FUncertainty func(eF,ecurrentCov,sigma_high);
		Eigen::Matrix3d eFoldtransposed=eF.transpose()*1;
		Eigen::Matrix<double,9,9> eCovoldtransposed;
		permuteCov(ecurrentCov,permutation,eCovoldtransposed);
		FUncertainty func2(eFoldtransposed,eCovoldtransposed,sigma_high);

		int tot=0;
		for (size_t i = 0; i < is_core0.size(); ++i) {
			if(is_core0[i])tot++;
		}
		//std::cout<< "Starting guided matching with " << tot << " core points" << std::endl;
		FVMatcher::SIFTGuidedMatching guided=FVMatcher::SIFTGuidedMatching(func,func2,image0,image1,currentMatches0,currentMatches1,is_core0,is_core1,depth,th,sigma_high,sigma_low,epsilon,num_pts,k_squared);

		std::vector<FVMatcher::FVMatch> new_matches=guided.computeMatches();

		std::vector<cv::Point2f> new_p0;
		std::vector<cv::Point2f> new_p1;
		rawToPoint2f(new_matches,new_p0,new_p1);


		std::vector<cv::Point2f> tot0;
		std::vector<cv::Point2f> tot1;
		for (size_t i = 0; i < currentMatches0.size(); ++i) {
			tot0.push_back(currentMatches0[i]);
			tot1.push_back(currentMatches1[i]);
		}
		for (size_t i = 0; i < new_p0.size(); ++i) {
			tot0.push_back(new_p0[i]);
			tot1.push_back(new_p1[i]);
		}

		std::vector<bool> mask2(tot0.size());
		F=cv::Mat::zeros(3,3,cv::DataType<double>::type);

		try{
			estimator->estimateF(tot0,tot1,F,mask2);
		}catch(EstimationErrorException & e){
			throw e;
		}
		cv2eigen(F,eF);

		currentMatches0.clear();
		currentMatches1.clear();
		for (size_t i = 0; i < mask2.size(); ++i) {
			if(mask2[i]){
				currentMatches0.push_back(tot0[i]);
				currentMatches1.push_back(tot1[i]);
			}
		}

		inliers_image0.clear();
		inliers_image1.clear();
		for (size_t i = 0; i < currentMatches0.size(); ++i) {
			inliers_image0.push_back(currentMatches0[i]);
			inliers_image1.push_back(currentMatches1[i]);
		}

		refinement(currentMatches0,currentMatches1,eF);
		eigen2cv(eF,F);
		optimizer->getCovarF(raw_covar);
		for (size_t i = 0; i < 9; ++i) {
			for (size_t j = 0; j < 9; ++j) {
				ecurrentCov(i,j)=raw_covar[i][j];
			}
		}
		eigen2cv(ecurrentCov,currentCov);
	}

	return F;
}
