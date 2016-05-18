#ifndef __CHARSET_CONVERTER
#define __CHARSET_CONVERTER
//sys,c,cpp
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <cerrno>
#include <string>
//third_party
#include <iconv/iconv.h>

class CharsetConverter {

 public:
  CharsetConverter() : cd_((iconv_t)-1) {}
  CharsetConverter(const char* pszFromCode, const char* pszToCode) {
    init(pszFromCode, pszToCode);
    //assert( init(pszFromCode, pszToCode) );
  }
  ~CharsetConverter() {
    if (cd_ != (iconv_t)-1)
      iconv_close(cd_);
  }

  //UTF-8, UTF-16, UTF-32, GBK, GB18030, BIG5
  bool init(const char* pszFromCode, const char* pszToCode);
  bool convert(std::string& in_s, std::string& out_s);

 private:
  size_t convert(char* in_buf, size_t in_left, char* out_buf, size_t out_len);

 private:
  iconv_t cd_;
  const char* from_code_;
  const char* to_code_;
};

#endif

