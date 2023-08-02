#include"match.h"
using namespace std;
using namespace cv;

void Match::StereoCorrespond(cv::Mat left_phase, cv::Mat right_phase,
                             cv::Mat left_thresh,cv::Mat right_thresh)


{
    cout << "Stereo Correspond " << endl;
    Point2f lk, rk; // 保存得到的左右图对应点

    //遍历图片
    for(int y = 0; y < left_phase.rows; y++)
    {

        float *left_ptr = left_phase.ptr<float>(y);
        float *right_ptr = right_phase.ptr<float>(y);
        uchar *l_thresh_ptr = left_thresh.ptr<uchar>(y);
        uchar *r_thresh_ptr = right_thresh.ptr<uchar>(y);
        for(int x = 0; x < left_phase.cols; x++)
        {
            map<float, Point2f> intps_ft_pt;//保存插值和坐标点：。
            map<myPoint, float> right_phase_pt_ft; //保存左图相 opencv 的Point没有重载 所以不能做为键值
            vector<float> intps; //保存对应的插值点

            //判断是否在感兴趣区间
            if( left_ptr[x] == 0.f  ||  l_thresh_ptr[x] != 255)
            {
                continue;
            }
            // k = 0;//遍历右图的下标

            //左图一个点 对应 右图同一行的所有像素点
            for(int k = 0; k < right_phase.cols; k++)
            {
                //右图不在感兴趣区间则跳过，
                if( (r_thresh_ptr[k] != 255) )// || isnan(left_ptr[x]) || isnan(right_ptr[k]) ) 之前的相位图 atan后会得到 nan
                {
                    continue;
                }
                //阈值处理
                //阈值愈大可接受的值越多，就越慢
                float tmp = abs(left_ptr[x] - right_ptr[k]);
                if(tmp > THS)
                    continue;

                //保存符合阈值的所以点， 找到其中最小的匹配点
                intps.push_back(tmp);
                //保存插值以及其后面排序找到最小的两个点的坐标
                intps_ft_pt.insert(pair<float, Point2f>(tmp,Size(k,y) )) ;
                myPoint mp;
                mp.p.y = y;
                mp.p.x = k;
                //保存坐标及其插值， 便于使用坐标查找其插值相位
                right_phase_pt_ft.insert(pair<myPoint, float>(mp, right_ptr[k]));

            }//结束右图的行
            //找完右图的一行，继续
            if(intps.size() > 1)
            {
                //排序后取最小的两个0，1
                //对插值数组排序， 然后用最小的两个去找坐标点。
                sort(intps.begin(), intps.end());
                Point2f right_position;

                //
                Point2f right_pst1; Point2f right_pst2;

                //因为map排序后， 右图的最小相位值 坐标不一定最小
                if(intps_ft_pt[ intps[0]].x < intps_ft_pt[intps[1]].x)
                {
                    right_pst1 = intps_ft_pt[ intps[0] ];

                    right_pst2 = intps_ft_pt[ intps[1] ];

                }
                else {
                    right_pst1 = intps_ft_pt[ intps[1] ];
                    right_pst2 = intps_ft_pt[ intps[0] ];
                }

                //使用自定义的map
                myPoint mp1;
                mp1.p = right_pst1;
                myPoint mp2;
                mp2.p = right_pst2;
                right_position.y = y;

                //单线性插值算法
                //输入: 左图两左边pst1， pst2, pst1的相位， pst2的相位。 左图的相位
                //输出: pst1 和 pst2 插值的相位
                right_position.x = SubPixel(
                            right_pst1, right_pst2,
                            right_phase_pt_ft[mp1], right_phase_pt_ft[mp2],
                            left_ptr[x]
                        );
                lk.x = x; lk.y = y;
                rk.x = right_position.x; rk.y = right_position.y;
                //得到对应点
                //left_key.push_back(lk);
                this->leftpoints.push_back(lk);
                //right_key.push_back(rk);
                this->rightpoints.push_back(rk);
            }
        }
    }
    cout << "end of Stereo Correspond" << endl;
}

float Match::SubPixel(cv::Point2f x1, cv::Point2f x2, float p1, float p2, float p_left)
{
    Point2f xr;
    float division = p2 - p1;
    if(division == 0.f)
       division = 0.000001f;
    xr.x = x2.x - ( (p2-p_left)*(x2.x-x1.x) / (division) );
    return xr.x;
}
void Match::StereoCorrespondSave(cv::Mat left_phase, cv::Mat right_phase,
                             cv::Mat left_thresh,cv::Mat right_thresh)


{
    cout << "Stereo Correspond " << endl;
    Point2f lk, rk; // 保存得到的左右图对应点

    //遍历图片
    for(int y = 0; y < left_phase.rows; y++)
    {
        for(int x = 0; x < left_phase.cols; x++)
        {
            map<float, Point2f> intps_ft_pt;//保存插值和坐标点：。
            map<myPoint, float> right_phase_pt_ft; //保存左图相 opencv 的Point没有重载 所以不能做为键值
            vector<float> intps; //保存对应的插值点

            //判断是否在感兴趣区间
            if(  left_phase.at<float>(y,x) == 0.f  ||   left_thresh.at<uchar>(y,x) != 255)
            {
                continue;
            }
            // k = 0;//遍历右图的下标

            //左图一个点 对应 右图同一行的所有像素点
            for(int k = 0; k < right_phase.cols; k++)
            {
                //右图不在感兴趣区间则跳过，
                if( ( right_thresh.at<uchar>(y,k)!= 255) )// || isnan(left_ptr[x]) || isnan(right_ptr[k]) ) 之前的相位图 atan后会得到 nan
                {
                    continue;
                }
                //阈值处理
                //阈值愈大可接受的值越多，就越慢
                float tmp = abs(left_phase.at<float>(y,x) - right_phase.at<float>(y,k));
                if(tmp > THS)
                    continue;

                //保存符合阈值的所以点， 找到其中最小的匹配点
                intps.push_back(tmp);
                //保存插值以及其后面排序找到最小的两个点的坐标
                intps_ft_pt.insert(pair<float, Point2f>(tmp,Size(k,y) )) ;
                myPoint mp;
                mp.p.y = y;
                mp.p.x = k;
                //保存坐标及其插值， 便于使用坐标查找其插值相位
                right_phase_pt_ft.insert(pair<myPoint, float>(mp, right_phase.at<float>(y,k)));

            }//结束右图的行
            //找完右图的一行，继续
            if(intps.size() > 1)
            {
                //排序后取最小的两个0，1
                //对插值数组排序， 然后用最小的两个去找坐标点。
                sort(intps.begin(), intps.end());
                Point2f right_position;

                //
                Point2f right_pst1; Point2f right_pst2;

                //因为map排序后， 右图的最小相位值 坐标不一定最小
                if(intps_ft_pt[ intps[0]].x < intps_ft_pt[intps[1]].x)
                {
                    right_pst1 = intps_ft_pt[ intps[0] ];

                    right_pst2 = intps_ft_pt[ intps[1] ];

                }
                else {
                    right_pst1 = intps_ft_pt[ intps[1] ];
                    right_pst2 = intps_ft_pt[ intps[0] ];
                }

                //使用自定义的map
                myPoint mp1;
                mp1.p = right_pst1;
                myPoint mp2;
                mp2.p = right_pst2;
                right_position.y = y;

                //单线性插值算法
                //输入: 左图两左边pst1， pst2, pst1的相位， pst2的相位。 左图的相位
                //输出: pst1 和 pst2 插值的相位
                right_position.x = SubPixel(
                            right_pst1, right_pst2,
                            right_phase_pt_ft[mp1], right_phase_pt_ft[mp2],
                            left_phase.at<float>(y,x)
                        );
                lk.x = x; lk.y = y;
                rk.x = right_position.x; rk.y = right_position.y;
                //得到对应点
                //left_key.push_back(lk);
                this->leftpoints.push_back(lk);
                //right_key.push_back(rk);
                this->rightpoints.push_back(rk);
            }
        }
    }
    cout << "end of Stereo Correspond" << endl;
}
