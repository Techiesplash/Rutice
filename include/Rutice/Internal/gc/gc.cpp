// GC.cpp: implementation of the GC class.
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include "config.hpp"
#include "gc.hpp"

using namespace std;

namespace GC{
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


std::vector<void **> GarbageCollector::_PointersOnStack;
std::vector<unsigned int> GarbageCollector::_SizeOfObjects;
std::vector<Generation *> GarbageCollector::_Generations;
int GarbageCollector::BytesAllocated = 0;
int GarbageCollector::ptrCount = 0;
extern int g_Count;

void *GarbageCollector::operator new(size_t Size, void **pStackPtr)
{
    GarbageCollector::BytesAllocated += Size;

    Generation *pGeneration = NULL;

    // If the vector is Empty
    if (_Generations.begin() == _Generations.end())
    {
        pGeneration = new Generation(0);
        _Generations.push_back(pGeneration);
    }
    else
    {
        pGeneration = _Generations[_Generations.size() - 1];

        // If the existing generation is full
        if (pGeneration->GetRemainingMemory() < Size)
        {
            pGeneration = new Generation(GetMaxGeneration() + 1);
            _Generations.push_back(pGeneration);
        }
    }

    _PointersOnStack.push_back(pStackPtr);
    _SizeOfObjects.push_back(Size);

    return pGeneration->Allocate(Size);
}

void GarbageCollector::Collect()
{
    Generation *pGeneration = NULL;

    int Size = _Generations.size();

    if (GC::verboseLogging)
    {
        std::cout << std::endl
                  << "-----------Initiating garbage collection-----------" << std::endl;
    }

    for (int i = 0; i < _Generations.size(); i++)
    {
        if (GC::verboseLogging)
        {

            cout << "Generation: " << _Generations[i]->GetGenerationNumber();
        }

        Collect(_Generations[i]);
        pGeneration = _Generations[i];

        // The  generation needs to be deleted from the vector
        if (pGeneration == NULL)
        {
            for (int j = i; j < _Generations.size() - 1; j++)
            {
                _Generations[j] = _Generations[j + 1];
            }

            _Generations.pop_back();
        }

        std::cout << std::endl;
    }

    if (Size == 0)
    {
        if (GC::verboseLogging)
        {

            std::cout << "No garbage left to collect";

            std::cout << std::endl;
        }
    }
}

int GarbageCollector::GetMaxGeneration()
{
    if (_Generations.begin() == _Generations.end())
    {
        return 1;
    }
    else
    {
        Generation *pGeneration = _Generations[_Generations.size() - 1];
        return pGeneration->GetGenerationNumber();
    }
}

void GarbageCollector::Collect(Generation *&pGeneration)
{
    Generation *pNewMemory = new Generation(pGeneration->GetGenerationNumber());

    int LocationsMoved = 0;
    int nLocations = pGeneration->_Pointers.size();

    for (int i = 0; i < pGeneration->_Pointers.size(); i++)
    {
        for (int j = 0; j < _PointersOnStack.size(); j++)
        {
            BYTE **pA = (BYTE **)_PointersOnStack[j];
            BYTE *Data = (*pA);

            if (pGeneration->_Pointers[i] == Data)
            {
                LocationsMoved++;
                void *pNewLocation = pGeneration->CopyBitByBit(Data,
                                                               _SizeOfObjects[i],
                                                               pNewMemory);
                *_PointersOnStack[i] = pNewLocation;
            }
        }
    }

    delete pGeneration;
    pGeneration = NULL;

    // If no location is moved, then we can delete the region
    if (LocationsMoved == 0)
    {
        delete pNewMemory;
        pNewMemory = NULL;
        if (GC::verboseLogging)
        {
            std::cout << "\tAll data from the generation has been collected";
        }
    }
    else
    {
        if (GC::verboseLogging)
        {
            std::cout << "\tReachable data : " << LocationsMoved << "\tCollected data: " << nLocations - LocationsMoved;
        }
        pGeneration = pNewMemory;
    }
}

}