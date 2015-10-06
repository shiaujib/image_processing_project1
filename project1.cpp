#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
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
			
		//	srcx=min(cvRound(i*x_ratio),matDst.cols-1);
		//	srcy=min(cvRound(j*y_ratio),matDst.rows-1);
			srcx=cvRound(i*x_ratio);
			srcy=cvRound(j*y_ratio);
			matDst.at<int>(j,i)=matSrc.at<int>(srcy,srcx);
		}
	}
//	imwrite("output.tif",matDst);
	imshow("Result",matDst);
	waitKey(0);
}
	
		
		

	


int main(){
	double x,y;
	cout<<"input the scaling ratio:"<<endl;
//	cin>>y>>x;
	cin>>x;
	nearest_neighbor(x);
	return 0;	


}
