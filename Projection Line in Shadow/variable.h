//�����ģ����õ���ȫ�ֱ���
//ͷ�ļ���Ҫʹ��extern�ؼ�������ȫ�ֱ����������壩
//�����������ж���ļ��õ��������½�һ��cpp�������ж��壬�����cpp���빤�̼��ɡ�
//ע�⣺ͷ�ļ��벻Ҫ�����κα��������Ƿǳ�ҵ�����Ϊ������������

/*#include <iostream>
#include <string>
#include <highgui/highgui.hpp>
using namespace std;
*/
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace cv;
using namespace std;

extern string scenePath, backPath;
extern string picName;
extern IplImage *scene,*back;  
extern Mat backgroundMat;

