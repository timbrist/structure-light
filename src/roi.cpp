#include "roi.h"
using namespace  cv;
using namespace std;

Roi::Roi(int row, int col)
{
    this->roi= cv::Mat(col,row,CV_32FC1,cv::Scalar(0.0));
}

void Roi::SetRoi(std::string src)
{
    this->roi = imread(src, 0);
    GaussianBlur(this->roi, this->roi, Size(3,3), 1);
    medianBlur(this->roi, this->roi,3);
    threshold(this->roi, this->roi, 30, 255, THRESH_BINARY);
}
