/*
------------------------------------------------
Author: CIEL
Date: 2017/04/20
Function: 保存各模块会用到的全局变量
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
//#include "variable.h"  //注意：因为variable.h已经用extern声明了全局变量，所以不需要再#include "variable.h"
using namespace cv;
using namespace std;


string scenePath="F:\\Code\\Projection Line in Shadow\\Data\\Scene\\";  //场景图路径
string backPath="F:\\Code\\Projection Line in Shadow\\Data\\Background\\";  //背景图路径
string picName="20170228111043";  //图片名

IplImage *scene=cvLoadImage((scenePath+picName+"_scene.jpg").data()); //场景图像
IplImage *back=cvLoadImage((backPath+picName+"_back.jpg").data());  //背景图像

//cvarrToMat()：将Mat类型转换为IplImage，默认不拷贝数据，所以第二个参数要置为true
Mat backgroundMat=cvarrToMat(back,true);  //背景图像
Mat sceneMat=cvarrToMat(scene,true);   //场景图像

