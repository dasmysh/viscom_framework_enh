/**
 * @file   AdditionalInfoContainer.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2019.02.12
 *
 * @brief  Container for classes that contain additional information of any type.
 */

#include "AdditionalInfoContainer.h"

namespace mysh::core {

    void AdditionalInfoContainer::SetAnyAdditionalInfo(const std::string_view& renderer, const std::any& info)
    {
        if (auto search = additionalMeshInfo_.find(renderer); search != additionalMeshInfo_.end()) {
            search->second = std::move(info);
        }
        else {
            additionalMeshInfo_.insert(std::make_pair(renderer, info));
        }
    }
}
