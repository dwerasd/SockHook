#pragma once

#ifndef DARK_TOOLS_CRITICAL_SECTION_INCLUDED__
#define DARK_TOOLS_CRITICAL_SECTION_INCLUDED__


#include <windows.h>


#if (WINVER < 0x603)
class C_CRITICAL_SECTION
{
private:
	CRITICAL_SECTION cs;

public:
	C_CRITICAL_SECTION() { InitializeCriticalSection(&cs); }
	//C_CRITICAL_SECTION() { InitializeCriticalSectionAndSpinCount(&cs, 1000); }
	~C_CRITICAL_SECTION() { DeleteCriticalSection(&cs); }

	void Enter() { EnterCriticalSection(&cs); }
	void Leave() { LeaveCriticalSection(&cs); }
};

class C_AUTO_RELEASE_LOCK : public C_CRITICAL_SECTION
{
public:
	C_AUTO_RELEASE_LOCK() { Enter(); }
	~C_AUTO_RELEASE_LOCK() { Leave(); }
};

class C_REMOTE_RELEASE_LOCK
{
private:
	C_CRITICAL_SECTION &cs;

public:
	C_REMOTE_RELEASE_LOCK(C_CRITICAL_SECTION &pCs) : cs(pCs)
	{
		cs.Enter();
	}
	~C_REMOTE_RELEASE_LOCK()
	{
		cs.Leave();
	}
};


#else

#include <timeapi.h>
#include <mutex>
class C_CRITICAL_SECTION
{
private:
	std::mutex cs;

public:
	C_CRITICAL_SECTION() { }
	~C_CRITICAL_SECTION() { }

	void Enter() { cs.lock(); }
	void Leave() { cs.unlock(); }
};

class C_AUTO_RELEASE_LOCK : public C_CRITICAL_SECTION
{
public:
	C_AUTO_RELEASE_LOCK() { Enter(); }
	~C_AUTO_RELEASE_LOCK() { Leave(); }
};

class C_REMOTE_RELEASE_LOCK
{
private:
	C_CRITICAL_SECTION &cs;

public:
	C_REMOTE_RELEASE_LOCK(C_CRITICAL_SECTION &pCs) : cs(pCs)
	{
		cs.Enter();
	}
	~C_REMOTE_RELEASE_LOCK()
	{
		cs.Leave();
	}
};

class C_MUTEX_LOCK
{
private:
	std::mutex mtx_lock;
public:
	C_MUTEX_LOCK() { }
	~C_MUTEX_LOCK() { }

	void Enter() { mtx_lock.lock(); }
	void Leave() { mtx_lock.unlock(); }
};

//////////////////////////////////////////////////////////////////////////


class C_FAST_SPIN_LOCK
{
public:
	C_FAST_SPIN_LOCK();
	~C_FAST_SPIN_LOCK();

	void EnterLock()
	{
		for (int nloops = 0;; nloops++)
		{
			if (InterlockedExchange(&mLockFlag, 1) == 0)
				return;

			UINT uTimerRes = 1;
			timeBeginPeriod(uTimerRes);
			Sleep((DWORD)min(10, nloops));
			timeEndPeriod(uTimerRes);
		}
	}
	void LeaveLock()
	{
		InterlockedExchange(&mLockFlag, 0);
	}

private:
	C_FAST_SPIN_LOCK(const C_FAST_SPIN_LOCK& rhs);
	C_FAST_SPIN_LOCK& operator=(const C_FAST_SPIN_LOCK& rhs);

	volatile long mLockFlag;
};

class C_FAST_SPIN_LOCK_GUARD
{
public:
	C_FAST_SPIN_LOCK_GUARD(C_FAST_SPIN_LOCK& lock) : mLock(lock)
	{
		mLock.EnterLock();
	}

	~C_FAST_SPIN_LOCK_GUARD()
	{
		mLock.LeaveLock();
	}

private:
	C_FAST_SPIN_LOCK& mLock;
};

template <class TargetClass>
class ClassTypeLock
{
public:
	struct LockGuard
	{
		LockGuard()
		{
			TargetClass::mLock.EnterLock();
		}

		~LockGuard()
		{
			TargetClass::mLock.LeaveLock();
		}

	};

private:
	static C_FAST_SPIN_LOCK mLock;

};

template <class TargetClass>
C_FAST_SPIN_LOCK ClassTypeLock<TargetClass>::mLock;

#endif


#endif //DARK_TOOLS_CRITICAL_SECTION_INCLUDED__
