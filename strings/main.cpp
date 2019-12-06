// String searching lab exercise.
// Edwyn Mckie
//failed at A* and ran away

#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "utils.h"

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::vector;

#define d 256 //number of characters in the english alphabet

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

/** skip ahead by the length of the string if we do not match. */
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

/** skip ahead by the length of the string if we do not match. */
Position find_bm(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	int skip[256];
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len; // Not in the pattern.
	}
	for (int i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}


	for (Position i = 0; i < text_len - pat_len; ++i) {
		int s = skip[int(text[i + pat_len - 1])]; 
		if (s != 0) 
		{
			i += s - 1; // Skip forwards.
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

/** skip ahead by the length of the string if we do not match. */
void find_bm_multiple(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();
	vector<Position> Results;

	int skip[256];
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len; // Not in the pattern.
	}
	for (int i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}


	for (Position i = 0; i < text_len - pat_len; ++i) {
		int s = skip[int(text[i + pat_len - 1])];
		if (s != 0)
		{
			i += s - 1; // Skip forwards.
			continue;
		}
		Position j;
		//show position we're currently at
		//feed it the text we want,and the current position
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			 // Matched here add to the vector
			Results.push_back(i);
			//print results to the screen
			cout << "Match found: " << Results[Results.size() - 1] << endl;
		}
	}
}

void Rabin_Karp(const string& pat, const string& text, int PrimeNumber) 
{
	int PatternLength = pat.size();
	int TextLength = text.size();

	int j; //counter
	int p = 0; //pattern hash
	int t = 0; //text hash
	int h = 1; // hash
	
	for (int i = 0; i < PatternLength - 1; i++)
	
	{
		h = (h * d) % PrimeNumber;
	}

	for (int i = 0; i < PatternLength; i++)
	{
		p = (d * p + pat[i]) % PrimeNumber;
		t = (d * t + pat[i]) % PrimeNumber;
	}

	for (int i = 0; i <= TextLength - PatternLength; i++)
	{
		if (p == t)
		{
			for (j = 0; j < PatternLength; j++)
			{
				if (text[i + j] != pat[j])
				{
					break;
				}
			}
			if (j == PatternLength)
			{
				cout << "Pattern found at index: " << i << endl;
			}
		}
		if (i < TextLength - PatternLength)
		{
			t = (d * (t - text[i] * h) + text[i + PatternLength]) % PrimeNumber;
			if (t < 0)
			{
				t = t + PrimeNumber;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	string text;//declare text as a string
	text = "University of Abertay Dundee, Bell Street, Dundee, Scotland";

	load_jute_book(text); //call the load function and pass it the file .txt
	//load_file("my-file.txt", text);

	string pat = "Dundee"; //pat = pattern


	//Position pos = find_bruteforce(pat, text);
	//Position pos = find_skipping(pat, text);
	//Position pos = find_bm(pat, text);
	//find_bm_multiple(pat, text);
	Rabin_Karp(pat, text,2);
	//cout << "Found '" << pat << "' at position " << pos << ":\n";
	//show_context(text, pos);

	return 0;
}