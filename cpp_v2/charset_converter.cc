#include "charset_converter.h"

bool CharsetConverter::init(const char* pszFromCode, const char* pszToCode) {
  if (pszFromCode == NULL || pszToCode == NULL)
    return false;
  from_code_ = pszFromCode;
  to_code_ = pszToCode;
  cd_ = iconv_open(to_code_, from_code_);
  if (cd_ == (iconv_t)-1)
    return false;
  return true;
}

bool CharsetConverter::convert(std::string& in_s, std::string& out_s) {
  char* pin = (char*)in_s.c_str();
  size_t inBytesLeft = in_s.size();
  size_t outBytesLen = inBytesLeft * 4 + 256;
  char* pout = new char[outBytesLen];

  size_t nSequenceLen = convert(pin, inBytesLeft, pout, outBytesLen);

  if ((signed int)nSequenceLen <= 0 || 
      (signed int)nSequenceLen >= (signed int)outBytesLen) {
    out_s = "";
    delete[] pout;
    return false;
  } else {
    pout[nSequenceLen] = '\0';
    out_s = std::string(pout);
    delete[] pout;
    return true;
  }
}

size_t CharsetConverter::convert(char* in_buf, size_t in_left, char* out_buf, size_t out_len) {
  if (cd_ == (iconv_t)-1)
    return (size_t)-1;

  size_t outBytesLeft = out_len;
  while ((int)in_left > 0 && *in_buf != '\0') {
    size_t ret = iconv(cd_, &in_buf, &in_left, &out_buf, &outBytesLeft);
    if (ret == (size_t)0) 
      break;
    if (ret == (size_t)-1 && errno == E2BIG)
      return (size_t)-1;
    in_buf ++;
    in_left --;
  }
  int nRealLen = out_len - outBytesLeft;
  return (size_t)nRealLen;
}

