#!/usr/bin/python
# coding: utf-8
# @author: zuotaoliu@126.com
# @created: 2014-01-09
import os
import sys
import re
cur_path = os.path.dirname(__file__)
cn_freq_file = os.path.join(cur_path, './conf/freq_character')


class CharsetDetect(object):
    p_header_charset = re.compile(r'charset\s*=\s*([\w\-]{2,})')
    p_html_charset = re.compile(r'charset\s*=\s*([\w\-]{2,})')
    p_html_charset2 = re.compile(r'charset\s*=\s*\"([\w\-]{2,})\"')
    
    def __init__(self, p_freq=cn_freq_file):
        self.most_freq_cn = open(p_freq).read().strip().decode('utf-8')
        self.all_candidates = ['utf-8', 'gbk', 'big5', 'gb18030']
        self.freq_word_map = {}
        for charset in self.all_candidates:
            if charset != 'big5':
                self.freq_word_map[charset] = {}
                for v in self.most_freq_cn:
                    v2 = v.encode(charset)
                    self.freq_word_map[charset][v2] = 1
            else:
                self.freq_word_map[charset] = {} 

    
    def get_header_coding(self, raw_html, content_type):
        match = self.p_header_charset.search(content_type)
        if match:
            coding = match.groups()[0] 
            if coding and self.check_coding(raw_html, coding):
                return coding
        return ''

    def get_meta_coding(self, raw_html):
        body_idx = raw_html.find('<body')
        first_part = raw_html[:max(0, body_idx)]
        match = self.p_html_charset.search(first_part)
        if not match:
            match = self.p_html_charset2.search(first_part)
        if match:
            coding = match.groups()[0].lower()
            if coding and self.check_coding(raw_html, coding):
                return coding
        return ''
        
    def check_coding(self, raw_html, coding):
        try:
            html2 = raw_html.decode(coding, 'ignore')
            html3 = html2.encode(coding)
            if len(html3) > len(raw_html) * 0.95:
                return True
        except:
            pass
        return False

    def guess_coding(self, raw_html):
        high_props = []
        low_props = []
        for coding in self.all_candidates: 
            try:
                html2 = raw_html.decode(coding, 'ignore')
                html3 = html2.encode(coding)
                if len(html3) == len(raw_html):
                    high_props.append( (coding, html2) ) 
                if len(html3) > len(raw_html) * 0.95:
                    low_props.append( (coding, len(html3)) )
            except:
                pass
        if len(high_props) == 1:
            return high_props[0][0]
        elif len(high_props) > 1:
            high_props2 = []
            for coding, u_str in high_props:
                num = 0
                for v in u_str: 
                    if v.encode(coding) in self.freq_word_map[coding]:
                        num += 1
                high_props2.append( (coding, num) )
            sort_list = sorted(high_props2, key=lambda d:-d[1])
            return sort_list[0][0]
        elif len(low_props) >= 1:
            sort_list = sorted(low_props, key=lambda d:-d[1])
            return sort_list[0][0]
        return ''

    def to_utf8(self, raw_str):
        if not raw_str:
            return raw_str
        if type(raw_str) == type(u'abc'):
            #unicode
            new_str = raw_str.encode('utf-8', 'ignore')
            coding = self.guess_coding(new_str)
            return new_str.decode(coding, 'ignore').encode('utf-8', 'ignore')
        coding = self.guess_coding(raw_str)
        return raw_str.decode(coding, 'ignore').encode('utf-8', 'ignore')


def test_encode():
    util = CharsetDetect()
    word_list = ['北京', '北京一夜', '上海']

    print 'guess_coding'.center(40, '*')
    for word in word_list:
        coding = util.guess_coding(word)
        print word, coding, word.decode(coding)

    word_list = ['北京', '北京一夜', '上海', u'北京', u'北京一夜']
    for word in word_list:
        print type(word), util.to_utf8(word), type(util.to_utf8(word))


if __name__ == '__main__':
    import sys
    test_encode()

