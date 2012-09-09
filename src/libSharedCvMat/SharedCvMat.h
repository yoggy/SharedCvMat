#pragma once

#pragma warning(disable : 4251)

#ifdef LIB_SHARED_CV_MAT_EXPORTS
	#define LIB_SHARED_CV_MAT_API __declspec(dllexport)
#else
	#define LIB_SHARED_CV_MAT_API __declspec(dllimport)
	#ifdef _DEBUG
		#pragma comment(lib, "libSharedCvMatD.lib")
	#else
		#pragma comment(lib, "libSharedCvMat.lib")
	#endif
#endif

#include <string>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <boost/shared_ptr.hpp>


class LIB_SHARED_CV_MAT_API SharedCvMat
{
public:
	SharedCvMat(const char *name, const int &width, const int &height, const int &channels, const bool &use_mutex=true);
	~SharedCvMat(void);

	int shmem_size() const;
	cv::Size size() const;
	int channels() const;

	void upload(const cv::Mat &image);
	void download(cv::Mat &image);


protected:
	char		name_[256];
	HANDLE		shmem_;
	HANDLE		mutex_;
	uchar		*buf_;
	cv::Mat		image_;
};

typedef boost::shared_ptr<SharedCvMat> shared_cv_mat_ptr;
