#ifndef MATCH_H
#define MATCH_H

#include<opencv2/opencv.hpp>
#include<iostream>
/*
 * 根据左右两边的相位图(矩阵行成单调递增) 找到两个相对于的匹配点
 **/
class Match
{
private:
    //只用于线性插值 map的查找
    struct myPoint
    {
        cv::Point2f p;
        bool operator < (const myPoint &o) const
        {
            return p.x < o.p.x || p.y < o.p.y;
        }
    };
    //匹配阈值
    const int THS = 1;
public:
    std::vector<cv::Point2f> leftpoints;
    std::vector<cv::Point2f> rightpoints;
    //匹配算法
    //输入： 左边相位图， 右边相位图， 左边感兴趣区间， 右边感兴趣区间，
    //输出： 左图坐标点， 右图坐标点
    //依赖自定义数据结构, 单线性插值
    void StereoCorrespond(cv::Mat left_phase, cv::Mat right_phase,
                      cv::Mat left_thresh, cv::Mat right_thresh);
    void StereoCorrespondSave(cv::Mat left_phase, cv::Mat right_phase,
                      cv::Mat left_thresh, cv::Mat right_thresh);

private:
    //单线性插值
    //只用于 匹配函数
    float SubPixel(cv::Point2f x1, cv::Point2f x2,
          float p1, float p2,
          float p_left);
};
#endif // MATCH_H
