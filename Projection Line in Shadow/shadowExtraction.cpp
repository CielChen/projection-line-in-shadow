/*
------------------------------------------------
Author: CIEL
Date: 2017/04/07
Function: 画阴影区BR线的算法
step1.读入场景图和背景图，将有效的阴影区提取出来
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
#include "shadowExtraction.h"

using namespace cv;
using namespace std;

#define WIDTH 1216  //HoloLens视频截图宽度
#define HEIGHT 684  //HoloLens视频截图高度

Mat processedMat;  //检测结果图像
Mat sceneShadowMat, backShadowMat;  //存储场景和背景中的有效阴影
Mat BRMat;

int processedRGB_B[HEIGHT][WIDTH],processedRGB_G[HEIGHT][WIDTH],processedRGB_R[HEIGHT][WIDTH];  //检测结果图像的RGB分量
int backgroundShadowRGB_B[HEIGHT][WIDTH],backgroundShadowRGB_G[HEIGHT][WIDTH],backgroundShadowRGB_R[HEIGHT][WIDTH];  //背景图像的RGB分量
int backRGB_B[HEIGHT][WIDTH],backRGB_G[HEIGHT][WIDTH],backRGB_R[HEIGHT][WIDTH];
double br_B[HEIGHT][WIDTH],br_G[HEIGHT][WIDTH],br_R[HEIGHT][WIDTH];  //亮度差的RGB分量

struct pixelInfo{   //结构体存放每个像素点的信息
	int category;  //判断像素点的种类。0，无效像素；1，阴影
	int scene_B;  //场景图BGR
	int scene_G;
	int scene_R;
	int back_B;  //背景图BGR
	int back_G;
	int back_R;
	double BR_B;  //BR
	double BR_G;
	double BR_R;
	bool border;  //判断是否为阴影边缘：true,边缘；false，非边缘
};
struct pixelInfo graph[HEIGHT][WIDTH];

int shadowExtraction()
{
	//读入已经区分好物体和阴影的图片
	extern Mat finalMat;
	processedMat=finalMat.clone();
	//---------------------------显示图片，可以注释-----------------------------
	//namedWindow("shadow in processed picture");
	//imshow("shadow in processed picture",processedMat);
	//waitKey(0);

	//--------------------自适应提取有效阴影--------------------
	//想法：认为面积最大的阴影区是有效阴影区
	Mat processedGrayMat;
	//转灰度图
	cvtColor(processedMat,processedGrayMat,CV_BGR2GRAY);
	//进行平滑
	//blur(processedGrayMat,processedGrayMat,Size(3,3));
	
	//---------------------------利用到的八邻域统计，可以注释-------------------------
	/*cout<<"小连通域个数："<<count<<endl;
	cout<<"up个数："<<up<<endl;
	cout<<"right个数："<<right<<endl;
	cout<<"down个数："<<down<<endl;
	cout<<"left个数："<<left<<endl;
	cout<<"upleft个数："<<upleft<<endl;
	cout<<"upright个数："<<upright<<endl;
	cout<<"downleft个数："<<downleft<<endl;
	cout<<"downright个数："<<downright<<endl;
	cout<<"none个数："<<none<<endl;
	*/
	
	//-----------------显示填充的小轮廓，可以注释----------------------
	//cvNamedWindow("fill image", CV_WINDOW_AUTOSIZE);
	//cvShowImage("fill image", test);
	//cvWaitKey(0);
	//cvDestroyWindow("fill image");


	//------------保存新的图像-------
	Mat testMat(test, 0);
	for(int i=0; i<testMat.rows; i++)
	{
		for(int j=0; j<testMat.cols; j++)
		{
			//将所有被重新填充过的像素标注出来，即graph[i][j].revise=1
			if( (testMat.at<Vec3b>(i,j)[0]==0 && testMat.at<Vec3b>(i,j)[1]==255 && testMat.at<Vec3b>(i,j)[2]==255) || (testMat.at<Vec3b>(i,j)[0]==0 && testMat.at<Vec3b>(i,j)[1]==0 && testMat.at<Vec3b>(i,j)[2]==255) || (testMat.at<Vec3b>(i,j)[0]==0 && testMat.at<Vec3b>(i,j)[1]==255 && testMat.at<Vec3b>(i,j)[2]==0))
				graph[i][j].revise=1;
		}
	}

	finalMat=spacialMat.clone();
	for(int i=0;i<finalMat.rows;i++)
	{
		for(int j=0;j<finalMat.cols;j++)
		{
			//在“四邻域像素的亮度比”结果的基础上重新修改像素
			if(graph[i][j].revise==1)
			{
				finalMat.at<Vec3b>(i,j)[0]=testMat.at<Vec3b>(i,j)[0];   
				finalMat.at<Vec3b>(i,j)[1]=testMat.at<Vec3b>(i,j)[1];
				finalMat.at<Vec3b>(i,j)[2]=testMat.at<Vec3b>(i,j)[2];
			}
		}
	}





























	//遍历每个像素
	for(int i=0;i<processedMat.rows;i++)
	{
		const Vec3b* processedPoint=processedMat.ptr<Vec3b>(i);
		for(int j=0;j<processedMat.cols;j++)
		{
			Vec3b intensity=*(processedPoint+j);
			processedRGB_B[i][j]=intensity[0];
			processedRGB_G[i][j]=intensity[1];
			processedRGB_R[i][j]=intensity[2];

			//初始化结构体颜色，颜色同检测结果图像
			if(processedRGB_B[i][j]==0 && processedRGB_G[i][j]==255 && processedRGB_R[i][j]==0)  //阴影
				graph[i][j].category=1;
			else
				graph[i][j].category=0;  //无效像素
		}
	}

	//----------------------------------------提取场景中的有效阴影----------------------------------------------
	sceneShadowMat=sceneMat.clone();
	//---------------------------显示场景图片，可以注释-----------------------------
	//namedWindow("scene picture");
	//imshow("scene picture",sceneShadowMat);
	//waitKey(0);

	//遍历每个像素
	for(int i=0;i<sceneShadowMat.rows;i++)
	{
		for(int j=0;j<sceneShadowMat.cols;j++)
		{
			if(graph[i][j].category==0)  //无效像素，设置为黑色
			{
				sceneShadowMat.at<Vec3b>(i,j)[0]=0;
				sceneShadowMat.at<Vec3b>(i,j)[1]=0;
				sceneShadowMat.at<Vec3b>(i,j)[2]=0;
			}
		}
	}
	//---------------------------显示去掉背景的场景图片，可以注释-----------------------------
	//namedWindow("scene without background");
	//imshow("scene without background",sceneShadowMat);
	//waitKey(0);

	//更新结构体graph
	for(int i=0;i<sceneShadowMat.rows;i++)
	{
		const Vec3b* sceneShadowPoint=sceneShadowMat.ptr<Vec3b>(i);			
		for(int j=0;j<sceneShadowMat.cols;j++)
		{
			Vec3b intensity=*(sceneShadowPoint+j);
			graph[i][j].scene_B=intensity[0];
			graph[i][j].scene_G=intensity[1];
			graph[i][j].scene_R=intensity[2];
		}
	}

	//----------------------------提取背景中的有效阴影----------------------------
	backShadowMat=backgroundMat.clone();
	//---------------------------显示背景图片，可以注释-----------------------------
	//namedWindow("background picture");
	//imshow("background picture",backShadowMat);
	//waitKey(0);

	//遍历每个像素
	for(int i=0;i<backShadowMat.rows;i++)
	{
		for(int j=0;j<backShadowMat.cols;j++)
		{
			if(graph[i][j].category==0)  //无效像素，设置为黑色
			{
				backShadowMat.at<Vec3b>(i,j)[0]=0;
				backShadowMat.at<Vec3b>(i,j)[1]=0;
				backShadowMat.at<Vec3b>(i,j)[2]=0;
			}
		}
	}
	//---------------------------显示去掉场景背景的背景图片，可以注释-----------------------------
	//namedWindow("off background");
	//imshow("off background",backShadowMat);
	//waitKey(0);

	//更新结构体graph
	for(int i=0;i<backShadowMat.rows;i++)
	{
		const Vec3b* backShadowPoint=backShadowMat.ptr<Vec3b>(i);			
		for(int j=0;j<backShadowMat.cols;j++)
		{
			Vec3b intensity=*(backShadowPoint+j);
			graph[i][j].back_B=intensity[0];
			graph[i][j].back_G=intensity[1];
			graph[i][j].back_R=intensity[2];
		}
	}

	//计算阴影区的BR
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			if(graph[i][j].category==0)  //无效像素，BR为MAX
			{
				br_B[i][j]=INT_MAX;
				br_G[i][j]=INT_MAX;
				br_R[i][j]=INT_MAX;
			}
			else
			{
				//注：可能会出现分母为零的情况！！！！一定要判断！！！！如果分母为零，则将其设为无穷小！！！！
				if(backgroundShadowRGB_B[i][j]==0)
					backgroundShadowRGB_B[i][j]=INT_MIN;  
				if(backgroundShadowRGB_G[i][j]==0)
					backgroundShadowRGB_G[i][j]=INT_MIN;
				if(backgroundShadowRGB_R[i][j]==0)
					backgroundShadowRGB_R[i][j]=INT_MIN;

				//注：INT_MIN为负，算出的br值可能为负
				br_B[i][j]=(double)graph[i][j].scene_B/backgroundShadowRGB_B[i][j];   
				br_G[i][j]=(double)graph[i][j].scene_G/backgroundShadowRGB_G[i][j];
				br_R[i][j]=(double)graph[i][j].scene_R/backgroundShadowRGB_R[i][j];

				//--------------查看各像素的BR值-----------------
				//cout<<br_B[i][j]<<"\t"<<br_G[i][j]<<"\t"<<br_R[i][j]<<endl;
			}
		}
	}

	//更新结构体graph
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			graph[i][j].BR_B=br_B[i][j];
			graph[i][j].BR_G=br_G[i][j];
			graph[i][j].BR_R=br_R[i][j];
		}
	}


	//------------------------------------将各像素的信息保存至txt文件，可以注释------------------------------------
	/*string graphInfoPath="F:\\Code\\Projection Line in Shadow\\Data\\GraphInfomation\\";
	string graphInfoName="pixelInfo.txt";
	ofstream graphInfoTxt(graphInfoPath+graphInfoName);  //打开文件
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			graphInfoTxt<<graph[i][j].category<<","<<graph[i][j].scene_B<<"/"<<graph[i][j].scene_G<<"/"<<graph[i][j].scene_R<<","<<graph[i][j].back_B<<"/"<<graph[i][j].back_G<<"/"<<graph[i][j].back_R<<","<<graph[i][j].BR_B<<"/"<<graph[i][j].BR_G<<"/"<<graph[i][j].BR_R;
		}
		graphInfoTxt<<endl;   //每行输出结束，添加换行
	}
	graphInfoTxt.close();
	*/

	//--------------------------去掉阴影区的边缘，并利用邻域再次计算非边缘阴影的BR-----------------------------
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			if(graph[i][j].category==1)
			{
				if(graph[i-1][j].category==1 && graph[i][j-1].category==1 && graph[i][j+1].category==1 && graph[i+1][j].category==1)
				{
					graph[i][j].border=false;
					graph[i][j].BR_B=(graph[i][j].BR_B+graph[i-1][j].BR_B+graph[i][j-1].BR_B+graph[i][j+1].BR_B+graph[i+1][j].BR_B)/5;
					graph[i][j].BR_G=(graph[i][j].BR_G+graph[i-1][j].BR_G+graph[i][j-1].BR_G+graph[i][j+1].BR_G+graph[i+1][j].BR_G)/5;
					graph[i][j].BR_R=(graph[i][j].BR_R+graph[i-1][j].BR_R+graph[i][j-1].BR_R+graph[i][j+1].BR_R+graph[i+1][j].BR_R)/5;
				}
				else
					graph[i][j].border=true;
			}
		}
	}

	return 0;
}

