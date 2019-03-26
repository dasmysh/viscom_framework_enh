/**
 * @file   BaseAnimation.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.08.29
 *
 * @brief  Declaration of the base class for animations.
 */

#pragma once

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>

namespace viscom::enh {

    class BaseAnimation
    {
    public:
        BaseAnimation();
        virtual ~BaseAnimation();

        virtual void StartAnimation(float currentTime);
        void StopAnimation() { animationRunning_ = false; }
        virtual bool DoAnimationStep(float currentTime);

        bool IsAnimationRunning() const { return animationRunning_; }

    protected:
        float GetAnimationStartTime() const { return animationStartTime_; }
        float GetCurrentAnimationTime(float currentTime) const { return currentTime - animationStartTime_; }

    private:
        /** Needed for serialization */
        friend class cereal::access;

        template<class Archive>
        void save(Archive&, const std::uint32_t) const {}

        template<class Archive>
        void load(Archive&, const std::uint32_t)
        {
            // we do not serialize this as it is a running state.
            animationRunning_ = false;
            animationStartTime_ = 0.0f;
        }

        /** Hold whether the animation is running. */
        bool animationRunning_ = false;
        /** Holds the current animation time. */
        float animationStartTime_ = 0.0f;
    };
}

CEREAL_CLASS_VERSION(viscom::enh::BaseAnimation, 1)
