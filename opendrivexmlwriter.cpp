#include "opendrivexmlwriter.hpp"
#include <QtGlobal>
#include <QDir>

OpenDRIVEXMLWriter::OpenDRIVEXMLWriter(const QString &filename) {
    Q_ASSERT(filename.size() > 0);
    _filename = QDir::currentPath()+filename;
    _file = new QFile(_filename);
    Q_CHECK_PTR(_file);
    if(!_file->open(QIODevice::ReadOnly)) {
        qCritical("Could not open file %s", qUtf8Printable(_filename));
    }
    _stream = new QXmlStreamWriter(_file);
    Q_CHECK_PTR(_stream);
    _stream->setAutoFormatting(true);
}

OpenDRIVEXMLWriter::~OpenDRIVEXMLWriter() {
    _file->close();
    delete _file;
    delete _stream;
}
