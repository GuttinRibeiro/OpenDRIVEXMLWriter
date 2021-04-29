#ifndef OPENDRIVEXMLWRITER_HPP
#define OPENDRIVEXMLWRITER_HPP

#include <QXmlStreamWriter>
#include <QFile>
#include <QString>
#include <QList>
#include "t_opendrive.hpp"

class OpenDRIVEXMLWriter {
private:
    QString _filename;
    QFile *_file;
    QXmlStreamWriter *_stream;
    void writeHeader(t_header header) const;
    void writeRoads(QList<Road::t_road> roads) const;
    void writeObjectComponents(Object::t_road_objects objects) const;
    void writeLaneComponents(Lane::t_road_lanes lane) const;
    void writeLaneSectionLaneAttributes(Lane::t_road_lanes_laneSection_lr_lane elem) const;
    void writeGeometryComponent(Road::t_road_planView_geometry geometry) const;
    void writePredecessorSucessor(Road::t_road_link_predecessorSucessor elem) const;
    void writeJunctions(QList<Junction::t_junction> junctions) const;
    void writeConnections(QList<Junction::t_junction_connection> connections) const;
    void writeControllers(QList<Signal::t_controller> controllers) const;
public:
    OpenDRIVEXMLWriter(const QString &filename);
    ~OpenDRIVEXMLWriter();
    void writeData(OpenDRIVE::t_OpenDRIVE data) const;
};

#endif // OPENDRIVEXMLWRITER_HPP
