#include <iostream>
#include <string>

using namespace std;

string reverse(string str){
  size_t length = str.length();

  if(length == 1)
  {
    //The string is a single charater.
    return str;
  }
  else
  {
    char lastCharacter = str[length - 1];
    str.pop_back();
    return lastCharacter + reverse(str);
  }
}

bool compare(string str1, string str2){
  if (str1.length() != str2.length()){
    return false;
  }
  for (size_t i=0; i<str1.length() ; i++){
    if (str1[i] != str2[i])
    {
      return false;
    }
  }
  return true;
}

bool isPalindrome(string str1, string str2){
  str1 = reverse(str1);
  return compare(str1, str2);
}

string omit(string str, int indexToOmit){
  if (indexToOmit >= str.length()){
    throw;
  }
  string omittedString = "";
  for (int i=0 ; i<str.length() ; i++){
    if (i != indexToOmit){
      omittedString += str[i];
    }
  }
  return omittedString;
}

void printAllPermutations(string str, string leading=""){
  if (str.length() <= 1){
    cout << leading << str << endl;
    return;
  }

  for (int i=0 ; i<str.length() ; i++){
    string newLeading = leading;
    newLeading += str[i];
    string omitted = omit(str, i);
    printAllPermutations(omitted, newLeading);
  }
}

char firstDuplicate(string str){
  char last;
  for (int i=0 ; i<str.length() ; i++){
    if (str[i] == last){
      return last;
    }
    last = str[i];
  }
  return ' ';
}

char firstNonRepeating(string str){
  char last = '*';
  bool isRepeating = true; //Indicates that the last character is part of a repeating group
  for (int i=0 ; i<str.length() ; i++){
    char current = str[i];
    if (current != last){
      //Character change
      if (!isRepeating) {
        //The last character didn't repeat!
        return last;
      }
      isRepeating = false;
    } else {
      //The current character is repeating
      isRepeating = true;
    }
    last = current;
  }
  if (isRepeating == false){
    //The last letter in the list didn't repeat
    return last;
  }
  return ' ';
}

int fibonacci(int n) {
  if (n > 1) {
    return fibonacci(n-1) + fibonacci(n-2);
  }
  if (n == 1) return 1;
  if (n < 0) throw;
  return 0;
}

void printFibonacci(int n) {
  for (int i=0 ; i<n ; i++){
    cout << fibonacci(i) << endl;
  }
}

void printFibonacciMemoized(int n, int printed = 0, int back1 = 1, int back2 = 0){
  if (n < 2) {
    if (n < 1) throw;
    cout << 0 << endl;
    return;
  }

  if (printed < n){
    if (printed == 0) cout << 0 << endl;
    else if (printed == 1) cout << 1 << endl;
    else {
      int current = back1 + back2;
      cout << current << endl;
      printFibonacciMemoized(n, printed + 1, current, back1);
    }
    printFibonacciMemoized(n, printed + 1);
  }
} 



void doubleLine(){
  cout << endl << endl;
}

int main(){
  printFibonacciMemoized(10);
}


