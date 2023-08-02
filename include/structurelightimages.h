/*用于加载结构光的图片信息，相机外参数*/
#ifndef STRUCTURELIGHTIMAGES_H
#define STRUCTURELIGHTIMAGES_H
#include"imagelist.h"
#include<opencv2/opencv.hpp>
class StructureLightImages:public ImageList
{
public:
    //物体左右图片原图,用作感兴趣区间提前
    std::string left_roi;
    std::string right_roi;
    //校正后的前6张格雷码编码图片
    std::vector<std::string> left_dec;
    std::vector<std::string> right_dec;
    //校正后的后4张余弦编码图片
    std::vector<std::string> Left_cosin;
    std::vector<std::string> right_cosin;
    cv::Mat P1;
    cv::Mat P2;
public:
    void InitImages();
};

#endif // STRUCTURELIGHTIMAGES_H
