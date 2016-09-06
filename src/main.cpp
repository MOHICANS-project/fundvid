/*
 * main.cpp
 *
 *  Created on: 26 ago 2016
 *      Author: Nicola Pellicanò
 */

/** @file main.cpp
 *
 *  This file contains the demo's
 *  main() function.
 *
 */
#include <gvars3/instances.h>

#include <iostream>
#include <cstdlib>
#include <regex>

#include "reader/imagesreader.h"
#include "matchers/sift_opencv_matcher.h"
#include "estimation/orsa_estimator.h"
#include "estimation/lm_fundmat_optimizer.h"
#include "fundamentalmat_solver.h"

#include "exceptions/estimation_error_exception.h"
#include "exceptions/optimization_exception.h"


/**
 * Main function.
 * List of input arguments:
 * 	- argv[1] config file
 * 	- argv[2] output folder
 */
int main(int argc, char **argv) {

	GVars3::GUI.LoadFile(argv[1]);
	if(argc!=3){
		std::cerr<< "Wrong number of input arguments (2 expected):" << std::endl;
		std::cerr<< "1. configuration file path" << std::endl;
		std::cerr<< "2. output folder" << std::endl;
		return 1;
	}
	std::string im0f=GVars3::GV3::get<std::string>("images0_folder");
	std::string im1f=GVars3::GV3::get<std::string>("images1_folder");
	std::string logf=GVars3::GV3::get<std::string>("log_path");

	std::string fname=GVars3::GV3::get<std::string>("first_img_name");
	if(!std::regex_match(fname.c_str(),std::regex("[A-Za-z]+\\d+[.]{1}\\w+"))){
		std::cerr << "Image file name formatting error" << std::endl;
		return 2;
	}

	std::smatch match;
	std::regex_search(fname,match,std::regex("([A-Za-z]+)\\d+[.]{1}\\w+"));
	std::string base=match[1].str();
	std::smatch match2;
	std::regex_search(fname,match2,std::regex("[A-Za-z]+\\d+[.]{1}(\\w+)"));
	std::string ext=match2[1].str();

	double alpha=GVars3::GV3::get<double>("alpha");
	if(alpha<=0 || alpha>=1) {
		std::cerr << "Error in the alpha parameter: good values are in the interval ]0,1[." << std::endl;
		return 3;
	}

	int depth=GVars3::GV3::get<int>("k_depth");
	if(depth<=0){
		std::cerr << "Error in the k_depth parameter: good values are greater than 0." << std::endl;
		return 4;
	}
	double th=GVars3::GV3::get<double>("SIFT_th");
	if(th<=0 || th>=1) {
		std::cerr << "Error in the SIFT_th parameter: good values are in the interval ]0,1[." << std::endl;
		return 5;
	}
	int num_iter=GVars3::GV3::get<int>("number_iterations");
	if(num_iter<=0) {
		std::cerr << "Error in the number_iterations parameter: good values are greater than 0." << std::endl;
		return 6;
	}
		int num_pts=GVars3::GV3::get<int>("num_pts");
	if(num_pts<=0){
		std::cerr << "Error in the num_pts parameter: good values are greater than 0." << std::endl;
		return 7;
	}
	float epsilon=GVars3::GV3::get<float>("epsilon");
	if(epsilon<=0){
		std::cerr << "Error in the epsilon parameter: good values are greater than 0." << std::endl;
		return 8;
	}
	double sigma_high=GVars3::GV3::get<double>("sigma_high");
	if(sigma_high<=0){
		std::cerr << "Error in the sigma_high parameter: good values are greater than 0." << std::endl;
		return 9;
	}
	double sigma_low=GVars3::GV3::get<double>("sigma_low");
	if(sigma_low<=0){
		std::cerr << "Error in the sigma_low parameter: good values are greater than 0." << std::endl;
		return 10;
	}
	int last_frame=GVars3::GV3::get<int>("last_frame");
	float precision=GVars3::GV3::get<float>("precision");


	ImagesReader* r1=new ImagesReader(im0f,base,ext,logf);
	ImagesReader* r2=new ImagesReader(im1f,base,ext,logf);
	cv::Mat image0,image1;
	r1->getNextFrame(image0);
	r2->getNextFrame(image1);
	FVMatcher::SIFTOpenCVMatcher* in_matcher=new FVMatcher::SIFTOpenCVMatcher(image0,image1,2,true,true,th);
	int height=image0.rows;
	int width=image1.cols;
	ORSAEStimator* orsa=new ORSAEStimator(precision,num_iter,height,width);
	LMFundMatOptimizer* optimizer=new LMFundMatOptimizer(true);



	FundamentalMatSolver* solver=new FundamentalMatSolver(r1,r2,in_matcher,orsa,optimizer,alpha,num_pts,epsilon,sigma_high,sigma_low,depth,th,last_frame);
	try{
		cv::Mat Fsol=solver->solve();
		std::ofstream out;
		std::string outfolder(argv[2]);
		std::string outfundamental=outfolder+"/f.txt";
		out.open(outfundamental.c_str());
		out << Fsol.at<float>(0,0) << " " << Fsol.at<float>(0,1) << " " << Fsol.at<float>(0,2) << std::endl;
		out << Fsol.at<float>(1,0) << " " << Fsol.at<float>(1,1) << " " << Fsol.at<float>(1,2) << std::endl;
		out << Fsol.at<float>(2,0) << " " << Fsol.at<float>(2,1) << " " << Fsol.at<float>(2,2) << std::endl;
		out.close();
		std::cout << "Output fundamental matrix:\n" << std::endl;
		std::cout << Fsol << std::endl;
		std::vector<cv::Point2f> inl0=solver->getInliersImage0();
		std::vector<cv::Point2f> inl1=solver->getInliersImage1();
		std::string outinl0=outfolder+"/inliers0.txt";
		out.open(outinl0.c_str());
		for (size_t i = 0; i < inl0.size(); ++i) {
			out << inl0[i].x << " "<<inl0[i].y << std::endl;
		}
		out.close();
		std::string outinl1=outfolder+"/inliers1.txt";
		out.open(outinl1.c_str());
		for (size_t i = 0; i < inl1.size(); ++i) {
			out << inl1[i].x << " "<<inl1[i].y << std::endl;
		}
		out.close();

	}catch(EstimationErrorException & e){
		std::cerr << e.what() << std::endl;
	}catch(OptimizationException & e){
		std::cerr << e.what() << std::endl;
	}



	delete solver;

}

