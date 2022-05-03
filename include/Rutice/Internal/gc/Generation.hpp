// Generation.h: interface for the Generation class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HEAPMEMORY_H
#define _HEAPMEMORY_H

#include <Rutice/Internal/basics.hpp>
#include "config.hpp"
namespace GC
{
typedef uint8_t BYTE;
class Generation  
{
	friend class GarbageCollector;

private:
// The generation number
	int _GenerationNumber;
// Pointers to the objects in the generation
	std::vector< void* > _Pointers;
// Points to the top of memory available in the generation
	void* _pTopOfMemory;
// The generation allocates memory from this location	
	void* _pNextObjPtr;
// Returns maximum size available for one generation
	enum { MaxSize = GC_MAXSIZE };
// Table of memory inside generation
	int MemoryTable[MaxSize];
	
public:
// Gets the remaining memory of the Generation
	int GetRemainingMemory() const;
// Returns maximum memory that can be allocated for one generation
	int GetTotalMemory() const { return MaxSize; }
// Allocates memory for an object and returns its void*
	void* Allocate( size_t Size );
// Gets the generation number
	int GetGenerationNumber() const { return _GenerationNumber; }
// Performs bitbybit copying 
	static void* CopyBitByBit( const void* pSourceLocation, size_t Size, Generation* pTargetGeneration );
	
	void operator delete( void* v )
	{
		free( v );		
	}
	
public:
	Generation( int GenNumber = 0 );
	virtual ~Generation();

};

}
#include "Generation.cpp"
#endif 
