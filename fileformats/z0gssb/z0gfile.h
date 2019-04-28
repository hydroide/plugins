#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef Z0GFILE_H
#define Z0GFILE_H

#include <QtCore>

#include "zbfile.h"

class Z0gFile : public ZBFile
{
public:
    Z0gFile(const QString&);

    void readFile();
    void writeFile();

};

#endif // Z0GFILE_H
