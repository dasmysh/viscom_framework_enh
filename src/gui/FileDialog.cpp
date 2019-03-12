/**
 * @file   FileDialog.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2018.12.14
 *
 * @brief  A file dialog in ImGui based on https://github.com/aiekick/ImGuiFileDialog.
 */

#include "FileDialog.h"
#include <imgui.h>

#include <string_view>
#include <locale>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui_stdlib.h>

namespace gui {

    std::array<char, FileDialog::MAX_FILE_DIALOG_NAME_BUFFER> FileDialog::fileNameBuffer_;

    void FileDialog::Reset()
    {
        selectedFile_ = std::filesystem::path();
    }

    bool FileDialog::ShowOpenFileDialog(std::string_view vName, const std::vector<std::string>& vFilters, const std::filesystem::path& path)
    {
        return ShowFileDialog(vName, true, false, vFilters, path, "");
    }

    bool FileDialog::ShowSaveFileDialog(std::string_view vName, const std::vector<std::string>& vFilters, std::string_view vDefaultFileName, const std::filesystem::path& path)
    {
        return ShowFileDialog(vName, false, false, vFilters, path, vDefaultFileName);
    }

    bool FileDialog::ShowNewDirectoryDialog(std::string_view vName, std::string_view vDefaultFileName, const std::filesystem::path& path)
    {
        return ShowFileDialog(vName, false, true, {}, path, vDefaultFileName);
    }

    bool FileDialog::ShowFileDialog(std::string_view vName, bool existingOnly, bool directoriesOnly,
        const std::vector<std::string>& vFilters, const std::filesystem::path& path, std::string_view vDefaultFileName)
    {
        namespace fs = std::filesystem;

        bool res = false;
        isOk_ = false;
        std::locale loc;
        bool pathClick = false;

        ImGui::SetNextWindowSize(ImVec2(700, 510), ImGuiCond_FirstUseEver);
        ImGui::Begin(vName.data());

        if (fileList_.empty()) {
            if (!vDefaultFileName.empty()) selectedFile_ = currentPath_ / vDefaultFileName;

            if (path != std::filesystem::path()) currentPath_ = path;
            ScanCurrentPath();
        }

        std::vector<std::string> supportedDriveLetters = { "A:/", "B:/", "C:/", "D:/", "E:/", "F:/", "G:/", "H:/",
            "I:/", "J:/", "K:/", "L:/", "M:/", "N:/", "O:/", "P:/", "Q:/", "R:/", "S:/", "T:/", "U:/", "V:/", "W:/", "X:/", "Y:/", "Z:/" };
        bool firstDrive = true;
        for (const auto& dl : supportedDriveLetters) {
            std::error_code ec;
            bool isDir = fs::is_directory(dl, ec);
            if (!ec && isDir) {
                if (!firstDrive) ImGui::SameLine();
                if (ImGui::Button(dl.c_str())) {
                    currentPath_ = dl;
                    pathClick = true;
                    break;
                }
                firstDrive = false;
            }
        }

        // show current path
        bool root = true;
        for (const auto& pathElement : currentPath_)
        {
            if (!root) ImGui::SameLine();
            if (ImGui::Button(pathElement.string().c_str())) {
                auto newCurrentPath = currentPath_;
                while (newCurrentPath.stem() != pathElement) newCurrentPath = newCurrentPath.parent_path();
                currentPath_ = newCurrentPath;
                pathClick = true;
                break;
            }

            root = false;
        }

        ImVec2 size = ImGui::GetContentRegionMax() - ImVec2(0.0f, 120.0f);

        ImGui::BeginChild("##FileDialog_FileList", size);
        if (ImGui::Selectable("[Dir] ..", false)) {
            currentPath_ = currentPath_.parent_path();
            pathClick = true;
        }

        for (const auto& fileInfo : fileList_) {
            std::string str;
            if (fs::is_directory(fileInfo)) str = "[Dir] " + fileInfo.stem().string();
            if (fs::is_symlink(fileInfo)) str = "[Link] " + fileInfo.filename().string();
            if (fs::is_regular_file(fileInfo)) str = "[File] " + fileInfo.filename().string();
            auto lcExtension = fileInfo.extension().string();
            std::transform(lcExtension.begin(), lcExtension.end(), lcExtension.begin(), ::tolower);

            if (!vFilters.empty() && fs::is_regular_file(fileInfo)) {
                if (currentFilterIndex_ != vFilters.size()) {
                    if (lcExtension != vFilters[currentFilterIndex_]) continue;
                }
                else {
                    if (std::find(vFilters.begin(), vFilters.end(), lcExtension) == vFilters.end()) continue;
                }
            }
            if (directoriesOnly && !fs::is_directory(fileInfo)) continue;

            if (ImGui::Selectable(str.c_str(), (fileInfo == selectedFile_))) {
                if (fs::is_directory(fileInfo)) {
                    currentPath_ = fileInfo;
                    pathClick = true;
                }
                else selectedFile_ = fileInfo;
                break;
            }
        }

        if (pathClick == true) ScanCurrentPath();

        ImGui::EndChild();

        ImGui::Text("File Name : ");
        ImGui::SameLine();

        float width = ImGui::GetContentRegionAvailWidth();
        if (!vFilters.empty()) width -= 120.0f;
        ImGui::PushItemWidth(width);
        auto filename = selectedFile_.filename().string();
        //auto lastPos = selectedFile_.filename().string().copy(fileNameBuffer_.data(), MAX_FILE_DIALOG_NAME_BUFFER - 1, 0);
        //fileNameBuffer_[lastPos] = '\0';
        //ImGui::InputText("##FileName", fileNameBuffer_.data(), MAX_FILE_DIALOG_NAME_BUFFER);
        ImGui::InputText("##FileName", &filename);
        selectedFile_ = currentPath_ / filename;
        ImGui::PopItemWidth();

        if (!vFilters.empty()) {
            ImGui::SameLine();
            ImGui::PushItemWidth(100.0f);
            if (ImGui::BeginCombo("##Filters", currentFilterIndex_ >= vFilters.size() ? "All" : vFilters[currentFilterIndex_].c_str())) {
                std::size_t currentFilter = 0;
                for (const auto& filter : vFilters) {
                    if (ImGui::Selectable(filter.c_str(), currentFilter == currentFilterIndex_)) currentFilterIndex_ = currentFilter;
                    ++currentFilter;
                }
                if (ImGui::Selectable("All", currentFilter == currentFilterIndex_)) currentFilterIndex_ = currentFilter;

                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
        }

        if (ImGui::Button("Cancel")) {
            isOk_ = false;
            res = true;
        }

        ImGui::SameLine();

        bool showOkButton = !existingOnly;
        showOkButton = showOkButton || (fs::exists(selectedFile_) && fs::is_directory(selectedFile_) && directoriesOnly);
        showOkButton = showOkButton || (fs::exists(selectedFile_) && fs::is_regular_file(selectedFile_) && !directoriesOnly);
        if (showOkButton && ImGui::Button("OK")) {
            isOk_ = true;
            res = true;
        }

        ImGui::End();

        if (res == true) {
            fileList_.clear();
            currentFilterIndex_ = 0;
        }

        return res;
    }

    void FileDialog::ScanCurrentPath()
    {
        fileList_.clear();

        namespace fs = std::filesystem;
        for (auto& p : fs::directory_iterator(currentPath_)) {
            fileList_.push_back(p.path());
        }

        std::sort(fileList_.begin(), fileList_.end(), [](const auto &a, const auto &b) {
            if (fs::is_directory(a) && !fs::is_directory(b)) return true;
            else if (!fs::is_directory(a) && fs::is_directory(b)) return false;
            else return a < b;
        });
    }
}
