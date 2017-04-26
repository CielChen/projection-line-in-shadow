/*
------------------------------------------------
Author: CIEL
Date: 2017/04/07
Function: ����Ӱ��BR�ߵ��㷨
step1.���볡��ͼ�ͱ���ͼ������Ч����Ӱ����ȡ����
step2.������Ч��Ӱ����ÿ�����ص�BR
step3.��x�᷽���ϣ��ҵ�BR��С�����أ�ȥ������Ӱ���ı߽磩
step4.������С���˷�����ϳ�BR��
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

#define WIDTH 1216  //HoloLens��Ƶ��ͼ���
#define HEIGHT 684  //HoloLens��Ƶ��ͼ�߶�

Mat processedMat;  //�����ͼ��
Mat sceneShadowMat, backShadowMat;  //�洢�����ͱ����е���Ч��Ӱ
Mat BRMat;

int processedRGB_B[HEIGHT][WIDTH],processedRGB_G[HEIGHT][WIDTH],processedRGB_R[HEIGHT][WIDTH];  //�����ͼ���RGB����
int backgroundShadowRGB_B[HEIGHT][WIDTH],backgroundShadowRGB_G[HEIGHT][WIDTH],backgroundShadowRGB_R[HEIGHT][WIDTH];  //����ͼ���RGB����
int backRGB_B[HEIGHT][WIDTH],backRGB_G[HEIGHT][WIDTH],backRGB_R[HEIGHT][WIDTH];
double br_B[HEIGHT][WIDTH],br_G[HEIGHT][WIDTH],br_R[HEIGHT][WIDTH];  //���Ȳ��RGB����

struct pixelInfo{   //�ṹ����ÿ�����ص����Ϣ
	int category;  //�ж����ص�����ࡣ0����Ч���أ�1����Ӱ
	int scene_B;  //����ͼBGR
	int scene_G;
	int scene_R;
	int back_B;  //����ͼBGR
	int back_G;
	int back_R;
	double BR_B;  //BR
	double BR_G;
	double BR_R;
	bool border;  //�ж��Ƿ�Ϊ��Ӱ��Ե��true,��Ե��false���Ǳ�Ե
};
struct pixelInfo graph[HEIGHT][WIDTH];

int shadowExtraction()
{
	//�����Ѿ����ֺ��������Ӱ��ͼƬ
	extern Mat finalMat;
	processedMat=finalMat.clone();
	//---------------------------��ʾͼƬ������ע��-----------------------------
	//namedWindow("shadow in processed picture");
	//imshow("shadow in processed picture",processedMat);
	//waitKey(0);

	//--------------------����Ӧ��ȡ��Ч��Ӱ--------------------
	//�뷨����Ϊ���������Ӱ������Ч��Ӱ��
	Mat processedGrayMat;
	//ת�Ҷ�ͼ
	cvtColor(processedMat,processedGrayMat,CV_BGR2GRAY);
	//����ƽ��
	//blur(processedGrayMat,processedGrayMat,Size(3,3));
	
	//---------------------------���õ��İ�����ͳ�ƣ�����ע��-------------------------
	/*cout<<"С��ͨ�������"<<count<<endl;
	cout<<"up������"<<up<<endl;
	cout<<"right������"<<right<<endl;
	cout<<"down������"<<down<<endl;
	cout<<"left������"<<left<<endl;
	cout<<"upleft������"<<upleft<<endl;
	cout<<"upright������"<<upright<<endl;
	cout<<"downleft������"<<downleft<<endl;
	cout<<"downright������"<<downright<<endl;
	cout<<"none������"<<none<<endl;
	*/
	
	//-----------------��ʾ����С����������ע��----------------------
	//cvNamedWindow("fill image", CV_WINDOW_AUTOSIZE);
	//cvShowImage("fill image", test);
	//cvWaitKey(0);
	//cvDestroyWindow("fill image");


	//------------�����µ�ͼ��-------
	Mat testMat(test, 0);
	for(int i=0; i<testMat.rows; i++)
	{
		for(int j=0; j<testMat.cols; j++)
		{
			//�����б��������������ر�ע��������graph[i][j].revise=1
			if( (testMat.at<Vec3b>(i,j)[0]==0 && testMat.at<Vec3b>(i,j)[1]==255 && testMat.at<Vec3b>(i,j)[2]==255) || (testMat.at<Vec3b>(i,j)[0]==0 && testMat.at<Vec3b>(i,j)[1]==0 && testMat.at<Vec3b>(i,j)[2]==255) || (testMat.at<Vec3b>(i,j)[0]==0 && testMat.at<Vec3b>(i,j)[1]==255 && testMat.at<Vec3b>(i,j)[2]==0))
				graph[i][j].revise=1;
		}
	}

	finalMat=spacialMat.clone();
	for(int i=0;i<finalMat.rows;i++)
	{
		for(int j=0;j<finalMat.cols;j++)
		{
			//�ڡ����������ص����ȱȡ�����Ļ����������޸�����
			if(graph[i][j].revise==1)
			{
				finalMat.at<Vec3b>(i,j)[0]=testMat.at<Vec3b>(i,j)[0];   
				finalMat.at<Vec3b>(i,j)[1]=testMat.at<Vec3b>(i,j)[1];
				finalMat.at<Vec3b>(i,j)[2]=testMat.at<Vec3b>(i,j)[2];
			}
		}
	}





























	//����ÿ������
	for(int i=0;i<processedMat.rows;i++)
	{
		const Vec3b* processedPoint=processedMat.ptr<Vec3b>(i);
		for(int j=0;j<processedMat.cols;j++)
		{
			Vec3b intensity=*(processedPoint+j);
			processedRGB_B[i][j]=intensity[0];
			processedRGB_G[i][j]=intensity[1];
			processedRGB_R[i][j]=intensity[2];

			//��ʼ���ṹ����ɫ����ɫͬ�����ͼ��
			if(processedRGB_B[i][j]==0 && processedRGB_G[i][j]==255 && processedRGB_R[i][j]==0)  //��Ӱ
				graph[i][j].category=1;
			else
				graph[i][j].category=0;  //��Ч����
		}
	}

	//----------------------------------------��ȡ�����е���Ч��Ӱ----------------------------------------------
	sceneShadowMat=sceneMat.clone();
	//---------------------------��ʾ����ͼƬ������ע��-----------------------------
	//namedWindow("scene picture");
	//imshow("scene picture",sceneShadowMat);
	//waitKey(0);

	//����ÿ������
	for(int i=0;i<sceneShadowMat.rows;i++)
	{
		for(int j=0;j<sceneShadowMat.cols;j++)
		{
			if(graph[i][j].category==0)  //��Ч���أ�����Ϊ��ɫ
			{
				sceneShadowMat.at<Vec3b>(i,j)[0]=0;
				sceneShadowMat.at<Vec3b>(i,j)[1]=0;
				sceneShadowMat.at<Vec3b>(i,j)[2]=0;
			}
		}
	}
	//---------------------------��ʾȥ�������ĳ���ͼƬ������ע��-----------------------------
	//namedWindow("scene without background");
	//imshow("scene without background",sceneShadowMat);
	//waitKey(0);

	//���½ṹ��graph
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

	//----------------------------��ȡ�����е���Ч��Ӱ----------------------------
	backShadowMat=backgroundMat.clone();
	//---------------------------��ʾ����ͼƬ������ע��-----------------------------
	//namedWindow("background picture");
	//imshow("background picture",backShadowMat);
	//waitKey(0);

	//����ÿ������
	for(int i=0;i<backShadowMat.rows;i++)
	{
		for(int j=0;j<backShadowMat.cols;j++)
		{
			if(graph[i][j].category==0)  //��Ч���أ�����Ϊ��ɫ
			{
				backShadowMat.at<Vec3b>(i,j)[0]=0;
				backShadowMat.at<Vec3b>(i,j)[1]=0;
				backShadowMat.at<Vec3b>(i,j)[2]=0;
			}
		}
	}
	//---------------------------��ʾȥ�����������ı���ͼƬ������ע��-----------------------------
	//namedWindow("off background");
	//imshow("off background",backShadowMat);
	//waitKey(0);

	//���½ṹ��graph
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

	//������Ӱ����BR
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			if(graph[i][j].category==0)  //��Ч���أ�BRΪMAX
			{
				br_B[i][j]=INT_MAX;
				br_G[i][j]=INT_MAX;
				br_R[i][j]=INT_MAX;
			}
			else
			{
				//ע�����ܻ���ַ�ĸΪ��������������һ��Ҫ�жϣ������������ĸΪ�㣬������Ϊ����С��������
				if(backgroundShadowRGB_B[i][j]==0)
					backgroundShadowRGB_B[i][j]=INT_MIN;  
				if(backgroundShadowRGB_G[i][j]==0)
					backgroundShadowRGB_G[i][j]=INT_MIN;
				if(backgroundShadowRGB_R[i][j]==0)
					backgroundShadowRGB_R[i][j]=INT_MIN;

				//ע��INT_MINΪ���������brֵ����Ϊ��
				br_B[i][j]=(double)graph[i][j].scene_B/backgroundShadowRGB_B[i][j];   
				br_G[i][j]=(double)graph[i][j].scene_G/backgroundShadowRGB_G[i][j];
				br_R[i][j]=(double)graph[i][j].scene_R/backgroundShadowRGB_R[i][j];

				//--------------�鿴�����ص�BRֵ-----------------
				//cout<<br_B[i][j]<<"\t"<<br_G[i][j]<<"\t"<<br_R[i][j]<<endl;
			}
		}
	}

	//���½ṹ��graph
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			graph[i][j].BR_B=br_B[i][j];
			graph[i][j].BR_G=br_G[i][j];
			graph[i][j].BR_R=br_R[i][j];
		}
	}


	//------------------------------------�������ص���Ϣ������txt�ļ�������ע��------------------------------------
	/*string graphInfoPath="F:\\Code\\Projection Line in Shadow\\Data\\GraphInfomation\\";
	string graphInfoName="pixelInfo.txt";
	ofstream graphInfoTxt(graphInfoPath+graphInfoName);  //���ļ�
	for(int i=0;i<HEIGHT;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			graphInfoTxt<<graph[i][j].category<<","<<graph[i][j].scene_B<<"/"<<graph[i][j].scene_G<<"/"<<graph[i][j].scene_R<<","<<graph[i][j].back_B<<"/"<<graph[i][j].back_G<<"/"<<graph[i][j].back_R<<","<<graph[i][j].BR_B<<"/"<<graph[i][j].BR_G<<"/"<<graph[i][j].BR_R;
		}
		graphInfoTxt<<endl;   //ÿ�������������ӻ���
	}
	graphInfoTxt.close();
	*/

	//--------------------------ȥ����Ӱ���ı�Ե�������������ٴμ���Ǳ�Ե��Ӱ��BR-----------------------------
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

