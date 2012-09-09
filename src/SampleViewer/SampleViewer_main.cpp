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
	// 共有する先と名前とサイズをあわせておく
	SharedCvMat shared_img("capture_image", 640, 480, 3, false);

	cv::Mat image;
	while(true) {
		// 共有メモリからイメージをダウンロード
		shared_img.download(image);

		cv::imshow("shared image", image);

		int c = cv::waitKey(33);
		if (c == 27) break;
	}

	return 0;
}

