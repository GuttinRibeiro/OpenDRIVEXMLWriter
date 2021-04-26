#include <iostream>
#include <QString>
#include <QFile>
#include <QXmlStreamWriter>

int main() {
    QString filename("C:\\Users\\augus\\Documents\\codes\\OpenDRIVE\\XMLWriter\\out.xml");
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeEndDocument();
    writer.writeStartElement("OpenDRIVE");
    writer.writeEndElement(); //OpenDRIVE
    writer.writeStartElement("header");
    writer.writeAttribute("revmajor", "1");
    writer.writeAttribute("revminor", "6");
    writer.writeAttribute("name", "testMap");
    writer.writeAttribute("revminor", "6");
    writer.writeAttribute("version", "1");
    writer.writeAttribute("date", "2021-04-26");
    writer.writeEndElement(); //header

//    writer.writeEndDocument();
    std::cout << "Done\n";
    return 0;
}
