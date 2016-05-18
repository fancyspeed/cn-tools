#include <fstream>
#include "charset_converter.h"
#include "charset_recognizer.h"
#include "cn_normalizer.h"

void test1() {
  CharsetConverter utf8_2_gbk("UTF-8", "GBK");
  CharsetConverter utf8_2_gb18030("UTF-8", "GB18030");

  printf("-----------test converter-------------\n");

  char buf[8192];
  std::ofstream out("out.gbk.txt");

  while (fgets(buf, 8191, stdin)) {
    std::string in_s(buf), out_s;
    utf8_2_gbk.convert(in_s, out_s);
    out << out_s << std::endl;
  }

  std::string a = "中";
  std::string b, c;
  utf8_2_gbk.convert(a, b);
  utf8_2_gb18030.convert(a, c);
  printf("utf8:%s, %d\n", a.c_str(), a.length());
  printf("gbk:%s, %d\n", b.c_str(), b.length());
  printf("gb18030:%s, %d\n", c.c_str(), c.length());

  a = "abc";
  utf8_2_gbk.convert(a, b);
  utf8_2_gb18030.convert(a, c);
  printf("utf8:%s, %d\n", a.c_str(), a.length());
  printf("gbk:%s, %d\n", b.c_str(), b.length());
  printf("gb18030:%s, %d\n", c.c_str(), c.length());

  a = "abc中国";
  utf8_2_gbk.convert(a, b);
  utf8_2_gb18030.convert(a, c);
  printf("utf8:%s, %d\n", a.c_str(), a.length());
  printf("gbk:%s, %d\n", b.c_str(), b.length());
  printf("gb18030:%s, %d\n", c.c_str(), c.length());

  printf("-----------prepare recognizer-------------\n");

  char* f = (char*)common_utf8.c_str();
  for (int i=0; i<common_utf8.length(); i++)
    if ((int)f[i] > 0 && i+1<common_utf8.length() && (int)f[i+1] > 0)
      printf("warning: %c%c\n", f[i], f[i+1]);

  printf("-----------test recognizer-------------\n");

  CharsetRecognizer recog;
  std::string code;
  std::string result;

  code = recog.guess_code(a);
  printf("%s, %s\n", code.c_str(), recog.guess_and_to_utf8(a).c_str());
  code = recog.guess_code(b);
  printf("%s, %s\n", code.c_str(), recog.guess_and_to_utf8(b).c_str());
  code = recog.guess_code(c);
  printf("%s, %s\n", code.c_str(), recog.guess_and_to_utf8(c).c_str());
  code = recog.guess_code("国");
  printf("%s, %s\n", code.c_str(), recog.guess_and_to_utf8("国").c_str());
  code = recog.guess_code("abc");
  printf("%s, %s\n", code.c_str(), recog.guess_and_to_utf8("abc").c_str());
  code = recog.guess_code("a");
  printf("%s, %s\n", code.c_str(), recog.guess_and_to_utf8("a").c_str());

  printf("-----------test normalizer-------------\n");

  CnNormalizer norm; 
  std::string s1 = "你好Ma H啊哈。独领風騷！　|";
  printf("origin : %s\n", s1.c_str());
  printf("simple : %s\n", norm.to_simp(s1).c_str());
  printf("banjiao: %s\n", norm.to_banjiao(s1).c_str());
  printf("lower  : %s\n", norm.to_lower(s1).c_str());
  printf("batch  : %s\n", norm.batch_convert(s1).c_str());

  s1 = "";
  printf("origin : %s\n", s1.c_str());
  printf("simple : %s\n", norm.to_simp(s1).c_str());
  printf("banjiao: %s\n", norm.to_banjiao(s1).c_str());
  printf("lower  : %s\n", norm.to_lower(s1).c_str());
  printf("batch  : %s\n", norm.batch_convert(s1).c_str());
}

int main(int argc,char **argv) {
  test1();
  return 0;
}

