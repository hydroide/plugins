#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef Z0GSSBFILEPAIR_H
#define Z0GSSBFILEPAIR_H

#include <memory>
using namespace std;

#include "z0gfile.h"
#include "ssbfile.h"

class Z0gSsbFilePair
{
public:
    SpZ0gFile _spZ0gFile;
    SpSsbFile _spSsbFile;
};

typedef shared_ptr<Z0gSsbFilePair> SpZ0gSsbFilePair;

#endif // Z0GSSBFILEPAIR_H
