#include "ResultBlock.h"

#include <qmutex.h>

SharedPtr<ResultBlock> ResultBlock::create_block()
{
	return SharedPtr<ResultBlock>(new ResultBlock);
}

bool ResultBlock::Start()
{
	emit OnStarted("Result Block is started!");
	auto payload = this->property(this->m_payload_name);

	if (!payload.isValid() || payload.isNull()) return false; 

	for (const auto& path : payload.value<List<String>>())
	{
		m_files_with_faces.push_back(std::move(path));
	}
	//replace value for block
	this->setProperty(this->m_payload_name, "");
	
	call_next_block();

	return true;
}

bool ResultBlock::Stop()
{
	emit OnStoped("Result Block was stopped!");
	return true;
}

List<String> ResultBlock::GetResult()
{
	return std::move(this->m_files_with_faces);
}
