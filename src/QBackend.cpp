#include "QBackend.h"

#include <QRunnable>
#include <QThreadPool>
#include <QtConcurrentRun>

#include "FaceDetectorBlock.h"
#include "FileSourceBlock.h"
#include "ResultBlock.h"


QBackend::QBackend()
{
	set_m_count_handlers(1);
	set_status("[STATUS]: Wait start...");
}

String QBackend::get_directory_searching() const
{
	return m_directory_searching;
}

int QBackend::get_count_handlers() const
{
	return m_count_handlers;
}

void QBackend::set_m_directory_searching(const QString& payload)
{
	m_directory_searching = std::move(payload);
	emit m_directory_searching_changed(m_directory_searching);
}

auto QBackend::get_status() const -> String
{
	return m_status;
}

auto QBackend::set_status(const QString& payload) -> void
{
	m_status = std::move(payload);
	emit m_status_changed(m_status);
}


void QBackend::set_m_count_handlers(const int& count)
{
	m_count_handlers = count;
	emit m_count_handlers_changed(m_count_handlers);
}

int QBackend::get_m_current_page() const
{
	return m_current_page;
}

void QBackend::set_m_current_page(const int& count)
{
	if (m_current_page == 0) clean_pipeline();
	
	m_current_page = count;
	emit m_current_page_changed(m_current_page);
}

QList<QVariant> QBackend::get_file_model() const
{
	return m_file_model;
}

void QBackend::set_file_model(const QList<QVariant>& payload)
{
	m_file_model = std::move(payload);
	emit m_file_model_changed(m_file_model);
}

void QBackend::open_dialog(QString path)
{
	set_m_directory_searching(path);
}

void QBackend::start_search()
{
	QtConcurrent::run([&]() 
	{
		create_pipeline();
	});
	
}

void QBackend::OnStatus(const QString& message)
{
	set_status(String("%1%2").arg("[STATUS]: ").arg(message));
}

void QBackend::OnError(const QString& message, const bool& fatal)
{
	String prefix = fatal ? "[FATAL]: " : "[ERROR]: ";	
	set_status(String("%1%2").arg(prefix).arg(message));
}

void QBackend::clean_pipeline()
{
	if (m_pipeline.size() <= 0) return;


	for (int i = 0; i < m_pipeline.count(); i++)
		if(m_pipeline[i])
			m_pipeline[i].reset();

	m_pipeline.clear();

	set_status("[STATUS]: Cleanup pipeline.");
}

void QBackend::create_pipeline()
{
	clean_pipeline();

	set_status("[STATUS]: Starting pipeline.");
	String search_dir = get_directory_searching();
	int count_handlers = get_count_handlers();


	auto file_source = FileSourceBlock::create_block(search_dir);
	auto detector = FaceDetectorBlock::create_block(count_handlers);
	auto result_block = ResultBlock::create_block();

	/////////////////////////////////////////////////////
	m_pipeline.push_back(file_source);
	m_pipeline.push_back(detector);
	m_pipeline.push_back(result_block);
	/////////////////////////////////////////////////////
	file_source->LinkToBlock(detector);
	detector->LinkToBlock(result_block);


	connect(file_source.get(), &FileSourceBlock::OnStarted, this, &QBackend::OnStatus, Qt::QueuedConnection);
	connect(file_source.get(), &FileSourceBlock::OnStoped, this, &QBackend::OnStatus, Qt::QueuedConnection);

	connect(detector.get(), &FaceDetectorBlock::OnStarted, this, &QBackend::OnStatus, Qt::QueuedConnection);
	connect(detector.get(), &FaceDetectorBlock::OnStoped, this, &QBackend::OnStatus, Qt::QueuedConnection);

	connect(result_block.get(), &ResultBlock::OnStarted, this, &QBackend::OnStatus, Qt::QueuedConnection);
	connect(result_block.get(), &ResultBlock::OnStoped, this, &QBackend::OnStatus, Qt::QueuedConnection);

	connect(file_source.get(), &FileSourceBlock::OnError, this, &QBackend::OnError, Qt::QueuedConnection);
	connect(detector.get(), &FaceDetectorBlock::OnError, this, &QBackend::OnError, Qt::QueuedConnection);
	connect(result_block.get(), &ResultBlock::OnError, this, &QBackend::OnError, Qt::QueuedConnection);




	connect(file_source.get(), &FileSourceBlock::OnStarted, this, [&](const String& message)
		{
			set_m_current_page(1);
		}, Qt::QueuedConnection);
	connect(detector.get(), &FaceDetectorBlock::OnStarted, this, [&](const String& message)
		{
			set_m_current_page(1);
		}, Qt::QueuedConnection);
	connect(result_block.get(), &ResultBlock::OnStoped, this, [&, result_block](const String& message)
		{
			set_m_current_page(2);
			List<String> images;
			if (result_block)
				images = result_block->GetResult();

			set_status(String("[STATUS]: Found image with faces - %1").arg(images.count()));

			QList<QVariant> for_model;

			for (auto& path : images)
				for_model.push_back(QVariant::fromValue(std::move(path)));

			set_file_model(for_model);

			clean_pipeline();
		}, Qt::QueuedConnection);


	file_source->Start();	
	
}
