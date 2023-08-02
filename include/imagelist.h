/*处理res中的xml文件， 用于读取res/img中的图片*/
#ifndef IMAGELIST_H
#define IMAGELIST_H

#include<string>
#include<vector>
class ImageList
{
public:
    class ResFiles
    {
    public:
        //棋盘格图片路径
        const std::string CLB_IMGS_PATH         = "../res/calib_images.xml";
        //保存相机内外参数的路径
        const std::string INTRINSICS_PATH  = "../res/intrinsics.yml";
        const std::string EXTRINSICS_PATH  = "../res/extrinsics.yml";
        //原图片路径
        const std::string ORG_IMGS_PATH         = "../res/origin_images.xml";
        //校正后的图片路径
        const std::string RCT_IMGS_PATH          = "../res/rected_images.xml";
    };
    ResFiles filenames;
    std::vector<std::string> caliblist;
    std::vector<std::string> fringelist;
    std::vector<std::string> rectimagelist;
public:
    //读图片列表
    bool ReadStringList( const std::string filename, std::vector<std::string>& l );

    //获取图片列表
    std::vector<std::string> GetRectImageList();
};

#endif // IMAGELIST_H
