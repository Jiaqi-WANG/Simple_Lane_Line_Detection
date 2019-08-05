#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//�����Ե����������С��ֵ
int low_threshold = 40;
int high_threshold = 150;

int main(int argc, const char** argv)
{

	//��Ƶ��ȡ
	VideoCapture capture("solidWhiteRight.mp4");
	while (1) {
		Mat frame, image;
		vector<Vec4i> lines;
		capture >> image;

		//��ȡͼƬ
		//image = imread("line.jpg");

		//ת�����Ҷ�ͼ
		cvtColor(image, frame, COLOR_BGR2GRAY);

		//��Ե���
		Canny(frame, frame, low_threshold, high_threshold);

		//�������Ȥ�����������������(ͼƬ�����·�������)��ͼƬ��СΪ960*540�����Ͻ�����Ϊ(0,0)
		Point root_points[1][3];
		root_points[0][0] = Point(0, 539);
		root_points[0][1] = Point(480, 270);
		root_points[0][2] = Point(959, 539);

		//����߶�
		const Point* ppt[1] = { root_points[0] };
		int npt[] = { 3 };
		polylines(frame, ppt, npt, 1, 1, Scalar(0, 0, 0), 1, 8, 0);

		//������Ȥ������ȫ���ڻ�
		Mat mask, dst, line_image;
		frame.copyTo(mask);
		mask.setTo(cv::Scalar::all(0));
		fillPoly(mask, ppt, npt, 1, Scalar(255, 255, 255));
		frame.copyTo(dst, mask);

		//����ЧӦ���ͼƬ��ֱ��
		HoughLinesP(dst, lines, 1, CV_PI / 180, 80, 20, 1);

		//����Ȥ������ֱ����ʾ
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
