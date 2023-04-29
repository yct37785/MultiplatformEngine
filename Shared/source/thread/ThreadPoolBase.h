#pragma once
#include <cstdarg>
#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#ifdef PLATFORM_WINDOWS
#include "../../../Windows/Multiplatform/ctpl_stl.h"
#elif __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/wasm_worker.h>
#include <emscripten/threading.h>
#endif

/**
* Emecripten batch class
*/
//class EmscriptenBatch
//{
//	std::vector<emscripten_wasm_worker_t>;
//};


/******************************************************************************
* Platform unified API for multi-threading
******************************************************************************/
#define p_ThreadPool ThreadPoolBase::getInstance()
class ThreadPoolBase
{
	static std::unique_ptr<ThreadPoolBase> instance;

	// platform specific multi-thread helpers
#ifdef PLATFORM_WINDOWS
	std::unique_ptr<ctpl::thread_pool> p;
	std::unordered_map<int, std::vector<std::future<void>>> batches;
#endif

public:
	static std::unique_ptr<ThreadPoolBase>& getInstance();

	void Init();
	void WaitBatch(int batchId);

	// template functions
	template<typename F, typename... Rest>
	void RunJob(int batchId, F&& f, Rest&&... rest)
	{
#ifdef PLATFORM_WINDOWS
		batches[batchId].push_back(p->push(f, rest...));
#endif
	}
};
