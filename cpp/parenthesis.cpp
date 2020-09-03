#include <string>
#include <vector>
#include <iostream>
#include <array>

using namespace std;

class BalancedChars
{
public:
  static bool isOpenChar(char ch){
    for (auto pair : pairs){
      if (pair.open == ch){
        return true;
      }
    }
    return false;
  }
  static bool isCloseChar(char ch){
    for (auto pair : pairs){
      if (pair.close == ch){
        return true;
      }
    }
    return false;
  }

  static char getMatching(char ch){
    for (auto pair: pairs){
      if (pair.close == ch){
        return pair.open;
      }
      else if (pair.open == ch){
        return pair.close;
      }
    }
    return '\0';
  }

private:
  struct CharPair
  {
    char open;
    char close;
  };

  static inline const vector<CharPair> pairs = {
    {'{', '}'},
    {'[', ']'},
    {'"', '"'}
  };
};

void printVector(const vector<char>& vect){
  for (auto item : vect){
    cout << item;
  }
  cout << endl;
}

bool areParenthesisBalanced(const string& input){
  vector<char> matchStack;
  cout << input << endl;
  for (int i=0 ; i<input.size() ; i++){
    char ch = input[i];
    if (BalancedChars::isOpenChar(ch)){
      matchStack.push_back(ch);
    }
    else if (BalancedChars::isCloseChar(ch)){
      if (matchStack.back() == BalancedChars::getMatching(ch)) {
        matchStack.pop_back();
      } else {
        cout << "Not Balanced :/" << endl;
        return false;
      }
    }
  }
  if (matchStack.size() > 0){
    cout << "Not Balanced :/" << endl;
    return false;
  }
  cout << "Balanced :)" << endl;
  return true;
}

void balanceParenthesis(string& input){
  vector<char> matchStack;
  cout << input << endl;
  bool modified = false;
  for (int i=0 ; i<input.size() ; i++){
    char ch = input[i];
    if (BalancedChars::isOpenChar(ch)){
      matchStack.push_back(ch);
    }
    else if (BalancedChars::isCloseChar(ch)){
      if (matchStack.size() > 0){
        if (matchStack.back() != BalancedChars::getMatching(ch)) {
          modified = true;
          input.insert(i, 1, BalancedChars::getMatching(matchStack.back()));
        }
        matchStack.pop_back();
      }
    }
  }

  if (matchStack.size() > 0){
    modified = true;
    while(matchStack.size() > 0){
      input.push_back(matchStack.back());
      matchStack.pop_back();
    }
  }

  if (modified) {
    cout << input << endl;
  } else {
    cout << "Balanced" << endl;
  }
}


int main(){
  std::vector<string> tests = {
    "{[ hello, world ]}",
    "well { he's cool } [  ] {{d[[yes] sleep doggo ]well }okay }",
    "{[ happy ]]}",
    " okay { []}}",
  };

  for (string& test : tests){
    // areParenthesisBalanced(test);
    balanceParenthesis(test);
    cout << endl;
  }

  return 1;
}