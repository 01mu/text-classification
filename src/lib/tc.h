/*
* text-classification
* github.com/01mu
*/

#ifndef TC_H_
#define TC_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <map>

using namespace std;

struct Attribute
{
    string word;
    double frequency;
};

struct Review
{
    string text;
    vector<Attribute> word_occurances;
    int quality_class;
};

struct WordProb
{
    string word;
    double prob_pos;
    double prob_neg;
    int count;
};

bool comp_len_pos(const WordProb &, const WordProb &);
bool comp_len_neg(const WordProb &, const WordProb &);
vector<string> tokenize(const char *, char);

void get_text_from_file();
void get_tokenized_words();
void tokenize_text();
void determine_word_probs();
void determine_input_class();

void output_test_set(vector<WordProb> &);
void output_training_set(vector<WordProb> &);

void topHamTestHS(int, vector<WordProb> &);
void topSpamTestHS(int, vector<WordProb> &);
void ProbDiffPos(vector<WordProb> &);
void ProbDiffNeg(vector<WordProb> &);
void TopPos(vector<string> &, vector<string> &, vector<WordProb> &);
void TopNeg(vector<string> &, vector<string> &, vector<WordProb> &);
void InPosNotNeg(vector<string> &, vector<string> &);
void InNegNotPos(vector<string> &, vector<string> &);
void InNegAndPos(vector<string> &, vector<string> &);

extern double pos_prob;
extern double neg_prob;
extern double total;
extern double pos_word_total;
extern double neg_word_total;

extern double test_pos;
extern double test_neg;
extern double test_total;

extern vector<string> rev_txt;
extern vector<string> words;
extern map<string, string> words_map;
extern vector<Review> reviews;
extern vector<WordProb> word_probs;
extern vector<int> quality;

#endif
