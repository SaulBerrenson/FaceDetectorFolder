#include "FaceDetector.h"

#include <qdir.h>
#include <QFile>

SharedPtr<FaceDetector> FaceDetector::create()
{
	return SharedPtr<FaceDetector>(new FaceDetector());
}

FaceDetector::FaceDetector()
{
	auto path = QDir::cleanPath(QDir::currentPath() + QDir::separator() + QString::fromStdString(m_model_name));
	if (!QFile(path).exists()) { throw std::exception("File is not exist!"); return; }	
	m_face_detector.load(path.toStdString());
}

bool FaceDetector::set_image(const String& path_to_image)
{
	if (!QFile(path_to_image).exists()) return false;

	m_image.release();
	m_image = imread(path_to_image.toStdString(), cv::IMREAD_COLOR);

}

bool FaceDetector::try_found_any_face()
{
	if (m_image.empty()) return false;
	std::vector<cv::Rect> faces;

	cv::Mat img_gray;
	cv::cvtColor(m_image, img_gray, cv::COLOR_BGR2GRAY);
	m_image.release();
	m_face_detector.detectMultiScale(img_gray, faces, 1.1, 2, 0 | 2, cv::Size(10, 10));
	img_gray.release();
	
	if(faces.size() > 0) return true;
	return false;	
}


