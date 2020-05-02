/**
 * @file   serialization_helper.cpp
 * @author Sebastian Maisch <sebastian.maisch@googlemail.com>
 * @date   2017.05.03
 *
 * @brief  Implementation of serialization helper classes and functions.
 */

#include "serialization_helper.h"
#ifndef __APPLE_CC__
#include <filesystem>
#endif
#include <spdlog/spdlog.h>

namespace viscom::enh {

    BinaryIAWrapper::BinaryIAWrapper(const std::string& filename) :
        ArchiveWrapper{ filename }
    {
#ifndef __APPLE_CC__
        if (IsValid()) {
            auto lastModTime = std::filesystem::last_write_time(filename);
            auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(lastModTime.time_since_epoch()).count();
            decltype(timestamp) binTimestamp;
            (*this)(cereal::make_nvp("timestamp", binTimestamp));
            if (binTimestamp < timestamp) {
                spdlog::warn("Will not load binary file. Falling back to original.\nFilename: {}\nDescription: Timestamp older than original file.", GetBinFilename(filename));
                Close();
            }
        } else {
            spdlog::warn("Will not load binary file. Falling back to original.\nFilename: {}\nDescription: File does not exist.", GetBinFilename(filename));
        }
#endif
    }

    BinaryOAWrapper::BinaryOAWrapper(const std::string& filename) :
        ArchiveWrapper{ filename }
    {
#ifndef __APPLE_CC__
        auto lastModTime = std::filesystem::last_write_time(filename);
        auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(lastModTime.time_since_epoch()).count();
        (*this)(cereal::make_nvp("timestamp", timestamp));
#endif
    }
}
