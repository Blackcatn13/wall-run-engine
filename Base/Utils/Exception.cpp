
#include "Exception.h"
#include "Utils/BaseUtils.h"


CException::CException (const std::string &inFile, int inLine, const std::string &inMessage) {
  std::string line = "";
  baseUtils::FormatSrting(line, "%d", inLine);
  std::string description = "";
  description += "----------------------------------------------------\n";
  description += "File:" + inFile + "\n";
  description += "Line:" + line + "\n";
  description += "----------------------------------------------------\n";
  description += "Description:" + inMessage;
  SetDescription (description);
}

