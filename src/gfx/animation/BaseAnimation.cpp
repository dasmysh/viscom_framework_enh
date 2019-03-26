/**
 * @file   BaseAnimation.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.08.29
 *
 * @brief  Implementation of the animation base class.
 */

#include "BaseAnimation.h"

namespace viscom::enh {
    
    BaseAnimation::BaseAnimation()
    {
    }

    BaseAnimation::~BaseAnimation() = default;

    void BaseAnimation::StartAnimation(float currentTime)
    {
        animationRunning_ = true;
        animationStartTime_ = currentTime;
    }

    bool BaseAnimation::DoAnimationStep(float currentTime)
    {
        if (!animationRunning_) return false;
        return true;
    }
}
