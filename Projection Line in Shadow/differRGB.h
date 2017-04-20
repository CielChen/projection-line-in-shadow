#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace cv;
using namespace std;

//extern Mat foreMat;
extern IplImage *fore;
int differRGB();  //差分法，得到的前景为RGB图
