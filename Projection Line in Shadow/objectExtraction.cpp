/*
------------------------------------------------
Author: CIEL
Date: 2017/05/11
Function: ����Ч������������ȡ��������
step1.������������������
step2.��Ϊ����ڶ��������������һ��Ŀ϶���ͼƬ�߿�����Ч������
------------------------------------------------
*/

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
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

#define WIDTH 1216  //HoloLens��Ƶ��ͼ���
#define HEIGHT 684  //HoloLens��Ƶ��ͼ�߶�

Mat processedMat;  //�����ͼ��
Mat sceneShadowMat, backShadowMat;  //�洢�����ͱ����е���Ч��Ӱ
Mat BRMat;

//�Զ��������㷨:�������������
bool contoursAreaSort(vector<Point> contour1, vector<Point> contour2) 
{
	return contourArea(contour1,false)>contourArea(contour2,false);
}

//����Ӧ��ȡ��Ч����������
int objectExtraction()
{
	//�����Ѿ����ֺ��������Ӱ��ͼƬ
	extern Mat finalMat;
	processedMat=finalMat.clone();
	//---------------------------��ʾͼƬ������ע��-----------------------------
	//namedWindow("shadow in processed picture");
	//imshow("shadow in processed picture",processedMat);
	//waitKey(0);

	//--------------------����Ӧ��ȡ��Ч����--------------------
	//�뷨����Ϊ����ڶ��������������һ��Ŀ϶���ͼƬ�߿�����Ч������
	Mat processedGrayMat;
	//ת�Ҷ�ͼ
	cvtColor(processedMat,processedGrayMat,CV_BGR2GRAY);
	//����ƽ��
	//blur(processedGrayMat,processedGrayMat,Size(3,3));
	//namedWindow("�Ҷ�ͼ");
	//imshow("�Ҷ�ͼ",processedGrayMat);
	//waitKey(0);

	Mat threshold_object;
	vector<vector<Point>> contours;  //��⵽������
	//hierarchy:���������ļ̳й�ϵ,ÿ��Ԫ����һ�������ĸ�������������.hierarchy[i][0],hierarchy[i][1],hierarchy[i][2],hierarchy[i][3]
	//�ֱ��ʾ���ǵ�i������(contours[i])����һ����ǰһ���������ĵ�һ������(��һ��������)�Ͱ�����������(������)��
	vector<Vec4i> hierarchy;  

	//-------------------������⣺��ȡ������-------------------------
	//step1:��ֵ�ָ�
	//THRESH BINARY����������ֵ���趨һ����ʼ��ֵ��100�������100���趨Ϊ255����С�ڸ���ֵ���趨Ϊ0��
	threshold(processedGrayMat,threshold_object,100,255,THRESH_BINARY);
	//---------------------------��ʾͼƬ������ע��-----------------------------
	//namedWindow("����");
	//imshow("����",threshold_object);
	//waitKey(0);

	//step2����ȡ����
	//CV_RETR_TREE������������������ҽ������еļ̳�(����)��ϵ
	//CV_CHAIN_APPROX_NONE�������������еĵ�洢��
	findContours(threshold_object,contours,hierarchy,CV_RETR_TREE,CHAIN_APPROX_NONE,Point(0,0));

	//----------------------����αƽ�����----------------------
	vector<vector<Point>> contours_poly(contours.size());
	for(int i=0;i<contours.size();i++)
	{
		if(i==1)  //����ֻ��������ڶ�������
		{
			//approxPolyDP����ָ���ĵ㼯���бƽ�
			//��һ������������ĵ㼯
			//�ڶ�������������ĵ㼯����ǰ�㼯������С����ָ���㼯�ġ�draw��������һ������Σ�
			//������������ָ���ľ��ȣ�Ҳ����ԭʼ�������������֮��������롣
			//���ĸ�����bool closed����Ϊtrue,��˵�����������Ǳպϵģ�������λ��������
			approxPolyDP(Mat(contours[i]),contours_poly[i],0,true);  //�ƽ������
		}
	}

	//---------------------ֱ�۸����£���������������ע��---------------------------------------
	/*Mat drawingObject=Mat::zeros(threshold_object.size(),CV_8SC3);
	for(int i=0;i<contours.size();i++)
	{
		Scalar color=Scalar(211,55,155);
		drawContours(drawingObject,contours,i,color,2,8,vector<Vec4i>(),0,Point());
	}
	cout<<"���б�Ϊ�������������:"<<contours.size()<<endl;
	namedWindow("���б�Ϊ�������������");
	imshow("���б�Ϊ�������������",drawingObject);
	waitKey(0);
	*/

	//------------------------�������򣺽�������-------------------------
	sort(contours.begin(),contours.end(),contoursAreaSort);
	
	//----------������--------
	Mat objectMat=Mat::zeros(threshold_object.size(),CV_8SC3);
	Scalar color=Scalar(0,0,255);
	//������������1��˵�����Ƶڶ��������������壩
	//���һ��������-1����ʾ�������
	drawContours(objectMat,contours,1,color,-1);

	//----------------��ʾͼƬ����ע��-------
	//namedWindow("����");
	//imshow("����",object);
	//waitKey(0);
	//------����ͼƬ����ע��--------
	//string objectPath="F:\\Code\\Projection Line in Shadow\\Data\\Object\\";
	//imwrite(objectPath+picName+"_object.bmp",objectMat);

	return 0;
}

