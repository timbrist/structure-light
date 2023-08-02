#include"include/graycode.h"

using namespace std;
using namespace cv;
Graycode::Graycode()
{

    this->dec = Mat(Size(1280,1024), CV_8UC1, Scalar(0,0));
    this->threshold_value = 30;
    this->gaussian_ksize = 3;
}
Graycode::Graycode(int row, int col)
{
     this->dec = Mat(Size(col,row), CV_8UC1, Scalar(0,0));
    this->threshold_value = 30;
    this->gaussian_ksize = 3;
}
Graycode::Graycode(int row, int col, int threshold, int gaussian)
{
    this->dec = Mat(Size(col,row), CV_8UC1, Scalar(0,0));
    this->threshold_value = threshold;
    this->gaussian_ksize = gaussian;
}
void Graycode::DecodeGray(std::vector<std::string> _imgs)
{
    vector<Mat> imgs;
    for(size_t i = 0; i < _imgs.size(); i++)
    {
        imgs.push_back(imread(_imgs[i],0));
        //imshow("imgs: " ,imgs[i]);
        //waitKey(2000);
    }
    //blur + thresh
    for(size_t i = 0; i < imgs.size(); i++)
    {
        GaussianBlur(imgs[i], imgs[i], Size(this->gaussian_ksize,this->gaussian_ksize), 1);
        medianBlur(imgs[i], imgs[i],3);
        threshold(imgs[i], imgs[i], this->threshold_value, 1, THRESH_BINARY);

    }
    //转二进制
    for(size_t i = 1; i < imgs.size(); i++)
    {
        bitwise_xor(imgs[i-1], imgs[i], imgs[i]);
        //imshow("imgs: " ,imgs[i-1]);
        //waitKey(3000); test: ok
    }
    // 矩阵可以直接运算
    dec = imgs[0]*32 +
            imgs[1]*16 +
            imgs[2]*8 +
            imgs[3]*4 +
            imgs[4]*2 +
            imgs[5]*1;

}
