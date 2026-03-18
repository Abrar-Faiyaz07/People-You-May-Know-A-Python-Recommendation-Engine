#include "CSVParser.h"
#include "AppExceptions.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>

std::vector<std::vector<std::string> > CSVParser::readRows(const std::string& path, bool skipHeader) {
    std::vector<std::vector<std::string> > rows;
    std::ifstream input(path.c_str());
    if (!input.is_open()) {
        throw DataAccessException("Unable to open CSV file for read: " + path);
    }

    std::string line;
    bool first = true;
    while (std::getline(input, line)) {
        if (first && skipHeader) {
            first = false;
            continue;
        }
        first = false;

        if (line.empty()) {
            continue;
        }
        rows.push_back(splitLine(line));
    }

    return rows;
}

bool CSVParser::writeRows(const std::string& path, const std::vector<std::string>& header,
                          const std::vector<std::vector<std::string> >& rows) {
    std::ofstream output(path.c_str(), std::ios::trunc);
    if (!output.is_open()) {
        throw DataAccessException("Unable to open CSV file for write: " + path);
    }

    for (std::size_t i = 0; i < header.size(); ++i) {
        output << header[i];
        if (i + 1 < header.size()) {
            output << ',';
        }
    }
    output << '\n';

    for (std::size_t i = 0; i < rows.size(); ++i) {
        const std::vector<std::string>& row = rows[i];
        for (std::size_t j = 0; j < row.size(); ++j) {
            std::string cell = row[j];
            bool hasComma = cell.find(',') != std::string::npos;
            bool hasQuote = cell.find('"') != std::string::npos;
            if (hasQuote) {
                std::string escaped;
                for (std::size_t k = 0; k < cell.size(); ++k) {
                    escaped.push_back(cell[k]);
                    if (cell[k] == '"') {
                        escaped.push_back('"');
                    }
                }
                cell = escaped;
            }
            if (hasComma || hasQuote) {
                output << '"' << cell << '"';
            } else {
                output << cell;
            }
            if (j + 1 < row.size()) {
                output << ',';
            }
        }
        output << '\n';
    }

    return true;
}

std::vector<std::string> CSVParser::splitLine(const std::string& line) {
    std::vector<std::string> cells;
    std::string current;
    bool inQuotes = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                current.push_back('"');
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            cells.push_back(trim(current));
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    cells.push_back(trim(current));

    return cells;
}

std::string CSVParser::trim(const std::string& input) {
    std::size_t start = 0;
    while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start]))) {
        ++start;
    }

    std::size_t end = input.size();
    while (end > start && std::isspace(static_cast<unsigned char>(input[end - 1]))) {
        --end;
    }

    return input.substr(start, end - start);
}

bool CSVParser::toInt(const std::string& input, int& output) {
    char* endPtr = NULL;
    long value = std::strtol(input.c_str(), &endPtr, 10);
    if (endPtr == input.c_str() || *endPtr != '\0') {
        return false;
    }
    output = static_cast<int>(value);
    return true;
}

bool CSVParser::toDouble(const std::string& input, double& output) {
    char* endPtr = NULL;
    double value = std::strtod(input.c_str(), &endPtr);
    if (endPtr == input.c_str() || *endPtr != '\0') {
        return false;
    }
    output = value;
    return true;
}

std::string CSVParser::joinSet(const std::vector<std::string>& values, char delimiter) {
    std::ostringstream out;
    for (std::size_t i = 0; i < values.size(); ++i) {
        out << values[i];
        if (i + 1 < values.size()) {
            out << delimiter;
        }
    }
    return out.str();
}

std::vector<std::string> CSVParser::splitTokenList(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::string current;
    for (std::size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c == delimiter) {
            std::string t = trim(current);
            if (!t.empty()) {
                tokens.push_back(t);
            }
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    std::string t = trim(current);
    if (!t.empty()) {
        tokens.push_back(t);
    }
    return tokens;
}
