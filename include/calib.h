/*
 * 标定类； 复制 "calib.h" "chessboard_calib.h" "imagelist.h" "chessboard_calib.cpp" "imagelist.cpp";
 * 在main主函数中直接使用:
 *
 *  Chessboard cb(false, true);
    Calib *c = &cb;
    c->OneButtonCalib();         //在imagelist.h中修改标定图片目录， 内外参数目录
 * */
#ifndef CALIB_H
#define CALIB_H
#include "imagelist.h"
#include <opencv2/opencv.hpp>
class Calib
{
protected:
    //外参数
    class Extrinsic
    {
    public:
        cv::Mat R;
        cv::Mat T;
        cv::Mat R1;
        cv::Mat R2;
        cv::Mat P1;
        cv::Mat P2;
        cv::Mat Q;
    };
    class Intrinsic
    {
    public:
        cv::Mat cameraMatrix[2];
        cv::Mat distCoeffs[2];
    };

    //虚构函数 未来也许会有不同的标定方式；
    //In:一组标定图片
    //Out: xml文件
public:
    virtual void StereoCalib(const std::vector<std::string> &imagelist,
                            const std::string &save_intrinsics_path,
                            const std::string &save_extrinsics_path) = 0;
    //依赖 imagelist.h
    virtual void OneButtonCalib() = 0;
    virtual ~Calib(){}
};

#endif // CALIB_H
