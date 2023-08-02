#include "include/rectification.h"

using namespace  std;
using namespace cv;

//输入： 相机内参文件， 相机外参文件， 原图片（含编码）列表,
//输出： 校正后的图片列表
//依赖函数readStringList
void Rectification::ImgRectify(const std::string& intrinsic_filename,
                  const std::string& extrinsic_filename,
                  vector<string>& imagelist,
                  vector<string>& Rectimagelist)
{
    cout << "Start Img Rectified..." << endl;
    bool  showRect = true;
    if( imagelist.size() % 2 != 0 )
    {
        cout << "Error: the image list contains odd (non-even) number of elements\n";
        return;
    }

    cout << "reading images ..." << endl;

    if( Rectimagelist.size() % 2 != 0 )
    {
        cout << "Error: the image list contains odd (non-even) number of elements\n";
        return;
    }
    cout << " read imags ok" << endl;
    int i, nimages = static_cast<int>(imagelist.size()/2);
    string  filename = imagelist[0];
    Mat img = imread(filename, 0);

    Size img_size = img.size();
    Rect roi1, roi2;
    Mat Q;

    // reading intrinsic parameters
    FileStorage fs(intrinsic_filename, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("Failed to open file intrinsic_filename.\n");
        return ;
    }

    Mat M1, D1, M2, D2;
    fs["M1"] >> M1;
    fs["D1"] >> D1;
    fs["M2"] >> M2;
    fs["D2"] >> D2;

    fs.open(extrinsic_filename, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("Failed to open file extrinsic_filename.\n");
        return ;
    }

    Mat R, T, R1, P1, R2, P2;
    fs["R"] >> R;
    fs["T"] >> T;
    fs["R1"] >> R1;
    fs["R2"] >> R2;
    fs["P1"] >> P1;
    fs["P2"] >> P2;

    //  stereoRectify( M1, D1, M2, D2, img_size, R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, img_size, &roi1, &roi2 );


    Mat map11, map12, map21, map22;
    initUndistortRectifyMap(M1, D1, R1, P1, img_size, CV_16SC2, map11, map12);
    initUndistortRectifyMap(M2, D2, R2, P2, img_size, CV_16SC2, map21, map22);

    for(i=0; i < nimages; i++) {
        filename = imagelist[2 * i];
        Mat img1 = imread(filename, 0);
        filename = imagelist[2 * i + 1];
        Mat img2 = imread(filename, 0);

        Mat img1r, img2r;
        remap(img1, img1r, map11, map12, INTER_LINEAR);
        remap(img2, img2r, map21, map22, INTER_LINEAR);

        filename = Rectimagelist[2 * i];
        imwrite(filename, img1r);
        filename = Rectimagelist[2 * i + 1];
        imwrite(filename, img2r);
        if (showRect) {
            Mat canvas;
            double sf;
            int w, h;
            sf = 600. / MAX(img_size.width, img_size.height);  //600
            w = cvRound(img_size.width * sf);
            h = cvRound(img_size.height * sf);
            canvas.create(h, w * 2, CV_8UC3);

            cvtColor(img1r, img1, COLOR_GRAY2BGR);
            cvtColor(img2r, img2, COLOR_GRAY2BGR);

            Mat canvasPart = canvas(Rect(w * 0, 0, w, h));
            resize(img1, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);
            canvasPart = canvas(Rect(w * 1, 0, w, h));
            resize(img2, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);

            for (int j = 0; j < canvas.rows; j += 16)
                line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);

            imshow("rectified", canvas);
            waitKey(0);
            char c = static_cast<char>(waitKey());
            if (c == 27 || c == 'q' || c == 'Q')
                break;
        }
    }
}
void Rectification::DisplayRectification(Mat img1, Mat img2,
                                        Mat img1r, Mat img2r)
{
    //if (showRect)
    if(true)
    {
        Mat canvas;
        double sf;
        int w, h;
        sf = 600. / MAX(img1.cols, img1.rows);  //600
        w = cvRound(img1.cols * sf);
        h = cvRound(img1.rows * sf);
        canvas.create(h, w * 2, CV_8UC3);

        cvtColor(img1r, img1, COLOR_GRAY2BGR);
        cvtColor(img2r, img2, COLOR_GRAY2BGR);

        Mat canvasPart = canvas(Rect(w * 0, 0, w, h));
        resize(img1, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);
        canvasPart = canvas(Rect(w * 1, 0, w, h));
        resize(img2, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);

        for (int j = 0; j < canvas.rows; j += 16)
            line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);

        imshow("rectified", canvas);
        waitKey(0);
        char c = static_cast<char>(waitKey());
        if (c == 27 || c == 'q' || c == 'Q')
            return;
    }
}
void Rectification::OneButtonRectify()
{
    ImageList imagelist;
    bool ok2 = imagelist.ReadStringList(imagelist.filenames.ORG_IMGS_PATH, imagelist.fringelist);
    bool ok3 = imagelist.ReadStringList(imagelist.filenames.RCT_IMGS_PATH, imagelist.rectimagelist);
    if(!ok2 ||!ok3 || imagelist.fringelist.empty() || imagelist.rectimagelist.empty())
    {
        cout << "can not open "  << " or the string list is empty" << endl;
    }
    this->ImgRectify(imagelist.filenames.INTRINSICS_PATH, imagelist.filenames.EXTRINSICS_PATH,imagelist.fringelist,imagelist.rectimagelist);
    cout << "end of Image rectification" << endl;
}
