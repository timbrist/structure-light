/*解格雷码， 1.将格雷码转为自然二进制，2.将自然二进制转为十进制。数据应成阶梯递增*/
#ifndef GRAYCODE_H
#define GRAYCODE_H
#include<iostream>
#include<opencv2/opencv.hpp>
class Graycode
{
private:
    int threshold_value;
    int gaussian_ksize;
public:
    cv::Mat dec;
    Graycode();
    Graycode(int row, int col, int threshold, int gaussian);
    Graycode(int row, int col);
    //en: decode gray
    //input: a list of images which covered gray code project on it
    //ouput: a CV_8UC1 Mat, it's a gray to whith img from left to right
    void DecodeGray(std::vector< std::string > _imgs);
};

#endif // GRAYCODE_H
