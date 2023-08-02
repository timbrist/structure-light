#include "structurelightimages.h"
using namespace std;


void StructureLightImages::InitImages()
{
    vector<string> images = GetRectImageList();
    this->left_roi = images[0];
    this->right_roi = images[1];
    for(unsigned int i = 2; i <= 12; i = i+2)
        this->left_dec.push_back(images[i]);
    for(unsigned int j = 3; j <= 13; j = j+2)
        this->right_dec.push_back(images[j]);
    for(unsigned int x = 14; x <= 20; x = x+2)
        this->Left_cosin.push_back((images[x]));
    for(unsigned int y = 15; y <= 21; y = y+2)
        this->right_cosin.push_back(images[y]);
    cv::FileStorage fs(ImageList::filenames.EXTRINSICS_PATH,cv::FileStorage::READ);
    if(!fs.isOpened())
    {
        throw "Failed to open file extrinsics_filenames";
    }

    fs["P1"] >> P1;
    fs["P2"] >> P2;
}
