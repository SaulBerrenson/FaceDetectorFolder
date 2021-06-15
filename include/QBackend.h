#pragma once
#include <qobject.h>

#include "Forwards.h"
#include "IPipelineBlock.h"

class QBackend : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(QBackend)

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	Q_PROPERTY(QString m_status READ get_status WRITE set_status NOTIFY m_status_changed)
	Q_PROPERTY(QString m_directory_searching READ get_directory_searching WRITE set_m_directory_searching NOTIFY m_directory_searching_changed)
	Q_PROPERTY(int m_count_handlers READ get_count_handlers WRITE set_m_count_handlers NOTIFY m_count_handlers_changed)
	Q_PROPERTY(int m_current_page READ get_m_current_page WRITE set_m_current_page NOTIFY m_current_page_changed)
	Q_PROPERTY(QList<QVariant> m_file_model READ get_file_model WRITE set_file_model NOTIFY m_file_model_changed)
	///////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	QBackend();
	~QBackend() override = default;

		

	QString get_directory_searching() const;
	void set_m_directory_searching(const QString& payload);
	QString get_status() const;
	void set_status(const QString& payload);
	int get_count_handlers() const;
	void set_m_count_handlers(const int& count);
	int get_m_current_page() const;
	void set_m_current_page(const int& count);
	QList<QVariant> get_file_model() const;
	void set_file_model(const QList<QVariant>& payload);
	

	Q_INVOKABLE void open_dialog(QString path);
	Q_INVOKABLE void start_search();
signals:
	void m_directory_searching_changed(QString);
	void m_status_changed(QString);
	void m_count_handlers_changed(int);
	void m_current_page_changed(int);
	void m_file_model_changed(QList<QVariant>);


private:
	

	String m_directory_searching;
	String m_status;
	int m_count_handlers = 1, m_current_page = 0;
	QList<QVariant> m_file_model;
	
	List<SharedPtr<IPipelineBlock>> m_pipeline;
private slots:
	void OnStatus(const QString& message);
	void OnError(const QString& message, const bool& fatal = false);


private:
	void clean_pipeline();
	void create_pipeline();

};
