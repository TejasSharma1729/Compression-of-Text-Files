#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include "constants.h"
using std::string, std::bitset;
typedef unsigned long ulg;

int max(int a, int b) {
	if (a > b) return a;
	else return b;
}

int min(int a, int b) {
	if (a > b) return b;
	else return a;
}

#include <cmath>
void kmpTable(const string& P, int m, int* H) {
	int i = 1;
	int j = 0;
	H[0] = -1;
	while (i < m) {
		if (P[i] == P[j]) H[i] = H[j];
		else {
			H[i] = j;
			while (j >= 0 && P[i] != P[j]) j = H[j];
		}
		i++; j++;
	}
	H[m] = j;
}

// a struct for the backreference. A casting operator to string that can used (it is not optimal) as encoding is provided for convenience.
struct backref{
    ulg offset, length;
    backref(const ulg &offset, const ulg &length): offset(offset), length(length){}
    operator string() const{
        return encode_backref();
    }
    string encode_backref() const{
        return string(1, -1) + bitset<15>(offset).to_string() + string(1, -1) + bitset<8>(length-3).to_string() + string(1, -1);
    }
};

// find the longest match for buf[i:i+lookahead_size] as a substring in the window buf[max(i-window_size,0):] and return appropriate backreference. Note that the longest match can exceed buf[i], that is, for buf=aaaa the longest match at i = 1 returns (0, 3).
backref find_longest_match(const string &buf, ulg i, ulg window_size, ulg lookahead_size)
{
	int st = max(0, i-window_size);
	string P (lookahead_size, ' ');
	for (int j = 0; j < lookahead_size; j++) P[j] = buf[i+j];
	
	int j = 0;
	int* H = new int [lookahead_size + 1];
	kmpTable(P, lookahead_size, H);
	backref A (0, 0);

	while (st < i) {
		if (buf[st] == P[j]) {
			j++; st++;
			if (j > A.length) {
				A = backref(st-j, j);
			}
			if (j == lookahead_size) {
				return A;
			}
		}
		else {
			while (j >= 0 && P[j] != buf[st]) j = H[j];
			j++; st++;
		}
	}
	if (A.length > 2) return A; 
	else return backref(-1, 0);
}

// compress the string using lz77 and return the compressed binary string containing characters and string encodings of backreferences.
string deflate_lz77(const string &infile_str)
{
	string compressed;
	int n = infile_str.length();
	int lookahead_size = 216;

	for (int i = 0; i < n; i++) {
		backref A = find_longest_match(infile_str, i, 216, min(216, n-i));
		if (A.length < 3) {
			compressed += infile_str[i];
		}
		else {
			i += A.length - 1;
			compressed += A.encode_backref();
		}
	}
	return compressed;
}

// decompress the string (contains string encodings of backreferences, etc) using lz77 and return the original string.
string inflate_lz77(const string &compressed)
{
	string inflated;
	int n = compressed.length();
	for (int i = 0; i < n; i++) 
	{
		if ((int)compressed[i] == -1) {
			int offset = 0;
			for (int j = i+1; j < i+16; j++) {
				offset *= 2;
				if (compressed[j] == '1') offset += 1;
			}
			int chunk = 0;
			for (int j = i+17; j < i+25; j++) {
				chunk *= 2;
				if (compressed[j] == '1') chunk += 1;
			}
			chunk += 3;
			for (int j = offset; j < offset+chunk; j++) {
				inflated += inflated[j];
			}
			i += 25;
		}
		else {
			inflated += compressed[i];
		}
	}
	return inflated;
}















