/*
------------------------------------------------
Author: CIEL
Date: 2017/02/23
Function: ��ַ�����ȡͼƬǰ�����õ���ǰ��ͼƬ��RGBͼ
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
//#include "differRGB.h"  //ע�⣺��ΪdifferRGB.h�Ѿ���extern������ȫ�ֱ��������Բ���Ҫ��#include "differRGB.h"
#include "variable.h"

using namespace cv;
using namespace std;

int differRGB()
{
	//---------------��ʾͼ�񣺴˴�����ע�͵�-------------------
	//cvNamedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("����ͼ��", CV_WINDOW_AUTOSIZE);
	//cvShowImage("����ͼ��", scene);
	//cvShowImage("����ͼ��", back);
	//cvWaitKey(0);

	//IplImage *fore;
	IplImage *fore=cvCreateImage(cvGetSize(scene), scene->depth, scene->nChannels);  //����ǰ��ͼ��

	//��˹�˲���ƽ��ͼ�� 
	cvSmooth(scene, scene, CV_GAUSSIAN, 3, 3);
	cvSmooth(back, back, CV_GAUSSIAN, 3, 3);

	//---------------��ʾͼ�񣺴˴�����ע�͵�-------------------
	//cvNamedWindow("��˹�˲���ĳ���ͼ��", CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("��˹�˲���ı���ͼ��", CV_WINDOW_AUTOSIZE);
	//cvShowImage("��˹�˲���ĳ���ͼ��", scene);
	//cvShowImage("��˹�˲���ı���ͼ��", back);
	//cvWaitKey(0);

	//cvAbsDiff():�������������ľ���ֵ
	cvAbsDiff(scene, back, fore);  //��ַ�������-����=ǰ��

	uchar* foreData=(uchar *)fore->imageData;   //char *imageData:ָ�����е�ͼ������
	int foreStep=fore->widthStep/sizeof(uchar);  //int widthStep:���е�ͼ���д�С�����ֽ�Ϊ��λ
	int foreChannels=fore->nChannels;
	uchar* sceneData=(uchar *)scene->imageData;   //char *imageData:ָ�����е�ͼ������
	double similar=0;
	int threshold;
	for(int i=0;i<fore->height;i++)
	{
		for(int j=0;j<fore->width;j++)
		{
			//���㳡��ͼ���뱳��ͼ��ÿ�����ص�����ƶȣ�������ƣ�����Ϊ�������ص���ͬ����ô�ͽ��õ��ж�Ϊ��������ɫ��������Ϊǰ������ɫ��
			//�����ص�����ƶȶ��壺R,G,B���������Ĳ�ֵƽ����
			similar=pow(foreData[i*foreStep+j*foreChannels+0],2) + pow(foreData[i*foreStep+j*foreChannels+1],2) + pow(foreData[i*foreStep+j*foreChannels+2],2);
			threshold=5000;  //�ж����ƶȵ���ֵ
			if(similar<threshold)  //���ƶ�С��5000������Ϊ�������ص���ͬ����ô�ͽ��õ��ж�Ϊ��������ɫ��
			{ 
				foreData[i*foreStep+j*foreChannels+0]=0;  //B����
				foreData[i*foreStep+j*foreChannels+1]=255;  //G����
				foreData[i*foreStep+j*foreChannels+2]=255;  //R����
			}
			else  //����Ϊǰ������ɫ�볡����ͬ��
			{
				foreData[i*foreStep+j*foreChannels+0]=sceneData[i*foreStep+j*foreChannels+0];  //B����
				foreData[i*foreStep+j*foreChannels+1]=sceneData[i*foreStep+j*foreChannels+1];  //G����
				foreData[i*foreStep+j*foreChannels+2]=sceneData[i*foreStep+j*foreChannels+2];  //R����
			}
			
		}
	}
	//---------------��ʾͼ�񣺴˴�����ע�͵�-------------------
	//cvNamedWindow("δ�������foreground", CV_WINDOW_AUTOSIZE);
	//cvShowImage("δ�������foreground", fore);
	//cvWaitKey(0);


	//--------------------ʵ�ʲ�û�в��ö�ͼƬ�ĸ�ʴ������---------------------------
/*	//��ǰ��ͼƬ���и�ʴ�����ʹ���Ŀ�����£�
	//1.��������
	//2.�ָ��������ͼ��Ԫ�أ���ͼ�����������ڵ�Ԫ��
	//3.Ѱ��ͼ���е����Լ���ֵ�����Сֵ����
	//4.���ͼ����ݶ�
	cvErode(fore, fore, 0, 1);  //��ʴ
	cvNamedWindow("��ʴforeground", CV_WINDOW_AUTOSIZE);
	cvShowImage("��ʴforeground", fore); 
	cvWaitKey(0);

	//���˸о������ø�ʴЧ�������
	cvDilate(fore, fore, 0, 1);   //����
	cvNamedWindow("��ʴ+����foreground", CV_WINDOW_AUTOSIZE);
	cvShowImage("��ʴ+����foreground", fore);
	cvWaitKey(0);	
*/

	//Mat foreMat=cvarrToMat(fore);
	cvSaveImage((forePath+picName+"_fore.bmp").data(), fore);  //����ǰ��ͼƬ 

	cvReleaseImage(&scene);
	cvReleaseImage(&back);
	cvReleaseImage(&fore);

	return 0;
}