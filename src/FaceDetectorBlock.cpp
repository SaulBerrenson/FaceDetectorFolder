#include "FaceDetectorBlock.h"
#include <QRunnable>
#include <qvariant.h>
#include <QThreadPool>

SharedPtr<FaceDetectorBlock> FaceDetectorBlock::create_block(const int& count_handlers)
{
	auto block = SharedPtr<FaceDetectorBlock>(new FaceDetectorBlock(count_handlers));
	if(block)
	{
		block->create_handlers();
		return block;
	}

	return nullptr;
}

bool FaceDetectorBlock::Start()
{

	emit OnStarted("Face detector is searching!");
	
	fill_files();

	QThreadPool thread_pool;
	thread_pool.setMaxThreadCount(m_handler_count);
	std::atomic<int> founded = 0;
	
	for (const auto& handler : m_handlers)
	{
		thread_pool.start(QRunnable::create(std::bind([&](QSharedPointer<FaceDetector> detector, std::atomic<int>& counter)
		{
			while (!this->m_files.isEmpty())
			{
				String file_path;
				{QMutexLocker ml(&this->m_mutex);
				file_path = m_files.dequeue(); }

				if (!detector->set_image(file_path)) continue;
				if (!detector->try_found_any_face()) continue;
				++counter;
				emit OnStarted(String("Found - %1").arg(counter));
				this->add_file_with_face(file_path);
			}

		}, handler, std::ref(founded))));
	}

	
	thread_pool.waitForDone();

	destroy_handlers();
	
	
	call_next_block(QVariant::fromValue(m_files_with_faces));
	return true;
}

bool FaceDetectorBlock::Stop()
{
	emit OnStoped("Face detector is stopped!");
	return true;
}


void FaceDetectorBlock::fill_files()
{
	auto payload = this->property(this->m_payload_name);
	
	if (!payload.isValid() || payload.isNull()) return;

	QMutexLocker ml(&m_mutex);
	for(const auto& path : payload.value<List<String>>())
	{
		m_files.enqueue(path);
	}
	//replace value for block
	this->setProperty(this->m_payload_name, "");
}

void FaceDetectorBlock::create_handlers()
{
	for (int i=0; i< m_handler_count;i++)	
		m_handlers.push_back(FaceDetector::create());	
}

void FaceDetectorBlock::destroy_handlers()
{
	for (int i = 0; i < m_handler_count; i++)
		m_handlers[i].reset();
}

void FaceDetectorBlock::add_file_with_face(const String& path)
{	
	QMutexLocker ml(&m_mutex);
	m_files_with_faces.push_back(path);	
}
