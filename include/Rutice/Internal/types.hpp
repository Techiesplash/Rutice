#ifndef _H_TYPES_I_HPP_
#define _H_TYPES_I_HPP_
#include "basics.hpp"
// Spatial types
typedef struct
{
    int x, y;
} ivect2;

typedef struct
{
    float x, y;
} fvect2;

typedef struct
{
    double x, y;
} dvect2;

typedef struct
{
    int x, y, z;
} ivect3;

typedef struct
{
    float x, y, z;
} fvect3;

typedef struct
{
    double x, y, z;
} dvect3;

typedef struct
{
    int x, y, z, w;
} ivect4;

typedef struct
{
    float x, y, z, w;
} fvect4;

typedef struct
{
    double x, y, z, w;
} dvect4;

// ASCII string.
typedef std::string string;

// Unicode 16 string.
typedef std::u16string ustring;

typedef struct
{
    int dest;
    std::vector<int> ids;
    u16 frameCount;
    u16 frameTimer;
    u16 index;
} Anim;

#endif