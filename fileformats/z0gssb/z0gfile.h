#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef Z0GFILE_H
#define Z0GFILE_H

#include <QtCore>

class Z0gFile
{
public:
    Z0gFile(const QString&);

    void readFile();
    void writeFile();

protected:
    QString _filename;
    QFile *_pfile{nullptr};
};

#endif // Z0GFILE_H
