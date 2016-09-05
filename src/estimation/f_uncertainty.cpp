/*
 * FUncertainty.cpp
 *
 *  Created on: 26 feb 2016
 *      Author: nicola
 */

#include "f_uncertainty.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>


double FUncertainty::geometricError(Eigen::Vector3d ep0h,Eigen::Vector3d ep1h){
	double numerator=((ep1h.transpose())*F*ep0h)(0);
	Eigen::Vector3d epi1=F*ep0h;
	Eigen::Vector3d epi2=F.transpose()*ep1h;
	double denumerator1=epi1(0)*epi1(0)+epi1(1)*epi1(1);
	double denumerator2=epi2(0)*epi2(0)+epi2(1)*epi2(1);
	denumerator1=sqrt(denumerator1);
	denumerator2=sqrt(denumerator2);
	double error1=abs(numerator/denumerator1);
	double error2=abs(numerator/denumerator2);
	double error=(error1+error2)/2;
	return error;
}

void FUncertainty::getEpipolarLineCovariance(const Eigen::Vector3d & x, Eigen::Matrix<double,3,3> & Cov_line){

	//It's all about evaluating the Jacobian
	Eigen::Matrix<double,3,9> J;

	Eigen::Vector3d l=F*x;

	double norm=l.norm();
	//cout << norm << endl;
	double L1=l(0);
	double L2=l(1);
	double L3=l(2);

	J(0,0)=x(0)*(1-(L1*L1)/(norm*norm));
	J(0,1)=x(1)*(1-(L1*L1)/(norm*norm));
	J(0,2)=(1-(L1*L1)/(norm*norm));
	J(0,3)=-(x(0)*L1*L2)/(norm*norm);
	J(0,4)=-(x(1)*L1*L2)/(norm*norm);
	J(0,5)=-(L1*L2)/(norm*norm);
	J(0,6)=-(x(0)*L1*L3)/(norm*norm);
	J(0,7)=-(x(1)*L1*L3)/(norm*norm);
	J(0,8)=-(L1*L3)/(norm*norm);

	J(1,0)=-(x(0)*L1*L2)/(norm*norm);
	J(1,1)=-(x(1)*L1*L2)/(norm*norm);
	J(1,2)=-(L1*L2)/(norm*norm);
	J(1,3)=x(0)*(1-(L2*L2)/(norm*norm));
	J(1,4)=x(1)*(1-(L2*L2)/(norm*norm));
	J(1,5)=(1-(L2*L2)/(norm*norm));
	J(1,6)=-(x(0)*L2*L3)/(norm*norm);
	J(1,7)=-(x(1)*L2*L3)/(norm*norm);
	J(1,8)=-(L2*L3)/(norm*norm);

	J(2,0)=-(x(0)*L1*L3)/(norm*norm);
	J(2,1)=-(x(1)*L1*L3)/(norm*norm);
	J(2,2)=-(L1*L3)/(norm*norm);
	J(2,3)=-(x(0)*L2*L3)/(norm*norm);
	J(2,4)=-(x(1)*L2*L3)/(norm*norm);
	J(2,5)=-(L2*L3)/(norm*norm);
	J(2,6)=x(0)*(1-(L3*L3)/(norm*norm));
	J(2,7)=x(1)*(1-(L3*L3)/(norm*norm));
	J(2,8)=(1-(L3*L3)/(norm*norm));

	J/=norm;

	Eigen::Matrix<double,3,2> jacobX;
	double val_col_0=L1*F(0,0)+L2*F(1,0)+L3*F(2,0);
	double val_col_1=L1*F(0,1)+L2*F(1,1)+L3*F(2,1);

	jacobX(0,0)=F(0,0)-((L1*val_col_0)/(norm*norm));
	jacobX(0,1)=F(0,1)-((L1*val_col_1)/(norm*norm));

	jacobX(1,0)=F(1,0)-((L2*val_col_0)/(norm*norm));
	jacobX(1,1)=F(1,1)-((L2*val_col_1)/(norm*norm));

	jacobX(2,0)=F(2,0)-((L3*val_col_0)/(norm*norm));
	jacobX(2,1)=F(2,1)-((L3*val_col_1)/(norm*norm));

	jacobX/=norm;

	Cov_line=J*Cov*J.transpose()+sigma*sigma*jacobX*jacobX.transpose();
}
