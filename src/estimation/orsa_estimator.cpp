/*
 * orsa_utils.cpp
 *
 *  Created on: 30 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "orsa_estimator.h"
#include "src/third_party/orsa-fundamental_1.0/src/libOrsa/fundamental_model.hpp"
#include "src/exceptions/estimation_error_exception.h"

void point2ftoOrsaMatch(const std::vector<cv::Point2f> & in0,const std::vector<cv::Point2f> & in1,std::vector<Match> & out){
	for (size_t i = 0; i < in0.size(); ++i) {
		Match m;
		m.x1=in0[i].x;
		m.y1=in0[i].y;
		m.x2=in1[i].x;
		m.y2=in1[i].y;
		out.push_back(m);
	}
}

std::pair<double,double> stats(const std::vector<Match>& vec_matchings,
                                              const std::vector<int>& vec_inliers,
                                              const libNumerics::matrix<double>& F) {
    std::vector<int>::const_iterator it=vec_inliers.begin();
    double l2=0, linf=0, l2m=0,linfm=0;
    //cout << "Suoi:" << endl;
    for(; it!=vec_inliers.end(); ++it) {
      const Match& m=vec_matchings[*it];
      //cout << m.x1 << " " << m.y1 << " " << m.x2 << m.y2 << endl;
      double a = F(0,0) * m.x1 + F(1,0) * m.y1 + F(2,0);
      double b = F(0,1) * m.x1 + F(1,1) * m.y1 + F(2,1);
      double c = F(0,2) * m.x1 + F(1,2) * m.y1 + F(2,2);
      double d = a*m.x2 + b*m.y2 + c;
      double e =  (d*d) / (a*a + b*b);

      double am = F(0,0) * m.x2 + F(0,1) * m.y2 + F(0,2);
      double bm = F(1,0) * m.x2 + F(1,1) * m.y2 + F(1,2);
      //double cm = F(2,0) * m.x2 + F(2,1) * m.y2 + F(2,2);
      double emine=(sqrt(e)+sqrt((d*d) / (am*am + bm*bm)))/2;

      l2 += e; l2m+=emine*emine;
      if(linf < e)
        linf = e;
      if(linfm < emine*emine)
        linfm = emine*emine;

    }
    std::pair<double,double> err(sqrt(l2/vec_inliers.size()),sqrt(linf));
    std::pair<double,double> err2(sqrt(l2m/vec_inliers.size()),sqrt(linfm));
    return err2;
}



bool exec_orsa(const std::vector<Match>& vec_matchings, int w1,int h1, int w2,int h2,
          double precision,
          libNumerics::matrix<double>& F, std::vector<int>& vec_inliers, int numiter)
{
  const int n = static_cast<int>(vec_matchings.size());
  if(n < 7)
  {
    std::cerr << "Error: ORSA needs 7 matches or more to proceed" <<std::endl;
    return false;
  }
  libNumerics::matrix<double> xA(2,n), xB(2,n);

  for (int i=0; i < n; ++i)
  {
    xA(0,i) = vec_matchings[i].x1;
    xA(1,i) = vec_matchings[i].y1;
    xB(0,i) = vec_matchings[i].x2;
    xB(1,i) = vec_matchings[i].y2;
  }

  orsa::FundamentalModel model(xA, w1, h1, xB, w2, h2,true);
  //model.setConvergenceCheck(true);

  if(model.orsa(vec_inliers, numiter, &precision, &F, false)>0.0)
    return false;
  std::pair<double,double> err; // (RMSE,max)
  err = stats(vec_matchings, vec_inliers, F);
  libNumerics::matrix<double> F2(3,3);
  if( model.ComputeModel(vec_inliers,&F2) ) // Re-estimate with all inliers
  {
    double maxBefore = err.second;
    if(stats(vec_matchings, vec_inliers, F2).first <= maxBefore)F = F2;

  } else
    std::cerr << "Warning: error in refinement, result is suspect" <<std::endl;
  return true;
}

void ORSAEStimator::estimateF(const std::vector<cv::Point2f> & p0,const std::vector<cv::Point2f> & p1,cv::Mat & F, std::vector<bool> & inl_mask){
	libNumerics::matrix<double> Fx(3,3); //Used by ORSA library
	std::vector<int> vec_inliers;
	std::vector<Match> vec_matchings;
	point2ftoOrsaMatch(p0,p1,vec_matchings);

	bool ok=exec_orsa(vec_matchings,width,height,width,height,precision,Fx,vec_inliers,max_iterations);
	if(!ok){
		throw EstimationErrorException();
	}

	F.at<double>(0,0)=Fx(0,0);F.at<double>(0,1)=Fx(0,1);F.at<double>(0,2)=Fx(0,2);
	F.at<double>(1,0)=Fx(1,0);F.at<double>(1,1)=Fx(1,1);F.at<double>(1,2)=Fx(1,2);
	F.at<double>(2,0)=Fx(2,0);F.at<double>(2,1)=Fx(2,1);F.at<double>(2,2)=Fx(2,2);
	F=F.t();

	for (size_t i = 0; i < vec_inliers.size(); ++i) {
		inl_mask[vec_inliers[i]]=true;
	}

}
