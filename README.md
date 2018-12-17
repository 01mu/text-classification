# text-classification
Determines whether text message input from a test set (`spam`) is spam or ham based on the input of a training set (`hamspam`). All of the texts in `spam` are spam messages. Whether a text is spam or ham is known for `hamspam` (0 = ham, 1 = spam). In `hamspam`, 13.4% are spam and 86.5% are ham. Based on the 13.4% of text messages for `hamspam` that are spam, the algorithm determines that 85.5% of texts in `spam` are spam (14.4 percentage points off).
## Result
```
Naive Bayes text classification with SMS spam/ham

Getting input from 'hamspam'
Getting individual words from 'hamspam'
Tokenizing and assigning text and classification inputs to training set vector
Determining probabilities for each word in training set
Determining whether each input in 'spam' is spam or ham

Test set (spam):z
Spam %: 85.5876
Ham %: 14.4124
Total: 1353

Number of unique words in test set: 5629

Training set (hamspam):
Spam %: 13.4015
Ham %: 86.5985
Total: 5574

Number of unique words in training set: 12978
```
## Output
`InNegAndPos`: in both ham and spam (training)
`InNegNotPos`: in ham text and not spam (training)
`InPosNotNeg`: in spam text and not ham (training)
`ProbDiffNeg`: greatest difference with ham prob being greater (training)
`ProbDiffPos`: greatest difference with spam prob being greater (training)
`TopNeg`: top ham (training)
`TopPos`: top spam (training)
`topHamTestHS`: top ham (test)
`topSpamTestHS`: top spam (test)
## Sources
* UCI Machine Learning Repository: SMS Spam Collection Data Set
** https://archive.ics.uci.edu/ml/datasets/sms+spam+collection
* Computing at Dublin Institute of Technology - SMS Spam Dataset
** http://www.dit.ie/computing/research/resources/smsdata/
* Text Classification Using Naive Bayes
** https://www.youtube.com/watch?v=EGKeC2S44Rs
