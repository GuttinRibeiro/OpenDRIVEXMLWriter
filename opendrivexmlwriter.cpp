#include "opendrivexmlwriter.hpp"
#include <QtGlobal>
#include <QDir>
#include <iostream>

OpenDRIVEXMLWriter::OpenDRIVEXMLWriter(const QString &filename) {
    Q_ASSERT(filename.size() > 0);
    QString folder = QDir::currentPath();
    _filename = folder+"/"+filename;
    std::cout << "Creating output file named " << _filename.toStdString() << "\n";
    _file = new QFile(_filename);
    Q_CHECK_PTR(_file);
    if(!_file->open(QIODevice::WriteOnly)) {
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

void OpenDRIVEXMLWriter::writeHeader(t_header header) const {
    _stream->writeStartElement(header.delimiter);
    _stream->writeAttribute("revMajor", QString::number(header.revMajor));
    _stream->writeAttribute("revMinor", QString::number(header.revMinor));
    _stream->writeAttribute("name", header.name);
    _stream->writeAttribute("version", header.version);
    _stream->writeAttribute("date", header.date);
    _stream->writeAttribute("north", QString::number(header.north, 'g', 17));
    _stream->writeAttribute("south", QString::number(header.south, 'g', 17));
    _stream->writeAttribute("east", QString::number(header.east, 'g', 17));
    _stream->writeAttribute("west", QString::number(header.west, 'g', 17));
    _stream->writeAttribute("vendor", header.vendor);
    _stream->writeEndElement();
}

void OpenDRIVEXMLWriter::writeConnections(QList<Junction::t_junction_connection> connections) const {
    for(auto &connection : connections) {
        _stream->writeStartElement(connection.delimiter);
        // TODO: lane links and predecessor/sucessor
        _stream->writeEndElement();
    }
}

void OpenDRIVEXMLWriter::writeJunctions(QList<Junction::t_junction> junctions) const {
    if(junctions.isEmpty() == true) {
        return;
    }

    for(auto &junction : junctions) {
        _stream->writeAttribute("id", junction.id);
        if(junction.name.isEmpty() == false) {
            _stream->writeAttribute("name", junction.name);
        }
        _stream->writeAttribute("type", junction.type);

        // Mandatory element: connection
        Q_ASSERT(junction.connection.size() > 0);
        writeConnections(junction.connection);

        // TODO: write other possible information
    }
}

void OpenDRIVEXMLWriter::writeRoads(QList<Road::t_road> roads) const {
    Q_ASSERT(roads.isEmpty() == false);
    _stream->writeStartElement(roads[0].delimiter);
    for(auto &road : roads) {
        if(road.name.isEmpty() == false) {
            _stream->writeAttribute("name", road.name);
        }
        _stream->writeAttribute("length", QString::number(road.length, 'g', 17));
        _stream->writeAttribute("id", road.id);
        _stream->writeAttribute("junction", road.junction);
        if(road.rule.isEmpty() == false) {
            _stream->writeAttribute("rule", road.rule);
        }
        // TODO: write road components
    }
    _stream->writeEndElement();
}

void OpenDRIVEXMLWriter::writeControllers(QList<Signal::t_controller> controllers) const {
    if(controllers.isEmpty() == true) {
        return;
    }

    for(auto &controller : controllers) {
        _stream->writeStartElement(controller.delimiter); // controller

        // <controller> attributes
        if(controller.name.isEmpty() == false) {
            _stream->writeAttribute("name", controller.name);
        }
        _stream->writeAttribute("id", controller.id);
        // Failure handling
        if(controller.sequence < 0) {
            _stream->writeAttribute("sequence", QString::number(0));
        }
        else {
            _stream->writeAttribute("sequence", QString::number(controller.sequence));
        }

        // <control> elements
        Q_ASSERT(controller.control.isEmpty() == false);
        for(auto &control : controller.control) {
            _stream->writeStartElement(control.delimiter); //control
            _stream->writeAttribute("signalId", control.signalId);
            _stream->writeAttribute("type", control.type);
            _stream->writeEndElement(); //control
        }

        _stream->writeEndElement(); // controller
    }
}

void OpenDRIVEXMLWriter::writeData(OpenDRIVE::t_OpenDRIVE data) const {
    _stream->writeStartDocument("1.0");
    _stream->writeStartElement("OpenDRIVE");
    writeHeader(data.getHeader());
    writeRoads(data.getRoads());
    writeJunctions(data.getJunctions());
    writeControllers(data.getControllers());
    // TODO: junction groups
    // TODO: railroad
    _stream->writeEndElement();
    _stream->writeEndDocument();
}
