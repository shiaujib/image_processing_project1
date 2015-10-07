#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;




int near(double *x,double *y){   //find the nearest point
	int result;
	*x=cvRound(*x);
	*y=cvRound(*y);
        //result=cvRound(x);
	return result;
        }
double min(double x,double y){
	if(x>=y)
		return y;
	else
		return x;
}



void nearest_neighbor(double ratio)
{	
	int srcx,srcy;
	double sdx,sdy;
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
			
			sdx=i*x_ratio;
			sdy=j*y_ratio;
			near(&sdx,&sdy);
			cout<<sdx<<" \t"<<sdy<<endl;
		//	srcx=near(sdx);
		//	srcy=near(sdy);
	//		matDst.at<int>(j,i)=matSrc.at<int>(srcy,srcx);
			matDst.at<int>(j,i)=matSrc.at<int>(sdy,sdx);
		}
	}
//	imwrite("output.tif",matDst);
	imshow("Result",matDst);
	waitKey(0);
}



void rotate(double angle){
	double rx,ry;
	double x_ratio,y_ratio;
	Mat matSrc,matDst,matOut;
	angle=angle*3.1415/180;
	matSrc=imread("test.tif",1);
	matDst=Mat(matSrc.size(),matSrc.type());
	imshow("input",matSrc);
	waitKey(0);
	for(int i=0;i<matSrc.cols;i++)
		for(int j=0;j<matSrc.rows;j++){
			rx=i*cos(angle)-sin(angle)*j;
			ry=i*sin(angle)+cos(angle)*j;
			rx=min(rx,matSrc.cols-1);
			ry=min(ry,matSrc.rows-1);
			near(&rx,&ry);
			matDst.at<int>(j,i)=matSrc.at<int>(ry,rx);
			}
	imshow("Result",matDst);
	waitKey(0);			
	
		
		
}
	


int main(){
	double x,y;
	cout<<"input the scaling ratio:"<<endl;
//	cin>>y>>x;
	cin>>x;
//	nearest_neighbor(x);
	rotate(x);
	return 0;	


}
