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

void OpenDRIVEXMLWriter::writeGeometryComponent(Road::t_road_planView_geometry geometry) const {
    // Each geometry can have only one valid geometry field
    if(geometry.line != nullptr) {
        _stream->writeStartElement(geometry.line->delimiter);
        _stream->writeEndElement();
    }
    else if (geometry.arc != nullptr) {
        _stream->writeStartElement(geometry.arc->delimiter);
        _stream->writeAttribute("curvature", QString::number(geometry.arc->curvature, 'g', 17));
        _stream->writeEndElement();
    }
    else if (geometry.spiral != nullptr) {
        _stream->writeStartElement(geometry.spiral->delimiter);
        _stream->writeAttribute("curvStart", QString::number(geometry.spiral->curvStart, 'g', 17));
        _stream->writeAttribute("curvEnd", QString::number(geometry.spiral->curvEnd, 'g', 17));
        _stream->writeEndElement();
    }
    else if (geometry.poly3 != nullptr) {
        qWarning("Poly3 is deprecated. Please, use paramPoly3 instead.");
        _stream->writeStartElement(geometry.poly3->delimiter);
        _stream->writeAttribute("a", QString::number(geometry.poly3->a, 'g', 17));
        _stream->writeAttribute("b", QString::number(geometry.poly3->b, 'g', 17));
        _stream->writeAttribute("c", QString::number(geometry.poly3->c, 'g', 17));
        _stream->writeAttribute("d", QString::number(geometry.poly3->d, 'g', 17));
        _stream->writeEndElement();
    }
    else if (geometry.paramPoly3 != nullptr) {
        _stream->writeStartElement(geometry.paramPoly3->delimiter);
        _stream->writeAttribute("aU", QString::number(geometry.paramPoly3->aU, 'g', 17));
        _stream->writeAttribute("bU", QString::number(geometry.paramPoly3->bU, 'g', 17));
        _stream->writeAttribute("cU", QString::number(geometry.paramPoly3->cU, 'g', 17));
        _stream->writeAttribute("dU", QString::number(geometry.paramPoly3->dU, 'g', 17));
        _stream->writeAttribute("aV", QString::number(geometry.paramPoly3->aV, 'g', 17));
        _stream->writeAttribute("bV", QString::number(geometry.paramPoly3->bV, 'g', 17));
        _stream->writeAttribute("cV", QString::number(geometry.paramPoly3->cV, 'g', 17));
        _stream->writeAttribute("dV", QString::number(geometry.paramPoly3->dV, 'g', 17));
        _stream->writeEndElement();
    }
    else {
        qFatal("A road geometry element must be associated with at least one geometry type.");
    }
}

void OpenDRIVEXMLWriter::writePredecessorSucessor(Road::t_road_link_predecessorSucessor elem) const {
    _stream->writeStartElement(elem.delimiter);
    _stream->writeAttribute("elemetType",elem.elementType);
    _stream->writeAttribute("elemetId", elem.elementId);
    if(elem.contactPoint.isEmpty() == false) {
        _stream->writeAttribute("contactPoint",elem.contactPoint);
    }
    if(elem.elementS >= 0) {
        _stream->writeAttribute("elementS", QString::number(elem.elementS, 'g', 17));
    }
    if(elem.elementDir.isEmpty() == false) {
        _stream->writeAttribute("elementDir",elem.elementDir);
    }
    _stream->writeEndElement();
}

void OpenDRIVEXMLWriter::writeLaneSectionLaneAttributes(Lane::t_road_lanes_laneSection_lr_lane elem) const {
    _stream->writeAttribute("type", elem.type);
    QString level = elem.level ? "true" : "false";
    _stream->writeAttribute("level", level);

    // Lane link
    if(elem.link != nullptr) {
        _stream->writeStartElement(elem.delimiter); // link

        // Predecessors
        for(auto &predecessor : elem.link->predecessor) {
            _stream->writeStartElement(predecessor.delimiter);
            _stream->writeAttribute("id", QString::number(predecessor.id));
            _stream->writeEndElement();
        }

        // Sucessors
        for(auto &sucessor : elem.link->sucessor) {
            _stream->writeStartElement(sucessor.delimiter);
            _stream->writeAttribute("id", QString::number(sucessor.id));
            _stream->writeEndElement();
        }
        _stream->writeEndElement(); // link
    }

    // Road mark
    if(elem.roadMark.isEmpty() == false) {
        for(auto &mark : elem.roadMark) {
            _stream->writeStartDocument(mark.delimiter); // roadMark
            _stream->writeAttribute("sOffset", QString::number(mark.sOffset, 'g', 17));
            _stream->writeAttribute("type", mark.type);
            _stream->writeAttribute("material", mark.material);
            QList<QString> keys = mark.attributes.keys();
            for(auto &key : keys) {
                _stream->writeAttribute(key, mark.attributes.value(key));
            }
            _stream->writeEndDocument(); // roadMark
        }
    }

    // Lane height
    if(elem.height.isEmpty() == false) {
        for(auto &height : elem.height) {
            _stream->writeStartElement(height.delimiter);
            _stream->writeAttribute("sOffset", QString::number(height.sOffset, 'g', 17));
            _stream->writeAttribute("inner", QString::number(height.inner, 'g', 17));
            _stream->writeAttribute("outer", QString::number(height.outer, 'g', 17));
            _stream->writeEndElement();
        }
    }

    // Mandadory field: lane geometries
    Q_ASSERT(elem.laneGeometry.isEmpty() == false);
    for(auto &geometry : elem.laneGeometry) {
        // borders
        for(auto &border : geometry.border) {
            _stream->writeStartElement(border.delimiter);
            _stream->writeAttribute("sOffset", QString::number(border.sOffset, 'g', 17));
            _stream->writeAttribute("a", QString::number(border.a, 'g', 17));
            _stream->writeAttribute("b", QString::number(border.b, 'g', 17));
            _stream->writeAttribute("c", QString::number(border.c, 'g', 17));
            _stream->writeAttribute("d", QString::number(border.d, 'g', 17));
            _stream->writeEndElement();
        }

        // width
        for(auto &width : geometry.width) {
            _stream->writeStartElement(width.delimiter);
            _stream->writeAttribute("sOffset", QString::number(width.sOffset, 'g', 17));
            _stream->writeAttribute("a", QString::number(width.a, 'g', 17));
            _stream->writeAttribute("b", QString::number(width.b, 'g', 17));
            _stream->writeAttribute("c", QString::number(width.c, 'g', 17));
            _stream->writeAttribute("d", QString::number(width.d, 'g', 17));
            _stream->writeEndElement();
        }
    }

    // TODO: lane access
    // TODO: lane material
    // TODO: lane speed
    // TODO: lane rule
}

void OpenDRIVEXMLWriter::writeLaneComponents(Lane::t_road_lanes lane) const {
    // laneOffset
    for(auto &offset : lane.laneOffset) {
        _stream->writeStartElement(offset.delimiter); // laneOffset
        _stream->writeAttribute("s", QString::number(offset.s, 'g', 17));
        _stream->writeAttribute("a", QString::number(offset.a, 'g', 17));
        _stream->writeAttribute("b", QString::number(offset.b, 'g', 17));
        _stream->writeAttribute("c", QString::number(offset.c, 'g', 17));
        _stream->writeAttribute("d", QString::number(offset.d, 'g', 17));
        _stream->writeEndElement(); // laneOffset
    }

    // laneSection
    for(auto &section : lane.laneSection) {
        _stream->writeStartElement(section.delimiter); // laneSection
        _stream->writeAttribute("s", QString::number(section.s, 'g', 17));
        QString singleSide = section.singleSide ? "true" : "false";
        _stream->writeAttribute("singleSide", singleSide);

        // Left lane:
        if(section.left != nullptr) {
            Q_ASSERT(section.left->left.isEmpty() == false);
            _stream->writeStartElement(section.left->delimiter); // left
            for(auto &elem : section.left->left) {
                _stream->writeStartElement(elem.delimiter); // lane
                _stream->writeAttribute("id", QString::number(elem.id));
                writeLaneSectionLaneAttributes(elem);
                _stream->writeEndElement(); // lane
            }
            _stream->writeEndElement(); // left
        }

        // Mandatory field: center lane
        Q_ASSERT(section.center.center.isEmpty() == false);
        _stream->writeStartElement(section.center.delimiter); // center
        for(auto &elem : section.center.center) {
            _stream->writeStartElement(elem.delimiter); // lane
            _stream->writeAttribute("id", QString::number(elem.id));
            writeLaneSectionLaneAttributes(elem);
            _stream->writeEndElement(); // lane
        }
        _stream->writeEndElement(); // center


        // Right lane:
        if(section.right != nullptr) {
            Q_ASSERT(section.right->right.isEmpty() == false);
            _stream->writeStartElement(section.right->delimiter); // right
            for(auto &elem : section.right->right) {
                _stream->writeStartElement(elem.delimiter); // lane
                _stream->writeAttribute("id", QString::number(elem.id));
                writeLaneSectionLaneAttributes(elem);
                _stream->writeEndElement(); // lane
            }
            _stream->writeEndElement(); // right
        }

        _stream->writeEndElement(); // laneSection
    }
}

void OpenDRIVEXMLWriter::writeObjectComponents(Object::t_road_objects objects) const {
    // Optional field: objects
    for(auto &object : objects.object) {
        _stream->writeStartElement(object.delimiter); // object
        _stream->writeAttribute("id", object.id);
        if(object.name.isEmpty() == false) {
            _stream->writeAttribute("name", object.name);
        }
        _stream->writeAttribute("s", QString::number(object.s, 'g', 17));
        _stream->writeAttribute("t", QString::number(object.t, 'g', 17));
        _stream->writeAttribute("zOffset", QString::number(object.zOffset, 'g', 17));
        _stream->writeAttribute("hdg", QString::number(object.hdg, 'g', 17));
        _stream->writeAttribute("roll", QString::number(object.roll, 'g', 17));
        _stream->writeAttribute("pitch", QString::number(object.pitch, 'g', 17));
        _stream->writeAttribute("orientation", object.orientation);
        _stream->writeAttribute("type", object.type);
        // Angular object check
        if(object.radius > 0) {
            _stream->writeAttribute("radius", QString::number(object.radius, 'g', 17));
        }
        else {
            _stream->writeAttribute("length", QString::number(object.length, 'g', 17));
            _stream->writeAttribute("width", QString::number(object.width, 'g', 17));
        }
        _stream->writeAttribute("height", QString::number(object.height, 'g', 17));
        // TODO: more attributes

        // Repeat
        if(object.repeat.isEmpty() == false) {
            for(auto &repeat : object.repeat) {
                _stream->writeStartElement(repeat.delimiter);
                _stream->writeAttribute("s", QString::number(repeat.s, 'g', 17));
                _stream->writeAttribute("length", QString::number(repeat.length, 'g', 17));
                _stream->writeAttribute("distance", QString::number(repeat.distance, 'g', 17));
                _stream->writeAttribute("tStart", QString::number(repeat.tStart, 'g', 17));
                _stream->writeAttribute("tEnd", QString::number(repeat.tEnd, 'g', 17));
                _stream->writeAttribute("heightStart", QString::number(repeat.heightStart, 'g', 17));
                _stream->writeAttribute("heightEnd", QString::number(repeat.heightEnd, 'g', 17));
                _stream->writeAttribute("zOffsetStart", QString::number(repeat.zOffsetStart, 'g', 17));
                _stream->writeAttribute("zOffsetEnd", QString::number(repeat.zOffsetEnd, 'g', 17));
                if(repeat.radiusStart > 0) {
                    _stream->writeAttribute("radiusStart", QString::number(repeat.radiusStart, 'g', 17));
                    _stream->writeAttribute("radiusEnd", QString::number(repeat.radiusEnd, 'g', 17));
                }
                else {
                    _stream->writeAttribute("widthStart", QString::number(repeat.widthStart, 'g', 17));
                    _stream->writeAttribute("widthEnd", QString::number(repeat.widthEnd, 'g', 17));
                    _stream->writeAttribute("lengthStart", QString::number(repeat.lengthStart, 'g', 17));
                    _stream->writeAttribute("lengthEnd", QString::number(repeat.lengthEnd, 'g', 17));
                }
                _stream->writeEndElement();
            }
        }

        // TODO: parking space

        // Outlines
        if(object.outlines != nullptr) {
            _stream->writeStartElement(object.outlines->delimiter); // outlines
            for(auto &outline : object.outlines->outline) {
                // cornerLocal
                if(outline.cornerLocal.isEmpty() == false) {
                    for(auto &corner : outline.cornerLocal) {
                        _stream->writeStartElement(corner.delimiter); // cornerLocal
                        _stream->writeAttribute("id", QString::number(corner.id, 'g', 17));
                        _stream->writeAttribute("u", QString::number(corner.u, 'g', 17));
                        _stream->writeAttribute("v", QString::number(corner.v, 'g', 17));
                        _stream->writeAttribute("z", QString::number(corner.z, 'g', 17));
                        _stream->writeAttribute("height", QString::number(corner.height, 'g', 17));
                        _stream->writeEndElement(); // cornerLocal
                    }
                }

                // cornerRoad
                if(outline.cornerRoad.isEmpty() == false) {
                    for(auto &corner : outline.cornerRoad) {
                        _stream->writeStartElement(corner.delimiter); // cornerRoad
                        _stream->writeAttribute("id", QString::number(corner.id, 'g', 17));
                        _stream->writeAttribute("u", QString::number(corner.s, 'g', 17));
                        _stream->writeAttribute("v", QString::number(corner.t, 'g', 17));
                        _stream->writeAttribute("z", QString::number(corner.dz, 'g', 17));
                        _stream->writeAttribute("height", QString::number(corner.height, 'g', 17));
                        _stream->writeEndElement(); // cornerRoad
                    }
                }
            }
            _stream->writeEndElement(); // outlines
        }

        // TODO: borders
        // TODO: validity
        // TODO: markings
        _stream->writeEndElement(); // object
    }

    // TODO: tunnel
    // TODO: bridge
    // TODO: objectReference
}

void OpenDRIVEXMLWriter::writeRoads(QList<Road::t_road> roads) const {
    Q_ASSERT(roads.isEmpty() == false);
    for(auto &road : roads) {
        _stream->writeStartElement(road.delimiter); // roads
        if(road.name.isEmpty() == false) {
            _stream->writeAttribute("name", road.name);
        }
        _stream->writeAttribute("length", QString::number(road.length, 'g', 17));
        _stream->writeAttribute("id", road.id);
        _stream->writeAttribute("junction", road.junction);
        if(road.rule.isEmpty() == false) {
            _stream->writeAttribute("rule", road.rule);
        }

        // Optional fied: link
        if(road.link != nullptr) {
            _stream->writeStartElement(road.link->delimiter); // link

            // predecessor:
            if(road.link->predecessor != nullptr) {
                writePredecessorSucessor(*road.link->predecessor);
            }

            // sucessor:
            if(road.link->sucessor != nullptr) {
                writePredecessorSucessor(*road.link->sucessor);
            }

            _stream->writeEndElement(); // link
        }

        // Optional field: type
        if(road.type != nullptr) {
            _stream->writeStartElement(road.type->delimiter); // type
            _stream->writeAttribute("s", QString::number(road.type->s, 'g', 17));
            _stream->writeAttribute("type", road.type->type);
            if(road.type->country.isEmpty() == false) {
                _stream->writeAttribute("country", road.type->country);
            }
            _stream->writeEndElement(); // type
        }

        // Mandatory field: planView
        _stream->writeStartElement(road.planView.delimiter); // planView

        // Mandatory field of planView: geometry
        Q_ASSERT(road.planView.geometry.isEmpty() == false);
        for(auto &geometry : road.planView.geometry) {
            _stream->writeStartElement(geometry.delimiter); // geometry
            _stream->writeAttribute("s", QString::number(geometry.s, 'g', 17));
            _stream->writeAttribute("x", QString::number(geometry.x, 'g', 17));
            _stream->writeAttribute("y", QString::number(geometry.y, 'g', 17));
            _stream->writeAttribute("hdg", QString::number(geometry.hdg, 'g', 17));
            _stream->writeAttribute("length", QString::number(geometry.length, 'g', 17));
            // Geometry component
            writeGeometryComponent(geometry);
            _stream->writeEndElement(); // geometry
        }

        _stream->writeEndElement(); // planView

        // Optional field: elevation profile
        if(road.elevationProfile!= nullptr) {
            _stream->writeStartElement(road.elevationProfile->delimiter); // elevationProfile

            // Write elevations:
            for(auto &elevation : road.elevationProfile->elevation) {
                _stream->writeStartElement(elevation.delimiter); // elevation
                _stream->writeAttribute("s", QString::number(elevation.s, 'g', 17));
                _stream->writeAttribute("a", QString::number(elevation.a, 'g', 17));
                _stream->writeAttribute("b", QString::number(elevation.b, 'g', 17));
                _stream->writeAttribute("c", QString::number(elevation.c, 'g', 17));
                _stream->writeAttribute("d", QString::number(elevation.d, 'g', 17));
                _stream->writeEndElement(); // elevation
            }
            _stream->writeAttribute("s", QString::number(road.type->s, 'g', 17));
            _stream->writeAttribute("type", road.type->type);
            if(road.type->country.isEmpty() == false) {
                _stream->writeAttribute("country", road.type->country);
            }
            _stream->writeEndElement(); // elevationProfile
        }

        // Optional filed: lateral profile
        if(road.lateralProfile != nullptr) {
            _stream->writeStartElement(road.lateralProfile->delimiter); // lateralProfile

            // Superelevation
            for(auto &superelevation : road.lateralProfile->superelevation) {
                _stream->writeStartElement(superelevation.delimiter); // superelevation
                _stream->writeAttribute("s", QString::number(superelevation.s, 'g', 17));
                _stream->writeAttribute("a", QString::number(superelevation.a, 'g', 17));
                _stream->writeAttribute("b", QString::number(superelevation.b, 'g', 17));
                _stream->writeAttribute("c", QString::number(superelevation.c, 'g', 17));
                _stream->writeAttribute("d", QString::number(superelevation.d, 'g', 17));
                _stream->writeEndElement(); //superelevation
            }

            // Shape
            for(auto &shape: road.lateralProfile->shape) {
                _stream->writeStartElement(shape.delimiter); // shape
                _stream->writeAttribute("s", QString::number(shape.s, 'g', 17));
                _stream->writeAttribute("t", QString::number(shape.t, 'g', 17));
                _stream->writeAttribute("a", QString::number(shape.a, 'g', 17));
                _stream->writeAttribute("b", QString::number(shape.b, 'g', 17));
                _stream->writeAttribute("c", QString::number(shape.c, 'g', 17));
                _stream->writeAttribute("d", QString::number(shape.d, 'g', 17));
                _stream->writeEndElement(); // shape
            }
            _stream->writeEndDocument(); // lateralProfile
        }

        // TODO: road surface

        // Mandatory field: lane
        _stream->writeStartElement(road.lanes.delimiter); // lanes
        writeLaneComponents(road.lanes);
        _stream->writeEndElement(); // lanes

        // Optional field: objects
        if(road.objects != nullptr) {
            _stream->writeStartElement(road.objects->delimiter); // objects
            writeObjectComponents(*road.objects);
            _stream->writeEndElement(); // objects
        }

        // Optional field: signals
    }
    _stream->writeEndElement(); // roads
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
