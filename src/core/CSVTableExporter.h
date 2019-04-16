/**
 * @file   CSVTableExporter.h
 * @author Sebastian Maisch <sebastian.maisch@googlemail.com>
 * @date   2019.04.02
 *
 * @brief  Declaration of an CSV exporter for table data.
 */

#pragma once

#include <vector>
#include <string>

namespace viscom::enh {

    class CSVTableExporter
    {
    public:
        CSVTableExporter(const std::string& filename, const std::string& firstColumnName);

        void SetNumCurrentColumns(std::size_t numCurrent);
        void SetCurrentColumnIndexName(std::size_t currentIndex, const std::string& name);
        void SetCurrentRowName(const std::string& currentRowName);
        void SetEntry(std::size_t currentIndex, double value);
        void WriteTable();
    private:
        using RowEntry = std::pair<std::string, std::vector<double>>;

        std::string csvFilename_;
        std::string firstColumnName_;
        std::vector<std::string> columns_;
        std::vector<RowEntry> rows_;

        std::vector<std::size_t> currentColumnIndexToInternalColumnId;
        std::size_t currentRowIndex_ = 0;
    };
}
