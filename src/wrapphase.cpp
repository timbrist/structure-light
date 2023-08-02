#include"wrapphase.h"
using namespace std;
using namespace cv;


WrapPhase::WrapPhase()
{
    this->wrapped_phase = cv::Mat(Size(1280,1024), CV_32FC1, Scalar(0,0));
}
WrapPhase::WrapPhase(int row, int col)
{
    this->wrapped_phase = cv::Mat(Size(col,row), CV_32FC1, Scalar(0,0));
}
/*
WrapPhase::WrapPhase(int row, int col)
{
    Mat wp(Size(col,row), CV_32FC1, Scalar(0,0));
    this->wrapped_phase = wp;
}
*/
void WrapPhase::DecodeWrap(std::vector<std::string> _imgs)
{
    vector<Mat> imgs;
    for(size_t i = 0; i < _imgs.size(); i++)
    {
        Mat tmp = imread(_imgs[i],0);
        //GaussianBlur(tmp, tmp, Size(GAUSSIAN_KSIZE,GAUSSIAN_KSIZE), 1);
        //medianBlur(tmp, tmp,3);
        //Mat element = getStructuringElement(MORPH_RECT, Size(7,7));
        //erode(tmp, tmp, element);
        tmp.convertTo(tmp, CV_32FC1);
        imgs.push_back(tmp);
        //imwrite("wrap"+to_string(i)+".jpg" ,tmp);
        //imshow("wp:",tmp);
        //waitKey(2000);

    }
    //cout << "img1 " << imgs[0].size() << endl;
    //Mat mat1;
    //divide(( imgs[3] - imgs[1]), ( imgs[0] - imgs[2]), mat1 );
    Mat x = (imgs[3] - imgs[1]);
    Mat y = (imgs[0] - imgs[2]);
    for(int i = 0; i < y.rows; i++)
    {
        float *y_ptr = y.ptr<float>(i);
        for(int j = 0; j < y.cols; j++)
        {
            if(y_ptr[j] == 0.f)
                y_ptr[j] = 0.000001f;
        }
    }
    Mat z  = ( imgs[3] - imgs[1]) /   ( y );
    for(int i = 0; i < wrapped_phase.rows; i++)
    {
        float *wp_ptr = wrapped_phase.ptr<float>(i);
        float *z_ptr = z.ptr<float>(i);
        float *x_ptr = x.ptr<float>(i);
        float *y_ptr = y.ptr<float>(i);
        //float *m_ptr = mat1.ptr<float>(i);
        for(int j = 0; j < wrapped_phase.cols; j++)
        {
            //if(z_ptr[j] == 0)
            //    z_ptr[j] = 0.000001;
            //if(y_ptr[j] == 0.f)
                //y_ptr[j] = 0.000001f;
            wp_ptr[j] = atan2(x_ptr[j],y_ptr[j]);
            //wp_ptr[j] = fastAtan2(x_ptr[j], y_ptr[j]);
            //wp_ptr[j] = atan(z_ptr[j]);
            //wp_ptr[j] = atan(m_ptr[j]);
        }
    }
}
void WrapPhase::DecodeWrapSave(std::vector<std::string> _imgs)
{
    vector<Mat> imgs;
    for(size_t i = 0; i < _imgs.size(); i++)
    {
        Mat tmp = imread(_imgs[i],0);
        tmp.convertTo(tmp, CV_32FC1);
        imgs.push_back(tmp);
    }
    Mat x = (imgs[3] - imgs[1]);
    Mat y = (imgs[0] - imgs[2]);
    for(int i = 0; i < this->wrapped_phase.rows; ++i)
    {
        for(int j = 0; j < this->wrapped_phase.cols; ++j)
        {
            this->wrapped_phase.at<float>(i,j) = atan2(x.at<float>(i,j), y.at<float>(i,j));
        }
    }
}

