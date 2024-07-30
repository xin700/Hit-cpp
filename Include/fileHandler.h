//
// Created by xin on 2024/7/26.
//

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <regex>
#include <string>
#include <filesystem>

class fileHandler
{
public:
    static  std::vector<std::string> getFilesInDirectory(const std::string &directoryPath);
};


#endif //FILEHANDLER_H
