#include "IPipelineBlock.h"

void IPipelineBlock::NeedStopBlock()
{
	m_need_stop = true;
}

void IPipelineBlock::LinkToBlock(SharedPtr<IPipelineBlock> block)
{
	if (block)
		m_next_block = block;	
}

void IPipelineBlock::Set_Payload(const String& key, const QVariant& value)
{

	this->setProperty(key.toStdString().c_str(), std::move(value));
}

void IPipelineBlock::call_next_block(const QVariant& value)
{
	if(m_need_stop)
	{
		emit OnStoped("Manual request stop pipeline!");
		return;
	}

	if (!m_next_block)
	{
		emit OnStoped("Not have next block.");
		return;
	}

	m_next_block->setProperty(this->m_payload_name, value);

	m_next_block->Start();
}
