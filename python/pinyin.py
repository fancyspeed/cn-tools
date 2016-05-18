#!/usr/bin/env python
#encoding: utf-8
import os
import sys
reload(sys)
sys.setdefaultencoding('utf8')


def load_pinyin_dict(pin):
  char_dict = {}
  for line in open(pin):
    line_u = line.strip().decode('utf-8')
    word = line_u[0]
    pinyin = line_u[1:].split(',')[0][:-1]
    char_dict[word] = pinyin
  return char_dict
pinyin_dict = load_pinyin_dict(os.path.join(os.path.dirname(__file__), 'conf/convert-utf-8.txt'))

def load_pinyins_dict(pin):
  char_dict = {}
  for line in open(pin):
    line_u = line.strip().decode('utf-8')
    word = line_u[0]
    pinyins = [v[:-1] for v in line_u[1:].split(',')]
    char_dict[word] = pinyins
  return char_dict
pinyins_dict = load_pinyins_dict(os.path.join(os.path.dirname(__file__), 'conf/convert-utf-8.txt'))


def str_to_pinyin(line, char_dict=pinyin_dict):
    line_u = line.decode('utf-8')
    new_line = ''
    for u in line_u:
        new_line += char_dict.get(u, u) 
    return new_line

def str_to_short_pinyin(line, char_dict=pinyin_dict):
    line_u = line.decode('utf-8')
    new_line = ''
    for u in line_u:
        new_line += char_dict.get(u, u)[0] 
    return new_line

def str_to_pinyins(line, char_dict=pinyins_dict):
    line_u = line.decode('utf-8')
    new_lines = [['','']] 
    for u in line_u:
        tmps = []
        for v in char_dict.get(u, [u]): 
            for tmp1, tmp2 in new_lines: 
                tmps.append( [tmp1+v, tmp2+v[0]] )
        new_lines = tmps
    return new_lines

def str_to_short_pinyins(line, char_dict=pinyins_dict):
    line_u = line.decode('utf-8')
    new_lines = [''] 
    for u in line_u:
        tmps = []
        for v in char_dict.get(u, [u]): 
            for tmp in new_lines: 
                tmps.append( tmp + v[0] )
        new_lines = tmps
    return new_lines


if __name__ == '__main__':
    chinese = 'python 之美'
    print str_to_pinyin(chinese, pinyin_dict)
    print str_to_short_pinyin(chinese, pinyin_dict)
    chinese = '重庆'
    print str_to_pinyin(chinese, pinyin_dict)
    print str_to_short_pinyin(chinese, pinyin_dict)
    print str_to_pinyins(chinese, pinyins_dict)
    print str_to_short_pinyins(chinese, pinyins_dict)

