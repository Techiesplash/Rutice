// GC.h: interface for the CGarbageCollector class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Generation.hpp"

namespace GC
{

	class GarbageCollector
	{

	private:
		// Holds the size of objects that are made on the stack
		static std::vector<unsigned int> _SizeOfObjects;
		// Array of pointers to pointers that are made on the stack
		static std::vector<void **> _PointersOnStack;

		// Holds all the generations
		static std::vector<Generation *> _Generations;
		// Holds total bytes allocated on the heap
		static int BytesAllocated;
		// Class would not have any instance.
		GarbageCollector() {}

		virtual ~GarbageCollector() {}

	public:
		// -Holds the number of pointers registered
		static int ptrCount;
		// Invokes the GC for all generations
		static void Collect();
		// Invokes the GC only upto and including the generation specified
		static void Collect(Generation *&pGeneration);
		// Call this to allocate memory from the garbage collector
		static void *operator new(size_t, void **pStackPtr);
		// Gets maximum number of generations that have been made
		static int GetMaxGeneration();
		// Gets the total memory (bytes) that has been allocated on the heap
		static int GetTotalBytesAllocated() { return BytesAllocated; }
		// Returns the total number of generations in the GC
		static int GetGenerationCount() { return _Generations.size(); }
		// Sets the total bytes that have been allocated by the garbage collector
		static void SetTotalBytesAllocated(int Value) { BytesAllocated = Value; }
	};

	template <class T>
	class Pointer
	{
	private:
		void Destroy()
		{

			p = NULL;
			GarbageCollector::Collect();
		}

	public:
		T *p;

		Pointer(T *p_ = NULL) : p(p_)
		{
			GarbageCollector::ptrCount++;
			p = GarbageCollector::operator new(sizeof(p_), (void **)this);
		}

		~Pointer()
		{
			GarbageCollector::ptrCount--;
			GarbageCollector::SetTotalBytesAllocated(GarbageCollector::GetTotalBytesAllocated() - sizeof(*p));

			p->~T(); // Explicitely call the destructor

			Destroy();
		}

		Pointer &operator=(Pointer<T> &p_)
		{
			return operator=((T *)p_);
		}

		Pointer &operator=(T *p_)
		{
			Destroy();
			p = p_;
			return *this;
		}

		operator T *()
		{
			return p;
		}

		T &operator*()
		{
			return *p;
		}

		T *operator->()
		{
			return p;
		}

		// For automatic type conversion during new call
		operator void **()
		{
			return (void **)&p;
		}
		operator void *()
		{
			return (void *)&p;
		}

		T &operator[](int i)
		{
			return p[i];
		}
	};

}

#include "gc.cpp"
