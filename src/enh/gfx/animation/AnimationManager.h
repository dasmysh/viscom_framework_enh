/**
 * @file   AnimationManager.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2016.08.25
 *
 * @brief  Declaration of the animation manager.
 */

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "WayPointAnimation.h"
#include "RotationAnimation.h"
#include "OrbitAnimation.h"

namespace viscom::enh {

    template<class T>
    class TAnimationManager
    {
    public:
        explicit TAnimationManager(const std::string& dir, const CameraHelper* camera);
        ~TAnimationManager();

        template<typename... Args>
        unsigned int AddAnimation(const std::string& name, Args&&... ctorArgs) {
            animations_.emplace_back(std::forward<Args>(ctorArgs)...);
            names_.emplace_back(name);
            return AddLastAnimation(name);
        }
        T& operator[] (std::size_t id) { return animations_[id]; }
        const T& operator[] (std::size_t id) const { return animations_[id]; }
        T& operator[] (const std::string& name) { return *animationsByName_[name].second; }
        const T& operator[] (const std::string& name) const { return *animationsByName_.at(name).second; } //-V659
        T& GetCurrent() { return animations_[currentAnimation_]; }
        const T& GetCurrent() const { return animations_[currentAnimation_]; }
        int GetCurrentId() const { return currentAnimation_; }
        typename T::Editor& GetEditor() { return editor_; }

        void StartAnimation();
        void StopAnimation();
        bool DoAnimationStep(float elapsedTime);

        // void ShowAnimationMenu(const std::string& name);
        void ShowAnimationMenu(const std::string& name, bool showMenu, bool showEdit);

        void UnselectCurrent() { currentAnimation_ = -1; editor_.SetCurrentEdited(nullptr); }

        void LoadAnimation(const std::string& filename, std::size_t set);
        void SaveAnimation(const std::string& filename, std::size_t set);
        void LoadAll(const std::string& filename);
        void SaveAll(const std::string& filename);
        void LoadAllFromStream(std::istream& stream);
        void SaveAllFromStream(std::ostream& stream);

    private:
        unsigned int AddLastAnimation(const std::string& name);

        /** Holds all animations. */
        std::vector<T> animations_;
        /** Holds all names. */
        std::vector<std::string> names_;
        /** Holds animations by name. */
        std::map<std::string, std::pair<std::size_t, T*>> animationsByName_;
        /** Holds the currently edited animation id. */
        int currentAnimation_ = -1;
        /** Holds the save/load directory. */
        std::string directory_;
        /** Holds the editor for this type. */
        typename T::Editor editor_;
    };

    using WPAnimationManager = TAnimationManager<WayPointAnimation>;
    using RotationAnimationManager = TAnimationManager<RotationAnimation>;
    using OrbitAnimationManager = TAnimationManager<OrbitAnimation>;

    enum class AnimationType
    {
        WAYPOINT,
        ROTATION,
        ORBIT
    };

    class AnimationManager
    {
    public:
        explicit AnimationManager(const std::string& dir, const CameraHelper* camera);
        ~AnimationManager();

        void StartAnimation();
        void StopAnimation();
        bool DoAnimationStep(float elapsedTime);

        void ShowAnimationMenu(const std::string& name, bool showMenu);
        void LoadAnimations(const std::string& fileName);
        void SaveAnimations(const std::string& fileName);

        const WPAnimationManager& GetWaypoints() const { return wpAnimations_; }
        WPAnimationManager& GetWaypoints() { return wpAnimations_; }
        const RotationAnimationManager& GetRotations() const { return rotAnimations_; }
        RotationAnimationManager& GetRotations() { return rotAnimations_; }
        const OrbitAnimationManager& GetOrbits() const { return orbAnimations_; }
        OrbitAnimationManager& GetOrbits() { return orbAnimations_; }

        const WayPointAnimation& GetWaypoint(std::size_t id) const { return wpAnimations_[id]; }
        WayPointAnimation& GetWaypoint(std::size_t id) { return wpAnimations_[id]; }
        const RotationAnimation& GetRotation(std::size_t id) const { return rotAnimations_[id]; }
        RotationAnimation& GetRotation(std::size_t id) { return rotAnimations_[id]; }
        const OrbitAnimation& GetOrbit(std::size_t id) const { return orbAnimations_[id]; }
        OrbitAnimation& GetOrbit(std::size_t id) { return orbAnimations_[id]; }

        const WayPointAnimation& GetCurrentWaypoint() const { return wpAnimations_.GetCurrent(); }
        WayPointAnimation& GetCurrentWaypoint() { return wpAnimations_.GetCurrent(); }
        const RotationAnimation& GetCurrentRotation() const { return rotAnimations_.GetCurrent(); }
        RotationAnimation& GetCurrentRotation() { return rotAnimations_.GetCurrent(); }
        const OrbitAnimation& GetCurrentOrbit() const { return orbAnimations_.GetCurrent(); }
        OrbitAnimation& GetCurrentOrbit() { return orbAnimations_.GetCurrent(); }

        AnimationType GetCurrentType() const { return currentAnimation_; }

    private:
        /** Holds the waypoint animations. */
        TAnimationManager<WayPointAnimation> wpAnimations_;
        /** Holds the rotation animations. */
        TAnimationManager<RotationAnimation> rotAnimations_;
        /** Holds the orbit animations. */
        TAnimationManager<OrbitAnimation> orbAnimations_;
        /** Holds the currently edited animation manager. */
        AnimationType currentAnimation_ = AnimationType::WAYPOINT;
        /** Holds the save/load directory. */
        std::string directory_;
    };
}
