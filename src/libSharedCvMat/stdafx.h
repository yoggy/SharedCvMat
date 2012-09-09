#pragma once

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

#include <boost/shared_ptr.hpp>

// for OpenCV2
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