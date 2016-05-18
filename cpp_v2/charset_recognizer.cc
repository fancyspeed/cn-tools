#include "charset_recognizer.h"

extern int g_common_len;
extern unsigned short* g_common_dict;
extern int g_freq_len;
extern unsigned short* g_freq_dict;

CharsetRecognizer::CharsetRecognizer() {
  gbk_to_utf8_.init("GBK", "UTF-8");
  gb18030_to_utf8_.init("GB18030", "UTF-8");
  big5_to_utf8_.init("BIG5", "UTF-8");
  utf8_to_gbk_.init("UTF-8", "GBK");
  utf8_to_gb18030_.init("UTF-8", "GB18030");
  utf8_to_big5_.init("UTF-8", "BIG5");
  //utf16_to_utf8_.init("UTF-16", "UTF-8");
  //utf32_to_utf8_.init("UTF-32", "UTF-8");
  //utf8_to_utf16_.init("UTF-8", "UTF-16");
  //utf8_to_utf32_.init("UTF-8", "UTF-32");

  memset(common_gbk_, 0, sizeof(int)*65536);
  unsigned short* p = g_common_dict;
  for (int i=0; i<g_common_len; i++)
    if ((int)*(p+i) < 65536 && (int)*(p+i) >= 0)
      common_gbk_[(int)*(p+i)] = 1;
  memset(most_freq_gbk_, 0, sizeof(int)*65536);
  p = g_freq_dict; 
  for (int i=0; i<g_freq_len; i++)
    if ((int)*(p+i) < 65536 && (int)*(p+i) >= 0)
      most_freq_gbk_[(int)*(p+i)] = 1;
}

CharsetRecognizer::~CharsetRecognizer() {
}

std::string CharsetRecognizer::guess_and_to_utf8(std::string& str) {
  if (str.length() == 0)
    return "";
  std::string code_type = guess_code(str);
  std::string tmp1;
  if (code_type.compare("gbk") == 0)
    gbk_to_utf8_.convert(str, tmp1);
  else if (code_type.compare("gb18030") == 0)
    gb18030_to_utf8_.convert(str, tmp1);
  else if (code_type.compare("big5") == 0)
    big5_to_utf8_.convert(str, tmp1);
  //else if (code_type.compare("utf16") == 0)
  //  utf16_to_utf8_.convert(str, tmp1);
  //else if (code_type.compare("utf32") == 0)
  //  utf32_to_utf8_.convert(str, tmp1);
  else //utf8
    tmp1 = str;
  return tmp1;
}

std::string CharsetRecognizer::guess_code(std::string& str) {
  int best_score = 0, tmp_score;
  std::string best_code = "";
  tmp_score = check_code(str, "gbk");
  if (tmp_score > best_score) 
    best_score = tmp_score, best_code = "gbk"; 
  tmp_score = check_code(str, "utf8");
  if (tmp_score > best_score) 
    best_score = tmp_score, best_code = "utf8"; 
  tmp_score = check_code(str, "gb18030");
  if (tmp_score > best_score) 
    best_score = tmp_score, best_code = "gb18030"; 
  tmp_score = check_code(str, "big5");
  if (tmp_score > best_score) 
    best_score = tmp_score, best_code = "big5"; 
  //tmp_score = check_code(str, "utf16");
  //if (tmp_score > best_score) 
  //  best_score = tmp_score, best_code = "utf16"; 
  //tmp_score = check_code(str, "utf32");
  //if (tmp_score > best_score) 
  //  best_score = tmp_score, best_code = "utf32"; 
  return best_code;
}

//try to convert and count the number of chars
int CharsetRecognizer::check_code(std::string& str, std::string code_type) {
  std::string tmp1, tmp2, tmp3; //convert_to_mid, mid_to_origin, mid_to_gbk
  if (code_type.compare("gbk") == 0) {
    gbk_to_utf8_.convert(str, tmp1);
    utf8_to_gbk_.convert(tmp1, tmp2);
    tmp3 = tmp2;
  } else if (code_type.compare("utf8") == 0) {
    utf8_to_gbk_.convert(str, tmp1);
    gbk_to_utf8_.convert(tmp1, tmp2);
    tmp3 = tmp1;
  } else if (code_type.compare("gb18030") == 0) {
    gb18030_to_utf8_.convert(str, tmp1);
    utf8_to_gb18030_.convert(tmp1, tmp2);
    utf8_to_gbk_.convert(tmp1, tmp3);
  } else if (code_type.compare("big5") == 0) {
    big5_to_utf8_.convert(str, tmp1);
    utf8_to_big5_.convert(tmp1, tmp2);
    utf8_to_gbk_.convert(tmp1, tmp3);
  //} else if (code_type.compare("utf16") == 0) {
  //  utf16_to_utf8_.convert(str, tmp1);
  //  utf8_to_utf16_.convert(tmp1, tmp2);
  //  utf8_to_gbk_.convert(tmp1, tmp3);
  //} else if (code_type.compare("utf32") == 0) {
  //  utf32_to_utf8_.convert(str, tmp1);
  //  utf8_to_utf32_.convert(tmp1, tmp2);
  //  utf8_to_gbk_.convert(tmp1, tmp3);
  } else {
    return 0;
  }

  int c1 = 0, c2 = 0; //num_common_cn, num_most_freq_gbk
  char* p = (char*) tmp3.c_str(); 
  //for (unsigned short* i=p; *i!=(unsigned short)0; i++) {
  for (char* i=p; *i != '\0'; i++) {
    if ((int)*i < 0 && *(i+1) != '\0') { //cn 
      if ((int)*(unsigned short*)i < 65536 && (int)*(unsigned short*)i >= 0) { //varify
        if (common_gbk_[(int)*(unsigned short*)i] == 1)
          c1 += 1;
        if (most_freq_gbk_[(int)*(unsigned short*)i] == 1)
          c2 += 1;
      }
      i++;
    }
  }
  return c1 + c2 + (int)(tmp2.length());
}

