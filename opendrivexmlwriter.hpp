#ifndef OPENDRIVEXMLWRITER_HPP
#define OPENDRIVEXMLWRITER_HPP

#include <QXmlStreamWriter>
#include <QFile>
#include <QString>

class OpenDRIVEXMLWriter {
private:
    QString _filename;
    QFile *_file;
    QXmlStreamWriter *_stream;
public:
    OpenDRIVEXMLWriter(const QString &filename);
    ~OpenDRIVEXMLWriter();
};

#endif // OPENDRIVEXMLWRITER_HPP
