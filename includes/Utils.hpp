#pragma once


#include <sys/stat.h>
#include <string>
/**
 * static methods
 */

class Utils
{
public:
    static bool	Utils::isDir(const std::string path);
    static bool	Utils::isFile(const std::string path);
};