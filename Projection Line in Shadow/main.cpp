#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "shadow_detection.h"
#include "objectExtraction.h"
using namespace cv;
using namespace std;

int main()
{
	//--------------------step1. ��ַ����õ�ǰ��RGBͼ--------------------------
	differRGB();

	//----------------------step2.��Ӱ���-------------------------------------
	//(1)ɫ�Ȳ���Ӱ���
	//(2)���ȱ���Ӱ���
	//��3����������Ӱ����Ż�
	//��4����С��ͨ����Ӱ����Ż�
	shadowDetection();

	//--------------------step3. ��ȡ��Ч��������-----------------------
	objectExtraction();

	system("pause");
	return 0;
}
