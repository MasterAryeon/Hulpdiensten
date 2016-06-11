*	cmake gtest

*	make

*	g++ -I./gtest/include -L./ -g UnitTest.cpp stadsdelen.cpp tinyxml.cpp tinyxmlerror.cpp tinystr.cpp tinyxmlparser.cpp -std=c++11 -o main -lgtest -lgtest_main -lpthread

*	g++ Main.cpp stadsdelen.cpp tinyxml.cpp tinyxmlerror.cpp tinystr.cpp tinyxmlparser.cpp -std=c++11 -o main && ./main




cd /usr/src/gtest
sudo cmake .
sudo make
sudo mv libg* /usr/lib/
