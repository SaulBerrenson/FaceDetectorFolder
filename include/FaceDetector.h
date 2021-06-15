#pragma once
#include <opencv2/opencv.hpp>

#include "Forwards.h"

class FaceDetector
{
public:
	static SharedPtr<FaceDetector> create();
	
	bool set_image(const String& path_to_image);
	bool try_found_any_face();

public:
	FaceDetector();
	~FaceDetector() = default;
private:	
	cv::CascadeClassifier	m_face_detector;
	cv::Mat					m_image;
	const cv::String		m_model_name = "model_face.xml";
};
