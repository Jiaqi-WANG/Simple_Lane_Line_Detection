#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//定义边缘化检测最大最小阈值
int low_threshold = 40;
int high_threshold = 150;

int main(int argc, const char** argv)
{

	//视频读取
	VideoCapture capture("solidWhiteRight.mp4");
	while (1) {
		Mat frame, image;
		vector<Vec4i> lines;
		capture >> image;

		//读取图片
		//image = imread("line.jpg");

		//转化至灰度图
		cvtColor(image, frame, COLOR_BGR2GRAY);

		//边缘检测
		Canny(frame, frame, low_threshold, high_threshold);

		//定义感兴趣区域的三个定点坐标(图片中心下方三角形)，图片大小为960*540，左上角坐标为(0,0)
		Point root_points[1][3];
		root_points[0][0] = Point(0, 539);
		root_points[0][1] = Point(480, 270);
		root_points[0][2] = Point(959, 539);

		//拟合线段
		const Point* ppt[1] = { root_points[0] };
		int npt[] = { 3 };
		polylines(frame, ppt, npt, 1, 1, Scalar(0, 0, 0), 1, 8, 0);

		//将感兴趣区域外全部黑化
		Mat mask, dst, line_image;
		frame.copyTo(mask);
		mask.setTo(cv::Scalar::all(0));
		fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255));
		frame.copyTo(dst, mask);

		//霍夫效应检测图片中直线
		HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 20, 1);

		//感兴趣区域内直线显示
		image.copyTo(line_image);
		for (int i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			line(line_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
		}
		imshow("Test", line_image);
		waitKey(10);
	}
	return 0;
}
