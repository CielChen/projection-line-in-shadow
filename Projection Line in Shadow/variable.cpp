/*
------------------------------------------------
Author: CIEL
Date: 2017/04/20
Function: �����ģ����õ���ȫ�ֱ���
------------------------------------------------
*/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
//#include "variable.h"  //ע�⣺��Ϊvariable.h�Ѿ���extern������ȫ�ֱ��������Բ���Ҫ��#include "variable.h"
using namespace cv;
using namespace std;


string scenePath="F:\\Code\\Projection Line in Shadow\\Data\\Scene\\";  //����ͼ·��
string backPath="F:\\Code\\Projection Line in Shadow\\Data\\Background\\";  //����ͼ·��
string picName="20170228111043";  //ͼƬ��

IplImage *scene=cvLoadImage((scenePath+picName+"_scene.jpg").data()); //����ͼ��
IplImage *back=cvLoadImage((backPath+picName+"_back.jpg").data());  //����ͼ��

//cvarrToMat()����Mat����ת��ΪIplImage��Ĭ�ϲ��������ݣ����Եڶ�������Ҫ��Ϊtrue
Mat backgroundMat=cvarrToMat(back,true);  //����ͼ��
Mat sceneMat=cvarrToMat(scene,true);   //����ͼ��

