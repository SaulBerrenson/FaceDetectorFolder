#pragma once
#include "IPipelineBlock.h"


class FileSourceBlock final : public IPipelineBlock 
{
public:
	explicit FileSourceBlock(const String& m_dir)
		: m_dir(m_dir)
	{

	}

	~FileSourceBlock() override;

	static SharedPtr<FileSourceBlock> create_block(const String& dir);
	
public slots:
	bool Start() override;
	bool Stop() override;

private:
	String m_dir;
	List<String> m_files;
	const String m_extantion = "*.jpg";


};
