CXXFLAGS = -std=c++17 -Wall -Werror -pedantic -O3
CFLAGS = -I/opt/homebrew/opt/openssl/include/openssl
LDFLAGS = -L/opt/homebrew/opt/openssl/lib
LIBS = -ldl -lssl -lcrypto

bf: main.cpp
	g++ $(CXXFLAGS) $(CFLAGS)  $^ $(LDFLAGS) $(LIBS) -o $@ 

clean:
	rm -rf bf
