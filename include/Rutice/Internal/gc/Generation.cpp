// Generation.cpp: implementation of the Generation class.
//
//////////////////////////////////////////////////////////////////////


#include "Generation.hpp"
#include "gc.hpp"
#include "config.hpp"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace GC
{

Generation::Generation( int GenNumber ) : _GenerationNumber( GenNumber ), _pTopOfMemory(NULL),
																		  _pNextObjPtr(NULL)
{
	_pTopOfMemory = MemoryTable;
	_pNextObjPtr = _pTopOfMemory;
}

Generation::~Generation()
{
	_pTopOfMemory = NULL;
	_pNextObjPtr = NULL;
}

void* Generation::Allocate( size_t Size )
{
	if( GetRemainingMemory() < Size  )
		throw( "Heap Full" );

	BYTE* BytePtr = ( BYTE* ) _pNextObjPtr;
	void* pTemp = BytePtr;
	
	// Since the size is given in bytes
	BytePtr += Size;	
	
	_pNextObjPtr = BytePtr;

	_Pointers.push_back( pTemp );

	return pTemp;	
}

void* Generation::CopyBitByBit( const void* pSourceLocation, size_t Size, Generation* pTargetGeneration )
{
	if( pTargetGeneration->GetRemainingMemory() < Size )
		throw( "Heap Full" );

	void* pTargetHeapLocation = pTargetGeneration->Allocate( Size );
	
	pTargetHeapLocation = memcpy( pTargetHeapLocation, pSourceLocation, /* pTargetGeneration->_Pointers.size()*/ Size );

	return pTargetHeapLocation;		
}

int Generation::GetRemainingMemory() const
{
	int nUsedMemory = ( BYTE* ) _pNextObjPtr - ( BYTE* ) _pTopOfMemory;
	
	return ( MaxSize - nUsedMemory );
}
}