#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef SSBFILE_H
#define SSBFILE_H

#include "zbfile.h"

class SsbFile : public ZBFile
{
public:
    SsbFile(const QString&);

    void readFile();
    void writeFile();

};

#endif // SSBFILE_H
