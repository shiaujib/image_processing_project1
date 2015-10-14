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
	return result;
        }
double min(double x,double y){
	if(x>=y)
		return y;
	else
		return x;
}
	

void bilinear(float ratio){
       // uchar* dataDst=matSdst.data;
	//uchar* dataSrc=matDst.data;
	double fx,fy;
        int ix,iy;
	float t,u;
        for(int j=0;j<matSdst.rows;j++){
		fy=(j+0.5)*ratio-0.5;
		iy=cvRound(fy);
		u=fy-iy;
		max(iy,0);
	        min(iy,matDst.rows);
               	for(int i=0;i<matSdst.cols;i++){
			fx=(i+0.5)*ratio-0.5;
                       	ix=cvRound(fx);
			t=fx-ix;
		  	max(ix,0);
			min(ix,matDst.cols);
			for(int k=0;k<matSrc.channels();k++){
				matSdst.at<Vec3b>(j,i)[k] = matDst.at<Vec3b>(iy,ix)[k] *(1-t)*(1-u) +  
				matDst.at<Vec3b>(iy+1,ix)[k]*(1-t)*u+  
			  	matDst.at<Vec3b>(iy,ix+1)[k]*t*(1-u)+  
			      	matDst.at<Vec3b>(iy+1,ix+1)[k]*t*u; 
                        		  }
                		    }
			    }
        imshow("bilinear",matSdst);
	waitKey(0);
	  }

void bicubic(float ratio){
	double fx,fy;
	int ix,iy;
	float t,u;
	float A=-0.75f;
	float coefy[4],coefx[4];
	for(int j=0;j<matSdst.rows;j++ ){
		fy=(j+0.5)*ratio-0.5;
                iy=cvRound(fy);
		u=fy-iy;
		iy=min(iy,matDst.rows-3);
		iy=max(1,iy);
		coefy[0]=((A*(u+1)-5*A)*(u+1)+8*A)*(u+1)-4*A;  
	        coefy[1]=((A+2)*u-(A+3))*u*u+1;  
	        coefy[2]=((A+2)*(1-u)-(A+3))*(1-u)*(1-u)+1;  
                coefy[3]=1.f-coefy[0]-coefy[1]-coefy[2];  

		for(int i=0;i<matSdst.cols;i++){
			fx=(i+0.5)*ratio-0.5;
                        ix=cvRound(fx);
			t=fx-ix;
			ix=min(ix,matDst.cols);
			ix=max(ix,0);
			coefx[0] = ((A*(t+1) - 5*A)*(t + 1) + 8*A)*(t + 1) - 4*A;  
			coefx[1] = ((A + 2)*t - (A + 3))*t*t + 1;  
		        coefx[2] = ((A + 2)*(1 - t) - (A + 3))*(1 - t)*(1 - t) + 1;  
		        coefx[3] = 1.f - coefx[0] - coefx[1] - coefx[2]; 
			for(int k=0;k<matSdst.channels();k++){
				matSdst.at<Vec3b>(j,i)[k]=abs((
				matDst.at<Vec3b>(iy-1, ix-1)[k] * coefx[0] * coefy[0]+
				matDst.at<Vec3b>(iy, ix-1)[k] * coefx[0] * coefy[1] + 
				matDst.at<Vec3b>(iy+1, ix-1)[k] *coefx [0] *coefy [2] +
				matDst.at<Vec3b>(iy+2, ix-1)[k] *coefx [0] *coefy [3] +  
				matDst.at<Vec3b>(iy-1, ix)[k] *coefx [1] *coefy [0] +
				matDst.at<Vec3b>(iy, ix)[k] *coefx [1] *coefy [1] +  
		   	        matDst.at<Vec3b>(iy+1, ix)[k] *coefx [1] *coefy [2] +
				matDst.at<Vec3b>(iy+2, ix)[k] *coefx [1] *coefy [3] +  
				matDst.at<Vec3b>(iy-1, ix+1)[k] *coefx [2] *coefy [0] + 
				matDst.at<Vec3b>(iy, ix+1)[k] *coefx [2] *coefy [1] +  
				matDst.at<Vec3b>(iy+1, ix+1)[k] *coefx [2] *coefy [2] +
				matDst.at<Vec3b>(iy+2, ix+1)[k] *coefx [2] *coefy [3] +  
				matDst.at<Vec3b>(iy-1, ix+2)[k] *coefx [3] *coefy [0] +
				matDst.at<Vec3b>(iy, ix+2)[k] *coefx [3] *coefy [1] +  
				matDst.at<Vec3b>(iy+1, ix+2)[k] *coefx [3] *coefy [2] +
				matDst.at<Vec3b>(iy+2, ix+2)[k] *coefx [3] *coefy [3] ));
				}
			}
		}
		imshow("bicubic",matSdst);
		waitKey(0);
		}



void scale(double ratio,int algo)
{	
	int srcx,srcy;
	double sdx,sdy;
	double x_ratio,y_ratio;
//	cout<<matDst.cols<<endl;
//	cout<<matDst.rows<<endl;
//	matSrc=imread("lena.jpg",1);
//	imshow("input",matSrc);
//	waitKey(0);
//	cout<<"input height*width"<<matSrc.rows<<"*"<<matSrc.cols<<endl;
	matSdst=Mat(Size(matSrc.cols*ratio,matSrc.rows*ratio),matDst.type());
//	cout<<"onput height*width"<<matDst.rows<<"*"<<matDst.cols<<endl;
	x_ratio=(double)matSrc.cols/matSdst.cols;
	y_ratio=(double)matSrc.rows/matSdst.rows;
//	cout<<x_ratio<<"     "<<y_ratio;
	if(algo==0){
		for(int i=0;i<matSdst.cols;i++)
			for(int j=0;j<matSdst.rows;j++)
				{
					sdx=i*x_ratio;
					sdy=j*y_ratio;
					near(&sdx,&sdy);
					matSdst.at<Vec3b>(j,i)=matDst.at<Vec3b>(sdy,sdx);
				}
		imshow("nearest neighbor scale",matSdst);
		waitKey(0);
	}
	else if(algo==1)
		bilinear(x_ratio);
         
	else if(algo==2)		
		bicubic(x_ratio);
}



void rotate(double angle,int algo){
	double rx,ry;
	double fx,fy,u,t;
	int ix,iy;
	
	angle=-angle*CV_PI/180;
	
	matSrc=imread("lena.jpg",1);
	matDst=Mat(matSrc.size(),matSrc.type());
	imshow("input",matSrc);
	waitKey(0);
	if(algo==0){
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
	}
	else if(algo==1){
        for(int j=0;j<matDst.rows;j++){
               	for(int i=0;i<matDst.cols;i++){
			fx=((i-matSrc.cols/2)*cos(angle)-sin(angle)*(j-matSrc.rows/2)); //先做平移
			fy=((i-matSrc.cols/2)*sin(angle)+cos(angle)*(j-matSrc.rows/2));
			fx+=matSrc.cols/2;
			fy+=matSrc.rows/2;
                       	ix=cvRound(fx);
			t=fx-ix;
		  	max(ix,0);
		//	min(ix,matSrc.cols);
                       	iy=cvRound(fy);
			u=fy-iy;
		  	max(iy,0);
		//	min(iy,matSrc.rows);
			for(int k=0;k<matSrc.channels();k++){
			if(fx>=matDst.cols||fy>=matDst.rows||fx<=0||fy<=0)
				matDst.at<Vec3b>(j,i)=Vec3b(0,0);
			else{
				matDst.at<Vec3b>(j,i)[k] = matSrc.at<Vec3b>(iy,ix)[k] *(1-t)*(1-u) +  
				matSrc.at<Vec3b>(iy+1,ix)[k]*(1-t)*u+  
			  	matSrc.at<Vec3b>(iy,ix+1)[k]*t*(1-u)+  
			      	matSrc.at<Vec3b>(iy+1,ix+1)[k]*t*u;
				}
                          }
               	    }
	    }
	//imshow("bi rotate",matDst);
	//waitKey(0);
		}
		
}
void traslation_shear(double x,double y,double shear,bool type,int algo){
	double fx,fy,t,u;
	int ix,iy;
	double tx,ty;
	matTdst=Mat(matSdst.size(),matSdst.type());
	if(algo==0){
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
	if(algo==1){
        for(int j=0;j<matTdst.rows;j++){
		fy=(j-0.5*matTdst.rows)+y;
		fy+=0.5*matTdst.rows;
		iy=cvRound(fy);
		u=fy-iy;
		max(iy,0);
	        min(iy,matSdst.rows);
               	for(int i=0;i<matTdst.cols;i++){
			fx=((i-0.5*matSdst.cols)-x+shear*(j-0.5*matSdst.rows));
			fx+=0.5*matSdst.cols;
                       	ix=cvRound(fx);
			t=fx-ix;
		  	max(ix,0);
			min(ix,matSdst.cols);
			for(int k=0;k<matSrc.channels();k++){
				if(tx>=matSdst.cols||ty>=matSdst.rows||tx<=0||ty<=0)
					matTdst.at<Vec3b>(j,i)=Vec3b(0,0);
				else{
				matTdst.at<Vec3b>(j,i)[k] = matSdst.at<Vec3b>(iy,ix)[k] *(1-t)*(1-u) +  
				matSdst.at<Vec3b>(iy+1,ix)[k]*(1-t)*u+  
			  	matSdst.at<Vec3b>(iy,ix+1)[k]*t*(1-u)+  
			      	matSdst.at<Vec3b>(iy+1,ix+1)[k]*t*u; 
				}
                          }
                    }
	   }
	imshow("bitran",matTdst);
	waitKey(0);
		//bilinear(1.5);
	}

}


int main(){
	/*double scaler,angle,tranx,trany,shear,type;
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
	rotate(0);
//	scale(10,1);*/
	rotate(30,1);
	scale(1.5,2);
	traslation_shear(0,0,0.4,1,0);

	return 0;	


}
