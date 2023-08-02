#include "mainwindow.h"
#include <QApplication>
#include "include/chessboardcalib.h"
#include "include/rectification.h"
#include "include/structurelightimages.h"
#include "include/graycode.h"
#include "include/wrapphase.h"
#include "include/unwrapphase.h"
#include "include/roi.h"
#include "include/match.h"
#include "include/triangulation.h"
#define ROW 1024
#define COL 1280
using namespace cv;
using namespace std;
int main(int argc, char *argv[])
{

    //---------------1.camera calibation------------
#if 0
    Chessboard cb(true, true);
    Calib *c = &cb;
    c->OneButtonCalib();         //在imagelist.h中修改标定图片目录， 内外参数目录
#endif

    //---------------2.rectify image-----------------
#if 0
    Rectification rect;
    rect.OneButtonRectify();
#endif

#if 1
    //---------------3.Structure Light Image handler----------
    StructureLightImages sli;
    sli.InitImages();

    Graycode gcl(ROW,COL);
    gcl.DecodeGray(sli.left_dec);
    //imshow("test", gc.dec);
    //waitKey(1000000);
    WrapPhase wpl(ROW,COL);
    wpl.DecodeWrapSave(sli.Left_cosin);
    //imshow("test", wp.wrapped_phase);
    //waitKey(1000000);
    Roi rl(ROW,COL);
    rl.SetRoi(sli.left_roi);
    Mat leftroi = rl.roi;
    //imshow("testl", rl.roi);
    //waitKey(1000000);
    UnwrapPhase uwpl(ROW,COL);
    uwpl.GetPhraseSave(gcl.dec,wpl.wrapped_phase, rl.roi);
    Mat leftphase = uwpl._phs;

    Graycode gcr(ROW,COL);
    WrapPhase wpr(ROW,COL);
    Roi rr(ROW,COL);
    UnwrapPhase uwpr(ROW,COL);
    gcr.DecodeGray(sli.right_dec);
    wpr.DecodeWrapSave(sli.right_cosin);
    rr.SetRoi(sli.right_roi);
    uwpr.GetPhraseSave(gcr.dec,wpr.wrapped_phase,rr.roi);
    Mat rightphase = uwpr._phs;
    Mat rightroi = rr.roi;
    //phase testing
    Mat test1;
    Mat test2;
    leftphase.convertTo(test1,CV_8UC3);
    rightphase.convertTo(test2, CV_8UC3);
    imwrite("leftphase.jpg",leftphase);
    imwrite("rightphase.jpg", rightphase);

    //imshow("test1",test1);
    //imshow("test2",test2);
    //waitKey(1000000);
    //imshow("test",rr.roi);
    //waitKey(1000000);
#endif
#if 1
    //---------------4.stereo macth--------------------------
    int64 t = getTickCount();
    Match mc;
    //mc.StereoCorrespond(leftphase,rightphase,leftroi,rightroi);
    mc.StereoCorrespond(uwpl._phs,uwpr._phs,rl.roi,rr.roi);
    t = getTickCount() - t;
    cout << "Time elapsed: " << t*1000/getTickFrequency() << "ms" <<endl;
    //---------------5.triangulate---------------------------
    Triangulation tl(static_cast<int>(mc.leftpoints.size()));
    tl.TriangulationPoints(sli.P1, sli.P2, mc.leftpoints,mc.rightpoints);
    tl.Save("points2.txt");
#endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
