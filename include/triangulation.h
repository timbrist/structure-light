/*将得到的左右图片匹配点的坐标， 运用三角函数算出深度信息，并保存*/
#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include<opencv2/opencv.hpp>
class Triangulation
{

public:
    cv::Mat points;
    Triangulation(int size);
    void TriangulationPoints(cv::Mat P1, cv::Mat P2, std::vector<cv::Point2f> leftpoints, std::vector<cv::Point2f> rightpoints);
    void SavePoints(const char* filename);	//保存三维点云数据
    void Save(const char *filename);
    void SaveSave(const char *filename);
};

#endif // TRIANGULATION_H
