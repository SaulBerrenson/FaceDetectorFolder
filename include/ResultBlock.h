#pragma once
#include "IPipelineBlock.h"

class ResultBlock : public IPipelineBlock
{
public:
	static SharedPtr<ResultBlock> create_block();

public slots:
	bool Start() override;
	bool Stop() override;
public:
	ResultBlock() = default;
	~ResultBlock() override = default;

	List<String> GetResult();

private:
	List<String> m_files_with_faces;
};
