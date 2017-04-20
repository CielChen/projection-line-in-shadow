#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "differRGB.h"
#include "shadow_detection.h"
using namespace cv;
using namespace std;

int main()
{
	//--------------------step1. 差分法，得到前景RGB图--------------------------
	differRGB();

	//----------------------step2.阴影检测-------------------------------------
	//(1)色度差阴影检测
	//(2)亮度比阴影检测
	//（3）四邻域阴影检测优化
	//（4）最小连通域阴影检测优化
	shadowDetection();


	return 0;
}
