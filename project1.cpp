#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;



void nearest_neighbor(double ratio)
{	
	int srcx,srcy;
	double x_ratio,y_ratio;
	Mat matSrc,matDst,matOut;
	matSrc=imread("test.tif",1);
	imshow("input",matSrc);
	cout<<"input height*width"<<matSrc.rows<<"*"<<matSrc.cols<<endl;
	matDst=Mat(Size(matSrc.cols*ratio,matSrc.rows*ratio),matSrc.type());
	cout<<"onput height*width"<<matDst.rows<<"*"<<matDst.cols<<endl;
	matOut=Mat(matSrc.size(),matSrc.type());
	
	x_ratio=(double)matSrc.cols/matDst.cols;
	y_ratio=(double)matSrc.rows/matDst.rows;

	for(int i=0;i<matDst.cols;i++)
	{
		for(int j=0;j<matDst.rows;j++)
		{
			
			srcx=cvRound(i*x_ratio);
			srcy=cvRound(j*y_ratio);
			matDst.at<int>(j,i)=matSrc.at<int>(srcy,srcx);
		}
	}
//	imwrite("output.tif",matDst);
	imshow("Result",matDst);
	waitKey(0);
}

void near(double *x,double *y){
	
	*x=cvRound(x);
	*y=cvRound(y);

	}


void rotate(double angle){
	int rx,ry;
	double x_ratio,y_ratio;
	Mat matSrc,matDst,matOut;
	matSrc=imread("test.tif",1);
	imshow("input",matSrc);
	for(int i=0;i<matSrc.rows;i++)
		for(int j=0;j<matSrc.cols;j++){
			rx=j*cos(angle)-sin(angle)*i;
			ry=j*sin(angle)+cos(angle)*i;
			}
				
	
		
		

	


int main(){
	double x,y;
	cout<<"input the scaling ratio:"<<endl;
//	cin>>y>>x;
	cin>>x;
	nearest_neighbor(x);
	return 0;	


}
