/**
 * @file   CSVTableExporter.cpp
 * @author Sebastian Maisch <sebastian.maisch@uni-ulm.de>
 * @date   2019.04.02
 *
 * @brief  Definition of an CSV exporter for table data.
 */

#include "CSVTableExporter.h"
#include <filesystem>
#include <fstream>
#include <core/utils/utils.h>

namespace viscom::enh {

    CSVTableExporter::CSVTableExporter(const std::string& filename, const std::string& firstColumnName) :
        csvFilename_{ filename },
        firstColumnName_{ firstColumnName }
    {
        if (std::filesystem::exists(csvFilename_)) {
            std::ifstream csv_file{ csvFilename_ };
            std::string line;
            if (csv_file) {
                std::getline(csv_file, line);
                auto splitLine = viscom::utils::split(line, ',');
                if (splitLine.empty() || splitLine[0] != firstColumnName_) return;
                for (std::size_t i = 1; i < splitLine.size(); ++i) columns_.push_back(splitLine[i]);
            }
            while (csv_file) {
                std::getline(csv_file, line);
                auto splitLine = viscom::utils::split(line, ',');
                if (splitLine.empty()) continue;
                rows_.emplace_back(splitLine[0], splitLine.size() - 1);
                for (std::size_t i = 1; i < splitLine.size(); ++i) rows_.back().second[i - 1] = std::stod(splitLine[i]);
            }
        }
    }

    void CSVTableExporter::SetNumCurrentColumns(std::size_t numCurrent)
    {
        currentColumnIndexToInternalColumnId.resize(numCurrent, 0);
    }

    void CSVTableExporter::SetCurrentColumnIndexName(std::size_t currentIndex, const std::string& name)
    {
        if (auto it = std::find(columns_.begin(), columns_.end(), name); it == columns_.end()) {
            currentColumnIndexToInternalColumnId[currentIndex] = columns_.size();
            columns_.emplace_back(name);
            for (auto& row : rows_) row.second.emplace_back(0.0);
        }
        else {
            currentColumnIndexToInternalColumnId[currentIndex] = it - columns_.begin();
        }
    }

    void CSVTableExporter::SetCurrentRowName(const std::string& currentRowName)
    {
        if (auto it = std::find_if(rows_.begin(), rows_.end(), [&currentRowName](const RowEntry& re) { return re.first == currentRowName; }); it == rows_.end()) {
            currentRowIndex_ = rows_.size();
            rows_.emplace_back(currentRowName, columns_.size());
        }
        else {
            currentRowIndex_ = it - rows_.begin();
        }
    }

    void CSVTableExporter::WriteTable()
    {
        std::ofstream csv_file{ csvFilename_ };
        csv_file << firstColumnName_;
        for (const auto& column : columns_) csv_file << ',' << column;
        csv_file << '\n';

        for (const auto& row : rows_) {
            csv_file << row.first;
            for (auto value : row.second) csv_file << ',' << value;
            csv_file << '\n';
        }
    }

    void CSVTableExporter::SetEntry(std::size_t currentIndex, double value)
    {
        rows_[currentRowIndex_].second[currentColumnIndexToInternalColumnId[currentIndex]] = value;
    }

}
