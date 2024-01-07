#include "huffman-tree.cpp"
#include "constants.h"

map<char, string> findMapping(const tree* root) {
	map<char, string> result;
	if (root == nullptr) return result;
	if (root->value != "┐") {
		result.insert({root->value[0], ""}); 
	}
	auto left = findMapping(root->left);
	auto right = findMapping(root->right);
	for (auto itr = left.begin(); itr != left.end(); itr++) result.insert({itr->first, '0' + itr->second});
	for (auto itr = right.begin(); itr != right.end(); itr++) result.insert({itr->first, '1' + itr->second});
	return result;
}

map<string, char> findChar(const tree* root) {
	map<string, char> result;
	if (root == nullptr) return result;
	if (root->value != (string)"┐") result.insert({"", root->value[0]}); 
	auto left = findMapping(root->left);
	auto right = findMapping(root->right);
	for (auto itr = left.begin(); itr != left.end(); itr++) result.insert({'0' + itr->second, itr->first});
	for (auto itr = right.begin(); itr != right.end(); itr++) result.insert({'1' + itr->second, itr->first});
	return result;
}

// given a compressed binary string, convert it to the original string using the huffman tree
string inflate(const string &compressed, const tree *huffman_tree)
{ 
	string unzipped;
	int n = compressed.length();
	auto result = findChar(huffman_tree);
	string temp = "";

	for (int i = 0; i < n; i++) {
		temp += compressed[i];
		if (result.find(temp) != result.end()) {
			unzipped += result[temp];
			temp = "";
		}
	}
	return unzipped;
}

// given a string, compress it using the huffman tree and return the resulting binary string
string deflate(const string &buf, tree* &huffman_tree)
{
	// OK so tree is not really given.
	auto freq_map = get_freqs(buf);
	huffman_tree = huffman(freq_map);
	string zipped;
	int n = buf.length();
	auto result = findMapping(huffman_tree);

	for (int i = 0; i < n; i++) {
		zipped += result[buf[i]];
	}
	return zipped;
}
