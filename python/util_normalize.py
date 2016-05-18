#!/usr/bin/python
# coding: utf-8
# @author: zuotaoliu@126.com
# @created: 2014-01-09
import os
import sys
cur_path = os.path.dirname(__file__)
SIMP_DICT_PATH = os.path.join(cur_path, './conf/simp_words')
TRAD_DICT_PATH = os.path.join(cur_path, './conf/trad_words')

class NormalizeUtil(object):

    def __init__(self, p_simp=SIMP_DICT_PATH, p_trad=TRAD_DICT_PATH):
        self.simp2trad = {}
        self.trad2simp = {}

        simp_line = open(p_simp).readline().strip().decode('utf-8')
        trad_line = open(p_trad).readline().strip().decode('utf-8')
        if len(simp_line) == len(trad_line):
            for i in range(len(simp_line)):
                s, t = simp_line[i], trad_line[i]
                self.simp2trad[s] = t
                self.trad2simp[t] = s

    def all_normalize(self, line):
        new_line = self.to_simp(line)
        new_line2 = self.to_banjiao(new_line)
        return new_line2.lower()

    def to_simp(self, line):
        list_u = list(line.decode('utf-8', 'ignore'))
        for i, uchar in enumerate(list_u): 
            if uchar in self.trad2simp:
                list_u[i] = self.trad2simp[uchar]
        new_line = ''.join(list_u)
        return new_line.encode('utf-8')

    def to_banjiao(self, line):
        list_u = list(line.decode('utf-8', 'ignore'))
        for i, uchar in enumerate(list_u):
            inside_code = ord(uchar)
            if inside_code == 0x3000:
                inside_code = 0x0020
            elif inside_code == 0x00a0:
                inside_code = 0x0020
            else:
                inside_code -= 0xfee0
            if inside_code >= 0x0020 and inside_code <= 0x007e:
                list_u[i] = unichr(inside_code)
        new_line = ''.join(list_u)
        return new_line.encode('utf-8')


def test_normalize():
    norm = NormalizeUtil()
    line = '你好Ma H啊哈'
    print line, '==>>'
    print norm.all_normalize(line) 
    line = '你好Ma H啊哈, 独领風騷'
    print line, '==>>'
    print norm.all_normalize(line) 
    line = '台大校讯每週三出刊'
    print line, '==>>'
    print norm.all_normalize(line) 
    line = '你好Ma H啊哈。独领風騷！　'
    print line, '==>>'
    print norm.all_normalize(line) 
    line = '　|；|，|？|……|！|：'
    print line, '==>>'
    print norm.all_normalize(line) 
    line = ' old navy'
    print line, '==>>'
    print norm.all_normalize(line) 


if __name__ == '__main__':
    import sys
    test_normalize() 

