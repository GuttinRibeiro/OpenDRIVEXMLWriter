#include "t_opendrive.hpp"
#include <QDateTime>

using namespace OpenDRIVE;

t_OpenDRIVE::t_OpenDRIVE(string name, string version, string vendor) {
    _header.date = QDateTime::currentDateTime().toString("yyyy-mm-ddThh:mm:ss");
    _header.revMajor =  1;
    _header.revMinor = 6;
    _header.name = name;
    _header.version = version;
    _header.vendor = vendor;
    _header.north = 0.0000000000e+00;
    _header.south = 0.0000000000e+00;
    _header.east = 0.0000000000e+00;
    _header.west = 0.0000000000e+00;
}

t_header t_OpenDRIVE::getHeader() const {
    return _header;
}

QList<Road::t_road> t_OpenDRIVE::getRoads() const {
    return _road;
}

QList<Junction::t_junction> t_OpenDRIVE::getJunctions() const {
    return _junction;
}

QList<Signal::t_controller> t_OpenDRIVE::getControllers() const {
    return _controller;
}

void t_OpenDRIVE::setMaxInertialX(double value) {
    _header.east = value;
}

void t_OpenDRIVE::setMinInertialX(double value) {
    _header.west = value;
}

void t_OpenDRIVE::setMaxInertialY(double value) {
    _header.north = value;
}

void t_OpenDRIVE::setMinInertialY(double value) {
    _header.south = value;
}
