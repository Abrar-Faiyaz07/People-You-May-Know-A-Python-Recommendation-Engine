#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <vector>

class CSVParser {
public:
    static std::vector<std::vector<std::string> > readRows(const std::string& path, bool skipHeader);
    static bool writeRows(const std::string& path, const std::vector<std::string>& header,
                          const std::vector<std::vector<std::string> >& rows);

    static std::vector<std::string> splitLine(const std::string& line);
    static std::string trim(const std::string& input);
    static bool toInt(const std::string& input, int& output);
    static bool toDouble(const std::string& input, double& output);
    static std::string joinSet(const std::vector<std::string>& values, char delimiter);
    static std::vector<std::string> splitTokenList(const std::string& input, char delimiter);
};

#endif
