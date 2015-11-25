#pragma once

#include <mutex>

using namespace std;

class Synced
{
	mutex lock;

protected:
	Synced()
	{
	}

public:
	~Synced()
	{
	}

	void Lock()
	{
		lock.lock();
	}

	void Unlock()
	{
		lock.unlock();
	}
};