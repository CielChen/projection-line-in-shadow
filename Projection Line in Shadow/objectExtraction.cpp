/*
------------------------------------------------
Author: CIEL
Date: 2017/04/07
Function: 画阴影区BR线的算法
step1.读入场景图和背景图，将有效的物体区和阴影区提取出来
step2.计算有效阴影区中每个像素的BR
step3.在x轴方向上，找到BR最小的像素（去掉了阴影区的边界）
step4.利用最小二乘法，拟合出BR线
------------------------------------------------
*/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <cv.h>
#include <vector>
#include <cxcore.h>
#include <highgui.h>
#include <Windows.h>
#include <math.h>
#include "variable.h"
#include "objectExtraction.h"
using namespace cv;
using namespace std;

#define WIDTH 1216  //HoloLens视频截图宽度
#define HEIGHT 684  //HoloLens视频截图高度

Mat processedMat;  //检测结果图像
Mat sceneShadowMat, backShadowMat;  //存储场景和背景中的有效阴影
Mat BRMat;

//自定义排序算法:按轮廓面积升序
bool contoursAreaSort(vector<Point> contour1, vector<Point> contour2) 
{
	return contourArea(contour1,false)<contourArea(contour2,false);
}

//自适应获取有效的物体轮廓
int objectExtraction()
{
	//读入已经区分好物体和阴影的图片
	extern Mat finalMat;
	processedMat=finalMat.clone();
	//---------------------------显示图片，可以注释-----------------------------
	//namedWindow("shadow in processed picture");
	//imshow("shadow in processed picture",processedMat);
	//waitKey(0);

	//--------------------自适应提取有效物体--------------------
	//想法：认为面积第二大的物体区（第一大的肯定是图片边框）是有效物体区
	Mat processedGrayMat;
	//转灰度图
	cvtColor(processedMat,processedGrayMat,CV_BGR2GRAY);
	//进行平滑
	//blur(processedGrayMat,processedGrayMat,Size(3,3));
	//namedWindow("灰度图");
	//imshow("灰度图",processedGrayMat);
	//waitKey(0);

	Mat threshold_object;
	vector<vector<Point>> contours;  //检测到的轮廓
	//hierarchy:各个轮廓的继承关系,每个元素是一个包含四个整型数的向量.hierarchy[i][0],hierarchy[i][1],hierarchy[i][2],hierarchy[i][3]
	//分别表示的是第i条轮廓(contours[i])的下一条，前一条，包含的第一条轮廓(第一条子轮廓)和包含他的轮廓(父轮廓)。
	vector<Vec4i> hierarchy;  

	//-------------------轮廓检测：提取出物体-------------------------
	//THRESH BINARY：二进制阈值。设定一个初始阈值如125，则大于125的设定为255，而小于该阈值的设定为0。
	threshold(processedGrayMat,threshold_object,100,255,THRESH_BINARY);
	//---------------------------显示图片，可以注释-----------------------------
	//namedWindow("轮廓");
	//imshow("轮廓",threshold_object);
	//waitKey(0);

	//CV_RETR_TREE：检测所有轮廓，并且建立所有的继承(包含)关系
	//CV_CHAIN_APPROX_NONE：把轮廓上所有的点存储。
	findContours(threshold_object,contours,hierarchy,CV_RETR_TREE,CHAIN_APPROX_NONE,Point(0,0));

	//----------------------多边形逼近轮廓----------------------
	vector<vector<Point>> contours_poly(contours.size());
	for(int i=0;i<contours.size();i++)
	{
		if(i==1)  //这里只控制输出第二层轮廓
		{
			//approxPolyDP：对指定的点集进行逼近
			//第一个参数：输入的点集
			//第二个参数：输出的点集，当前点集是能最小包容指定点集的。draw出来即是一个多边形；
			//第三个参数：指定的精度，也即是原始曲线与近似曲线之间的最大距离。
			//第四个参数bool closed：若为true,则说明近似曲线是闭合的，它的首位都是相连
			approxPolyDP(Mat(contours[i]),contours_poly[i],0,true);  //逼近多边形
		}
	}
	//画所有轮廓
	Mat drawingObject=Mat::zeros(threshold_object.size(),CV_8SC3);
	Mat objectAllContours;
	objectAllContours=processedMat.clone();
	for(int i=0;i<contours.size();i++)
	{
		Scalar color=Scalar(211,55,155);
		drawContours(objectAllContours,contours,i,color,2,8,vector<Vec4i>(),0,Point());
	}
	//cout<<"被判别为物体的总轮廓数:"<<contours.size()<<endl;
	//namedWindow("被判别为物体的总轮廓数");
	//imshow("被判别为物体的总轮廓数",drawingObject);
	//waitKey(0);

	return 0;
}

