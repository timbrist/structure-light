#include "include/triangulation.h"
#include <iostream>
#include <fstream>

using namespace std;
Triangulation::Triangulation(int size)
{
    this->points = cv::Mat(4,size,CV_32F);
}
void Triangulation::TriangulationPoints(cv::Mat P1, cv::Mat P2, std::vector<cv::Point2f> leftpoints, std::vector<cv::Point2f> rightpoints)
{
    if(leftpoints.size() == 0)
    {
        throw "correspond points failed ...";
    }
    cv::triangulatePoints(P1,P2,leftpoints,rightpoints,this->points);
}
void Triangulation::SavePoints(const char *filename)
{
    FILE* fp = fopen(filename, "wt");
    cv::Mat pnts3Dimg(4000, 4000, CV_8UC1,cv::Scalar(0));

    float *pnts3D_row1 = this->points.ptr<float>(0);
    float *pnts3D_row2 = this->points.ptr<float>(1);
    float *pnts3D_row3 = this->points.ptr<float>(2);
    float *pnts3D_row4 = this->points.ptr<float>(3);
    int i, j, pixelsvel;

    for(int y = 0; y < this->points.cols; y++)
    {
        float pnts3D_data4 = *(pnts3D_row4 + y);

        float pnts3D_data1 = *(pnts3D_row1 + y) / pnts3D_data4;
        float pnts3D_data2 = *(pnts3D_row2 + y) / pnts3D_data4;
        float pnts3D_data3 = *(pnts3D_row3 + y) / pnts3D_data4;
        float ave = (pnts3D_data1 + pnts3D_data2 + pnts3D_data3)/3;
        //fprintf(fp, "%f   %f   %f \r\n", pnts3D_data1-ave, pnts3D_data2-ave, pnts3D_data3-ave);
        fprintf(fp, "%f   %f   %f \n", pnts3D_data1, pnts3D_data2, pnts3D_data3);

        i = (int)(10*pnts3D_data1) + 1000; // col
        j = (int)(10*pnts3D_data2) + 2000; // row
        pixelsvel = (int)(225*pnts3D_data3 / 1900.00);
        // pixelsvel = pnts3D_data3;
        if( i < pnts3Dimg.cols && i >0 && j < pnts3Dimg.rows && j >0 &&  pixelsvel < 255&&  pixelsvel >0)
            pnts3Dimg.at<uchar>(j, i) = pixelsvel;


    }
    //imwrite("pnts3D.jpg", pnts3Dimg);
    fclose(fp);
}
void Triangulation::Save(const char *filename)
{
    ofstream ofs(filename);
    float *pnts3D_row1 = this->points.ptr<float>(0);
    float *pnts3D_row2 = this->points.ptr<float>(1);
    float *pnts3D_row3 = this->points.ptr<float>(2);
    float *pnts3D_row4 = this->points.ptr<float>(3);

    for(int y = 0; y < this->points.cols; ++y)
    {
        float pnts3D_data4 = *(pnts3D_row4 + y);
        float pnts3D_data1 = *(pnts3D_row1 + y) / pnts3D_data4;
        float pnts3D_data2 = *(pnts3D_row2 + y) / pnts3D_data4;
        float pnts3D_data3 = *(pnts3D_row3 + y) / pnts3D_data4;
        ofs << pnts3D_data1 << " ";
        ofs << pnts3D_data2 << " ";
        ofs << pnts3D_data3 << " ";
        ofs << "\n";
    }
    ofs.close();
}
void Triangulation::SaveSave(const char *filename)
{
    ofstream ofs(filename);
    for(int x = 0; x < this->points.cols; ++x)
    {
        float points3d4 = this->points.at<float>(3,x);
        float points3d1 = this->points.at<float>(0,x)/points3d4;
        float points3d2 = this->points.at<float>(1,x)/points3d4;
        float points3d3 = this->points.at<float>(2,x)/points3d4;
        ofs << points3d1 << " ";
        ofs << points3d2 << " ";
        ofs << points3d3 << " ";
        ofs << "\n";
    }
    ofs.close();
}
