#ifndef UTILITIES_H
#define UTILITIES_H

#include "Common.h"

class Model;

enum LogLevel
{
	LOG_INFO = 0,
	LOG_WARNING,
	LOG_ERROR,
};

namespace Utils
{
unsigned char* LoadPng(std::string filename, int& w, int& h, int& bpp);

void FreePng(unsigned char* img);

GLuint LoadTexture(std::string filename);

std::string GetBasename(const std::string& path);

std::string GetDirname(const std::string& path);

std::vector<Model*> LoadObj(std::string filename);

void Log(LogLevel flag, std::string infoStr);
}

#endif // UTILITIES_H
