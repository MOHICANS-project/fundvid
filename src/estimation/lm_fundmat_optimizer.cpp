/*
 * lm_fundmat_optimizer.cpp
 *
 *  Created on: 30 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "lm_fundmat_optimizer.h"
#include <levmar.h>
#include "src/exceptions/optimization_exception.h"
#include <cmath>

#define SQR(x)      ((x)*(x))
#define FABS(x)     (((x)>=0)? (x) : -(x))

struct Fdata{
	Eigen::Vector3d* p0;
	Eigen::Vector3d* p1;
	int numinl;
	int* index_dependent;
};

void parametrize7(const Eigen::Matrix3d & F, double* p, int* index_dependent){

	double poss_sols[9][8];
	int index=0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			int cc=0;
			double vettorino[4];
			double iine[4];
			double jine[4];
			for (int ki = 0; ki < 3; ++ki) {
				for (int kj = 0; kj < 3; ++kj) {
					if(ki!=i && kj!=j){
						vettorino[cc]=F(ki,kj);
						iine[cc]=ki;
						jine[cc]=kj;
						cc++;
					}
				}
			}

			//for clarity
			double a=vettorino[0],b=vettorino[1],c=vettorino[2],d=vettorino[3];
			Eigen::Matrix2d A;
			A << -a,-b,
			     -c,-d;
			Eigen::Vector2d bb;
			bb << F(iine[0],j),F(iine[2],j);
			Eigen::Vector2d lambdas;
			lambdas=A.fullPivLu().solve(bb);

			Eigen::Matrix2d A2;
			A2 << -a,-c,
				  -b,-d;
			Eigen::Vector2d bb2;
			bb2 << F(i,jine[0]),F(i,jine[1]);
			Eigen::Vector2d lambdas2;
			lambdas2=A2.fullPivLu().solve(bb2);
			poss_sols[index][0]=a;
			poss_sols[index][1]=b;
			poss_sols[index][2]=c;
			poss_sols[index][3]=d;
			poss_sols[index][4]=lambdas[0];
			poss_sols[index][5]=lambdas[1];
			poss_sols[index][6]=lambdas2[0];
			poss_sols[index][7]=lambdas2[1];
			index++;
		}
	}
	double norm[9];
	for (int i = 0; i < 9; ++i) {
		norm[i]=sqrt(SQR(poss_sols[i][4])+SQR(poss_sols[i][5])+1)*sqrt(SQR(poss_sols[i][6])+SQR(poss_sols[i][7])+1)*SQR(poss_sols[i][0]*poss_sols[i][3]-poss_sols[i][1]*poss_sols[i][2]);
	}
	double *maxnorm=std::max_element(norm,norm+9);
	int id=std::distance(norm,maxnorm);

	double as=poss_sols[id][0];
	double bs=poss_sols[id][1];
	double cs=poss_sols[id][2];
	double ds=poss_sols[id][3];

	double max=fabs(as);
	int idmaxtraiquattro=0;
	if(fabs(bs)>max){
		max=fabs(bs);
		idmaxtraiquattro=1;
	}
	if(fabs(cs)>max){
		max=fabs(cs);
		idmaxtraiquattro=2;
		}
	if(fabs(ds)>max){
		max=fabs(ds);
		idmaxtraiquattro=3;
	}

	//Cheating
	idmaxtraiquattro=3;
	//Cheating


	int jwinner=id%3;
	int iwinner=(id-jwinner)/3;

	int numero=0;
	int iine[4];
	int jine[4];
	for (int ki = 0; ki < 3; ++ki) {
		for (int kj = 0; kj < 3; ++kj) {
			if(ki!=iwinner && kj!=jwinner){
				iine[numero]=ki;
				jine[numero]=kj;
				numero++;
			}
		}
	}

	index_dependent[0]=iwinner;
	index_dependent[1]=jwinner;
	index_dependent[2]=iine[idmaxtraiquattro];
	index_dependent[3]=jine[idmaxtraiquattro];
	index_dependent[4]=idmaxtraiquattro;

	int r=0;
	for (int d = 0; d < 4; ++d) {
		if(d!=idmaxtraiquattro){
			p[r++]=poss_sols[id][d]/poss_sols[id][idmaxtraiquattro];
		}
	}
	p[3]=poss_sols[id][4];
	p[4]=poss_sols[id][5];
	p[5]=poss_sols[id][6];
	p[6]=poss_sols[id][7];
}

void deparametrize7(double* p,const int* index_dependent,Eigen::Matrix3d & F){

	int idep=index_dependent[0];
	int jdep=index_dependent[1];
	int pos1i=index_dependent[2];
	int pos1j=index_dependent[3];

	int next=0;
	int c=0;
	int iine[4];
	int jine[4];
	for (int ki = 0; ki < 3; ++ki) {
		for (int kj = 0; kj < 3; ++kj) {
			if(ki!=idep && kj!=jdep){
				//Uno dei quattro
				if(ki==pos1i && kj==pos1j){
					F(ki,kj)=1;
				}else{
					F(ki,kj)=p[next];
					next++;
				}
				iine[c]=ki;
				jine[c]=kj;
				c++;
			}
		}
	}
	//cout << idep << " " << jdep << " " << pos1i << " " << pos1j<< " "<<index_dependent[5]<< endl;
	F(iine[0],jdep)=-F(iine[0],jine[0])*p[3]-F(iine[1],jine[1])*p[4];
	F(iine[2],jdep)=-F(iine[2],jine[2])*p[3]-F(iine[3],jine[3])*p[4];

	F(idep,jine[0])=-F(iine[0],jine[0])*p[5]-F(iine[2],jine[2])*p[6];
	F(idep,jine[1])=-F(iine[1],jine[1])*p[5]-F(iine[3],jine[3])*p[6];

	F(idep,jdep)=-F(idep,jine[0])*p[3]-F(idep,jine[1])*p[4];

}

static void jacXY(double ff[7], int* index_dependent,Eigen::Matrix<double,9,7> & result){
	int i=index_dependent[0];
	int j=index_dependent[1];
	//int posi=index_dependent[2];
	//int posj=index_dependent[3];
	int posmax=index_dependent[4];

	//First calculate 9x8
	Eigen::Matrix<double,9,8> intermediate;
	//quatrroparam=[a/d,b/d,c/d,1]
	double quattroparam[4];
	quattroparam[posmax]=1;
	int c=0;
	for (int k = 0; k < 4; ++k) {
		if(k!=posmax){
			quattroparam[k]=ff[c];
			c++;
		}
	}

	//derivative w.r.t. [a/d,b/d,c/d,1,x,y,x',y']
	c=0;
	for (int ki = 0; ki < 3; ++ki) {
		for (int kj = 0; kj < 3; ++kj) {
			int pos=ki*3+kj;
			if(ki!=i && kj!=j){
				intermediate(pos,0)=0;
				intermediate(pos,1)=0;
				intermediate(pos,2)=0;
				intermediate(pos,3)=0;
				intermediate(pos,4)=0;
				intermediate(pos,5)=0;
				intermediate(pos,6)=0;
				intermediate(pos,7)=0;
				intermediate(pos,c)=1;
				c++;
			}
		}
	}
	c=0;
	for (int k = 0; k < 3; ++k) {
		if(k!=i){
			int pos=k*3+j;
			if(c==0){
				intermediate(pos,0)=-ff[3];
				intermediate(pos,1)=-ff[4];
				intermediate(pos,2)=0;
				intermediate(pos,3)=0;
				intermediate(pos,4)=-quattroparam[0];
				intermediate(pos,5)=-quattroparam[1];
				intermediate(pos,6)=0;
				intermediate(pos,7)=0;
				c++;
			}else{
				intermediate(pos,0)=0;
				intermediate(pos,1)=0;
				intermediate(pos,2)=-ff[3];
				intermediate(pos,3)=-ff[4];
				intermediate(pos,4)=-quattroparam[2];
				intermediate(pos,5)=-quattroparam[3];
				intermediate(pos,6)=0;
				intermediate(pos,7)=0;
			}

		}
	}

	c=0;
		for (int k = 0; k < 3; ++k) {
			if(k!=j){
				int pos=i*3+k;
				if(c==0){
					intermediate(pos,0)=-ff[5];
					intermediate(pos,1)=0;
					intermediate(pos,2)=-ff[6];
					intermediate(pos,3)=0;
					intermediate(pos,4)=0;
					intermediate(pos,5)=0;
					intermediate(pos,6)=-quattroparam[0];
					intermediate(pos,7)=-quattroparam[2];
					c++;
				}else{
					intermediate(pos,0)=0;
					intermediate(pos,1)=-ff[5];
					intermediate(pos,2)=0;
					intermediate(pos,3)=-ff[6];
					intermediate(pos,4)=0;
					intermediate(pos,5)=0;
					intermediate(pos,6)=-quattroparam[1];
					intermediate(pos,7)=-quattroparam[3];
				}

			}
		}
	int pos=i*3+j;
	intermediate(pos,0)=ff[3]*ff[5];
	intermediate(pos,1)=ff[5]*ff[4];
	intermediate(pos,2)=ff[6]*ff[3];
	intermediate(pos,3)=ff[6]*ff[4];
	intermediate(pos,4)=quattroparam[0]*ff[5]+quattroparam[2]*ff[6];
	intermediate(pos,5)=quattroparam[1]*ff[5]+quattroparam[3]*ff[6];
	intermediate(pos,6)=quattroparam[0]*ff[3]+quattroparam[1]*ff[4];
	intermediate(pos,7)=quattroparam[2]*ff[3]+quattroparam[3]*ff[4];


	//Transform intermediate 9x8 in result 9x7 by removing the row corrensonding to posmax
	c=0;
	for (int k = 0; k < 8; ++k) {
		if(k!=posmax){
			result(0,c)=intermediate(0,k);
			result(1,c)=intermediate(1,k);
			result(2,c)=intermediate(2,k);
			result(3,c)=intermediate(3,k);
			result(4,c)=intermediate(4,k);
			result(5,c)=intermediate(5,k);
			result(6,c)=intermediate(6,k);
			result(7,c)=intermediate(7,k);
			result(8,c)=intermediate(8,k);
			c++;
		}
	}


}


static double computegradient(double p0[2],double p1[2],double f[7],Eigen::Matrix3d & F,double grads[7],int* index_dependent){

	//First part: independent from the parametrization

	double errornumerator=p0[0]*p1[0]*F(0,0)+p0[0]*p1[1]*F(1,0)+p0[0]*F(2,0)+p0[1]*p1[0]*F(0,1)+p0[1]*p1[1]*F(1,1)+p0[1]*F(2,1)+p1[0]*F(0,2)+p1[1]*F(1,2)+F(2,2);
	double errordenominator_squared=SQR(F(0,0)*p0[0]+F(0,1)*p0[1]+F(0,2))+SQR(F(1,0)*p0[0]+F(1,1)*p0[1]+F(1,2))+SQR(F(0,0)*p1[0]+F(1,0)*p1[1]+F(2,0))+SQR(F(0,1)*p1[0]+F(1,1)*p1[1]+F(2,1));
	double errordenominator=sqrt(errordenominator_squared);
	double error=errornumerator/errordenominator;

	Eigen::Matrix<double,1,9> gradient;
	gradient(0)=(p0[0]*p1[0]*errordenominator-error*(p0[0]*(F(0,0)*p0[0]+F(0,1)*p0[1]+F(0,2))+p1[0]*(F(0,0)*p1[0]+F(1,0)*p1[1]+F(2,0))))/errordenominator_squared;
	gradient(1)=(p0[1]*p1[0]*errordenominator-error*(p0[1]*(F(0,0)*p0[0]+F(0,1)*p0[1]+F(0,2))+p1[0]*(F(0,1)*p1[0]+F(1,1)*p1[1]+F(2,1))))/errordenominator_squared;
	gradient(2)=(p1[0]*errordenominator-error*((F(0,0)*p0[0]+F(0,1)*p0[1]+F(0,2))))/errordenominator_squared;
	gradient(3)=(p0[0]*p1[1]*errordenominator-error*(p0[0]*(F(1,0)*p0[0]+F(1,1)*p0[1]+F(1,2))+p1[1]*(F(0,0)*p1[0]+F(1,0)*p1[1]+F(2,0))))/errordenominator_squared;
	gradient(4)=(p0[1]*p1[1]*errordenominator-error*(p0[1]*(F(1,0)*p0[0]+F(1,1)*p0[1]+F(1,2))+p1[1]*(F(0,1)*p1[0]+F(1,1)*p1[1]+F(2,1))))/errordenominator_squared;
	gradient(5)=(p1[1]*errordenominator-error*(F(1,0)*p0[0]+F(1,1)*p0[1]+F(1,2)))/errordenominator_squared;
	gradient(6)=(p0[0]*errordenominator-error*(F(0,0)*p1[0]+F(1,0)*p1[1]+F(2,0)))/errordenominator_squared;
	gradient(7)=(p0[1]*errordenominator-error*(F(0,1)*p1[0]+F(1,1)*p1[1]+F(2,1)))/errordenominator_squared;
	gradient(8)=1.0/errordenominator;

	Eigen::Matrix<double,1,9> gradients;
	if(errornumerator<0){
		gradients=-1*gradient;
	}else{
		gradients=gradient;
	}

	//Second part: 36 different maps
	Eigen::Matrix<double,9,7> secondpart;

	jacXY(f,index_dependent,secondpart);

	Eigen::Matrix<double,1,7> jacobian;
	jacobian=gradients*secondpart;
	Eigen::Matrix<double,1,7> jacobians=jacobian;

	for (int i = 0; i < 7; ++i) {
		grads[i]=jacobians(i);
	}

	return error*error;

}

static void sampsonF(double *p,double *x, int m, int n, void *adata){
		register int i;
		struct Fdata *dat=(struct Fdata*)adata;
		Eigen::Matrix3d F;
		deparametrize7(p,dat->index_dependent,F);
		int size=dat->numinl;
		for (i = 0; i < size; ++i) {
			Eigen::Vector3d m1=dat->p1[i];
			Eigen::Vector3d m0=dat->p0[i];
			double numerator=((m1.transpose())*F*m0)(0);
			Eigen::Vector3d l1=F*m0;
			Eigen::Vector3d l0=F.transpose()*m1;
			double denominator=SQR(l0(0))+SQR(l0(1))+SQR(l1(0))+SQR(l1(1));
			denominator=sqrt(denominator);
			x[i]=fabs(numerator/denominator);
		}
}

static void jacobianFchanging(double *p, double *jac,int m,int n,void *adata){
		register int i;
		struct Fdata *dat=(struct Fdata *)adata;
		int numinl=dat->numinl;
		register double *jacrow;
		Eigen::Matrix3d F;
		deparametrize7(p,dat->index_dependent,F);
		parametrize7(F,p,dat->index_dependent);
		double toterrsq=0;
		for (i = 0,jacrow=jac; i < numinl; ++i,jacrow+=m){
			double p0[2];
			double p1[2];
			p0[0]=dat->p0[i](0);
			p0[1]=dat->p0[i](1);
			p1[0]=dat->p1[i](0);
			p1[1]=dat->p1[i](1);
			double errsq=computegradient(p0,p1,p,F,jacrow,dat->index_dependent);
			toterrsq+=errsq;
		}
}

void LMFundMatOptimizer::optFundMat(double F[3][3], double **p0, double **p1,int n){
	double fp[7];
	int index_dependent[5];
	Eigen::Matrix3d Fmat;
	Fmat<<	F[0][0],F[0][1],F[0][2],
			F[1][0],F[1][1],F[1][2],
			F[2][0],F[2][1],F[2][2];


	parametrize7(Fmat,fp,index_dependent);

	struct Fdata *dat=(struct Fdata*)malloc(sizeof(struct Fdata));
	Eigen::Vector3d* p0v=new Eigen::Vector3d[n];
	Eigen::Vector3d* p1v=new Eigen::Vector3d[n];
	for (int i = 0; i < n; ++i) {
		Eigen::Vector3d v;
		v<< p0[i][0],p0[i][1],1;
		p0v[i]=v;
		Eigen::Vector3d v2;
		v2<< p1[i][0],p1[i][1],1;
		p1v[i]=v2;
	}

	dat->p0=p0v;
	dat->p1=p1v;
	dat->numinl=n;
	dat->index_dependent=index_dependent;
	void (*err)(double *p, double *hx, int m, int n, void *adata)=sampsonF;

	//LAUNCH THE MINIMIZATION
	int numiter;
	double opts[LM_OPTS_SZ];
	opts[0]=LM_INIT_MU;
	opts[1]=1E-12; //LM_STOP_THRESH
	opts[2]=1E-12; //
	opts[3]=1E-15;
	double info[LM_INFO_SZ];/*
				0: sum squared error at the beginning,
				 *1: sum squared error at the end,
				 *5: number iterations,
				 *6: reasoning of terminating (small gradient, small Dp, itmax, singular matrix, no further reduction possible, small squared error, user error)
				 *7: functions evaluations
				 *8: Jacobian evaluations
				 *9: linear sistems solved*/
    double covar[7][7];

    if(derivative){
    	void (*jacerr)(double *p, double *j, int m, int n, void *adata)=jacobianFchanging;
		numiter=dlevmar_der(err,jacerr,fp,NULL,7,n,100000,opts,info,NULL,(double*)covar,(void *)dat);
	}else{
		numiter=dlevmar_dif(err,fp,NULL,7,n,100000,opts,info,NULL,(double*)covar,(void*) dat);
	}

    Eigen::Matrix3d Fnew;
    if(numiter>0){
    	deparametrize7(fp,dat->index_dependent,Fnew);
    	Eigen::Matrix<double,7,7> eCovar;
    	for (size_t i= 0; i < 7; ++i) {
			for (size_t j= 0; j < 7; ++j) {
				eCovar(i,j)=covar[i][j];
			}
		}
    	Eigen::Matrix<double,9,7> jacobianf;
		jacXY(fp,dat->index_dependent,jacobianf);
		covarF=jacobianf*eCovar*(jacobianf.transpose());

		F[0][0]=Fnew(0,0);F[0][1]=Fnew(0,1);F[0][2]=Fnew(0,2);
		F[1][0]=Fnew(1,0);F[1][1]=Fnew(1,1);F[1][2]=Fnew(1,2);
		F[2][0]=Fnew(2,0);F[2][1]=Fnew(2,1);F[2][2]=Fnew(2,2);

    }else{
    	std::cout << numiter << std::endl;
    	throw OptimizationException(info[6]);
    }
    delete[] p0v;
    delete[] p1v;
}


void LMFundMatOptimizer::getCovarF(double Cov[9][9]){
	for (size_t i = 0; i < 9; ++i) {
		for (size_t j = 0; j < 9; ++j) {
			Cov[i][j]=covarF(i,j);
		}
	}
}
