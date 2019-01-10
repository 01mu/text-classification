/*
* text-classification
* github.com/01mu
*/

#include "tc.h"

double pos_prob = 0;
double neg_prob = 0;
double total = 0;
double pos_word_total = 0;
double neg_word_total = 0;
double test_pos = 0;
double test_neg = 0;
double test_total = 0;

vector<string> words;
map<string, string> words_map;
vector<Review> reviews;
vector<WordProb> word_probs;
vector<string> rev_txt;
vector<int> quality;

vector<string> tokenize(const char * str, char c = ' ')
{
    vector<string> result;

    do
    {
        const char * begin = str;

        while(* str != c && * str)
        {
            str++;
        }

        result.push_back(string(begin, str));
    }
    while (0 != * str++);

    return result;
}

void get_text_from_file()
{
    cout << endl << "Getting input from 'hamspam'";

    string buffer;
    ifstream file("input/hamspam");

    while(file && getline(file, buffer))
    {
        if (buffer.length() == 0)
        {
            continue;
        }

        if(buffer.substr(0, 1) == "s")
        {
            rev_txt.push_back(buffer.substr(5, buffer.size()));
            quality.push_back(1);
        }
        else
        {
            rev_txt.push_back(buffer.substr(4, buffer.size()));
            quality.push_back(0);
        }
    }
}

void get_tokenized_words()
{
    vector<string> split_word;
    string word;

    cout << endl << "Getting individual words from 'hamspam'";

    for(int i = 0; i < rev_txt.size(); i++)
    {
        split_word = tokenize(rev_txt.at(i).c_str());

        for(int j = 0; j < split_word.size(); j++)
        {
            word = split_word.at(j);
            pair<string, string> pr(word, word);

            if(get<1>(words_map.insert(pr)) != false)
            {
                words.push_back(word);
            }
        }
    }
}

void tokenize_text()
{
    int cnt;

    vector<string> split_word;
    string word;

    cout << endl << "Tokenizing and assigning text and " <<
        "classification inputs to training set vector";

    for(int i = 0; i < rev_txt.size(); i++)
    {
        Attribute attribute;
        Review review;

        vector<Attribute> word_occurances;

        split_word = tokenize(rev_txt.at(i).c_str());
        map<string, string> checked;

        for(int j = 0; j < split_word.size(); j++)
        {
            word = split_word.at(j);
            pair<string, string> pr(word, word);

            if(get<1>(checked.insert(pr)) != false)
            {
                cnt = count(split_word.begin(), split_word.end(), word);

                attribute.word = word;
                attribute.frequency = cnt;

                word_occurances.push_back(attribute);
            }
        }

        review.text = rev_txt.at(i);
        review.word_occurances = word_occurances;
        review.quality_class = quality.at(i);

        for(int l = 0; l < review.word_occurances.size(); l++)
        {
            if(review.quality_class == 0)
            {
                neg_word_total += review.word_occurances.at(l).frequency;
            }
            else
            {
                pos_word_total += review.word_occurances.at(l).frequency;
            }
        }

        if(review.quality_class == 0)
        {
            neg_prob++;
        }
        else
        {
            pos_prob++;
        }

        total++;

        reviews.push_back(review);
    }

    neg_prob = neg_prob / total;
    pos_prob = pos_prob / total;
}

void determine_word_probs()
{
    double pos_word_freq = 0;
    double neg_word_freq = 0;

    cout << endl << "Determining probabilities for each word in training set";

    for(int i = 0; i < words.size(); i++)
    {
        WordProb wp;

        pos_word_freq = 0;
        neg_word_freq = 0;

        for(int l = 0; l < reviews.size(); l++)
        {
            for(int k = 0; k < reviews.at(l).word_occurances.size(); k++)
            {
                if(reviews.at(l).word_occurances.at(k).word == words.at(i))
                {
                    if(reviews.at(l).quality_class == 1)
                    {
                        pos_word_freq += reviews.at(l)
                            .word_occurances.at(k).frequency;
                    }
                    else
                    {
                        neg_word_freq += reviews.at(l)
                            .word_occurances.at(k).frequency;
                    }
                }
            }
        }

        wp.word = words.at(i);
        wp.prob_pos = (pos_word_freq + 1) / (pos_word_total + words.size());
        wp.prob_neg = (neg_word_freq + 1) / (neg_word_total + words.size());
        wp.count = 0;

        word_probs.push_back(wp);
    }

    output_training_set(word_probs);
}

void determine_input_class()
{
    cout << endl << "Determining whether each input in 'spam' is spam or ham";

    string buffer;
    ifstream file("input/spam");

    map<string, string> checked_words;
    vector<WordProb> checked_probs;

    double new_token_pos;
    double new_token_neg;

    string word;

    while(file && getline(file, buffer))
    {
        if(buffer.length() == 0)
        {
            continue;
        }

        vector<string> new_token = tokenize(buffer.c_str());

        new_token_pos = pos_prob;
        new_token_neg = neg_prob;

        for(int i = 0; i < new_token.size(); i++)
        {
            word = new_token.at(i);

            pair<string, string> pr(word, word);

            if(get<1>(checked_words.insert(pr)) != false)
            {
                WordProb temp;

                temp.word = new_token.at(i);

                for(int j = 0; j < word_probs.size(); j++)
                {
                    if(new_token.at(i) == word_probs.at(j).word)
                    {
                        temp.prob_pos = word_probs.at(j).prob_pos;
                        temp.prob_neg = word_probs.at(j).prob_neg;
                    }
                }

                checked_probs.push_back(temp);
            }

            for(int j = 0; j < word_probs.size(); j++)
            {
                if(new_token.at(i) == word_probs.at(j).word)
                {
                    new_token_pos *= word_probs.at(j).prob_pos;
                    new_token_neg *= word_probs.at(j).prob_neg;
                }
            }
        }

        if(new_token_pos < new_token_neg)
        {
            test_neg++;
        }
        else
        {
            test_pos++;
        }

        test_total++;
    }

    test_neg = test_neg / test_total * 100;
    test_pos = test_pos / test_total * 100;

    output_test_set(checked_probs);

    cout << endl << endl << "Test set (spam): "
        << endl << "Spam %: " << test_pos << endl << "Ham %: " << test_neg
        << endl << "Total: " << test_total << endl;

    cout << endl << "Number of unique words in test set: "
        << checked_probs.size();
}

bool comp_len_pos(const WordProb & a, const WordProb & b)
{
    return a.prob_pos < b.prob_pos;
}

bool comp_len_neg(const WordProb & a, const WordProb & b)
{
    return a.prob_neg < b.prob_neg;
}

void output_test_set(vector<WordProb> & checkedProbs)
{
    int checkedProbsSize = checkedProbs.size();

    topHamTestHS(checkedProbsSize, checkedProbs);
    topSpamTestHS(checkedProbsSize, checkedProbs);
}

void topHamTestHS(int checkedProbsSize, vector<WordProb> & checkedProbs)
{
    std::sort(checkedProbs.begin(), checkedProbs.end(), comp_len_neg);

    ofstream topHamHS;
    topHamHS.open("output/topHamTestHS");

    for(int i = checkedProbsSize - 1; i > checkedProbsSize - 250; i--)
    {
        topHamHS << checkedProbs.at(i).word << " "
            << checkedProbs.at(i).prob_neg << " "
            << checkedProbs.at(i).prob_pos << endl;
    }

    topHamHS.close();
}

void topSpamTestHS(int checkedProbsSize, vector<WordProb> & checkedProbs)
{

    std::sort(checkedProbs.begin(), checkedProbs.end(), comp_len_pos);

    ofstream topSpamHS;
    topSpamHS.open("output/topSpamTestHS");

    for(int i = checkedProbsSize - 1; i > checkedProbsSize - 250; i--)
    {
        topSpamHS << checkedProbs.at(i).word << " "
            << checkedProbs.at(i).prob_neg << " "
            << checkedProbs.at(i).prob_pos << endl;
    }

    topSpamHS.close();
}

void output_training_set(vector<WordProb> & word_probst)
{
    vector<string> top500Pos, top500Neg;

    ProbDiffPos(word_probst);
    ProbDiffNeg(word_probst);

    TopPos(top500Neg, top500Pos, word_probst);
    TopNeg(top500Neg, top500Pos, word_probst);

    sort(top500Pos.begin(), top500Pos.begin() + top500Pos.size());
    sort(top500Neg.begin(), top500Neg.begin() + top500Neg.size());

    InPosNotNeg(top500Neg, top500Pos);
    InNegNotPos(top500Neg, top500Pos);
    InNegAndPos(top500Neg, top500Pos);
}

void ProbDiffPos(vector<WordProb> & word_probst)
{
    vector<WordProb> diffsPos;
    WordProb temp;

    int word_probsSize = word_probst.size();

    for(int i = 0; i < (int) word_probst.size(); i++)
    {
        if(word_probst.at(i).prob_pos > word_probst.at(i).prob_neg)
        {
            temp.word = word_probst.at(i).word;
            temp.prob_pos = word_probst.at(i).prob_pos
                - word_probst.at(i).prob_neg;
            diffsPos.push_back(temp);
        }
    }

    std::sort(diffsPos.begin(), diffsPos.end(), comp_len_pos);

    ofstream diffsPosTxt;
    diffsPosTxt.open("output/ProbDiffPos");

    for(int i = 0; i < (int) diffsPos.size(); i++)
    {
        diffsPosTxt << diffsPos.at(i).word << " "
            << diffsPos.at(i).prob_pos << endl;
    }

    diffsPosTxt.close();
}

void ProbDiffNeg(vector<WordProb> & word_probst)
{
    vector<WordProb> diffsNeg;
    WordProb temp;

    for(int i = 0; i < (int) word_probst.size(); i++)
    {
        if(word_probst.at(i).prob_neg > word_probst.at(i).prob_pos)
        {
            temp.word = word_probst.at(i).word;
            temp.prob_neg = word_probst.at(i).prob_neg
                - word_probst.at(i).prob_pos;
            diffsNeg.push_back(temp);
        }
    }

    std::sort(diffsNeg.begin(), diffsNeg.end(), comp_len_neg);

    ofstream diffsNegTxt;
    diffsNegTxt.open("output/ProbDiffNeg");

    for(int i = 0; i < (int) diffsNeg.size(); i++)
    {
        diffsNegTxt << diffsNeg.at(i).word << " "
            << diffsNeg.at(i).prob_neg << endl;
    }

    diffsNegTxt.close();
}

void TopPos(vector<string> & top500Pos, vector<string> & top500Neg,
    vector<WordProb> & word_probst)
{
    int word_probsSize = word_probst.size();

    std::sort(word_probst.begin(), word_probst.end(), comp_len_pos);

    ofstream TopPos;
    TopPos.open("output/TopPos");

    for(int i = word_probsSize - 1; i > word_probsSize - 500; i--)
    {
        TopPos << word_probst.at(i).word <<
        " P(" << word_probst.at(i).word << " | S) = "
            << word_probst.at(i).prob_pos << " " <<
        " P(" << word_probst.at(i).word << " | H) = "
            << word_probst.at(i).prob_neg << endl;
    }

    TopPos.close();

    for(int i = word_probsSize - 1; i > word_probsSize - 500; i--)
    {
        if(std::find(top500Pos.begin(), top500Pos.end(),
            word_probst.at(i).word) == top500Pos.end())
        {
            top500Pos.push_back(word_probst.at(i).word);
        }
    }
}

void TopNeg(vector<string> & top500Pos, vector<string> & top500Neg,
    vector<WordProb> & word_probst)
{
    int word_probsSize = word_probst.size();

    std::sort(word_probst.begin(), word_probst.end(), comp_len_neg);

    ofstream TopNeg;
    TopNeg.open("output/TopNeg");

    for(int i = word_probsSize - 1; i > word_probsSize - 500; i--)
    {
        TopNeg << word_probst.at(i).word <<
        " P(" << word_probst.at(i).word << " | H) = "
            << word_probst.at(i).prob_neg << " " <<
        " P(" << word_probst.at(i).word << " | S) = "
            << word_probst.at(i).prob_pos << endl;
    }

    TopNeg.close();

    for(int i = word_probsSize - 1; i > word_probsSize - 500; i--)
    {
        if(std::find(top500Neg.begin(), top500Neg.end(),
            word_probst.at(i).word) == top500Neg.end())
        {
            top500Neg.push_back(word_probst.at(i).word);
        }
    }
}

void InNegNotPos(vector<string> & top500Neg, vector<string> & top500Pos)
{
    vector<string> setOp;

    set_difference(top500Pos.begin(), top500Pos.end(), top500Neg.begin(),
        top500Neg.end(), back_inserter(setOp));

    ofstream InPosNotNeg;
    InPosNotNeg.open("output/InNegNotPos");

    for(int i = 0; i < (int) setOp.size(); i++)
    {
        InPosNotNeg << setOp.at(i) << endl;
    }

    InPosNotNeg.close();
}

void InPosNotNeg(vector<string> & top500Neg, vector<string> & top500Pos)
{
    vector<string> setOp;

    set_difference(top500Neg.begin(), top500Neg.end(), top500Pos.begin(),
        top500Pos.end(), back_inserter(setOp));

    ofstream InNegNotPos;
    InNegNotPos.open("output/InPosNotNeg");

    for(int i = 0; i < (int) setOp.size(); i++)
    {
        InNegNotPos << setOp.at(i) << endl;
    }

    InNegNotPos.close();
}

void InNegAndPos(vector<string> & top500Neg, vector<string> & top500Pos)
{
    vector<string> setOp;

    set_intersection(top500Neg.begin(), top500Neg.end(), top500Pos.begin(),
        top500Pos.end(), back_inserter(setOp));

    ofstream InNegAndPos;
    InNegAndPos.open("output/InNegAndPos");

    for(int i = 0; i < (int) setOp.size(); i++)
    {
        InNegAndPos << setOp.at(i) << endl;
    }

    InNegAndPos.close();
}

