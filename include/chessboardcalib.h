/*
 * 棋盘格标定主要接口函数， StereoCalib可以直接使用
 * OneButtonCalib()需要imagelist.
 * */
#ifndef CHESSBOARD_CALIB_H
#define CHESSBOARD_CALIB_H
#include "calib.h"

class Chessboard : public Calib
{
private:
    cv::Size boardSize;
    float squareSize;
    Calib::Extrinsic extrinsic;
    Calib::Intrinsic intrinsic;
    bool isDisplayCorners;
    bool isDisplayRectification;
public:
    Chessboard();
    Chessboard(cv::Size boardSize_, float squareSize_);
    Chessboard(bool isDisplayCorners, bool isDisplayRectification);
    Chessboard(cv::Size boardSize_, float squareSize_, bool isDisplayCorners, bool isDisplayRectification);
    //In:一组标定图片
    //Out: xml文件

    virtual void StereoCalib(const std::vector<std::string> &imagelist,
                            const std::string &save_intrinsics_path,
                            const std::string &save_extrinsics_path);

    //依赖 imagelist.h: 调试测试可直接在main函数中使用
     virtual void OneButtonCalib();
private:
    //显示找到棋盘格，并在图片中显示出来
    void DisplayCorners(cv::Mat &img, std::vector<cv::Point2f> &corners,bool isFound);
    //显示校正后的左右棋盘格图片，并在图片中画出极线
    void DisplayRectification(std::vector<std::string> goodImageList,
                                          cv::Size &imageSize,
                                          int nimages,
                                          cv::Rect *validRoi);
};

#endif // CHESSBOARD_CALIB_H
