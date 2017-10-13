/*
 * sift_guided_matching.cpp
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "sift_guided_matching.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>

#include <eigen3/Eigen/Dense>
#include <opencv2/core/eigen.hpp>


namespace FVMatcher {

bool checkMatch(Eigen::Vector3d & xp, cv::Mat & Fold, double k_squared, Eigen::Matrix3d & Cov_line,Eigen::Vector3d & testpoint, int height,int width){
	//cout <<"------------------" << k_squared << endl;
	cv::Point3f pointc(xp(0),xp(1),xp(2));
	std::vector<cv::Point3f> psc;
	psc.push_back(pointc);
	std::vector<cv::Vec3f> linesc;
	computeCorrespondEpilines(psc,1,Fold,linesc);
	cv::Vec3f epilinec=linesc[0];
	float a=epilinec[0];
	float b=epilinec[1];
	float c=epilinec[2];

	Eigen::Matrix3d eFold;
	cv2eigen(Fold,eFold);
	Eigen::Vector3d l_exp=eFold*xp;
	l_exp/=l_exp.norm();

	//Conic
	Eigen::Matrix3d CC;
	CC=l_exp*l_exp.transpose()-k_squared*Cov_line;

	std::vector<cv::Point2f> vec;
	if(-height<=c/b && c/b<=0){
		cv::Point2f p1(0,-c/b);
		vec.push_back(p1);
	}
	if(-width<=c/a && c/a<=0){
		cv::Point2f p2(-c/a,0);
		vec.push_back(p2);
	}
	if(-height<=(((width)*a+c)/b) && (((width)*a+c)/b)<=0){
		cv::Point2f p3(width,-(((width)*a+c)/b));
		vec.push_back(p3);
	}
	if(-width<=(((height)*b+c)/a) && (((height)*b+c)/a)<=0){
		cv::Point2f p4(-((height)*b+c)/a,height);
		vec.push_back(p4);
	}

	if(vec.size()!=2) return false;
	int reference_sign;
	Eigen::Vector3d linepoint;
	linepoint << vec[0].x,vec[0].y,1;
	double val=linepoint.transpose()*CC*linepoint;
	reference_sign=val<0?-1:1;

	double val_test=testpoint.transpose()*CC*testpoint;
	int test_sign=val_test<0?-1:1;

	return reference_sign==test_sign;
}

void SIFTGuidedMatching::filter_band_matches(const std::vector<cv::KeyPoint> & keys0,const std::vector<cv::KeyPoint> & keys1,const std::vector< std::vector<cv::DMatch> > & initmatches, std::vector< std::vector<cv::DMatch> > & bandmatches, bool cam){
	Eigen::Matrix3d eF;
	cv::Mat F;
	eF = fu->getF();
	eigen2cv(eF,F);
	cv::Mat Ftransposed;
	Ftransposed=F.t();
	int height=image0.rows;
	int width=image1.cols;

	//std::vector<std::pair<double,double>> inliersPairs;

	for (size_t i = 0; i < initmatches.size(); ++i) {
		if (initmatches[i].empty())continue;
		std::vector<cv::DMatch> band;
		//Mantain only matches inside the band
		for (size_t mmm = 0; mmm < initmatches[i].size(); ++mmm) {
			cv::Point2f point0,point1;
			if(cam){
				point0=keys0[initmatches[i][mmm].queryIdx].pt;
				point1=keys1[initmatches[i][mmm].trainIdx].pt;
			}else{
				point0=keys0[initmatches[i][mmm].trainIdx].pt;
				point1=keys1[initmatches[i][mmm].queryIdx].pt;
			}

			//check if close to core point: TO DELETE
			bool white=false;
//			for (size_t k = 0; k < inliers0.size(); ++k) {
//				if(is_core0[k]){
//					cv::Point2f checkp=inliers0[k];
//					if(((point0.x-checkp.x)*(point0.x-checkp.x)+(point0.y-checkp.y)*(point0.y-checkp.y))<=100*100){
//						white=true;
//						break;
//					}
//				}
//			}

			if(white){
				fu->setSigma(sigmaFunction->getMinSigma());
				fu_inv->setSigma(sigmaFunction->getMinSigma());
			}else{
				double density = densityEstimator->estimateDensity(std::make_pair(point0.x, point0.y));
				double sigma = sigmaFunction->evaluateSigma(density);
				fu->setSigma(sigma);
				fu_inv->setSigma(sigma);
			}

			Eigen::Vector3d xp;
			xp << point0.x, point0.y,1;
			Eigen::Matrix3d Cov_line;
			fu->getEpipolarLineCovariance(xp, Cov_line);
			Eigen::Vector3d xptest;
			xptest << point1.x, point1.y, 1;
			if(!checkMatch(xp,F,k_squared,Cov_line,xptest,height,width)){
				if(mmm==0){
					band.clear();
					break;
				}
				continue;
			}

			Eigen::Vector3d xp2;
			xp2 << point1.x, point1.y, 1;
			Eigen::Matrix3d Cov_line2;
			fu_inv->getEpipolarLineCovariance(xp2, Cov_line2);
			Eigen::Vector3d xptest2;
			xptest2 << point0.x, point0.y, 1;
			if(!checkMatch(xp2,Ftransposed,k_squared,Cov_line2,xptest2,height,width)){
				if(mmm==0){
					band.clear();
					break;
				}
				continue;
			}
			band.push_back(initmatches[i][mmm]);
		}
		if (!band.empty()) bandmatches.push_back(band);
	}
//#ifdef DEBUG
//    std::cout << "Sigma high: " << numhigh << " vs Sigma low: " << numlow << std::endl;
//#endif
}

void SIFTGuidedMatching::filter_ratio_matches(const std::vector< std::vector<cv::DMatch> > & bandmatches, std::vector<cv::DMatch> & out_matches){
	for (size_t i = 0; i < bandmatches.size(); ++i) {
		  if(bandmatches[i].size()==1){
			  out_matches.push_back(bandmatches[i][0]);
			  continue;
		  }
		  if(bandmatches[i][0].distance<=th*bandmatches[i][1].distance){
			  out_matches.push_back(bandmatches[i][0]);
		  }
	}
}




std::vector<FVMatcher::FVMatch> SIFTGuidedMatching::computeMatches(){

	std::vector<cv::DMatch> matches;
	std::vector<cv::KeyPoint> keys0;
	std::vector<cv::KeyPoint> keys1;
	cv::Mat descriptor0;
	cv::Mat descriptor1;

	cv::Ptr<cv::FeatureDetector> detector=cv::FeatureDetector::create("SIFT");
	cv::Ptr<cv::DescriptorExtractor> extractor=cv::DescriptorExtractor::create("SIFT");
	cv::Ptr<cv::DescriptorMatcher> matcher=cv::DescriptorMatcher::create("BruteForce");
	detector->detect(image0,keys0);
	detector->detect(image1,keys1);
	extractor->compute(image0,keys0,descriptor0);
	extractor->compute(image1,keys1,descriptor1);

	std::vector< std::vector<cv::DMatch> > initmatches;
	matcher->knnMatch(descriptor0,descriptor1,initmatches,depth);
	std::vector< std::vector<cv::DMatch> > initmatches_inv;
	matcher->knnMatch(descriptor1,descriptor0,initmatches_inv,depth);


	std::vector< std::vector<cv::DMatch> > bandmatches;
	std::vector< std::vector<cv::DMatch> > bandmatches_inv;

	//Epipolar band filtered matches
	filter_band_matches(keys0,keys1,initmatches,bandmatches,true);
	filter_band_matches(keys0,keys1,initmatches_inv,bandmatches_inv,false);


	std::vector<cv::DMatch> direct_matches;
	std::vector<cv::DMatch> inverse_matches;

	//SIFT ratio filtered matches
	filter_ratio_matches(bandmatches,direct_matches);
	filter_ratio_matches(bandmatches_inv,inverse_matches);


	std::vector<int> inversecorrespondences(keys1.size(),-1);
	for (size_t i = 0; i < inverse_matches.size(); ++i) {
		inversecorrespondences[inverse_matches[i].queryIdx]=inverse_matches[i].trainIdx;
	}

	std::vector<cv::Point2f> final_matches0;
	std::vector<cv::Point2f> final_matches1;
	std::vector<double> distance;

	std::vector<cv::Point2f> cross_check0(inliers0.size());
	std::vector<cv::Point2f> cross_check1(inliers1.size());
	for (size_t i = 0; i < inliers0.size(); ++i) {
		cross_check0[i]=inliers0[i];
		cross_check1[i]=inliers1[i];
	}

	for (size_t i = 0; i < direct_matches.size(); ++i) {
		if(direct_matches[i].queryIdx==inversecorrespondences[direct_matches[i].trainIdx]){
			cv::Point2f point0=keys0[direct_matches[i].queryIdx].pt;
			cv::Point2f point1=keys1[direct_matches[i].trainIdx].pt;
			bool toadd=true;
			for (size_t kk = 0; kk < cross_check0.size(); ++kk) {
				cv::Point2f check0=cross_check0[kk];
				cv::Point2f check1=cross_check1[kk];
				if(fabs(check0.x-point0.x)<1 && fabs(check0.y-point0.y)<1 && fabs(check1.x-point1.x)<1 && fabs(check1.y-point1.y)<1){
					toadd=false;
					break;
				}
			}
			if(toadd){
				cross_check0.push_back(point0);
				cross_check1.push_back(point1);
				final_matches0.push_back(point0);
				final_matches1.push_back(point1);
				distance.push_back(direct_matches[i].distance);
			}
		}
	}

	std::vector<FVMatcher::FVMatch> out;
	for (size_t i = 0; i < final_matches0.size(); ++i) {

		std::pair<double,double> p0=std::make_pair(final_matches0[i].x,final_matches0[i].y);
		std::pair<double,double> p1=std::make_pair(final_matches1[i].x,final_matches1[i].y);
		double dist=distance[i];

		FVMatcher::FVMatch m={p0,p1,dist};
		out.push_back(m);
	}

	return out;

}

} /* namespace FVMatcher */
