//保存各模块会用到的全局变量
//头文件中要使用extern关键字声明全局变量（不定义）
//如果这个变量有多个文件用到，可以新建一个cpp，在其中定义，把这个cpp加入工程即可。
//注意：头文件请不要定义任何变量，那是非常业余的行为……！！！！

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

