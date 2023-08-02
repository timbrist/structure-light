#include"imagelist.h"
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//读图片列表
bool ImageList::ReadStringList( const string filename, vector<string>& l )
{
    l.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back(static_cast<string>(*it));
    return true;
}

vector<string> ImageList::GetRectImageList()
{
    vector<string> imagelist;
    FileStorage fs(this->filenames.RCT_IMGS_PATH, FileStorage::READ);
    if( !fs.isOpened() )
        throw "file is not opened";
    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        throw "file open failed";
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        imagelist.push_back(static_cast<string>(*it));
    return imagelist ;
}
