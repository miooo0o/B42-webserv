#include "Utils.hpp"


/// @brief Checks if a file exists.
/// @param path The path of the file.
bool	Utils::isFile(const std::string path) {
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISREG(buffer.st_mode))
		return (true);
	return (false);
}

/// @brief Checks if a file exists.
/// @param path The path of the file.
bool	Utils::isDir(const std::string path) {
	struct stat buffer;
	if (stat(path.c_str(), &buffer) != 0)
		return (false);
	if (S_ISDIR(buffer.st_mode))
		return (true);
	return (false);
}