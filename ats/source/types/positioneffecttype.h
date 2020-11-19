//
// Created by xhshu on 8/2/18.
//

#ifndef ATS_POSITIONEFFECTTYPE_H
#define ATS_POSITIONEFFECTTYPE_H


#include <string>
#include <xtp_api_struct.h>

namespace ats {

    enum class PositionEffectType {
        POSITION_EFFECT_INIT,
        POSITION_EFFECT_OPEN,
        POSITION_EFFECT_CLOSE,
        POSITION_EFFECT_FORCECLOSE,
        POSITION_EFFECT_CLOSETODAY,
        POSITION_EFFECT_CLOSEYESTERDAY,
        POSITION_EFFECT_FORCEOFF,
        POSITION_EFFECT_LOCALFORCECLOSE,
        UNKNOWN
    };

    PositionEffectType string_to_position_effect_type(const std::string &str);

    std::string position_effect_type_to_string(PositionEffectType position_effect);

    std::string xtp_position_effect_type_to_string(XTP_POSITION_EFFECT_TYPE position_effect);

    PositionEffectType xtp_position_effect_type_to_position_effect_type(XTP_POSITION_EFFECT_TYPE position_effect);

    XTP_POSITION_EFFECT_TYPE position_effect_type_to_xtp_position_effect_type(PositionEffectType position_effect);


}

#endif //ATS_POSITIONEFFECTTYPE_H