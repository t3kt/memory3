#pragma once

#include "CHOP_ApiWrapper.h"

namespace tekt {

  float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp = false);

  TD::Vector remap(
    float value,
    float inputMin, float inputMax,
    const TD::Vector& outputMin, const TD::Vector& outputMax,
    bool clamp = false);

}

