/*
 * sift_opencv_matcher.cpp
 *
 *  Created on: 29 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "sift_opencv_matcher.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>


namespace FVMatcher {

std::vector<FVMatcher::FVMatch> SIFTOpenCVMatcher::computeMatches(){

	//cv::initModule_nonfree();
	std::vector<FVMatcher::FVMatch> out;

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

	std::vector<cv::DMatch> matches;

	std::vector<std::vector<cv::DMatch>> initmatches;
	matcher->knnMatch(descriptor0,descriptor1,initmatches,depth);

	std::vector<cv::DMatch> direct_matches;
	for (size_t i = 0; i < initmatches.size(); ++i) {
		if(apply_th){
			if(initmatches[i].size()<2) continue;
			if(initmatches[i][0].distance<=th*initmatches[i][1].distance) direct_matches.push_back(initmatches[i][0]);
		}else{
			direct_matches.push_back(initmatches[i][0]);
		}
	}

	if(sym){

		std::vector< std::vector<cv::DMatch> > initmatches_inv;
		matcher->knnMatch(descriptor1,descriptor0,initmatches_inv,depth);

		std::vector<cv::DMatch> inverse_matches;
		for (size_t i = 0; i < initmatches_inv.size(); ++i) {
			if(apply_th){
				if(initmatches_inv[i].size()<2) continue;
				if(initmatches_inv[i][0].distance<=th*initmatches_inv[i][1].distance) inverse_matches.push_back(initmatches_inv[i][0]);
			}else{
				 inverse_matches.push_back(initmatches_inv[i][0]);
			}

		}

		std::vector<int> invcorrespondence_index(keys1.size(),-1);
		for (size_t i = 0; i < inverse_matches.size(); ++i) {
			invcorrespondence_index[inverse_matches[i].queryIdx]=inverse_matches[i].trainIdx;
		}

		for (int i = direct_matches.size()-1; i>=0; --i) {
			if(direct_matches[i].queryIdx!=invcorrespondence_index[direct_matches[i].trainIdx]){
				direct_matches.erase(direct_matches.begin()+i);
			}
		}

	}

	matches=direct_matches;

	for (size_t i = 0; i < matches.size(); ++i) {
			cv::Point2f actual0=keys0[matches[i].queryIdx].pt;
			cv::Point2f actual1=keys1[matches[i].trainIdx].pt;
			for (unsigned int j = 0; j < i; ++j) {
				cv::Point2f previous0=keys0[matches[j].queryIdx].pt;
				cv::Point2f previous1=keys1[matches[j].trainIdx].pt;
				//if(previous0==actual0 && previous1==actual1){
				if(abs(previous0.x-actual0.x)<1 && abs(previous0.y-actual0.y)<1 && abs(previous1.x-actual1.x)<1 && abs(previous1.y-actual1.y)<1){
					float distancemin=matches[j].distance<matches[i].distance?matches[j].distance:matches[i].distance;
					matches[j].distance=distancemin;
					matches.erase(matches.begin()+i);
					i--;
					//std::cout << "ALERT: double match found and removed" << std::endl;
					break;
				}
			}
	}


	for (size_t i = 0; i < matches.size(); ++i) {
		cv::Point2f actual0=keys0[matches[i].queryIdx].pt;
		cv::Point2f actual1=keys1[matches[i].trainIdx].pt;

		std::pair<double,double> p0=std::make_pair(actual0.x,actual0.y);
		std::pair<double,double> p1=std::make_pair(actual1.x,actual1.y);
		double distance=matches[i].distance;

		FVMatcher::FVMatch m={p0,p1,distance};
		out.push_back(m);
	}

#ifdef ENABLE_DEBUG
    std::string path("debug/fLM.png");
    cv::Mat outim;
    cv::drawMatches(image0,keys0,image1,keys1,matches,outim);
    cv::imwrite(path,outim);
#endif

	return out;
}



} /* namespace FVMatcher */
