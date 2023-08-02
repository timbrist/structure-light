/*
 * 矫正类: 该类只依赖 相机内外参数数据（该数据可以从标定类中得到，或者自定义内外参数xml文件）
 * 可在主函数中直接使用：
 *  Rectification rect;
    rect.OneButtonRectify();
 **/
#ifndef RECTIFICATION_H
#define RECTIFICATION_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include "imagelist.h"
class Rectification
{

public:
//输入： 相机内参文件， 相机外参文件， 原图片（含编码）列表,
//输出： 校正后的图片列表
void ImgRectify(const std::string& intrinsic_filename,
                  const std::string& extrinsic_filename,
                  std::vector<std::string>& imagelist,
                  std::vector<std::string>& Rectimagelist);

 //依赖 imagelist.h: 调试测试可直接在main函数中使用
void OneButtonRectify();
private:

void DisplayRectification(cv::Mat img1, cv::Mat img2,
                                        cv::Mat img1r, cv::Mat img2r);
};

#endif // RECTIFICATION_H
