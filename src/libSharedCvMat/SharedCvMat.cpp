#include "StdAfx.h"

#define LIB_SHARED_CV_MAT_EXPORTS
#include "SharedCvMat.h"

#pragma warning(disable : 4996)

class ScopedMutexLock {
protected:
	HANDLE mutex_;

public:
	ScopedMutexLock(HANDLE mutex) : mutex_(INVALID_HANDLE_VALUE) {
		mutex_ = mutex;
		if (mutex_ != INVALID_HANDLE_VALUE) {
			WaitForSingleObject(mutex_, INFINITE);
		}
	};

	virtual ~ScopedMutexLock() {
		if (mutex_ != INVALID_HANDLE_VALUE) {
			::ReleaseMutex(mutex_);
			mutex_ = INVALID_HANDLE_VALUE;
		}
	};
};


LIB_SHARED_CV_MAT_API 
SharedCvMat::SharedCvMat(const char *name, const int &width, const int &height, const int &channels, const bool &use_mutex)
	: shmem_(INVALID_HANDLE_VALUE), mutex_(INVALID_HANDLE_VALUE), buf_(NULL)
{
	assert(name_ != NULL);
	assert(width > 0 && height > 0 && channels > 0);
	assert(channels == 1 || channels == 3);

	strncpy(name_, name, 255);
	
	if (channels == 1) {
		image_.create(cv::Size(width, height), CV_8UC1);
	}
	else if (channels == 3) {
		image_.create(cv::Size(width, height), CV_8UC3);
	}

	if (use_mutex) {
		char mutex_name[512];
		_snprintf(mutex_name, 511, "mutex_%s", name_); 
		mutex_ = ::CreateMutex(NULL, FALSE, mutex_name);
	}

	char shared_memory_name[512];
	_snprintf(shared_memory_name, 511, "shared_%s", name_); 

	shmem_ = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		shmem_size(),
		shared_memory_name
		);

	buf_ = (uchar*)::MapViewOfFile(shmem_, FILE_MAP_ALL_ACCESS, 0, 0, shmem_size());

	image_.data = buf_;
}

LIB_SHARED_CV_MAT_API 
SharedCvMat::~SharedCvMat(void)
{
	if (buf_ != NULL) {
		::UnmapViewOfFile(buf_);
		buf_ = NULL;
	}

	if (shmem_ != INVALID_HANDLE_VALUE) {
		::CloseHandle(shmem_);
		shmem_ = INVALID_HANDLE_VALUE;
	}

	if (mutex_ != INVALID_HANDLE_VALUE) {
		::ReleaseMutex(mutex_);
		mutex_ = INVALID_HANDLE_VALUE;
	}
}

LIB_SHARED_CV_MAT_API 
void SharedCvMat::upload(const cv::Mat &image)
{
	ScopedMutexLock lock(mutex_);

	assert(buf_ != NULL);

	if (image.empty()) {
		image_.setTo(0);
		return;
	}

	if (image.size() == image_.size()) {
		image.copyTo(image_);
	}
	else {
		cv::resize(image, image_, image_.size());
	}
}

LIB_SHARED_CV_MAT_API 
void SharedCvMat::download(cv::Mat &image)
{
	ScopedMutexLock lock(mutex_);

	assert(buf_ != NULL);

	if (image.empty()) {
		if (channels() == 1) {
			image.create(size(), CV_8UC1);
		}
		else {
			image.create(size(), CV_8UC3);
		}
	}
	
	if (image.size() == image_.size()) {
		image_.copyTo(image);
	}
	else {
		cv::resize(image_, image, image.size());
	}	
}

LIB_SHARED_CV_MAT_API 
int SharedCvMat::shmem_size() const
{
	return size().width * size().height * channels();
}

LIB_SHARED_CV_MAT_API 
int SharedCvMat::channels() const
{
	return this->image_.channels();
}

LIB_SHARED_CV_MAT_API 
cv::Size SharedCvMat::size() const
{
	return this->image_.size();
}
