/*
------------------------------------------------
Author: CIEL
Date: 2017/02/23
Function: 差分法，提取图片前景，得到的前景图片是RGB图
------------------------------------------------
*/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include <Windows.h>
//#include "differRGB.h"  //注意：因为differRGB.h已经用extern声明了全局变量，所以不需要再#include "differRGB.h"
#include "variable.h"

using namespace cv;
using namespace std;

int differRGB()
{
	//---------------显示图像：此处可以注释掉-------------------
	//cvNamedWindow("场景图像", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("背景图像", CV_WINDOW_AUTOSIZE);
	//cvShowImage("场景图像", scene);
	//cvShowImage("背景图像", back);
	//cvWaitKey(0);

	//IplImage *fore;
	IplImage *fore=cvCreateImage(cvGetSize(scene), scene->depth, scene->nChannels);  //创建前景图像

	//高斯滤波，平滑图像 
	cvSmooth(scene, scene, CV_GAUSSIAN, 3, 3);
	cvSmooth(back, back, CV_GAUSSIAN, 3, 3);

	//---------------显示图像：此处可以注释掉-------------------
	//cvNamedWindow("高斯滤波后的场景图像", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("高斯滤波后的背景图像", CV_WINDOW_AUTOSIZE);
	//cvShowImage("高斯滤波后的场景图像", scene);
	//cvShowImage("高斯滤波后的背景图像", back);
	//cvWaitKey(0);

	//cvAbsDiff():计算两个数组差的绝对值
	cvAbsDiff(scene, back, fore);  //差分法：场景-背景=前景

	uchar* foreData=(uchar *)fore->imageData;   //char *imageData:指向排列的图像数据
	int foreStep=fore->widthStep/sizeof(uchar);  //int widthStep:排列的图像行大小，以字节为单位
	int foreChannels=fore->nChannels;
	uchar* sceneData=(uchar *)scene->imageData;   //char *imageData:指向排列的图像数据
	double similar=0;
	int threshold;
	for(int i=0;i<fore->height;i++)
	{
		for(int j=0;j<fore->width;j++)
		{
			//计算场景图像与背景图像每个像素点的相似度，如果相似，则认为两个像素点相同，那么就将该点判定为背景（黄色），否则为前景（红色）
			//两像素点的相似度定义：R,G,B三个分量的差值平方和
			similar=pow(foreData[i*foreStep+j*foreChannels+0],2) + pow(foreData[i*foreStep+j*foreChannels+1],2) + pow(foreData[i*foreStep+j*foreChannels+2],2);
			threshold=5000;  //判断相似度的阈值
			if(similar<threshold)  //相似度小于5000，则认为两个像素点相同，那么就将该点判定为背景（黄色）
			{ 
				foreData[i*foreStep+j*foreChannels+0]=0;  //B分量
				foreData[i*foreStep+j*foreChannels+1]=255;  //G分量
				foreData[i*foreStep+j*foreChannels+2]=255;  //R分量
			}
			else  //否则为前景（颜色与场景相同）
			{
				foreData[i*foreStep+j*foreChannels+0]=sceneData[i*foreStep+j*foreChannels+0];  //B分量
				foreData[i*foreStep+j*foreChannels+1]=sceneData[i*foreStep+j*foreChannels+1];  //G分量
				foreData[i*foreStep+j*foreChannels+2]=sceneData[i*foreStep+j*foreChannels+2];  //R分量
			}
			
		}
	}
	//---------------显示图像：此处可以注释掉-------------------
	//cvNamedWindow("未经处理的foreground", CV_WINDOW_AUTOSIZE);
	//cvShowImage("未经处理的foreground", fore);
	//cvWaitKey(0);


	//--------------------实际并没有采用对图片的腐蚀和膨胀---------------------------
/*	//对前景图片进行腐蚀和膨胀处理，目的如下：
	//1.消除噪声
	//2.分割出独立的图像元素，在图像中连接相邻的元素
	//3.寻找图像中的明显极大值区域或极小值区域
	//4.求出图像的梯度
	cvErode(fore, fore, 0, 1);  //腐蚀
	cvNamedWindow("腐蚀foreground", CV_WINDOW_AUTOSIZE);
	cvShowImage("腐蚀foreground", fore); 
	cvWaitKey(0);

	//个人感觉：仅用腐蚀效果会更好
	cvDilate(fore, fore, 0, 1);   //膨胀
	cvNamedWindow("腐蚀+膨胀foreground", CV_WINDOW_AUTOSIZE);
	cvShowImage("腐蚀+膨胀foreground", fore);
	cvWaitKey(0);	
*/

	//Mat foreMat=cvarrToMat(fore);
	cvSaveImage((forePath+picName+"_fore.bmp").data(), fore);  //保存前景图片 

	cvReleaseImage(&scene);
	cvReleaseImage(&back);
	cvReleaseImage(&fore);

	return 0;
}