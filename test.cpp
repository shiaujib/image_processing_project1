  #include <opencv2/core/core.hpp>
  #include <opencv2/highgui/highgui.hpp>
  using namespace cv;
    
  int main()  
  {  
      Mat src = imread("test.tif",1);
      imshow("src",src);
      waitKey(0);  
      return 0;
  }  

