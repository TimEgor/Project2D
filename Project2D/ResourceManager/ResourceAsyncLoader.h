#pragma once

#include <Multithreading/ThreadItemQueue.h>
#include <Multithreading/Thread.h>
#include <ResourceManager/ResourceName.h>

class ResourceHandler;
class FileSystem;

class ResourceAsyncLoader final {
	struct ResourceLoadingInformation final {
		ResourceHandler* resourceHandler;
		ResourceID resourceID;
	};

private:
	ThreadItemQueue<ResourceLoadingInformation> loadingQueue;
	Thread loadingThread;
	bool isRunning;

	static void loadingFunction(void* args);

public:
	ResourceAsyncLoader() : isRunning(false) {}
	~ResourceAsyncLoader() { release(); }

	bool init();
	void release();

	void loadResource(ResourceID resourceID, ResourceHandler& handler);
};