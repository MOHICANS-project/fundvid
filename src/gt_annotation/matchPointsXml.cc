#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <gvars3/instances.h>



using namespace cv;
using namespace std;
using namespace GVars3;

Mat F = Mat(3, 3, CV_64FC1);
Mat Img0, Img1, Img0BW, Img1BW;

float clickedX0 = 0, clickedY0 = 0;
float clickedX1 = 0, clickedY1 = 0;
Mat zoomed0, zoomed1, patch0, patch1;
Mat purezoomed0;

float finalX0 = 0, finalX1 = 0, finalY0 = 0, finalY1 = 0;

vector<Point2f> pt0;
vector<Point2f> pt1;

int PATCHSIZE;
int ZOOMPIXEL;
int sidex,sidey,offsetx,offsety,num_buckets,num_points;
int current_bucketx;
int current_buckety;
int* bucketoccupation;
bool selected=false;
bool point_left=false;
bool point_right=false;
int absposx0,absposx1,absposy0,absposy1,posx10,posy10,posx11,posy11;
vector<Point> boundingpoints;
vector<Point> covered;
int numboudingbpoints=0;

void saveMatch(){
	cout << "Match done : [" << finalX0 << " " << finalY0 << "] matched with [" << finalX1 << " " << finalY1 << "]" <<endl;
	pt0.push_back(Point2f(finalX0, finalY0));
	pt1.push_back(Point2f(finalX1, finalY1));
}

void clearPoints(){
	clickedX0 = 0;
	clickedY0 = 0;
	clickedX1 = 0; 
	clickedY1 = 0;
	finalX0 = 0;
	finalY0 = 0;
	finalX1 = 0; 
	finalY1 = 0;
	cvDestroyWindow("Image 0");
	cvDestroyWindow("Image 1");

	patch1.release();
	patch0.release();
	zoomed0.release();
	zoomed1.release();
}

int bothPointsSet(){
	return (finalX0 > 0 && finalX1 > 0);
}

void drawCross(int x, int y, Mat* im){
	Point2i Pts[2];
	Pts[0].x = x;
	Pts[0].y = y-3;
	Pts[1].x = x;
	Pts[1].y = y+3;
	line(*im,Pts[0], Pts[1], Scalar(0,0,255));
	Pts[0].x = x-3;
	Pts[0].y = y;
	Pts[1].x = x+3;
	Pts[1].y = y;
	line(*im,Pts[0], Pts[1], Scalar(0,0,255));

}

void drawAllCrosses(){
	double hScale = 1.0;
	double vScale = 1.0;
	int lineWidth = 1;
	CvFont font;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

	for(int i = 0; i < pt0.size();i++){
		drawCross(pt0[i].x, pt0[i].y, &Img0);
		drawCross(pt1[i].x, pt1[i].y, &Img1);
		std::ostringstream oss;
		oss << i;
		//putText(Img0,oss.str().c_str(), cvPoint(pt0[i].x - 30,pt0[i].y), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
		//putText(Img1,oss.str().c_str(), cvPoint(pt1[i].x - 30,pt1[i].y), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);

	}
}

void clickInZoomForBounding(int event, int x, int y, int flags, void* param){
	switch(event){
		case CV_EVENT_LBUTTONDOWN:
		boundingpoints.push_back(Point(x,y));
			numboudingbpoints++;
			//Draw current result
			purezoomed0.copyTo(zoomed0);
			for (int i = 0; i < numboudingbpoints; ++i) {
				if(i==numboudingbpoints-1){
					line(zoomed0,boundingpoints[i],boundingpoints[0],Scalar(0,0,255),2,8);
					continue;
				}
				line(zoomed0,boundingpoints[i],boundingpoints[i+1],Scalar(0,0,255),2,8);
			}
			imshow("Zoom0",zoomed0);
			break;
	}

}


void clickInZoom(int event, int x, int y, int flags, void* param){
	switch(event){
		case CV_EVENT_LBUTTONDOWN:
		float xOffset = x * 1.0/ZOOMPIXEL;
		float yOffset = y * 1.0/ZOOMPIXEL;
		int* idxP = (int*) param;
		float clickedX, clickedY;
		*idxP ? clickedX = clickedX1 : clickedX = clickedX0;
		*idxP ? clickedY = clickedY1 : clickedY = clickedY0;
		
		//float xInitial = clickedX - sidex/2 -0.5 + xOffset;
		//float yInitial = clickedY - sidey/2 -0.5 + yOffset;
		float xInitial = clickedX + xOffset;
		float yInitial = clickedY + yOffset;


		*idxP ? finalX1 = xInitial : finalX0 = xInitial;
		*idxP ? finalY1 = yInitial : finalY0 = yInitial;

		if(*idxP){
      		//image1
			resize(patch1, zoomed1,zoomed1.size(),ZOOMPIXEL,ZOOMPIXEL, INTER_NEAREST);
			
		}
		else{
      		//image0
			resize(patch0, zoomed0,zoomed0.size(),ZOOMPIXEL,ZOOMPIXEL, INTER_NEAREST);
			
		}

		*idxP ? drawCross(x,y,&zoomed1) : drawCross(x,y,&zoomed0);
		*idxP ? imshow("Zoom1", zoomed1) : imshow("Zoom0", zoomed0);
		*idxP ? point_right=true : point_left=true;
		break;
	}
}

void createZoom(int event, int x, int y, int flags, void* param){
	int* idxP = (int*) param;
	
	switch(event){
		case CV_EVENT_LBUTTONDOWN:
		if(*idxP){

			int x1=x-offsetx;
			int y1=y-offsety;
			posx11=x1/sidex;
			posy11=y1/sidey;
			absposx1=offsetx+posx11*sidex;
			absposy1=offsety+posy11*sidey;
			
      		//image1
			clickedX1 = 0; 
			clickedY1 = 0;
			finalX1 = 0;
			finalY1 = 0;
			patch1 = Mat(sidey,sidex, CV_32FC1);
			//getRectSubPix(Img1, patch1.size(), Point2f(absposx1+sidex/2+0.5,absposy1+sidey/2+0.5), patch1);
			//getRectSubPix(Img1, patch1.size(), Point2f(absposx1+sidex*1.0/2,absposy1+sidey*1.0/2), patch1);
			
			//patch1 = Img1(Rect(absposx1, absposy1, sidex, sidey));
			Img1(Rect(absposx1, absposy1, sidex, sidey)).copyTo(patch1);
			zoomed1 = Mat(sidey * ZOOMPIXEL, sidex * ZOOMPIXEL, CV_32FC1);
			resize(patch1, zoomed1,zoomed1.size(), ZOOMPIXEL, ZOOMPIXEL, INTER_NEAREST);
			
			cvNamedWindow("Zoom1",CV_WINDOW_AUTOSIZE);
			imshow("Zoom1", zoomed1);
			//clickedX1 = absposx1+sidex*1.0/2;
			//clickedY1 = absposy1+sidey*1.0/2;
			clickedX1 = absposx1;
			clickedY1 = absposy1;
			cvSetMouseCallback("Zoom1",clickInZoom,param);
		}
		else{
			clickedX0 = 0;
			clickedY0 = 0;
			finalX0 = 0;
			finalY0 = 0;

			//What is the bucket I am inside?
			int x1=x-offsetx;
			int y1=y-offsety;
			posx10=x1/sidex;
			posy10=y1/sidey;

			if(bucketoccupation[posx10+posy10*num_buckets]>=num_points){
				cout << "Selected bucket is already full" << endl;
				break;
			}

			absposx0=offsetx+posx10*sidex;
			absposy0=offsety+posy10*sidey;
			cout << "pos " << absposx0 << " " << absposy0 << endl;
      		//image0
			patch0 = Mat(sidey, sidex, CV_32FC1);
			//getRectSubPix(Img0, patch0.size(), Point2f(absposx0+sidex/2+0.5,absposy0+sidey/2+0.5), patch0);
			//getRectSubPix(Img0, patch0.size(), Point2f(absposx0+sidex*1.0/2,absposy0+sidey*1.0/2), patch0);
			Img0(Rect(absposx0, absposy0, sidex, sidey)).copyTo(patch0);
			//patch0 = Img0(Rect(absposx0, absposy0, sidex, sidey));
			zoomed0 = Mat(sidey * ZOOMPIXEL, sidex * ZOOMPIXEL, CV_32FC1);
			resize(patch0, zoomed0,zoomed0.size(),  ZOOMPIXEL,  ZOOMPIXEL, INTER_NEAREST);


			cvNamedWindow("Zoom0",CV_WINDOW_AUTOSIZE);
			imshow("Zoom0", zoomed0);
			//clickedX0 = absposx0+sidex/2+0.5;
			//clickedY0 = absposy0+sidey/2+0.5;
			clickedX0 = absposx0;
			clickedY0 = absposy0;
			
			cvSetMouseCallback("Zoom0",clickInZoom,param);
			char numssa[5];
			sprintf(numssa,"%d",num_points-bucketoccupation[posx10+posy10*num_buckets]);
			string remain="Remaining points to match: "+string(numssa);
			displayOverlay("Zoom0",remain.c_str(),0);
			selected=true;
		}
      	break;
      }


  }


  int main(int argc, char **argv)
  {

	// argv[1] - first image
	// argv[2] - second image
	// argv[3] - the matches file (xml format)
	//			 this is where the manual point matches are
	//			 loaded from and saved to
	//			 (if the file does not exist, it will be created)
	// argv[4] - number of buckets
    // argv[5] - number of points per bucket


  	if(argc < 6 ){
  		cout<<"Wrong number of command line parameters for "<< argv[0] << endl;
  		return 1;
  	}

  	num_buckets=atoi(argv[4]);
  	num_points=atoi(argv[5]);
  	ZOOMPIXEL=(num_buckets/2);


  	//FileStorage fs0(argv[3],FileStorage::READ);
  	//FileStorage fs1(argv[4],FileStorage::READ);
  	FileStorage fsMatches(argv[3],FileStorage::READ);


  	/*if (!fs0.isOpened() || !fs1.isOpened())
  	{
  		cout<<"Intrinsic parameter files are not present!"<<endl;
  		exit(1);
  	}

  	Mat intrinsic0 = Mat(3, 3, CV_64FC1);
  	Mat intrinsic1 = Mat(3, 3, CV_64FC1);
  	Mat distCoeffs0;
  	Mat distCoeffs1;
  	fs0["intrinsic"]>>intrinsic0;
  	fs0["distorsion"]>>distCoeffs0;
  	fs1["intrinsic"]>>intrinsic1;
  	fs1["distorsion"]>>distCoeffs1; 
  	fs0.release();
  	fs1.release();*/

  	fsMatches["Points0"]>>pt0;
  	fsMatches["Points1"]>>pt1;
  	fsMatches["Covered"]>>covered;
  	fsMatches.release();

  	Mat image0;
  	Mat image1;
  	string path0 = string(argv[1]); 
  	string path1 = string(argv[2]); 
  	image0 = imread(path0,0);
  	image1 = imread(path1,0);

  	Img0BW=image0;
  	Img1BW=image1;
  	//undistort(image1, Img1BW, intrinsic1, distCoeffs1);
  	//undistort(image0, Img0BW, intrinsic0, distCoeffs0);

  	cvtColor(Img0BW, Img0, CV_GRAY2RGB);
  	cvtColor(Img1BW, Img1, CV_GRAY2RGB);

  	int height=Img0.rows;
  	int width=Img1.cols;

  	sidex=width/num_buckets;
  	sidey=height/num_buckets;

  	offsetx=(width-sidex*num_buckets)/2;
  	offsety=(height-sidey*num_buckets)/2;
	cout << offsetx << " " << offsety << endl;
  	for (int i = 0; i <= num_buckets; ++i) {
  		line(Img0,Point(0,offsety+i*sidey),Point(Img0.cols-1,offsety+i*sidey),Scalar(255,0,0),2,4);
  		line(Img0,Point(offsetx+i*sidex,0),Point(offsetx+i*sidex,Img0.rows-1),Scalar(255,0,0),2,4);
  		//line(Img0,Point(Img0.cols/2+offsetx+i*sidex,0),Point(Img0.cols/2+offsetx+i*sidex,Img0.rows-1),Scalar(255,0,0),2,4);
  	}

	for (int i = 0; i <= num_buckets; ++i) {
  		line(Img1,Point(0,offsety+i*sidey),Point(Img1.cols-1,offsety+i*sidey),Scalar(255,0,0),2,4);
  		line(Img1,Point(offsetx+i*sidex,0),Point(offsetx+i*sidex,Img1.rows-1),Scalar(255,0,0),2,4);
  		//line(Img1,Point(Img1.cols/2+offsetx+i*sidex,0),Point(Img1.cols/2+offsetx+i*sidex,Img1.rows-1),Scalar(255,0,0),2,4);
  	}


  	int id0 = 0;
  	int id1 = 1;
  	imshow("First image",Img0);
  	imshow("Second image",Img1);

  	bucketoccupation=(int*)calloc(sizeof(int),num_buckets*num_buckets);

  	//Initialize bucket occupation with points read from file:
  	for (int i = 0; i < pt0.size(); ++i) {
  		float px=pt0[i].x;
  		float py=pt0[i].y;
  		px-=offsetx;
  		py-=offsety;
  		int posx=px/sidex;
  		int posy=py/sidey;
  		bucketoccupation[posx+posy*num_buckets]++;
	}
  	for (int i = 0; i < covered.size(); ++i) {
		bucketoccupation[covered[i].x+covered[i].y*num_buckets]=num_points;
	}

	while(true){
		selected=false;
		cvtColor(Img0BW, Img0, CV_GRAY2RGB);
		cvtColor(Img1BW, Img1, CV_GRAY2RGB);
		for (int i = 0; i <= num_buckets; ++i) {
	  		line(Img0,Point(0,offsety+i*sidey),Point(Img0.cols-1,offsety+i*sidey),Scalar(255,0,0),2,4);
	  		line(Img0,Point(offsetx+i*sidex,0),Point(offsetx+i*sidex,Img0.rows-1),Scalar(255,0,0),2,4);
	  		//line(Img0,Point(Img0.cols/2+offsetx+i*sidex,0),Point(Img0.cols/2+offsetx+i*sidex,Img0.rows-1),Scalar(255,0,0),2,4);
	  	}

		for (int i = 0; i <= num_buckets; ++i) {
	  		line(Img1,Point(0,offsety+i*sidey),Point(Img1.cols-1,offsety+i*sidey),Scalar(255,0,0),2,4);
	  		line(Img1,Point(offsetx+i*sidex,0),Point(offsetx+i*sidex,Img1.rows-1),Scalar(255,0,0),2,4);
	  		//line(Img1,Point(Img1.cols/2+offsetx+i*sidex,0),Point(Img1.cols/2+offsetx+i*sidex,Img1.rows-1),Scalar(255,0,0),2,4);
	  	}
		drawAllCrosses();

		//Show filled buckets
		for (int i = 0; i < num_buckets; ++i) {
			for (int j = 0; j < num_buckets; ++j) {
				if(bucketoccupation[j+i*num_buckets]>=num_points){
					int absposx0b=offsetx+j*sidex;
					int absposy0b=offsety+i*sidey;
					line(Img0,Point(absposx0b,absposy0b),Point(absposx0b+sidex,absposy0b+sidey),Scalar(0,255,0),4,8);
					line(Img0,Point(absposx0b+sidex,absposy0b),Point(absposx0b,absposy0b+sidey),Scalar(0,255,0),4,8);
				}
			}
		}

		imshow("First image",Img0);
		imshow("Second image",Img1);

		cvSetMouseCallback("First image",NULL,NULL);
		cvSetMouseCallback("Second image",NULL,NULL);

		//Select a bucket from first image
		displayOverlay("First image","Please select a bucket",0);
		displayOverlay("Second image","No action available",0);
		cvSetMouseCallback("First image",createZoom,&id0);
		char k=cvWaitKey(0);
		if(k=='q'){
			break;
		}
		if(!selected){
			continue;
		}
		cout << "Bucket ("<<posx10 << "," << posy10<< ") selected." << endl;
		displayOverlay("First image","Please fill current bucket before continuing",0);
		displayOverlay("Second image","Select a bucket",0);
		cvSetMouseCallback("Second image",createZoom,&id1);
		cvSetMouseCallback("First image",NULL,NULL);
		//Zoom0 is always open: if you try to close it who knows what can happen
		line(Img0,Point(absposx0,absposy0),Point(absposx0+sidex,absposy0+sidey),Scalar(0,255,255),4,8);
		line(Img0,Point(absposx0+sidex,absposy0),Point(absposx0,absposy0+sidey),Scalar(0,255,255),4,8);
		imshow("First image",Img0);
		displayOverlay("Zoom0","Press \'f\' to work with entire bucket, \'p\' to restrict area, \'d\' to mark as covered",0);
		k=cvWaitKey(0);
		while(k!='f' && k!='p' && k!='q' && k!='d'){
			k=cvWaitKey(0);
		}
		if(k=='q'){
			break;
		}
		if(k=='p'){
			zoomed0.copyTo(purezoomed0);
			displayOverlay("Zoom0","Select points of the polygon containing the visible region (3 <= p <= 10) and press a button to continue",0);
			numboudingbpoints=0;
			boundingpoints.clear();
			cvSetMouseCallback("Zoom0",clickInZoomForBounding,0);
			bool toclose=false;
			while(numboudingbpoints<3){
				char k2=cvWaitKey(0);
				if(k2=='q'){
					toclose=true;
					break;
				}
			}
			if(toclose){
				break;
			}
			//Abbiamo i punti adesso yeah
			long sum=0;
			for (int i = 0; i < numboudingbpoints-1; ++i) {
				sum+=(boundingpoints[i].x*boundingpoints[i+1].y);
			}
			for (int i = 0; i < numboudingbpoints-1; ++i) {
				sum-=(boundingpoints[i+1].x*boundingpoints[i].y);
			}
			sum+=boundingpoints[numboudingbpoints-1].x*boundingpoints[0].y;
			sum-=boundingpoints[0].x*boundingpoints[numboudingbpoints-1].y;
			float area=abs(sum)/2;
			float ratio=area/(1.0*sidex*sidey*ZOOMPIXEL*ZOOMPIXEL);
			cout << ratio << endl;
			int numpoints_needed=ceil(num_points*ratio);
			bucketoccupation[posx10+posy10*num_buckets]=(num_points-numpoints_needed);
			purezoomed0.copyTo(zoomed0);
			imshow("Zoom0",zoomed0);
			cvSetMouseCallback("Zoom0",clickInZoom,&id0);
			cout << "restriction done!" << endl;
		}
		if(k=='d'){
			bucketoccupation[posx10+posy10*num_buckets]=num_points;
			covered.push_back(Point(posx10,posy10));
			destroyWindow("Zoom0");
			continue;
		}

		bool qpressed=false;
		while(bucketoccupation[posx10+posy10*num_buckets]<num_points){
			//wait point selection
			char numssa[5];
			sprintf(numssa,"%d",num_points-bucketoccupation[posx10+posy10*num_buckets]);
			string remain="Remaining points to match: "+string(numssa);
			displayOverlay("Zoom0",remain.c_str(),0);
			point_left=false;
			point_right=false;
			k=cvWaitKey(0);
			if(k=='q'){
				qpressed=true;
				break;
			}
			if(!point_left || !point_right){
				cout << "Select all the points to match first!" << endl;
				continue;
			}
			//All selected: add point
			saveMatch();
			bucketoccupation[posx10+posy10*num_buckets]++;
		}
		if(qpressed){
			break;
		}
		covered.push_back(Point(posx10,posy10));
		//Finished matching for that bucket, close Zoom0 and Zoom1 windows
		destroyWindow("Zoom0");
		destroyWindow("Zoom1");
	}








  	/*imshow("cam0", Img0);
  	imshow("cam1", Img1);
  	cvSetMouseCallback("cam0",createZoom,&id0);
  	cvSetMouseCallback("cam1",createZoom,&id1);

  	while(cvWaitKey(100) != 'q'){

  		cvtColor(Img0BW, Img0, CV_GRAY2RGB);
  		cvtColor(Img1BW, Img1, CV_GRAY2RGB);
  		drawAllCrosses();
  		imshow("cam0", Img0);
  		imshow("cam1", Img1);

		//if the zoomed images have been brought up, show them too
  		if(clickedX0 != 0)
  			imshow("Zoom0", zoomed0);
  		if(clickedX1 != 0)
  			imshow("Zoom1", zoomed1);

  		if(bothPointsSet()){
  			saveMatch();
  			clearPoints();
  		}
  	}

*/

	

	//save the clicked points in a file
	FileStorage matches(argv[3],FileStorage::WRITE);
  	matches << "Points0"<<pt0<<"Points1"<<pt1<<"Covered"<<covered;
  	matches.release();
  	return 0;
  }
