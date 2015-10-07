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
		for(int j=0;j<matDst.rows;j++)
			{
				sdx=i*x_ratio;
				sdy=j*y_ratio;
				near(&sdx,&sdy);
				matDst.at<int>(j,i)=matSrc.at<int>(sdy,sdx);
			}
//	imwrite("output.tif",matDst);
	imshow("Result",matDst);
	waitKey(0);
}



void rotate(double angle){
	double rx,ry;
	double x_ratio,y_ratio;
	Mat matSrc,matDst,matOut;
	angle=angle*CV_PI/180;
	matSrc=imread("lena.jpg",1);
	matDst=Mat(matSrc.size(),matSrc.type());
	imshow("input",matSrc);
	waitKey(0);
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			rx=(i-matSrc.cols/2)*cos(angle)-sin(angle)*(j-matSrc.rows/2);
			ry=(i-matSrc.cols/2)*sin(angle)+cos(angle)*(j-matSrc.rows/2);
			rx+=matSrc.cols/2;
			ry+=matSrc.rows/2;
			near(&rx,&ry);
			if(rx>=matDst.cols||ry>=matDst.rows||rx<=0||ry<=0)
				matDst.at<Vec3b>(j,i)=Vec3b(0,0);
			else
				matDst.at<Vec3b>(j,i)=matSrc.at<Vec3b>(ry,rx);
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
