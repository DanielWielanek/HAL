#ifndef __INCLUDE_LOG_H_
#define __INCLUDE_LOG_H_

#include "log.h"
#include <cstdlib>
#include <string>

using namespace std;
namespace Hal {
  namespace ScottPratt {
    class CLog {
    public:
      static bool INTERACTIVE;
      static string logfilename;
      static FILE* fptr;
      static void Init(char* logfilename_in);
      static void Init(string& logfilename_in);
      // static void Fatal(string &message);
      static void Info(string message);
      static void Fatal(string message);
      // static void Info(string &message);
      // static void Fatal(char *message);
      // static void Info(char *message);
    };
  }  // namespace ScottPratt
}  // namespace Hal

#endif
