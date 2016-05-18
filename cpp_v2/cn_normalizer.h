#ifndef __CN_NORMALIZE
#define __CN_NORMALIZE
//sys,c,cpp
#include <string>
//other
#include "charset_converter.h"

class CnNormalizer {
 
 public:
  CnNormalizer();
  ~CnNormalizer();

  //input should be utf8
  std::string batch_convert(std::string& str);
  std::string to_simp(std::string& str);
  std::string to_banjiao(std::string& str);
  std::string to_lower(std::string& str);

 private:
  CharsetConverter utf8_to_gbk_;
  CharsetConverter gbk_to_utf8_;
  int trad_dict_gbk_[65536];
  int simp_dict_gbk_[65536];
};

#endif

