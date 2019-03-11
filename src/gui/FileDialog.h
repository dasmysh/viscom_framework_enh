/**
 * @file   FileDialog.h
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2018.12.14
 *
 * @brief  A file dialog in ImGui based on https://github.com/aiekick/ImGuiFileDialog.
 */

#pragma once

#include <vector>
#include <filesystem>
#include <array>

namespace gui {

    class FileDialog
    {
    public:
        FileDialog() = default;
        ~FileDialog() = default;

        bool ShowOpenFileDialog(std::string_view vName, const std::vector<std::string>& vFilters = {}, const std::filesystem::path& path = std::filesystem::path());
        bool ShowSaveFileDialog(std::string_view vName, const std::vector<std::string>& vFilters = {}, std::string_view vDefaultFileName = std::string_view(), const std::filesystem::path& path = std::filesystem::path());
        bool ShowNewDirectoryDialog(std::string_view vName, std::string_view vDefaultFileName = std::string_view(), const std::filesystem::path& path = std::filesystem::path());

        const std::filesystem::path& GetResult() const { return selectedFile_; }
        bool IsOK() const { return isOk_; }
        void Reset();

    private:
        static constexpr std::size_t MAX_FILE_DIALOG_NAME_BUFFER = 1024;
        static std::array<char, MAX_FILE_DIALOG_NAME_BUFFER> fileNameBuffer_;

        bool ShowFileDialog(std::string_view vName, bool existingOnly, bool directoriesOnly, const std::vector<std::string>& vFilters,
            const std::filesystem::path& path, std::string_view vDefaultFileName);

        void ScanCurrentPath();

        std::vector<std::filesystem::path> fileList_;
        std::filesystem::path selectedFile_;
        std::filesystem::path currentPath_ = std::filesystem::current_path();
        // std::vector<std::string> currentPath_Decomposition_;
        std::size_t currentFilterIndex_ = 0;
        bool isOk_ = false;
    };
}