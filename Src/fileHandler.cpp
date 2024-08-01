//
// Created by xin on 2024/7/26.
//

#include "fileHandler.h"

#include <iostream>


namespace fs = std::filesystem;
std::vector<std::string> fileHandler::getFilesInDirectory(const std::string& directoryPath)
{
    try {
        std::vector<std::string> matchedFiles;
        if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {

            for (const auto& entry : fs::directory_iterator(directoryPath)) {
                if (fs::is_regular_file(entry.status())) {
                    matchedFiles.emplace_back(entry.path().string());
                }
            }
            return matchedFiles;
        } else {
            std::cerr << "Directory does not exist or is not a directory: " << directoryPath << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    return {};
}
