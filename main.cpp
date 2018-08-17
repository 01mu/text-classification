/*
* Naive Bayes text classification (Spam/Ham)
*
* Naive Bayes text classification with SMS spam/ham
*
* github.com/01mu
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>

using namespace std;

struct attributes
{
    string word;
    double frequency;
};

struct review
{
    string text;
    vector<attributes> wordOccurances;
    int qualityClass;
};

struct wordProb
{
    string word;
    double probPos;
    double probNeg;
    int count;
};

bool compareByLengthPos(const wordProb &, const wordProb &);
bool compareByLengthNeg(const wordProb &, const wordProb &);
vector<string> tokenize(const char *, char);
void getTextFromFile();
void getTokenizedTerms();
void tokenizeText();
void determineWordProbs();
void determineNewInputClass();
void outputTestSetFiles(vector<wordProb>);
void outputTrainingSetFiles(vector<wordProb>);

// probability of positive reviews in training set (spam)
double posProb = 0;

// probability of negative reviews in training set (ham)
double negProb = 0;

// total number of reviews in training set
double total = 0;

// number of words that occur in positive reviews in training set (spam)
double posWordTotal = 0;

// number of words that occur in negative reviews in training set (ham)
double negWordTotal = 0;

// probability of positive reviews in test set (spam)
double testPos = 0;

// probability of negative reviews in test set (ham)
double testNeg = 0;

// total number of reviews in test set
double testTotal = 0;

// vector containing every word
vector<string> words;

// vector of reviews
vector<review> reviews;

// words that occur in positive reviews (spam) and negative reviews (ham)
vector<wordProb> wordProbVec;

// review text
vector<string> rev_txt;

// quality (0 = ham 1 = spam)
vector<int> quality;

int main()
{
    cout << "Naive Bayes text classification with SMS spam/ham" << endl;

    clock_t tStart = clock();

    // assign sms texts and spam/ham quality from file to vector
    getTextFromFile();

     // tokenize the review vector and gather individual words from learning set
    getTokenizedTerms();

    // assign text and quality to reviews vector
    tokenizeText();

    // assign spam/ham probabilities to each word in words vector
    determineWordProbs();

    // determine whether new set inputs are ham/spam based on old probabilities
    determineNewInputClass();

    posProb *= 100;
    negProb *= 100;

    cout << endl << endl << "Training set (hamspam.txt): "
        << endl << "Spam %: " << posProb << endl << "Ham %: " << negProb
        << endl << "Total: " << total;

    cout << endl << endl
        << "Number of unique words in training set: " << words.size();

    cout << endl << endl
        << ((double)(clock() - tStart) / CLOCKS_PER_SEC) <<  " seconds" << endl;

    return 0;
}

bool compareByLengthPos(const wordProb &a, const wordProb &b)
{
    return a.probPos < b.probPos;
}

bool compareByLengthNeg(const wordProb &a, const wordProb &b)
{
    return a.probNeg < b.probNeg;
}

vector<string> tokenize(const char *str, char c = ' ')
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
        {
            str++;
        }

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}

void getTextFromFile()
{
    cout << endl << "Getting input from hamspam.txt";

    string buffer;
    ifstream file("hamspam.txt");

    while (file && getline(file, buffer))
    {
        if (buffer.length() == 0)
            continue;

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

void getTokenizedTerms()
{
    cout << endl << "Getting individual words from hamspam.txt";

    for(int i = 0; i < (int) rev_txt.size(); i++)
    {
        vector<string> splitWord = tokenize(rev_txt.at(i).c_str());

        for(int j = 0; j < (int) splitWord.size(); j++)
        {
            if(std::find(words.begin(), words.end(), splitWord.at(j))
                == words.end())
            {
                words.push_back(splitWord.at(j));
            }
        }
    }
}

void tokenizeText()
{
    cout << endl << "Tokenizing and assigning text and " <<
        "classification inputs to training set vector";

    for(int i = 0; i < (int) rev_txt.size(); i++)
    {
        vector<string> splitWord = tokenize(rev_txt.at(i).c_str());
        vector<string> checkedWords;
        vector<attributes> tempOccurance;
        attributes tempAttribute;

        for(int j = 0; j < (int) splitWord.size(); j++)
        {
            if(std::find(words.begin(), words.end(), splitWord.at(j))
                != words.end() && std::find(checkedWords.begin(),
                checkedWords.end(), splitWord.at(j)) == checkedWords.end())
            {
                int count = std::count(splitWord.begin(),
                    splitWord.end(), splitWord.at(j));

                tempAttribute.word = splitWord.at(j);
                tempAttribute.frequency = count;
                tempOccurance.push_back(tempAttribute);

                checkedWords.push_back(splitWord.at(j));
            }
        }

        review temp;
        temp.text = rev_txt.at(i);
        temp.wordOccurances = tempOccurance;
        temp.qualityClass = quality.at(i);

        if(temp.qualityClass == 0)
        {
            negProb++;
        }
        else
        {
            posProb++;
        }

        total++;

        reviews.push_back(temp);
    }

    negProb = negProb / total;
    posProb = posProb / total;
}

void determineWordProbs()
{
    cout << endl << "Determining probabilities for each word in training set";

    for(int k = 0; k < (int) reviews.size(); k++)
    {
        if(reviews.at(k).qualityClass == 1)
        {
            for(int l = 0; l < (int) reviews.at(k).wordOccurances.size(); l++)
            {
                posWordTotal = reviews.at(k).wordOccurances.at(l).frequency
                    + posWordTotal;
            }
        }
        else
        {
            for(int l = 0; l < (int) reviews.at(k).wordOccurances.size(); l++)
            {
                negWordTotal = reviews.at(k).wordOccurances.at(l).frequency
                    + negWordTotal;
            }
        }
    }

    for(int i = 0; i < (int) words.size(); i++)
    {
        double posWordFrequency = 0;
        double negWordFrequency = 0;

        wordProb temp;
        temp.word = words.at(i);

        for(int l = 0; l < (int) reviews.size(); l++)
        {
            int woSize = reviews.at(l).wordOccurances.size();

            if(reviews.at(l).qualityClass == 1)
            {
                for(int k = 0; k < woSize; k++)
                {
                    if(reviews.at(l).wordOccurances.at(k).word == words.at(i))
                    {
                        posWordFrequency = reviews.at(l)
                            .wordOccurances.at(k).frequency + posWordFrequency;

                        break;
                    }
                }
            }
            else
            {
                for(int k = 0; k < woSize; k++)
                {
                    if(reviews.at(l).wordOccurances.at(k).word == words.at(i))
                    {
                        negWordFrequency = reviews.at(l)
                            .wordOccurances.at(k).frequency + negWordFrequency;

                        break;
                    }
                }
            }
        }

        temp.probPos = (posWordFrequency + 1) / (posWordTotal + words.size());
        temp.probNeg = (negWordFrequency + 1) / (negWordTotal + words.size());
        temp.count = 0;
        wordProbVec.push_back(temp);
    }

    outputTrainingSetFiles(wordProbVec);
}

void determineNewInputClass()
{
    cout << endl << "Determining whether each input in spam.xml is spam or ham";

    string buffer;
    ifstream file("spam.xml");
    vector<string> checkedWords;
    vector<wordProb> checkedProbs;

    while(file && getline(file, buffer))
    {
        if (buffer.length() == 0)
        {
            continue;
        }

        vector<string> newToken = tokenize(buffer.c_str());

        double newTokenPos = posProb;
        double newTokenNeg = negProb;

        for(int i = 0; i < (int) newToken.size(); i++)
        {
            if(std::find(checkedWords.begin(), checkedWords.end(),
                newToken.at(i)) == checkedWords.end())
            {
                wordProb temp;

                temp.word = newToken.at(i);

                for(int j = 0; j < (int) wordProbVec.size(); j++)
                {
                    if(newToken.at(i) == wordProbVec.at(j).word)
                    {
                        temp.probPos = wordProbVec.at(j).probPos;
                        temp.probNeg = wordProbVec.at(j).probNeg;
                        break;
                    }
                }

                checkedWords.push_back(newToken.at(i));
                checkedProbs.push_back(temp);
            }

            for(int j = 0; j < (int) wordProbVec.size(); j++)
            {
                if(newToken.at(i) == wordProbVec.at(j).word)
                {
                    newTokenPos = wordProbVec.at(j).probPos * newTokenPos;
                    newTokenNeg = wordProbVec.at(j).probNeg * newTokenNeg;
                    break;
                }
            }
        }

        if(newTokenPos < newTokenNeg)
        {
            testNeg++;
        }
        else
        {
            testPos++;
        }

        testTotal++;
    }

    testNeg = testNeg / testTotal * 100;
    testPos = testPos / testTotal * 100;

    outputTestSetFiles(checkedProbs);

    cout << endl << endl << "Test set (spam.xml): "
        << endl << "Spam %: " << testPos << endl << "Ham %: " << testNeg
        << endl << "Total: " << testTotal;

    cout << endl << "Number of unique words in test set: "
        << checkedProbs.size();
}

void outputTestSetFiles(vector<wordProb> checkedProbs)
{
    int checkedProbsSize = checkedProbs.size();

    // top ham from test set

    std::sort(checkedProbs.begin(), checkedProbs.end(), compareByLengthNeg);

    ofstream topHamHS;
    topHamHS.open("topHamTestHS.txt");

    for(int i = checkedProbsSize - 1; i > checkedProbsSize - 250; i--)
    {
        topHamHS << checkedProbs.at(i).word << " "
            << checkedProbs.at(i).probNeg << " "
            << checkedProbs.at(i).probPos << endl;
    }

    topHamHS.close();

    // top spam from test set

    std::sort(checkedProbs.begin(), checkedProbs.end(), compareByLengthPos);

    ofstream topSpamHS;
    topSpamHS.open("topSpamTestHS.txt");

    for(int i = checkedProbsSize - 1; i > checkedProbsSize - 250; i--)
    {
        topSpamHS << checkedProbs.at(i).word << " "
            << checkedProbs.at(i).probNeg << " "
            << checkedProbs.at(i).probPos << endl;
    }

    topSpamHS.close();
}

void outputTrainingSetFiles(vector<wordProb> wordProbVect)
{
    // highest difference with spam prob being greater

    vector<wordProb> diffsPos;
    vector<wordProb> diffsNeg;
    wordProb temp;

    int wordProbVecSize = wordProbVect.size();

    for(int i = 0; i < (int) wordProbVect.size(); i++)
    {
        if(wordProbVect.at(i).probPos > wordProbVect.at(i).probNeg)
        {
            temp.word = wordProbVect.at(i).word;
            temp.probPos = wordProbVect.at(i).probPos
                - wordProbVect.at(i).probNeg;
            diffsPos.push_back(temp);
        }
    }

    std::sort(diffsPos.begin(), diffsPos.end(), compareByLengthPos);

    ofstream diffsPosTxt;
    diffsPosTxt.open("ProbDiffPos.txt");

    for(int i = 0; i < (int) diffsPos.size(); i++)
    {
        diffsPosTxt << diffsPos.at(i).word << " "
            << diffsPos.at(i).probPos << endl;
    }

    diffsPosTxt.close();

    // highest difference with ham prob being greater

    for(int i = 0; i < (int) wordProbVect.size(); i++)
    {
        if(wordProbVect.at(i).probNeg > wordProbVect.at(i).probPos)
        {
            temp.word = wordProbVect.at(i).word;
            temp.probNeg = wordProbVect.at(i).probNeg
                - wordProbVect.at(i).probPos;
            diffsNeg.push_back(temp);
        }
    }

    std::sort(diffsNeg.begin(), diffsNeg.end(), compareByLengthNeg);

    ofstream diffsNegTxt;
    diffsNegTxt.open("ProbDiffNeg.txt");

    for(int i = 0; i < (int) diffsNeg.size(); i++)
    {
        diffsNegTxt << diffsNeg.at(i).word << " "
            << diffsNeg.at(i).probNeg << endl;
    }

    diffsNegTxt.close();

    // tom spam - training

    vector<string> top500Pos, top500Neg;

    std::sort(wordProbVect.begin(), wordProbVect.end(), compareByLengthPos);

    ofstream TopPos;
    TopPos.open("TopPos.txt");

    for(int i = wordProbVecSize - 1; i > wordProbVecSize - 500; i--)
    {
        TopPos << wordProbVect.at(i).word <<
        " P(" << wordProbVect.at(i).word << " | S) = "
            << wordProbVect.at(i).probPos << " " <<
        " P(" << wordProbVect.at(i).word << " | H) = "
            << wordProbVect.at(i).probNeg << endl;
    }

    TopPos.close();

    for(int i = wordProbVecSize - 1; i > wordProbVecSize - 500; i--)
    {
        if(std::find(top500Pos.begin(), top500Pos.end(),
            wordProbVect.at(i).word) == top500Pos.end())
        {
            top500Pos.push_back(wordProbVect.at(i).word);
        }
    }

    // top ham - training

    std::sort(wordProbVect.begin(), wordProbVect.end(), compareByLengthNeg);

    ofstream TopNeg;
    TopNeg.open("TopNeg.txt");

    for(int i = wordProbVecSize - 1; i > wordProbVecSize - 500; i--)
    {
        TopNeg << wordProbVect.at(i).word <<
        " P(" << wordProbVect.at(i).word << " | H) = "
            << wordProbVect.at(i).probNeg << " " <<
        " P(" << wordProbVect.at(i).word << " | S) = "
            << wordProbVect.at(i).probPos << endl;
    }

    TopNeg.close();

    for(int i = wordProbVecSize - 1; i > wordProbVecSize - 500; i--)
    {
        if(std::find(top500Neg.begin(), top500Neg.end(),
            wordProbVect.at(i).word) == top500Neg.end())
        {
            top500Neg.push_back(wordProbVect.at(i).word);
        }
    }

    // set operation training set outputs - in spam text and not ham

    vector<string> setOp;

    sort(top500Pos.begin(), top500Pos.begin() + top500Pos.size());
    sort(top500Neg.begin(), top500Neg.begin() + top500Neg.size());

    set_difference(top500Pos.begin(), top500Pos.end(), top500Neg.begin(),
        top500Neg.end(), back_inserter(setOp));

    ofstream InPosNotNeg;
    InPosNotNeg.open("InPosNotNeg.txt");

    for(int i = 0; i < (int) setOp.size(); i++)
    {
        InPosNotNeg << setOp.at(i) << endl;
    }

    InPosNotNeg.close();

    setOp.clear();

    // set operation training set outputs - in ham text and not spam

    set_difference(top500Neg.begin(), top500Neg.end(), top500Pos.begin(),
        top500Pos.end(), back_inserter(setOp));

    ofstream InNegNotPos;
    InNegNotPos.open("InNegNotPos.txt");

    for(int i = 0; i < (int) setOp.size(); i++)
    {
        InNegNotPos << setOp.at(i) << endl;
    }

    InNegNotPos.close();

    setOp.clear();

    // set operation training set outputs - in both ham and spam

    set_intersection(top500Neg.begin(), top500Neg.end(), top500Pos.begin(),
        top500Pos.end(), back_inserter(setOp));

    ofstream InNegAndPos;
    InNegAndPos.open("InNegAndPos.txt");

    for(int i = 0; i < (int) setOp.size(); i++)
    {
        InNegAndPos << setOp.at(i) << endl;
    }

    InNegAndPos.close();
}
