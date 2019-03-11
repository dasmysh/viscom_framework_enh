/**
 * @file   AdditionalInfoContainer.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2019.02.12
 *
 * @brief  Container for classes that contain additional information of any type.
 */

#pragma once

#include <map>
#include <any>
#include <string>
#include <string_view>

namespace mysh::core {

    class AdditionalInfoContainer
    {
    public:
        void SetAnyAdditionalInfo(const std::string_view& renderer, const std::any& info);
        template<class T> void SetAdditionalInfo(const std::string_view& renderer, std::shared_ptr<T>&& info) { SetAnyAdditionalInfo(renderer, std::any{ info }); }
        template<class T> const T* GetAdditionalInfo(const std::string_view& renderer) const { return std::any_cast<std::shared_ptr<T>>(additionalMeshInfo_.find(renderer)->second).get(); }
        template<class T> T* GetAdditionalInfo(const std::string_view& renderer) { return std::any_cast<std::shared_ptr<T>>(additionalMeshInfo_.find(renderer)->second).get(); }

    private:
        /** Contains additional information of a mesh based on the renderer. */
        std::map<std::string, std::any, std::less<>> additionalMeshInfo_;
    };
}
