#include "OSEngine.h"

//void first(int id, int jobid, const int delayms) {
//	std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
//	std::cout << "Job #" << jobid << " completed" << std::endl;
//}

int main()
{
	//p_ThreadPool->Init();
	//for (int i = 0; i < 8; ++i) {
	//	// add jobs
	//	p_ThreadPool->RunJob(0, first, 1, 1 * 1000);
	//	p_ThreadPool->RunJob(0, first, 2, 2 * 1000);
	//	p_ThreadPool->RunJob(1, first, 3, 3 * 1000);
	//	p_ThreadPool->RunJob(1, first, 4, 4 * 1000);
	//	// wait
	//	p_ThreadPool->WaitBatch(0);
	//	p_ThreadPool->WaitBatch(1);
	//}

	OSEngine::instance()->Init();
	OSEngine::instance()->Run();

	// delete all instances
	delete OSEngine::instance();

	// display memory leak report (on program termination)
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
}