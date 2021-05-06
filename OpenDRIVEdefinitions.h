#ifndef OPENDRIVEDEFINITIONS_H
#define OPENDRIVEDEFINITIONS_H
#include <QString>
#include <QList>
#include <QMap>

// [Begin] Definitions of variable types
typedef unsigned short int ushort;
typedef double t_grZero;
typedef double t_grEqZero;
typedef QString string;
typedef string e_trafficRule;
typedef bool e_direction;
typedef ushort e_road_surface_CRG_mode;
typedef ushort e_road_surface_CRG_purpose;
typedef double e_paramPoly3_pRange;
typedef string e_countryCode;
typedef string e_roadType;
typedef float t_maxSpeed;
typedef string e_unitSpeed;
typedef string e_road_link_elementType;
typedef string e_contactPoint;
typedef string e_elementDir;
typedef string e_laneType;
typedef string e_road_lanes_laneSection_lr_lane_access_rule;
typedef string e_accessRestrictionType;
typedef string e_junction_type;
typedef string e_outlineFillType;
typedef string e_road_objects_object_parkingSpace_access;
typedef string e_borderType;
typedef string e_sideType;
typedef string e_roadMarkWeight;
typedef string e_roadMarkColor;
typedef string e_tunnelType;
typedef float t_zeroOne;
typedef string e_bridgeType;
typedef string e_objectType;
typedef string e_orientation;
typedef string e_road_signals_signal_reference_elementType;
typedef string e_unit;
typedef string e_road_railroad_switch_position;
// [End] Definitions of variable types

// TODO: geoReference
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
        double sOffset;
        string type;
        string material;
        QMap<string, string> attributes;
    };

    struct t_road_lanes_laneSection_lr_lane_rule {
        const string delimiter = "rule";
        t_grEqZero sOffset;
        string value;
    };

    struct t_road_lanes_laneSection_lr_lane {
        const string delimiter = "lane";
        e_laneType type;
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
        int id;
    };

    struct t_road_lanes_laneSection_left_lane : t_road_lanes_laneSection_lr_lane {
        int id;
    };

    struct t_road_lanes_laneSection_right_lane : t_road_lanes_laneSection_lr_lane {
        int id;
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

    struct t_road_objects_object_laneValidity {
        const string delimiter = "validity";
        int fromLane;
        int toLane;
    };
}
// [End] Lane components

// [Begin] Junction components
namespace Junction {
    struct t_junction_predecessorSucessor {
        string elementType = "road";
        string elementId;
        t_grZero elementS;
        e_elementDir elementDir;
    };

    struct t_junction_connection_laneLink {
        const string delimiter = "laneLink";
        int from;
        int to;
    };

    struct t_junction_connection {
        const string delimiter = "connection";
        string id;
        e_junction_type type;
        string incomingRoad;
        string connectingRoad;
        e_contactPoint contactPoint;
        QList<t_junction_connection_laneLink> laneLink;
        t_junction_predecessorSucessor *predecessor;
        t_junction_predecessorSucessor *successor;
    };

    struct t_junction_priority {
        string high;
        string low;
        const string delimiter = "priority";
    };

    struct t_junction_controller {
        string id;
        string type;
        int sequence;
        const string delimiter = "controller";
    };

    struct t_junction_surface_CRG {
        string file;
        e_road_surface_CRG_mode mode;
        e_road_surface_CRG_purpose purpose;
        double zOffset;
        double zScale;
    };

    struct t_junction_surface {
        const string delimiter = "surface";
        QList<t_junction_surface_CRG> CRG;
    };

    struct t_junction {
        const string delimiter = "junction";
        string name;
        string id;
        e_junction_type type;
        t_junction_surface *surface;
        QList<t_junction_connection> connection;
        QList<t_junction_priority> priority;
        QList<t_junction_controller> controller;
    };
}
// [End] Junction components

// [Begin] Object components
namespace Object {
    struct t_road_objects_outlines_outline_cornerRoad {
        const string delimiter = "cornerRoad";
        t_grEqZero s;
        double t;
        double dz;
        double height;
        int id;
    };

    struct t_road_objects_outlines_outline_cornerLocal {
        const string delimiter = "cornerLocal";
        double u;
        double v;
        double z;
        double height;
        int id;
    };

    struct t_road_objects_object_outlines_outline {
        const string delimiter = "outline";
        int id;
        bool outer;
        bool closed;
        e_laneType laneType;
        e_outlineFillType fillType;
        QList<t_road_objects_outlines_outline_cornerLocal> cornerLocal;
        QList<t_road_objects_outlines_outline_cornerRoad> cornerRoad;
    };

    struct t_road_objects_object_outlines {
        const string delimiter = "outlines";
        QList<t_road_objects_object_outlines_outline> outline;
    };

    struct t_road_objects_object_material {
        string surface;
        t_grEqZero friction;
        t_grEqZero roughness;
        const string delimiter = "material";
    };

    struct t_road_objects_object_parkingSpace {
        e_road_objects_object_parkingSpace_access access;
        string restrictions;
        const string delimiter = "parkingSpace";
    };

    struct t_road_objects_object_repeat {
        t_grEqZero s;
        t_grEqZero length;
        t_grEqZero distance;
        double tStart;
        double tEnd;
        double heightStart;
        double heightEnd;
        double zOffsetStart;
        double zOffsetEnd;
        t_grEqZero widthStart;
        t_grEqZero widthEnd;
        t_grEqZero lengthStart;
        t_grEqZero lengthEnd;
        t_grEqZero radiusStart;
        t_grEqZero radiusEnd;
        const string delimiter = "repeat";
    };

    struct t_road_objects_object_markings_marking_cornerReference {
        int id;
        const string delimiter = "cornerReference";
    };

    struct t_road_objects_object_borders_border {
        QList<t_road_objects_object_markings_marking_cornerReference> cornerReference;
        t_grEqZero width;
        e_borderType type;
        int outlineId;
        bool useCompleteOutline;
    };

    struct t_road_objects_object_borders {
        QList<t_road_objects_object_borders_border> border;
        const string delimiter = "border";
    };

    struct t_road_objects_object_markings_marking {
        e_sideType side;
        e_roadMarkWeight weight;
        t_grZero width;
        e_roadMarkColor color;
        t_grEqZero zOffset;
        t_grEqZero spaceLength;
        t_grZero lineLength;
        double startOffset;
        double stopOffset;
        QList<t_road_objects_object_markings_marking_cornerReference> cornerReference;
    };

    struct t_road_objects_object_markings {
        const string delimiter = "marking";
        QList<t_road_objects_object_markings_marking> marking;
    };

    struct t_road_objects_tunnel {
        t_grEqZero s;
        t_grEqZero length;
        string name;
        string id;
        e_tunnelType type;
        t_zeroOne lighting;
        t_zeroOne daylight;
        QList<Lane::t_road_objects_object_laneValidity> validity;
        const string delimiter = "tunnel";
    };

    struct t_road_objects_bridge {
        t_grEqZero s;
        t_grEqZero length;
        string name;
        string id;
        e_bridgeType type;
        QList<Lane::t_road_objects_object_laneValidity> validity;
        const string delimiter = "bridge";
    };

    struct t_road_objects_objectReference {
        t_grEqZero s;
        double t;
        double zOffset;
        string id;
        t_grEqZero validLength;
        QList<Lane::t_road_objects_object_laneValidity> validity;
        const string delimiter = "objectReference";
    };

    struct t_road_objects_object {
        const string delimiter = "object";
        double t;
        double zOffset;
        e_objectType type;
        t_grEqZero validLength;
        e_orientation orientation;
        string subtype;
        bool dynamic;
        double hdg;
        string name;
        double pitch;
        double roll;
        string id;
        double height;
        t_grEqZero s;
        double length;
        double width;
        double radius;
        t_road_objects_object_outlines *outlines;
        t_road_objects_object_parkingSpace *parkingSpace;
        QList<t_road_objects_object_repeat> repeat;
        QList<t_road_objects_object_material> material;
        QList<Lane::t_road_objects_object_laneValidity> validity;
        t_road_objects_object_borders *borders;
        t_road_objects_object_markings *markings;
    };

    struct t_road_objects {
        QList<t_road_objects_object> object;
        QList<t_road_objects_tunnel> tunnel;
        QList<t_road_objects_bridge> bridge;
        QList<t_road_objects_objectReference> objectReference;
        const string delimiter = "objects";
    };

}
// [End] Object components

// [Begin] Signal components
namespace Signal {
    struct t_road_signals_signal_positionRoad {
        string roadId;
        t_grEqZero s;
        double t;
        double zOffset;
        double hOffset;
        double pitch;
        double roll;
        const string delimiter = "positionRoad";
    };

    struct t_road_signals_signal_positionInertial {
        double x;
        double y;
        double z;
        double hdg;
        double pitch;
        double roll;
        const string delimiter = "positionInertial";
    };

    struct t_physicalPosition {
        t_road_signals_signal_positionRoad positionRoad;
        t_road_signals_signal_positionInertial positionInertial;
    };

    struct t_road_signals_signal_reference {
        string type;
        string elementId;
        e_road_signals_signal_reference_elementType elementType;
    };

    struct t_road_signals_dependency {
        const string delimiter = "dependency";
        string id;
        string type;
    };

    struct t_road_signals_signal {
        const string delimiter = "signal";
        t_grEqZero s;
        double t;
        string id;
        string name;
        bool dynamic;
        e_orientation orientation;
        double zOffset;
        e_countryCode country;
        string countryRevision;
        string type;
        string subtype;
        double value;
        e_unit unit;
        t_grEqZero height;
        t_grEqZero width;
        string text;
        double hOffset;
        double pitch;
        double roll;
        QList<Lane::t_road_objects_object_laneValidity> validity;
        QList<t_road_signals_dependency> dependency;
        QList<t_road_signals_signal_reference> reference;
        t_physicalPosition *physicalPosition;
    };

    struct t_road_signals_signalReference {
        const string delimiter = "signalReference";
        QList<Lane::t_road_objects_object_laneValidity> validity;
        t_grEqZero s;
        double t;
        string id;
        e_orientation orientation;
    };

    struct t_road_signals {
        const string delimiter = "signals";
        QList<t_road_signals_signalReference> signalReference;
        QList<t_road_signals_signal> signal;
    };

    struct t_controller_control {
        const string delimiter = "control";
        string signalId;
        string type;
    };

    struct t_controller {
        const string delimiter = "controller";
        string id;
        string name;
        int sequence;
        QList<t_controller_control> control;
    };
}
// [End] Signal components

// [Begin] Railroad components
namespace Railroad {
    struct t_road_railroad_switch_mainTrack {
        string id;
        t_grEqZero s;
        e_elementDir dir;
        const string delimiter = "mainTrack";
    };

    struct t_road_railroad_switch_sideTrack {
        string id;
        t_grEqZero s;
        e_elementDir dir;
        const string delimiter = "sideTrack";
    };

    struct t_road_railroad_switch_partner {
        string name;
        string id;
        const string delimiter = "switchPartner";
    };

    struct t_road_railroad_switch {
        const string delimiter = "switch";
        string name;
        string id;
        e_road_railroad_switch_position position;
        t_road_railroad_switch_mainTrack mainTrack;
        t_road_railroad_switch_partner partner;
        t_road_railroad_switch_sideTrack sideTrack;
    };

    struct t_road_railroad {
        const string delimiter = "railroad";
        QList<t_road_railroad_switch> switches;
    };
}
// [End] Railroad components

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
        t_road_planView_geometry_arc *arc;
        t_road_planView_geometry_spiral *spiral;
        t_road_planView_geometry_line *line;
        t_road_planView_geometry_poly3 *poly3;
        t_road_planView_geometry_paramPoly3 *paramPoly3;
        string delimiter = "geometry";
    };

    struct t_road_planView {
        QList<t_road_planView_geometry> geometry;
        string delimiter = "planView";
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
        string delimiter = "type";
    };

    struct t_road_link_predecessorSucessor {
        string elementId;
        string delimiter;
        e_road_link_elementType elementType;
        e_contactPoint contactPoint;
        t_grEqZero elementS;
        e_elementDir elementDir;
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
        e_trafficRule rule;
        string delimiter = "road";
        t_road_surface *surface;
        t_road_lateralProfile *lateralProfile;
        t_road_elevationProfile *elevationProfile;
        t_road_planView planView;
        t_road_type *type;
        t_road_link *link;
        Lane::t_road_lanes lanes;
        Object::t_road_objects *objects;
        Signal::t_road_signals *signal;
        Railroad::t_road_railroad *railroad;
    };

}
// [End] Road components


#endif // OPENDRIVEDEFINITIONS_H
