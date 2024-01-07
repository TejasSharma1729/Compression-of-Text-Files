#include "rle.h"
#include <cassert>

int pow(int x) {
	if (x == 0) return 1;
	int Pow = pow(x/2);
	Pow *= Pow;
	if (x % 2 == 1) Pow *= 2;
	return Pow;
}

// Constructor
rle::rle(int gamma): gamma(gamma){
	maxSize = pow(gamma) - 1;
}

// Convert integer to gamma-bit binary string
string rle::f(int i)
{
	string converted(gamma, '1');
	if (i > maxSize) {
		converted += f(i - maxSize);
		return converted;
	}
	for (int j = 0; j < gamma; j++) {
		converted[j] = (char)((i / pow(gamma-j-1)) % 2 + 48);
	}
	return converted;
}

// Convert gamma-bit binary string to integer
int rle::g(string i)
{
	assert(i.length() == gamma);
	int G = 0;
	for (int k = 0; k < gamma; k++) {
		G *= 2;
		if (i[k] == '1') G += 1;
	}
	return G;
}

// Compress the input file
string rle::compress(string input_string)
{
	int n = 0;
	while (input_string[n] == '0' || input_string[n] == '1') n++;
	int currLength = 0;
	string compressed;
	if (input_string[0] == '0') compressed += '0'; else compressed += '1';

	for (int i = 0; i <= n; i++) {
		if (i == n || (i > 0 && input_string[i] != input_string[i-1])) {
			string temp = f(currLength);
			compressed += temp;
			currLength = 1;
		}
		else {
			currLength++;
		}
	}
	return compressed;
}

// Decompress the input file
string rle::decompress(string input_string)
{
	bool is1;
	char j = input_string[0];
	if (j == '1') is1 = 1; else is1 = 0;
	int n = input_string.length();
	assert ((n-1)% gamma == 0);
	string decompressed;

	for (int j = 0; j < (n-1)/gamma; j++) {
		int temp = 0;
		for (int k = 0; k < gamma; k++) temp = 2*temp + ((int)input_string[gamma*j+k+1] - 48);
		for (int k = 0; k < temp; k++) if (is1) decompressed += '1'; else decompressed += '0';
		if (is1) is1 = 0; else is1 = 1;
	}
	return decompressed;
}
