#pragma once
#include <qobject.h>
#include <qvariant.h>


#include "Forwards.h"

class IPipelineBlock : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(IPipelineBlock)
public:

	

	void NeedStopBlock();
	void LinkToBlock(SharedPtr<IPipelineBlock> block);
	void Set_Payload(const String& key, const QVariant& value);
public slots:
	virtual bool Start() = 0;
	virtual bool Stop() = 0;
	

signals:
	void OnError(const QString& message,const bool& fatal = false);
	void OnStarted(const QString& message);
	void OnStoped(const QString& message);


protected:
	IPipelineBlock() = default;
	~IPipelineBlock() override = default;

	
	
	SharedPtr<IPipelineBlock> m_next_block;
	void call_next_block(const QVariant& value = 0);
	std::atomic<bool> m_need_stop = false;
	const char* m_payload_name = "payload";
};
