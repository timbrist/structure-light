/*对4张余弦图片进行解光强相位*/
#ifndef WRAPPHASE_H
#define WRAPPHASE_H
#include<iostream>
#include<opencv2/opencv.hpp>

class WrapPhase
{
public:
    cv::Mat wrapped_phase;
    WrapPhase();
    WrapPhase(int row, int col);
    //WrapPhase(int row, int col);
    //cn: 解包裹相位
    //输入: 对4张余弦图片进行解光强相位
    //输出： CV_32FC1 的图片
    void DecodeWrap(std::vector< std::string > _imgs);

    void DecodeWrapSave(std::vector< std::string > _imgs);
};

#endif // WRAPPHASE_H
