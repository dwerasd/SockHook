#pragma once


#include <windows.h>

#if defined (_MSC_VER) && (_MSC_VER >= 1600)
#include <unordered_map>
#else
#include <boost/unordered_map.hpp>
#endif

#include <boost/lockfree/queue.hpp>



template <typename OBJECT_TYPE>
class C_OBJECT_POOL_DLF
{
private:
	typedef boost::lockfree::queue<OBJECT_TYPE *> LockFreeQueue;
	LockFreeQueue blfQueue;
#if defined (_MSC_VER) && (_MSC_VER >= 1600)
	typedef std::unordered_map<ULONG_PTR, ULONG> UnorderedMap;
#else
	typedef boost::unordered_map<ULONG_PTR, ULONG> UnorderedMap;
#endif
	UnorderedMap AddrMap;

	LPVOID pMemBlock;

public:
	C_OBJECT_POOL_DLF(ULONG_PTR nSize)
	{
		ULONG_PTR nAllocationSize = (sizeof(OBJECT_TYPE) * nSize);
		pMemBlock = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nAllocationSize);
		OBJECT_TYPE *pChunk = reinterpret_cast<OBJECT_TYPE *>((reinterpret_cast<ULONG_PTR>(pMemBlock)) + (sizeof(OBJECT_TYPE) * (nSize-1)));
		for(ULONG_PTR i = nSize; i > 0; i--)
		{
			blfQueue.push(pChunk);
			AddrMap.insert(UnorderedMap::value_type((ULONG_PTR)pChunk, 0));	// Key는 주소, Value 는 사용가능 여부이다.
			pChunk--;
		}
	}
	~C_OBJECT_POOL_DLF()
	{
		HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY | HEAP_GENERATE_EXCEPTIONS, pMemBlock);
	}
	OBJECT_TYPE* Allocate()
	{
		OBJECT_TYPE *pObject = NULL;
		blfQueue.pop(pObject);
		if (NULL != pObject)
		{
			UnorderedMap::iterator iter = AddrMap.find((ULONG_PTR)pObject);
			if (iter != AddrMap.end())
			{
				iter->second = 1;	// 사용중으로 표시
			}
		}
		return pObject;
	}
	BOOL Deallocate(OBJECT_TYPE *pObject)
	{
		BOOL bResult = false;
		UnorderedMap::iterator iter = AddrMap.find((ULONG_PTR)pObject);
		if (iter != AddrMap.end())
		{
			iter->second = 0;		// 사용가능으로 표시
			memset(pObject, 0, sizeof(OBJECT_TYPE));
			bResult = blfQueue.push(pObject);
		}
		return(bResult);
	}
};
