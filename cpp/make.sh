#g++ test.cpp -g -D_DEBUG=1 -Wall -I../../../../zampout/extractor/thrid_party/dist/include/ ../../../../zampout/extractor/thrid_party/dist/lib/libiconv.a -o test_iconv
g++ test.cpp -g -D_DEBUG=1 -Wall -o test_iconv
./test_iconv < for_test.txt 
