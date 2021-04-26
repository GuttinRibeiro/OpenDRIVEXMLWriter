#ifndef OPENDRIVEDEFINITIONS_H
#define OPENDRIVEDEFINITIONS_H
#define RHT 1
#define LHT 0
#include <QString>
#include <QList>

// [Begin] Definitions of variable types
typedef unsigned short int ushort;
typedef double t_grZero;
typedef double t_grEqZero;
typedef QString string;
typedef bool e_trafficRule;
typedef bool e_direction;
typedef ushort e_road_surface_CRG_mode;
typedef ushort e_road_surface_CRG_purpose;
typedef double e_paramPoly3_pRange;
typedef string e_countryCode;
typedef int e_roadType;
typedef float t_maxSpeed;
typedef string e_unitSpeed;
typedef string e_road_link_elementType;
typedef string e_contactPoint;
typedef char e_elementDir;
typedef string e_laneType;
typedef string e_road_lanes_laneSection_lr_lane_access_rule;
typedef string e_accessRestrictionType;
// [End] Definitions of variable types

struct t_header {
    ushort revMajor;
    ushort revMinor;
    string name;
    string version;
    string date;
    double north;
    double south;
    double east;
    double west;
    string vendor;
    string delimiter = "header";
};

// [Begin] Lane components
namespace Lane {
    struct t_road_lanes_laneOffset {
        t_grEqZero s;
        double a;
        double b;
        double c;
        double d;
        const string delimiter = "laneOffset";
    };

    struct t_road_lanes_laneSection_lcr_lane_link_pedecessorSucessor {
        string delimiter;
        int id;
    };

    struct t_road_lanes_laneSection_lcr_lane_link {
        const string delimiter = "link";
        QList<t_road_lanes_laneSection_lcr_lane_link_pedecessorSucessor> predecessor;
        QList<t_road_lanes_laneSection_lcr_lane_link_pedecessorSucessor> sucessor;
    };

    struct t_road_lanes_laneSection_lr_lane_height {
        const string delimiter = "height";
        t_grEqZero sOffset;
        double inner;
        double outer;
    };

    struct t_road_lanes_laneSection_lr_lane_width {
        t_grEqZero sOffset;
        double a;
        double b;
        double c;
        double d;
        const string delimiter = "width";
    };

    struct t_road_lanes_laneSection_lr_lane_border {
        t_grEqZero sOffset;
        double a;
        double b;
        double c;
        double d;
        const string delimiter = "border";
    };

    struct LaneGeometry {
        QList<t_road_lanes_laneSection_lr_lane_border> border;
        QList<t_road_lanes_laneSection_lr_lane_width> width;
    };

    struct t_road_lanes_laneSection_lr_lane_material {
        t_grEqZero sOffset;
        string surface;
        t_grEqZero friction;
        t_grEqZero roughness;
        const string delimiter = "material";
    };

    struct t_road_lanes_laneSection_lr_lane_speed {
        t_grEqZero sOffset;
        t_grEqZero max;
        e_unitSpeed unit;
        const string delimiter = "speed";
    };

    struct t_road_lanes_laneSection_lr_lane_access {
        t_grEqZero sOffset;
        e_road_lanes_laneSection_lr_lane_access_rule rule;
        e_accessRestrictionType restriction;
        const string delimiter = "access";
    };

    // TODO: implementar tipos de roadMark
    struct t_road_lanes_laneSection_lr_lane_roadMark {
        const string delimiter = "roadMark";
    };

    struct t_road_lanes_laneSection_lr_lane_rule {
        const string delimiter = "rule";
        t_grEqZero sOffset;
        string value;
    };

    struct t_road_lanes_laneSection_lr_lane {
        e_laneType lane;
        bool level;
        t_road_lanes_laneSection_lcr_lane_link *link;
        QList<t_road_lanes_laneSection_lr_lane_height> height;
        QList<LaneGeometry> laneGeometry;
        QList<t_road_lanes_laneSection_lr_lane_material> material;
        QList<t_road_lanes_laneSection_lr_lane_speed> speed;
        QList<t_road_lanes_laneSection_lr_lane_access> access;
        QList<t_road_lanes_laneSection_lr_lane_roadMark> roadMark;
        QList<t_road_lanes_laneSection_lr_lane_rule> rule;
    };

    struct t_road_lanes_laneSection_center_lane : t_road_lanes_laneSection_lr_lane {
        const int id = 0;
    };

    struct t_road_lanes_laneSection_left_lane : t_road_lanes_laneSection_lr_lane {
        const int id = 1;
    };

    struct t_road_lanes_laneSection_right_lane : t_road_lanes_laneSection_lr_lane {
        const int id = -1;
    };

    struct t_roads_lanes_laneSection_center {
        const string delimiter = "center";
        QList<t_road_lanes_laneSection_center_lane> center;
    };

    struct t_roads_lanes_laneSection_left {
        const string delimiter = "left";
        QList<t_road_lanes_laneSection_left_lane> left;
    };

    struct t_roads_lanes_laneSection_right {
        const string delimiter = "right";
        QList<t_road_lanes_laneSection_right_lane> right;
    };

    struct t_road_lanes_laneSection {
        t_grEqZero s;
        bool singleSide;
        const string delimiter = "laneSection";
        t_roads_lanes_laneSection_center center;
        t_roads_lanes_laneSection_left *left;
        t_roads_lanes_laneSection_right *right;
    };

    struct t_road_lanes {
        const string delimiter = "lanes";
        QList<t_road_lanes_laneOffset> laneOffset;
        QList<t_road_lanes_laneSection> laneSection;
    };
}

// [End] Lane components

// [Begin] Road components
namespace Road {
    struct t_road_surface_CRG {
        string file;
        t_grEqZero sStart;
        t_grEqZero sEnd;
        e_direction orientation;
        e_road_surface_CRG_mode mode;
        e_road_surface_CRG_purpose purpose;
        double sOffset;
        double tOffset;
        double zOffset;
        double zScale;
        double hOffset;
    };

    struct t_road_surface {
        QList<t_road_surface_CRG> CRG;
    };

    struct t_road_lateralProfile_superelevation {
        t_grEqZero s;
        double a;
        double b;
        double c;
        double d;
        string delimiter = "superelevation";
    };

    struct t_road_lateralProfile_shape {
        t_grEqZero s;
        double t;
        double a;
        double b;
        double c;
        double d;
        string delimiter = "shape";
    };

    struct t_road_lateralProfile {
        QList<t_road_lateralProfile_superelevation> superelevation;
        QList<t_road_lateralProfile_shape> shape;
        string delimiter = "lateralProfile";
    };

    struct t_road_elevationProfile_elevation {
        t_grEqZero s;
        double a;
        double b;
        double c;
        double d;
        string delimiter = "elevation";
    };

    struct t_road_elevationProfile {
        QList<t_road_elevationProfile_elevation> elevation;
        string delimiter = "elevationProfile";
    };

    struct t_road_planView_geometry_line {
        string delimiter = "line";
    };

    struct t_road_planView_geometry_spiral {
        string delimiter = "spiral";
        double curvStart;
        double curvEnd;
    };

    struct t_road_planView_geometry_arc {
        string delimiter = "arc";
        double curvature;
    };

    struct t_road_planView_geometry_poly3 {
        string delimiter = "poly3";
        double a;
        double b;
        double c;
        double d;
    };

    struct t_road_planView_geometry_paramPoly3 {
        string delimiter = "paramPoly3";
        double aU;
        double bU;
        double cU;
        double dU;
        double aV;
        double bV;
        double cV;
        double dV;
        e_paramPoly3_pRange pRange;
    };


    // TODO: cada plan view geometry só contém um dos campos
    // geométricos válidos. Tratar isso.
    struct t_road_planView_geometry {
        t_grEqZero s;
        double x;
        double y;
        double hdg;
        t_grZero length;
        t_road_planView_geometry_arc arc;
        t_road_planView_geometry_spiral spiral;
        t_road_planView_geometry_line line;
        t_road_planView_geometry_poly3 poly3;
        t_road_planView_geometry_paramPoly3 paramPoly3;
        string delimiter = "geometry";
    };

    struct t_road_planView {
        QList<t_road_planView_geometry> geometry;
        string delimitr = "planView";
    };

    struct t_road_type_speed {
        t_maxSpeed max;
        e_unitSpeed unit;
        string delimiter = "speed";
    };

    struct t_road_type {
        t_grEqZero s;
        e_roadType type;
        e_countryCode country;
        t_road_type_speed *speed;
        string demiter = "type";
    };

    struct t_road_link_predecessorSucessor {
        string elementId;
        string delimiter;
        e_road_link_elementType elementType;
        e_contactPoint *contactPoint;
        t_grEqZero *elementS;
        e_elementDir *elementDir;
    };

    struct t_road_link {
        string delimiter = "link";
        t_road_link_predecessorSucessor *predecessor;
        t_road_link_predecessorSucessor *sucessor;
    };

    struct t_road {
        string name;
        t_grZero length;
        string id;
        string junction;
        e_trafficRule rule = RHT;
        string delimiter = "road";
        t_road_surface *surface;
        t_road_lateralProfile *lateralProfile;
        t_road_elevationProfile *elevationProfile;
        t_road_planView *planView;
        t_road_type *type;
        t_road_link *link;
        Lane::t_road_lanes lanes;
    };

}

// [End] Road components

namespace OpenDRIVE {
    class t_OpenDRIVE {
    private:
        t_header _header;
        QList<Road::t_road> road;
    public:
        t_OpenDRIVE();
    };
}


#endif // OPENDRIVEDEFINITIONS_H
