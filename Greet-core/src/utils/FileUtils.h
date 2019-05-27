#pragma once

#include <stdio.h>  /* defines FILENAME_MAX */
#include <sys/stat.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define stat _stat
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include <string>
#include <cstring>
#include <logging/Log.h>
namespace Greet { 

  struct TimeModified
  {
    timespec time;
    TimeModified(const timespec& time)
      : time{time}{}
    TimeModified()
    {}

    bool operator<(const TimeModified& rhs)
    {
      if (time.tv_sec == rhs.time.tv_sec)
        return time.tv_nsec < rhs.time.tv_nsec;
      else
        return time.tv_sec < rhs.time.tv_sec;
    }
  };

  namespace FileUtils {

    inline void print_working_directory()
    {
      char cCurrentPath[FILENAME_MAX];

      if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))){
        Log::Error("Cannot print working directory");
        return;
      }
      Log::Info(cCurrentPath);
    }

    inline TimeModified GetTimeModified(const std::string& filename)
    {
      struct stat attrib;
      stat(filename.c_str(), &attrib);
      return TimeModified{attrib.st_mtim};
    }

    inline std::string read_file(const char* filepath)
    {
      FILE *file = fopen(filepath, "rt");
      if (!file)
      {
        Log::Error("File could not be read: ", filepath);
        return "";
      }
      fseek(file, 0, SEEK_END);
      unsigned long length = ftell(file);
      char* data = new char[length + 1];
      memset(data, 0, length + 1);
      fseek(file, 0, SEEK_SET);
      fread(data, 1, length, file);
      fclose(file);
      std::string result(data);
      delete[] data;
      return result;
    }

    inline void write_file(const char* filepath, const std::string& write)
    {
      FILE *file = fopen(filepath,"wt");
      fseek(file, 0, SEEK_END);
      fputs(write.c_str(),file);
      fclose(file);
    }

    inline bool file_exists(const char* filepath)
    {
      if (FILE *file = fopen(filepath, "r"))
      {
        fclose(file);
        return true;
      }
      else
      {
        return false;
      }
    }
  }}
