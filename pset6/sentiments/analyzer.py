import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # list for positive and negative word
        self.positives_words = []
        self.negatives_words = []
        # check each line, if it is an empty or comment, igonre it, otherwise, append to list 
        # positive word
        with open(positives) as lines:
            for line in lines:
                if line.startswith(';') or line.startswith(' '):
                    pass
                line = line.strip()
                self.positives_words.append(line)
        # negative word
        with open(negatives) as lines:
            for line in lines:
                if line.startswith(';') or line.startswith(' '):
                    pass
                line = line.strip()
                self.negatives_words.append(line)

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # total score
        score = 0
        # create instance
        tokenizer = nltk.tokenize.TweetTokenizer()
        # split it
        tokens = tokenizer.tokenize(text)
        for word in tokens:
            # become lowercase to check
            word = str.lower(word)
            if word in self.negatives_words:
                score -= 1
            elif word in self.positives_words:
                score += 1
            else:
                score += 0
        return score
