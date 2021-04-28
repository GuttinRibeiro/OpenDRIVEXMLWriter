#ifndef T_OPENDRIVE_HPP
#define T_OPENDRIVE_HPP

#include "OpenDRIVEdefinitions.h"

namespace OpenDRIVE {
    class t_OpenDRIVE {
    private:
        t_header _header;
        QList<Road::t_road> _road;
        QList<Junction::t_junction> _junction;
        QList<Signal::t_controller> _controller;
    public:
        t_OpenDRIVE(string name, string version, string vendor);
        t_header getHeader() const;
        QList<Road::t_road> getRoads() const;
        QList<Junction::t_junction> getJunctions() const;
        QList<Signal::t_controller> getControllers() const;
        void setMaxInertialY(double value);
        void setMaxInertialX(double value);
        void setMinInertialY(double value);
        void setMinInertialX(double value);
    };
}

#endif // T_OPENDRIVE_HPP
