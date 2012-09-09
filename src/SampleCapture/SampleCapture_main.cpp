#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#ifdef WIN32
	#ifdef _DEBUG
		#pragma comment(lib, "opencv_core241d.lib")
		#pragma comment(lib, "opencv_imgproc241d.lib")
		#pragma comment(lib, "opencv_highgui241d.lib")
	#else
		#pragma comment(lib, "opencv_core241.lib")
		#pragma comment(lib, "opencv_imgproc241.lib")
		#pragma comment(lib, "opencv_highgui241.lib")
	#endif
#endif

#include "SharedCvMat.h"

int main(int argc, char* argv[])
{
	cv::VideoCapture capture;
	capture.open(0);

	// 共有する先と名前とサイズをあわせておく
	SharedCvMat shared_img("capture_image", 640, 480, 3, false);

	cv::Mat image;
	while(true) {
		capture >> image;

		// イメージを共有メモリへアップロード
		shared_img.upload(image);

		cv::imshow("capture", image);

		int c = cv::waitKey(10);
		if (c == 27) break;
	}

	return 0;
}

