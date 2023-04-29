#include "ThreadPoolBase.h"
std::unique_ptr<ThreadPoolBase> ThreadPoolBase::instance;

std::unique_ptr<ThreadPoolBase>& ThreadPoolBase::getInstance()
{
	if (instance == nullptr)
	{
		instance = std::make_unique<ThreadPoolBase>();
	}
	return instance;
}

void ThreadPoolBase::Init()
{
#ifdef PLATFORM_WINDOWS
	int win_total_threads = std::thread::hardware_concurrency();
	p = std::make_unique<ctpl::thread_pool>(win_total_threads);
#endif
}

void ThreadPoolBase::WaitBatch(int batchId)
{
#ifdef PLATFORM_WINDOWS
	if (batches.find(batchId) == batches.end())
		return;
	for (int i = 0; i < batches[batchId].size(); ++i) {
		batches[batchId][i].get();
	}
	batches[batchId].clear();
	std::cout << "Batch id " << batchId << " completed--" << std::endl;
#endif
}