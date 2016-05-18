#include "cn_normalizer.h"

extern int g_simp_len;
extern int g_trad_len;
extern unsigned short* g_simp_dict;
extern unsigned short* g_trad_dict;

CnNormalizer::CnNormalizer() {
  utf8_to_gbk_.init("UTF-8", "GBK");
  gbk_to_utf8_.init("GBK", "UTF-8");
  memset(trad_dict_gbk_, 0, sizeof(int)*65536);
  memset(simp_dict_gbk_, 0, sizeof(int)*65536);
  for (int i=0; i<g_simp_len && i<g_trad_len; i++) {
    trad_dict_gbk_[(int)*(g_trad_dict+i)] = (int)*(g_simp_dict+i);
    simp_dict_gbk_[(int)*(g_simp_dict+i)] = (int)*(g_trad_dict+i);
  }
}

CnNormalizer::~CnNormalizer() {
}

std::string CnNormalizer::batch_convert(std::string& str) {
  std::string str_lower = to_lower(str);
  std::string str_banjiao = to_banjiao(str_lower);
  std::string str_simp = to_simp(str_banjiao); 
  return str_simp;
}

std::string CnNormalizer::to_simp(std::string& str) {
  std::string gbk_str;
  utf8_to_gbk_.convert(str, gbk_str);
  char* buf = new char[gbk_str.length() + 2];
  memset(buf, 0, sizeof(char)*(gbk_str.length()+2));
  strcpy(buf, gbk_str.c_str());

  for (char* i=buf; *i != '\0'; i++) {
    if ((int)*i < 0 && *(i+1) != '\0') { //cn
      if ((int)*(unsigned short*)i < 65536 && (int)*(unsigned short*)i >= 0) { //varify
        if (trad_dict_gbk_[(int)*(unsigned short*)i] != 0)
          *(unsigned short*)i = (unsigned short) trad_dict_gbk_[(int)*(unsigned short*)i];
      }
      i++;
    }
  }
  std::string gbk_simp(buf);
  delete[] buf;
  std::string utf8_simp;
  gbk_to_utf8_.convert(gbk_simp, utf8_simp);
  return utf8_simp;
}

std::string CnNormalizer::to_banjiao(std::string& str) {
  std::string gbk_str;
  utf8_to_gbk_.convert(str, gbk_str);
  char* buf = new char[gbk_str.length() + 2];
  memset(buf, 0, sizeof(char)*(gbk_str.length()+2));

  int pbuf = 0; 
  for (char* i=(char*)gbk_str.c_str(); *i != '\0'; i++) {
    if ((int)*i > 0) { //ascii
      buf[pbuf++] = *i;
    } else if ((unsigned char)*i == (unsigned char)163) { //quanjiao
      if (*(i+1) != '\0') {
        buf[pbuf++] = (unsigned char)*(i+1) - 128;
        i++;
      }
    } else if ((unsigned char)*i == (unsigned char)161 && 
               (unsigned char)*(i+1) == (unsigned char)161) { //quanjiao blackspace
      buf[pbuf++] = ' ';
      i++;
    } else { //non-ascii
      if (*(i+1) != '\0') {
        buf[pbuf++] = *i;
        buf[pbuf++] = *(i+1);
        i++;
      }
    }
  }
  std::string gbk_simp(buf);
  delete[] buf;
  std::string utf8_simp;
  gbk_to_utf8_.convert(gbk_simp, utf8_simp);
  return utf8_simp;
}

std::string CnNormalizer::to_lower(std::string& str) {
  std::string gbk_str;
  utf8_to_gbk_.convert(str, gbk_str);
  char* buf = new char[gbk_str.length() + 2];
  memset(buf, 0, sizeof(char)*(gbk_str.length()+2));
  strcpy(buf, gbk_str.c_str());

  for (char* i=buf; *i != '\0'; i++) {
    if ((int)*i < 0) {
      if (*(i+1) != '\0')
        i++;
    } else if ((int)*i >= (int)'A' && (int)*i <= (int)'Z') {
      *i = (char)((int)*i + (int)'a' - (int)'A');
    }
  }
  std::string gbk_simp(buf);
  delete[] buf;
  std::string utf8_simp;
  gbk_to_utf8_.convert(gbk_simp, utf8_simp);
  return utf8_simp;
}

