#pragma once
#include "FaceDetector.h"
#include "IPipelineBlock.h"
#include <QMutex>

class FaceDetectorBlock : public  IPipelineBlock 
{
public:

	~FaceDetectorBlock() override = default;

	static SharedPtr<FaceDetectorBlock> create_block(const int& count_handlers = 1);

public slots:
	bool Start() override;
	bool Stop() override;


public:
	explicit FaceDetectorBlock(int m_handler_count)
		: m_handler_count(m_handler_count)
	{
	}

private:
	Queue<String>	m_files;
	List<String>	m_files_with_faces;
	QMutex			m_mutex;
	List<SharedPtr<FaceDetector>> m_handlers;
	int				m_handler_count = 1;

	void fill_files();
	void create_handlers();
	void destroy_handlers();
	void add_file_with_face(const String& path);
};
