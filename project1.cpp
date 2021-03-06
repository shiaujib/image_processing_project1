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
	

void bilinear(float ratiox,float ratioy){
	double fx,fy;
        int ix,iy;
	float t,u;
        for(int j=0;j<matSdst.rows;j++){
		fy=(j+0.5)*ratioy-0.5;
		iy=cvFloor(fy);
		u=fy-iy;
		max(iy,0);
	        min(iy,matDst.rows);
               	for(int i=0;i<matSdst.cols;i++){
			fx=(i+0.5)*ratiox-0.5;
                       	ix=cvFloor(fx);
			t=fx-ix;
		  	max(ix,0);
			min(ix,matDst.cols);
			for(int k=0;k<matSrc.channels();k++){
				matSdst.at<Vec3b>(j,i)[k] = matDst.at<Vec3b>(iy,ix)[k] *(1-t)*(1-u) + //z1*(1-t)*(1-u) 
				matDst.at<Vec3b>(iy+1,ix)[k]*(1-t)*u+  //z2*(1-t)*u
			  	matDst.at<Vec3b>(iy,ix+1)[k]*t*(1-u)+  //z3*t*(1-u)
			      	matDst.at<Vec3b>(iy+1,ix+1)[k]*t*u;    //z4*t*u
                        		  }
                		    }
			    }
        //imshow("bilinear",matSdst);
	//waitKey(0);
	  }

void bicubic(float ratiox,float ratioy){
	double fx,fy;
	int ix,iy;
	float t,u;
	float Sy[4],Sx[4];
	for(int j=0;j<matSdst.rows;j++ ){
		fy=(j+0.5)*ratioy-0.5;
                iy=cvFloor(fy);
		u=fy-iy;
		iy=min(iy,matDst.rows-3);
		iy=max(1,iy);
		Sy[0]=4-8*(u+1)+5*(u+1)*(u+1)-abs(u+1)*abs(u+1)*abs(u+1); 
		Sy[1]=1-2*u*u+abs(u)*abs(u)*abs(u);
	        Sy[2]=1-2*(u-1)*(u-1)+abs(u-1)*abs(u-1)*abs(u-1);  
                Sy[3]=1-Sy[0]-Sy[1]-Sy[2];  

		for(int i=0;i<matSdst.cols;i++){
			fx=(i+0.5)*ratiox-0.5;
                        ix=cvFloor(fx);
			t=fx-ix;
			ix=min(ix,matDst.cols);
			ix=max(ix,0);
			Sx[0]=4-8*(t+1)+5*(t+1)*(t+1)-(t+1)*(t+1)*(t+1);
			Sx[1]=1-2*t*t+abs(t)*abs(t)*abs(t);
	       		Sx[2]=1-2*(t-1)*(t-1)+abs(t-1)*abs(t-1)*abs(t-1);  
		        Sx[3]=1-Sx[0]-Sx[1]-Sx[2]; 
			for(int k=0;k<matSdst.channels();k++){
				matSdst.at<Vec3b>(j,i)[k]=abs((
				matDst.at<Vec3b>(iy-1,ix-1)[k]*Sx[0]*Sy[0]+
				matDst.at<Vec3b>(iy,ix-1)[k]*Sx[0]*Sy[1]+ 
				matDst.at<Vec3b>(iy+1,ix-1)[k]*Sx[0]*Sy[2]+
				matDst.at<Vec3b>(iy+2,ix-1)[k]*Sx[0]*Sy[3]+  
				matDst.at<Vec3b>(iy-1,ix)[k]*Sx[1]*Sy[0]+
				matDst.at<Vec3b>(iy,ix)[k]*Sx[1]*Sy[1]+  
		   	        matDst.at<Vec3b>(iy+1,ix)[k]*Sx[1]*Sy[2]+
				matDst.at<Vec3b>(iy+2,ix)[k]*Sx[1]*Sy[3]+  
				matDst.at<Vec3b>(iy-1,ix+1)[k]*Sx[2]*Sy[0]+ 
				matDst.at<Vec3b>(iy,ix+1)[k]*Sx[2]*Sy[1]+  
				matDst.at<Vec3b>(iy+1,ix+1)[k]*Sx[2]*Sy[2]+
				matDst.at<Vec3b>(iy+2,ix+1)[k]*Sx[2]*Sy[3]+  
				matDst.at<Vec3b>(iy-1,ix+2)[k]*Sx[3]*Sy[0]+
				matDst.at<Vec3b>(iy,ix+2)[k]*Sx[3]*Sy[1]+  
				matDst.at<Vec3b>(iy+1,ix+2)[k]*Sx[3]*Sy[2]+
				matDst.at<Vec3b>(iy+2,ix+2)[k]*Sx[3]*Sy[3]));
				}
			}
		}
	//	imshow("bicubic",matSdst);
	//	waitKey(0);
		}



void scale(double ratiox,double ratioy,int algo)
{	
	int srcx,srcy;
	double sdx,sdy;
	double x_ratio,y_ratio;
	matSdst=Mat(Size(matSrc.cols*ratiox,matSrc.rows*ratioy),matDst.type());
	x_ratio=(double)matSrc.cols/matSdst.cols;
	y_ratio=(double)matSrc.rows/matSdst.rows;
	if(algo==0){
		for(int i=0;i<matSdst.cols;i++)
			for(int j=0;j<matSdst.rows;j++)
				{
					sdx=i*x_ratio;
					sdy=j*y_ratio;
					near(&sdx,&sdy);
					matSdst.at<Vec3b>(j,i)=matDst.at<Vec3b>(sdy,sdx);
				}
	//	imshow("nearest neighbor scale",matSdst);
	//	waitKey(0);
	}
	else if(algo==1){
		bilinear(x_ratio,y_ratio);
}
         
	else if(algo==2)		
		bicubic(x_ratio,y_ratio);
}



void rotate(double angle,int algo){
	double rx,ry;
	double fx,fy,u,t;
	int ix,iy;
	
	angle=angle*CV_PI/180;
	algo=0;
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
			min(ix,matSrc.cols);
                       	iy=cvRound(fy);
			u=fy-iy;
		  	max(iy,0);
			min(iy,matSrc.rows);
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
	}
		
}
void traslation_shear(double x,double y,double shear,bool type,int algo){
	double fx,fy,t,u;
	int ix,iy;
	double tx,ty;
	algo=0;
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
	imshow("Result",matTdst);
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
	int algo;
	double scaler,angle,tranx,trany,shear,type,x,y;
	cout<<"enter the interpolation you want (0:nearest,1:bilinear,2:bicubic)";
	cin>>algo;
	cout<<"enter the scaling ratio x and y:"<<endl;
	cin>>x>>y;
	cout<<"enter rotation angle"<<endl;
	cin>>angle;
	cout<<"enter translation x and y"<<endl;
	cin>>tranx>>trany;
	cout<<"enter type of shear (0:vertical 1:horizontal)"<<endl;
	cin>>type;
	cout<<"enter shear ratio (0~1)"<<endl;
	cin>>shear;
	rotate(angle,algo);
	scale(x,y,algo);
	traslation_shear(tranx,trany,shear,type,algo);

	return 0;	


}
