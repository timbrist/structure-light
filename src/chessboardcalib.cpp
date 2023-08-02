#include "chessboardcalib.h"
using namespace std;
using namespace cv;
Chessboard::Chessboard()
{
    boardSize = cv::Size(8,11);
    squareSize = 19.f;
    isDisplayCorners = true;
    isDisplayRectification = true;
}

Chessboard::Chessboard(cv::Size boardSize_, float squareSize_)
{
    this->boardSize = boardSize_;
    this->squareSize = squareSize_;
    isDisplayCorners = true;
    isDisplayRectification = true;
}

Chessboard::Chessboard(bool isDisplayCorners, bool isDisplayRectification)
{
    boardSize = cv::Size(8,11);
    squareSize = 19.f;
    this->isDisplayCorners = isDisplayCorners;
    this->isDisplayRectification = isDisplayRectification;
}
Chessboard::Chessboard(cv::Size boardSize_, float squareSize_, bool isDisplayCorners, bool isDisplayRectification)
{
    this->boardSize = boardSize_;
    this->squareSize = squareSize_;
    this->isDisplayCorners = isDisplayCorners;
    this->isDisplayRectification = isDisplayRectification;
}

void Chessboard::StereoCalib(const std::vector<std::string> &imagelist,
                            const std::string &save_intrinsics_path,
                            const std::string &save_extrinsics_path)
{
    if( imagelist.size() % 2 != 0 )
    {
        cout << "Error: the image list contains odd (non-even) number of elements\n";
        return;
    }

    const int maxScale = 2;
    //custom made:
    //const float squareSize = 37.f; //the sie of square
    // ARRAY AND VECTOR STORAGE:

    vector<vector<Point2f> > imagePoints[2];
    vector<vector<Point3f> > objectPoints;
    Size imageSize;

    int i, j, k, nimages = static_cast<int>(imagelist.size()/2);//   38/2 左右

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);

    vector<string> goodImageList;

    for( i = j = 0; i < nimages; i++ )
    {
        for( k = 0; k < 2; k++ )
        {
            const string& filename = imagelist[i*2+k];
            Mat img = imread(filename, 0);
            if(img.empty())
                break;
            if( imageSize == Size() )
                imageSize = img.size();
            else if( img.size() != imageSize )
            {
                cout << "The image " << filename << " has the size different from the first image size. Skipping the pair\n";
                break;
            }
            bool found = false;
            vector<Point2f>& corners = imagePoints[k][j];
            for( int scale = 1; scale <= maxScale; scale++ )
            {
                Mat timg;
                if( scale == 1 )//original: scale == 1
                    timg = img;
                else
                    resize(img, timg, Size(), scale, scale, INTER_LINEAR_EXACT);
                found = findChessboardCorners(timg, boardSize, corners,
                    CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
                if( found )
                {
                    if( scale > 1 )
                    {
                        Mat cornersMat(corners);
                        cornersMat *= 1./scale;
                    }
                    break;
                }
            }


            this->DisplayCorners(img, corners, found);

            if( !found )
                break;
            cornerSubPix(img, corners, Size(11,11), Size(-1,-1),
                         TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,
                                      30, 0.01));
        }
        if( k == 2 )
        {
            goodImageList.push_back(imagelist[i*2]);
            goodImageList.push_back(imagelist[i*2+1]);
            j++;
        }
    }
    cout << j << " pairs have been successfully detected.\n";
    nimages = j;
    if( nimages < 2 )
    {
        cout << "Error: too little pairs to run the calibration\n";
        return;
    }

    imagePoints[0].resize(nimages);
    imagePoints[1].resize(nimages);
    objectPoints.resize(nimages);

    for( i = 0; i < nimages; i++ )
    {
        for( j = 0; j < boardSize.height; j++ )
            for( k = 0; k < boardSize.width; k++ )
                objectPoints[i].push_back(Point3f(k*squareSize, j*squareSize, 0));
    }

    cout << "Running stereo calibration ...\n";

    /*
    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = initCameraMatrix2D(objectPoints,imagePoints[0],imageSize,0);
    cameraMatrix[1] = initCameraMatrix2D(objectPoints,imagePoints[1],imageSize,0);
    */
    intrinsic.cameraMatrix[0] = initCameraMatrix2D(objectPoints,imagePoints[0],imageSize,0);
    intrinsic.cameraMatrix[1] = initCameraMatrix2D(objectPoints,imagePoints[1],imageSize,0);
    //cameraMatrix[0] = Mat::eye(3,3,CV_64F);	//定义左相机矩阵
    //cameraMatrix[1] = Mat::eye(3,3,CV_64F); //定义右相机矩阵
    //Mat R; 外参
    //Mat T;
    Mat E, F;

    double rms = stereoCalibrate(objectPoints, imagePoints[0], imagePoints[1],
                    intrinsic.cameraMatrix[0], intrinsic.distCoeffs[0],
                    intrinsic.cameraMatrix[1], intrinsic.distCoeffs[1],
                    imageSize, extrinsic.R, extrinsic.T, E, F,
                    CALIB_FIX_ASPECT_RATIO +
                    CALIB_ZERO_TANGENT_DIST +
                    CALIB_USE_INTRINSIC_GUESS +
                    CALIB_SAME_FOCAL_LENGTH +
                    CALIB_RATIONAL_MODEL +
                    CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
                    TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5) );
    cout << "done with RMS error=" << rms << endl;

// CALIBRATION QUALITY CHECK
// because the output fundamental matrix implicitly
// includes all the output information,
// we can check the quality of calibration using the
// epipolar geometry constraint: m2^t*F*m1=0
    float err = 0;
    int npoints = 0;
    vector<Vec3f> lines[2];
    for( i = 0; i < nimages; i++ )//对 18进行遍历
    {
        int npt = static_cast<int>(imagePoints[0][i].size());
        Mat imgpt[2];
        for( k = 0; k < 2; k++ )
        {
            imgpt[k] = Mat(imagePoints[k][i]);
            undistortPoints(imgpt[k], imgpt[k], intrinsic.cameraMatrix[k], intrinsic.distCoeffs[k], Mat(), intrinsic.cameraMatrix[k]);
            computeCorrespondEpilines(imgpt[k], k+1, F, lines[k]);
        }
        for( j = 0; j < npt; j++ )
        {
            float errij = fabs(imagePoints[0][i][j].x*lines[1][j][0] +
                                imagePoints[0][i][j].y*lines[1][j][1] + lines[1][j][2]) +
                           fabs(imagePoints[1][i][j].x*lines[0][j][0] +
                                imagePoints[1][i][j].y*lines[0][j][1] + lines[0][j][2]);
            err += errij;
        }
        npoints += npt;
    }
    cout << "average epipolar err = " <<  err/npoints << endl;

    // save intrinsic parameters
    FileStorage fs(save_intrinsics_path, FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "M1" << intrinsic.cameraMatrix[0] << "D1" << intrinsic.distCoeffs[0] <<
            "M2" << intrinsic.cameraMatrix[1] << "D2" << intrinsic.distCoeffs[1];
        fs.release();
    }
    else
        cout << "Error: can not save the intrinsic parameters\n";

    //Mat R1, R2, P1, P2, Q;//extrinsics
    Rect validRoi[2];

    stereoRectify(intrinsic.cameraMatrix[0], intrinsic.distCoeffs[0],
                  intrinsic.cameraMatrix[1], intrinsic.distCoeffs[1],
                  imageSize, extrinsic.R, extrinsic.T, extrinsic.R1, extrinsic.R2, extrinsic.P1, extrinsic.P2, extrinsic.Q,
                  0, 1, imageSize, &validRoi[0], &validRoi[1]);//CALIB_ZERO_DISPARITY -> 0

    // OpenCV can handle left-right
    // or up-down camera arrangements
    //bool isVerticalStereo = fabs(extrinsic.P2.at<double>(1, 3)) > fabs(extrinsic.P2.at<double>(0, 3));

// COMPUTE AND DISPLAY RECTIFICATION
    //if( !showRectified )
    //if(!true)
        //return;

    //Mat rmap[2][2];
// CALIBRATED (BOUGUET'S METHOD)
    fs.open(save_extrinsics_path, FileStorage::WRITE);
    if( fs.isOpened() )
    {
        fs << "R" << extrinsic.R << "T" << extrinsic.T << "R1" << extrinsic.R1 << "R2" << extrinsic.R2 << "P1" << extrinsic.P1 << "P2" << extrinsic.P2 << "Q" << extrinsic.Q;
        fs.release();
    }
    else
        cout << "Error: can not save the extrinsic parameters\n";

    this->DisplayRectification(goodImageList,imageSize,nimages,validRoi);
}






//==================================private functions===========================================
void Chessboard::DisplayCorners(cv::Mat &img, std::vector<cv::Point2f> &corners,bool isFound)
{
            //if( displayCorners )
            if(this->isDisplayCorners)
            {
                //cout << filename << endl;
                Mat cimg, cimg1;
                cvtColor(img, cimg, COLOR_GRAY2BGR);
                drawChessboardCorners(cimg, boardSize, corners, isFound);
                double sf = 640./MAX(img.rows, img.cols);
                resize(cimg, cimg1, Size(), sf, sf, INTER_LINEAR_EXACT);
                imshow("corners", cimg1);
                //this this loop for show corners
                char c = static_cast<char>(waitKey(500));
                if( c == 27 || c == 'q' || c == 'Q' ) //Allow ESC to quit
                    exit(-1);
            }
            else
                putchar('.');

}

void Chessboard::DisplayRectification(vector<string> goodImageList,
                                      cv::Size &imageSize,
                                      int nimages,
                                      cv::Rect *validRoi)
{
    if(!this->isDisplayRectification) return;
    cv::Mat canvas;
    Mat rmap[2][2];
    // OpenCV can handle left-right
    // or up-down camera arrangements
    bool isVerticalStereo = fabs(extrinsic.P2.at<double>(1, 3)) > fabs(extrinsic.P2.at<double>(0, 3));

    //Precompute maps for cv::remap()
    initUndistortRectifyMap(intrinsic.cameraMatrix[0], intrinsic.distCoeffs[0], extrinsic.R1, extrinsic.P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
    initUndistortRectifyMap(intrinsic.cameraMatrix[1], intrinsic.distCoeffs[1], extrinsic.R2, extrinsic.P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);

    //Mat canvas;
    double sf;
    int w, h;
    if( !isVerticalStereo )
    {
        sf = 600./MAX(imageSize.width, imageSize.height);
        w = cvRound(imageSize.width*sf);
        h = cvRound(imageSize.height*sf);
        canvas.create(h, w*2, CV_8UC3);
    }
    else
    {
        sf = 300./MAX(imageSize.width, imageSize.height);
        w = cvRound(imageSize.width*sf);
        h = cvRound(imageSize.height*sf);
        canvas.create(h*2, w, CV_8UC3);
    }

    for( int i = 0; i < nimages; i++ )
    {
        for( int k = 0; k < 2; k++ )
        {
            Mat img = imread(goodImageList[i*2+k], 0), rimg, cimg;
            remap(img, rimg, rmap[k][0], rmap[k][1], INTER_LINEAR);
            cvtColor(rimg, cimg, COLOR_GRAY2BGR);
            Mat canvasPart = !isVerticalStereo ? canvas(Rect(w*k, 0, w, h)) : canvas(Rect(0, h*k, w, h));
            resize(cimg, canvasPart, canvasPart.size(), 0, 0, INTER_AREA);

            //if( useCalibrated )
            if(true)
            {
                Rect vroi(cvRound(validRoi[k].x*sf), cvRound(validRoi[k].y*sf),
                          cvRound(validRoi[k].width*sf), cvRound(validRoi[k].height*sf));
                rectangle(canvasPart, vroi, Scalar(0,0,255), 3, 8);
            }
        }

        if( !isVerticalStereo )
            for( int j = 0; j < canvas.rows; j += 16 )
                line(canvas, Point(0, j), Point(canvas.cols, j), Scalar(0, 255, 0), 1, 8);
        else
            for( int j = 0; j < canvas.cols; j += 16 )
                line(canvas, Point(j, 0), Point(j, canvas.rows), Scalar(0, 255, 0), 1, 8);
        //!!!!!!!!!!!!第二。 校正后的k图
        imshow("rectified", canvas);
        char c = static_cast<char>(waitKey());
        if( c == 27 || c == 'q' || c == 'Q' )
            break;
    }
}

void Chessboard::OneButtonCalib()
{
    ImageList imagelist;

    bool ok = imagelist.ReadStringList(imagelist.filenames.CLB_IMGS_PATH, imagelist.caliblist);
    if(!ok || imagelist.caliblist.empty())
    {
        cout << "can not open "  << " or the string list is empty" << endl;
    }
    this->StereoCalib(imagelist.caliblist,imagelist.filenames.INTRINSICS_PATH, imagelist.filenames.EXTRINSICS_PATH);
    cout << "end stereo Calib" << endl;
    cout << "Stereo Calib Handler finished" << endl;
}
