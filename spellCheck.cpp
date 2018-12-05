/*
  Author: John Genere
*/
#include <iostream>
#include <fstream>       // ifstream & ofstream
#include <vector>        // vector
#include <set>           // set
#include <iterator>      // iterator
#include <string>        // erase
#include <stdlib.h>      // exit, EXIT_FAILURE
#include <chrono>

void fillDictionaryContainer(std::set <std::string> &allWords, const std::string &fileName);
bool notInDictionary(const std::set <std::string> &allWords, const std::string &currentWord);
void changeToLowerCase(std::string &currentWord);   // makes all char lower-case
void removeSomePunctuation(std::string &currentWord);   // removes '.' & ','
bool hasApostrophe(const std::string &currentWord); // checks for words with apostrophe
void removeApostrophe(std::string &currentWord);    // gets char's left of the '
void removeApostropheOnly(std::string &currentWord);  // removes apostrophe
std::vector<std::string> changeLetter(std::string word, const std::set <std::string> &allWords);
std::vector<std::string> removeOneLetter(std::string word, const std::set <std::string> &allWords);
std::vector<std::string> swapLetters(std::string word, const std::set <std::string> &allWords);
std::vector<std::string> addOneLetter(std::string word, const std::set <std::string> &allWords);
std::vector<std::string> spellCheck(std::string word, const std::set <std::string> &allWords);
std::vector<std::string> checkForHyphen(std::string word, const std::set <std::string> &allWords);

int main(int argc, char** argv){
  std::ifstream checkIt;                       // file object to open text file
  std::set <std::string> allWords;   // for dictionary
  std::string currentWord;                     // current word from text being checked
  std::vector <std::string> possibleWords;     // contains all suggestions
  std::vector <std::string> hyphenCheck;

  checkIt.open(argv[1]);                       // opens text to check
  if(checkIt.fail()){                          // in case file can't open
    std::cout << "ERROR--Failure to Open Text File\n";
    return 0;
  }

  fillDictionaryContainer(allWords, argv[2]);  // puts dictionary into a container
  std::ofstream suggested("suggestedWords.txt");   // opens new file for suggestions

  std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

  while (checkIt >> currentWord){        // currentWord gets each word from text
    changeToLowerCase(currentWord);      // changes words in text to lower-case
    removeSomePunctuation(currentWord);      // removes punctuation in text
    if(notInDictionary(allWords, currentWord)){
      suggested << currentWord << ": ";

    if(hasApostrophe(currentWord)){
      removeApostrophe(currentWord);   // gets char's to the left of the '
      possibleWords = spellCheck(currentWord, allWords);
      for(auto i:possibleWords)
        suggested << i << " ";

      removeApostropheOnly(currentWord);  // just removes the apostrophe
    }

    possibleWords = spellCheck(currentWord, allWords);
    hyphenCheck = checkForHyphen(currentWord, allWords);

    if(!hyphenCheck.empty())   // if the vector has valid words
      possibleWords.insert(possibleWords.end(),hyphenCheck.begin(),hyphenCheck.end());

    for(auto i:possibleWords)
      suggested << i << " ";

    suggested << std::endl;
    }
  }

  auto t2 = std::chrono::steady_clock::now();
  std::cout << "Duration is " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << std::endl;

  suggested.close();      // closes suggested words file
  checkIt.close();        // closes text file
}

void fillDictionaryContainer(std::set<std::string> &allWords, const std::string &fileName){
  std::ifstream dictionary;
  std::string word;

  dictionary.open(fileName);     // opens dictionary file
  if(dictionary.fail()){          // in case file can't open
    std::cout << "ERROR--Failure to Open Text File\n";
    std::exit(EXIT_FAILURE);
  }

  while(dictionary >> word)
    allWords.insert(word);       // insert dictionary into container

  dictionary.close();            // closes dictionary file
}

bool notInDictionary(const std::set <std::string> &allWords, const std::string &currentWord){
  std::set <std::string>::const_iterator check = allWords.find(currentWord);
  if(check == allWords.end())
    return true;             // word is not found
  else
    return false;            // word is found
}

// makes all char lower-case
void changeToLowerCase(std::string &currentWord){
  for(int i = 0; i < currentWord.length(); i++){
    if(currentWord[i] >= 'A' && currentWord[i] <= 'Z')
      currentWord[i] = char(((int)currentWord[i])+32);
  }
}

// removes '.' & ','
void removeSomePunctuation(std::string &currentWord){
  int size = currentWord.length();
  if(currentWord[size-1] == '.' || currentWord[size-1] == ',')
    currentWord.erase(size - 1, 1);
}

// checks for words with apostrophe
bool hasApostrophe(const std::string &currentWord){
  int size = currentWord.length();
  if(((int)currentWord[size-2]) == 39)
    return true;
  else
    return false;
}

// gets char's left of the apostrophe
void removeApostrophe(std::string &currentWord){
  std::string newWord;
  for(int i = 0; i < currentWord.length(); i++){
    if(((int)currentWord[i]) == 39)    // currentWord[i] = apostrophe
      i = currentWord.length();
    else
      newWord += currentWord[i];
  }
  currentWord = newWord;
}

// removes apostrophe leaving other chars
void removeApostropheOnly(std::string &currentWord){
  std::string newWord;
  for(int i = 0; i < currentWord.length(); i++){
    if(((int)currentWord[i]) == 39)    // currentWord[i] = apostrophe
      continue;    // skips apostrophe
    else
      newWord += currentWord[i];
  }
  currentWord = newWord;
}

// changes each letter of the word to every other letter in the alphabet
// then checks if that new word is in the dictionary
std::vector<std::string> changeLetter(std::string word, const std::set <std::string> &allWords){
  std::vector <std::string> foundWords;
  for(int i = 0; i < word.length(); i++){
    char change = word[i];
    int limit = 0;
    while(limit < 25){
      if(((int)change) < 122){
        change++;
        word[i] = change;
      } else {
        change = (char)97;
        word[i] = change;
      }

      if(!notInDictionary(allWords, word))
        foundWords.push_back(word);

      limit++;
    }
  }
  return foundWords;
}

// removes one letter from the word at a time and then checks if the new word
// is in the dictionary
std::vector <std::string> removeOneLetter(std::string word, const std::set <std::string> &allWords){
  std::vector <std::string> foundWords;

  for(int i = 0; i < word.length(); i++){
    std::string copy = word;
    word.erase(word.begin()+i);

    if(!notInDictionary(allWords, word))
      foundWords.push_back(word);

    word = copy;
  }

  return foundWords;
}

// swaps the neighboring letters and checks if the new word is in the dictionary
std::vector<std::string> swapLetters(std::string word, const std::set <std::string> &allWords){
  std::vector <std::string> foundWords;
  for(int i = 0; i <= word.length()-2; i++){
    std::string copy = word;
    std::swap(word[i], word[i+1]);

    if(!notInDictionary(allWords, word))
      foundWords.push_back(word);

    word = copy;
  }
  return foundWords;
}

// adds a letter to the word at every position and then checks if the new word
// is in the dictionary
std::vector<std::string> addOneLetter(std::string word, const std::set <std::string> &allWords){
  std::vector <std::string> foundWords;
  for(int i = 0; i <= word.length(); i++){
    char add = 'a';
    int limit = 0;

    while(limit < 26){
      std::string copy = word;
      word.insert(i,1,add);

      if(!notInDictionary(allWords, word))
        foundWords.push_back(word);

      word = copy;
      add++;
      limit++;
    }
  }
  return foundWords;
}

// gets the result of all of the word changing functions
std::vector<std::string> spellCheck(std::string word, const std::set <std::string> &allWords){
  std::vector<std::string> v, v2, v3, v4;
  v = changeLetter(word, allWords);
  v2 = removeOneLetter(word, allWords);
  v3 = swapLetters(word, allWords);
  v4 = addOneLetter(word, allWords);

  v.insert(v.end(),v2.begin(),v2.end());
  v.insert(v.end(),v3.begin(),v3.end());
  v.insert(v.end(),v4.begin(),v4.end());

  return v;
}

/*
  returns a vector that contains the valid words in a word that contains a
  hyphen, this does not get suggestions for the left string or right string
  but rather only checks if the left string or right string is in the dictionary.
*/
std::vector<std::string> checkForHyphen(std::string word, const std::set <std::string> &allWords){
  std::string leftString;
  std::string rightString;
  std::vector <std::string> foundWords;

  for(int i = 0; i < word.length(); i++){
    if(word[i] == '-'){
      for(int j = 0; j < i; j++)
        leftString += word[j];
      for(int x = i+1; x < word.length(); x++)
        rightString += word[x];
      break;
    }
  }

  if(!notInDictionary(allWords, leftString))
    foundWords.push_back(leftString);
  if(!notInDictionary(allWords, rightString))
    foundWords.push_back(rightString);

  return foundWords;

}
