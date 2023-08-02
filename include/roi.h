/*框出物体在左右图片的位置，加快匹配效率。 跳过背景信息*/
#ifndef ROI_H
#define ROI_H
#include<opencv2/opencv.hpp>
#include<iostream>
class Roi
{
public:
    cv::Mat roi;
    Roi(int row, int col);
    void SetRoi(std::string src);
};
#endif // ROI_H
