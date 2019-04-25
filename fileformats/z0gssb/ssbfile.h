#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef SSBFILE_H
#define SSBFILE_H

#include <QtCore>

class SsbFile
{
public:
    SsbFile(const QString&);

    void readFile();
    void writeFile();

protected:
    QString _filename;
    QFile *_pfile{nullptr};
};

#endif // SSBFILE_H
