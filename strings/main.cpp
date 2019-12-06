// String searching lab exercise.
// Edwyn Mckie
//failed at A* and ran away

#include <cassert>
#include <iostream>
#include <list>
#include <string>

#include "utils.h"

using std::cout;
using std::endl;
using std::list;
using std::string;

/** Return first position of pat in text, or -1 if not found. */
Position find_bruteforce(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	for (Position i = 0; i < text_len - pat_len; ++i) {
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i); 
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

/** Return first position of pat in text, or -1 if not found. */
Position find_skipping(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	bool in_pattern[256];
	for (int i = 0; i < 256; ++i)
	{
		in_pattern[i] = false;
	}
	
	for (char c : pat)
	{
		in_pattern[int(c)] = true;
	}

	for (Position i = 0; i < text_len - pat_len; ++i) {
		if (!in_pattern[int(text[i + pat_len - 1])])
		{
			i += pat_len - 1; //skip forwards
			continue;
		}
		
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		show_context(text, i);
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

int main(int argc, char *argv[]) {
	string text;//declare text as a string
	text = "University of Abertay Dundee, Bell Street, Dundee, Scotland";
	//load_jute_book(text); //call the load function and pass it the file .txt
	//load_file("my-file.txt", text);

	string pat = "Dundee"; //pat = pattern
	Position pos = find_bruteforce(pat, text);
	//Position pos = find_skipping(pat, text);
	cout << "Found '" << pat << "' at position " << pos << ":\n";
	show_context(text, pos);

	return 0;
}