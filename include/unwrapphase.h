/*将解码的格雷码与包裹相位图生成解包裹相位图（使得测量物体上的每个点都有唯一编码*/
#ifndef UNWRAPPHASE_H
#define UNWRAPPHASE_H
#include<opencv2/opencv.hpp>

class UnwrapPhase
{

public:
    cv::Mat _phs;
    UnwrapPhase();
    UnwrapPhase(int row, int col);
    //cn: 得到相位值， 相位值是图片的唯一特征，用于后期的图像匹配
    void GetPhrase(cv::Mat _dec, cv::Mat _wp, cv::Mat _roi);
    void GetPhraseSave(cv::Mat _dec, cv::Mat _wp, cv::Mat _roi);
};

#endif // UNWRAPPHASE_H
