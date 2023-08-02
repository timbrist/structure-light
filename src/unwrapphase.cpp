#include"unwrapphase.h"

UnwrapPhase::UnwrapPhase()
{
    this->_phs = cv::Mat(1280,1024,CV_32FC1,cv::Scalar(0.0));
}
UnwrapPhase::UnwrapPhase(int row, int col)
{
    this->_phs = cv::Mat(col,row,CV_32FC1,cv::Scalar(0.0));
}


void UnwrapPhase::GetPhrase(cv::Mat _dec, cv::Mat _wp, cv::Mat _roi)
{
    _dec.convertTo(_dec, CV_32FC1);
    this->_phs = (_dec * CV_2PI) + _wp;


    //将背景像素设置为0

    for(int y = 0; y < _phs.rows; y++)
    {
        uchar *roi_ptr = _roi.ptr<uchar>(y);
        float *phs_ptr = _phs.ptr<float>(y);
        for(int x = 0; x < _phs.cols; x++)
        {
            if(roi_ptr[x] != 255)
                phs_ptr[x] = 0.f;
        }
    }

}
void UnwrapPhase::GetPhraseSave(cv::Mat _dec, cv::Mat _wp, cv::Mat _roi)
{
    _dec.convertTo(_dec, CV_32FC1);
    this->_phs = (_dec * CV_2PI) + _wp;
    for(int y = 0; y < this->_phs.rows; ++y)
    {
        for(int x = 0; x < this->_phs.cols; ++x)
        {
            if(_roi.at<uchar>(y,x) != 255)
                this->_phs.at<float>(y,x) = 0.f;
        }
    }
}
