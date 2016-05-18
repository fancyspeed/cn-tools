#ifndef __CHARSET_RECOGNIZER
#define __CHARSET_RECOGNIZER
//sys,c,cpp
//#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
//other
#include "charset_converter.h"

class CharsetRecognizer {

 public:
  CharsetRecognizer();
  ~CharsetRecognizer();

  std::string guess_and_to_utf8(std::string& str);
  //utf16, utf32, utf8, utf16, gbk, gb18030, big5 
  std::string guess_code(std::string& str);
  //utf16, utf32, utf8, utf16, gbk, gb18030, big5 
  int check_code(std::string& str, std::string code_type);

 private:
  CharsetConverter gbk_to_utf8_;
  CharsetConverter gb18030_to_utf8_;
  CharsetConverter big5_to_utf8_;
  CharsetConverter utf8_to_gbk_;
  CharsetConverter utf8_to_gb18030_;
  CharsetConverter utf8_to_big5_;
  //CharsetConverter utf16_to_utf8_;
  //CharsetConverter utf32_to_utf8_;
  //CharsetConverter utf8_to_utf16_;
  //CharsetConverter utf8_to_utf32_;

  int most_freq_gbk_[65536];
  int common_gbk_[65536];
};

#endif

