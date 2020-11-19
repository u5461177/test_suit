//
// Created by xhshu on 8/2/18.
//

#include "positioneffecttype.h"

namespace ats {

    using namespace std;

    string xtp_position_effect_type_to_string(XTP_POSITION_EFFECT_TYPE xtp_position_effect) {
        switch (xtp_position_effect) {
            case XTP_POSITION_EFFECT_INIT:
                return "XTP_POSITION_EFFECT_INIT";
            case XTP_POSITION_EFFECT_OPEN:
                return "XTP_POSITION_EFFECT_OPEN";
            case XTP_POSITION_EFFECT_CLOSE:
                return "XTP_POSITION_EFFECT_CLOSE";
            case XTP_POSITION_EFFECT_FORCECLOSE:
                return "XTP_POSITION_EFFECT_FORCECLOSE";
            case XTP_POSITION_EFFECT_CLOSETODAY:
                return "XTP_POSITION_EFFECT_CLOSETODAY";
            case XTP_POSITION_EFFECT_CLOSEYESTERDAY:
                return "XTP_POSITION_EFFECT_CLOSEYESTERDAY";
            case XTP_POSITION_EFFECT_FORCEOFF:
                return "XTP_POSITION_EFFECT_FORCEOFF";
            case XTP_POSITION_EFFECT_LOCALFORCECLOSE:
                return "XTP_POSITION_EFFECT_LOCALFORCECLOSE";
            default:
                return "UNKNOWN";
        }
    }

    string position_effect_type_to_string(PositionEffectType position_effect) {
        switch (position_effect) {
            case PositionEffectType::POSITION_EFFECT_INIT:
                return "POSITION_EFFECT_INIT";
            case PositionEffectType::POSITION_EFFECT_OPEN:
                return "POSITION_EFFECT_OPEN";
            case PositionEffectType::POSITION_EFFECT_CLOSE:
                return "POSITION_EFFECT_CLOSE";
            case PositionEffectType::POSITION_EFFECT_FORCECLOSE:
                return "POSITION_EFFECT_FORCECLOSE";
            case PositionEffectType::POSITION_EFFECT_CLOSETODAY:
                return "POSITION_EFFECT_CLOSETODAY";
            case PositionEffectType::POSITION_EFFECT_CLOSEYESTERDAY:
                return "POSITION_EFFECT_CLOSEYESTERDAY";
            case PositionEffectType::POSITION_EFFECT_FORCEOFF:
                return "POSITION_EFFECT_FORCEOFF";
            case PositionEffectType::POSITION_EFFECT_LOCALFORCECLOSE:
                return "POSITION_EFFECT_LOCALFORCECLOSE";
            default:
                return "UNKNOWN";
        }
    }

    PositionEffectType string_to_position_effect_type(const string &str) {
        if (str == "POSITION_EFFECT_INIT")
            return PositionEffectType::POSITION_EFFECT_INIT;
        if (str == "POSITION_EFFECT_OPEN")
            return PositionEffectType::POSITION_EFFECT_OPEN;
        if (str == "POSITION_EFFECT_CLOSE")
            return PositionEffectType::POSITION_EFFECT_CLOSE;
        if (str == "POSITION_EFFECT_FORCECLOSE")
            return PositionEffectType::POSITION_EFFECT_FORCECLOSE;
        if (str == "POSITION_EFFECT_CLOSETODAY")
            return PositionEffectType::POSITION_EFFECT_CLOSETODAY;
        if (str == "POSITION_EFFECT_CLOSEYESTERDAY")
            return PositionEffectType::POSITION_EFFECT_CLOSEYESTERDAY;
        if (str == "POSITION_EFFECT_FORCEOFF")
            return PositionEffectType::POSITION_EFFECT_FORCEOFF;
        if (str == "POSITION_EFFECT_LOCALFORCECLOSE")
            return PositionEffectType::POSITION_EFFECT_LOCALFORCECLOSE;
        return PositionEffectType::UNKNOWN;
    }

    XTP_POSITION_EFFECT_TYPE position_effect_type_to_xtp_position_effect_type(PositionEffectType position_effect) {
        switch (position_effect) {
            case PositionEffectType::POSITION_EFFECT_INIT:
                return XTP_POSITION_EFFECT_INIT;
            case PositionEffectType::POSITION_EFFECT_OPEN:
                return XTP_POSITION_EFFECT_OPEN;
            case PositionEffectType::POSITION_EFFECT_CLOSE:
                return XTP_POSITION_EFFECT_CLOSE;
            case PositionEffectType::POSITION_EFFECT_FORCECLOSE:
                return XTP_POSITION_EFFECT_FORCECLOSE;
            case PositionEffectType::POSITION_EFFECT_CLOSETODAY:
                return XTP_POSITION_EFFECT_CLOSETODAY;
            case PositionEffectType::POSITION_EFFECT_CLOSEYESTERDAY:
                return XTP_POSITION_EFFECT_CLOSEYESTERDAY;
            case PositionEffectType::POSITION_EFFECT_FORCEOFF:
                return XTP_POSITION_EFFECT_FORCEOFF;
            case PositionEffectType::POSITION_EFFECT_LOCALFORCECLOSE:
                return XTP_POSITION_EFFECT_LOCALFORCECLOSE;
            default:
                return XTP_POSITION_EFFECT_INIT;    // TODO
        }
    }

    PositionEffectType xtp_position_effect_type_to_position_effect_type(XTP_POSITION_EFFECT_TYPE xtp_position_effect) {
        switch (xtp_position_effect) {
            case XTP_POSITION_EFFECT_INIT:
                return PositionEffectType::POSITION_EFFECT_INIT;
            case XTP_POSITION_EFFECT_OPEN:
                return PositionEffectType::POSITION_EFFECT_OPEN;
            case XTP_POSITION_EFFECT_CLOSE:
                return PositionEffectType::POSITION_EFFECT_CLOSE;
            case XTP_POSITION_EFFECT_FORCECLOSE:
                return PositionEffectType::POSITION_EFFECT_FORCECLOSE;
            case XTP_POSITION_EFFECT_CLOSETODAY:
                return PositionEffectType::POSITION_EFFECT_CLOSETODAY;
            case XTP_POSITION_EFFECT_CLOSEYESTERDAY:
                return PositionEffectType::POSITION_EFFECT_CLOSEYESTERDAY;
            case XTP_POSITION_EFFECT_FORCEOFF:
                return PositionEffectType::POSITION_EFFECT_FORCEOFF;
            case XTP_POSITION_EFFECT_LOCALFORCECLOSE:
                return PositionEffectType::POSITION_EFFECT_LOCALFORCECLOSE;
            default:
                return PositionEffectType::UNKNOWN;
        }
    }
}
