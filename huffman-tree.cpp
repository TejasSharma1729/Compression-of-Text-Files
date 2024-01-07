#ifndef HUFFMAN
#define HUFFMAN

#include <string>
#include <map>
#include <iterator>
#include <set>
#include <iostream>
using std::cout, std::endl, std::string;
using std::map, std::set, std::pair, std::make_pair, std::multimap;

// a node of the huffman tree. Already written, observe what is going on.
class tree{
public:
    string value = "┐"; // ┐/0/1/character. for now we just use just ┐.
    tree *left = nullptr, *right = nullptr;

    tree(){}
    tree(char value): value(1, value){}
    tree(tree *left): left(left){}
    tree(tree *left, tree *right): left(left), right(right){}
    
    static tree *merge_trees(tree *left, tree *right){
        return new tree(left, right);
    }

    void print(const string &prefix="", bool isLeftChild=false){
        cout << prefix << (prefix==""?"└──":(isLeftChild? "├─0":"└─1"));
        // cout << "value:" << value << "|" << endl;
        if (isLeaf()) cout << "─[" << value << "]" << endl; else cout << value << endl;
        if (left) left->print(prefix + (isLeftChild?"│  ":"   "), true);
        if (right) right->print(prefix + (isLeftChild?"│  ":"   "), false);
    }
    bool isLeaf() const{ return !(left && right); }
    void get_codes(map<char, string> &map_in_progress, const string &prefix="") const{
        if (left){
            if (left->isLeaf()) map_in_progress[left->value[0]] = prefix + "0";
            else left->get_codes(map_in_progress, prefix + "0");
        }
        if (right){ 
            if (right->isLeaf()) map_in_progress[right->value[0]] = prefix + "1";
            else right->get_codes(map_in_progress, prefix + "1");
        }
    }
    ~tree() {
        delete left; delete right;
    }
};

// this returns the huffman tree using passed map of character to frequency
tree *huffman(const map<char, double> &freq_map)
{
	multimap<double, tree*> removalMin;
	for (auto itr = freq_map.begin(); itr != freq_map.end(); itr++) {
		tree* temp = new tree(itr->first);
		removalMin.insert({itr->second, temp});
	}
	while (removalMin.size() > 1) {
		auto left = removalMin.begin();
		int leftKey = left->first;
		tree* leftTree = left->second;
		removalMin.erase(removalMin.begin());

		auto right = removalMin.begin();
		int rightKey = right->first;
		tree* rightTree = right->second;
		removalMin.erase(removalMin.begin());

		tree* temp = new tree (leftTree, rightTree);
		removalMin.insert({leftKey + rightKey, temp});
	}
	return removalMin.begin()->second;
}

// compute the frequencies of characters in the string. For part 3, make sure to skip over backreferences and also set highest priority to the special character.
map<char, double> get_freqs(const string &buf) 
{ 
	map<char, double> freq_map;
	int n = buf.length();
	for (int i = 0; i < n; i++) {
		if (freq_map.find(buf[i]) != freq_map.end()) freq_map[buf[i]] += 1;
		else freq_map.insert({buf[i], 1});
	}
	return freq_map;
}

#endif
