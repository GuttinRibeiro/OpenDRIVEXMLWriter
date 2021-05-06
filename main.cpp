#include <iostream>
#include <QString>
#include <QFile>
#include <QXmlStreamWriter>
#include "opendrivexmlwriter.hpp"
#include "t_opendrive.hpp"

int main() {
    OpenDRIVE::t_OpenDRIVE map("timoteo", "0.0.1", "aribeiro");
    Road::t_road road;
//    road.planView.
    Lane::t_road_lanes lane;
    OpenDRIVEXMLWriter writer("out.xml");
    writer.writeData(map);
    std::cout << "Done\n";
    return 0;
}
