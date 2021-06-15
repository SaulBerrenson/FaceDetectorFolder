#include "FileSourceBlock.h"
#include <QDirIterator>
#include <qvariant.h>

FileSourceBlock::~FileSourceBlock()
{
	m_dir = nullptr;
	m_files.swap(std::move(List<String>{}));
}

SharedPtr<FileSourceBlock> FileSourceBlock::create_block(const String& dir)
{
	return SharedPtr<FileSourceBlock>(new FileSourceBlock(dir));
}

bool FileSourceBlock::Start()
{

	if (!QDir(m_dir).exists())
	{
		emit OnError(String("Directory %1 is not exist!").arg(m_dir));
		this->NeedStopBlock();
		Stop();
			return false;
	}

	emit OnStarted("File Source is Started");

	
	QDirIterator it(this->m_dir, QStringList() << this->m_extantion, QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext() && !this->m_need_stop)
		this->m_files.push_back(std::move(it.next()));

	
	
	call_next_block(QVariant::fromValue(m_files));
	return true;
}

bool FileSourceBlock::Stop()
{	
	emit OnStoped("File Source is stopped!");
	return true;
}

