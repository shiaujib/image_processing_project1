#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;


Mat matSrc,matDst,matSdst,matTdst;


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



void scale(double ratio)
{	
	int srcx,srcy;
	double sdx,sdy;
	double x_ratio,y_ratio;
	cout<<matDst.cols<<endl;
	cout<<matDst.rows<<endl;
//	matSrc=imread("test.tif",1);
//	imshow("input",matSrc);
//	cout<<"input height*width"<<matSrc.rows<<"*"<<matSrc.cols<<endl;
	matSdst=Mat(Size(matSrc.cols*ratio,matSrc.rows*ratio),matDst.type());
	cout<<"onput height*width"<<matDst.rows<<"*"<<matDst.cols<<endl;
	x_ratio=(double)matSrc.cols/matSdst.cols;
	y_ratio=(double)matSrc.rows/matSdst.rows;
	for(int i=0;i<matSdst.cols;i++)
		for(int j=0;j<matSdst.rows;j++)
			{
				sdx=i*x_ratio;
				sdy=j*y_ratio;
				near(&sdx,&sdy);
				matSdst.at<Vec3b>(j,i)=matDst.at<Vec3b>(sdy,sdx);
			}
	imshow("Result",matSdst);
	waitKey(0);
} 

/*void scale_rotate(double ratio,double andle)
{	
	int srcx,srcy;
	double sdx,sdy;
	double x_ratio,y_ratio;
	angle=angle*CV_PI/180;
	matSrc=imread("test.tif",1);
	imshow("input",matSrc);
	cout<<"input height*width"<<matSrc.rows<<"*"<<matSrc.cols<<endl;
	matDst=Mat(Size(matSrc.cols*ratio,matSrc.rows*ratio),matSrc.type());
	cout<<"onput height*width"<<matDst.rows<<"*"<<matDst.cols<<endl;
	x_ratio=(double)matSrc.cols/matDst.cols;
	y_ratio=(double)matSrc.rows/matDst.rows;
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++)
			{
				sdx=(i*x_ratio-matSrc.cols/2);
				sdy=j*y_ratio;
				near(&sdx,&sdy);
				matDst.at<int>(j,i)=matSrc.at<int>(sdy,sdx);
			}
	imshow("Result",matDst);
	waitKey(0);
}*/


void rotate(double angle){
	double rx,ry;
	angle=angle*CV_PI/180;
	
	matSrc=imread("test.tif",1);
	matDst=Mat(matSrc.size(),matSrc.type());
	imshow("input",matSrc);
	waitKey(0);
	for(int i=0;i<matDst.cols;i++)
		for(int j=0;j<matDst.rows;j++){
			rx=((i-matSrc.cols/2)*cos(angle)-sin(angle)*(j-matSrc.rows/2)); //先做平移
			ry=((i-matSrc.cols/2)*sin(angle)+cos(angle)*(j-matSrc.rows/2));
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
void traslation_shear(double x,double y,double shear,bool type){
	
	double tx,ty;
//	Mat matSrc,matDst;
//	matSrc=imread("test.tif",1);
//	matDst=Mat(matSrc.size(),matSrc.type());
//	imshow("input",matSrc);
//	waitKey(0);
	matTdst=Mat(matSdst.size(),matSdst.type());
	for(int i=0;i<matSdst.cols;i++)
		for(int j=0;j<matSdst.rows;j++){
			if(type==1){	
				tx=((i-0.5*matSdst.cols)-x+shear*(j-0.5*matSdst.rows));
				ty=(j-0.5*matSdst.rows)+y;
				tx+=0.5*matSdst.cols;
				ty+=0.5*matSdst.rows;
				near(&tx,&ty);
				if(tx>=matSdst.cols||ty>=matSdst.rows||tx<=0||ty<=0)
					matTdst.at<Vec3b>(j,i)=Vec3b(0,0);
				else
					matTdst.at<Vec3b>(j,i)=matSdst.at<Vec3b>(ty,tx);
				}
			else{
				tx=((i-0.5*matSdst.cols)-x);
				ty=(j-0.5*matSdst.rows)+y+shear*(i-0.5*matSdst.cols);
				tx+=0.5*matSdst.cols;
				ty+=0.5*matSdst.rows;
				near(&tx,&ty);
				if(tx>=matSdst.cols||ty>=matSdst.rows||tx<=0||ty<=0)
					matTdst.at<Vec3b>(j,i)=Vec3b(0,0);
				else
					matTdst.at<Vec3b>(j,i)=matSdst.at<Vec3b>(ty,tx);
				}
				

		}
	imshow("Transform+shear",matTdst);
	waitKey(0);
	}


int main(){
	double scaler,angle,tranx,trany,shear,type;
	cout<<"enter the scaling ratio:"<<endl;
	cin>>scaler;
	cout<<"enter rotation angle"<<endl;
	cin>>angle;
	cout<<"enter translation x and y"<<endl;
	cin>>tranx>>trany;
	cout<<"enter shear ratio (0~1)"<<endl;
	cin>>shear;
	cout<<"enter type of shear (0:vertical 1:horizontal)"<<endl;
	cin>>type;
	rotate(angle);
	scale(scaler);
	traslation_shear(tranx,trany,shear,type);
//	nearest_neighbor(x);
//	rotate(x);
//	traslation(x,x,shear);
	return 0;	


}
