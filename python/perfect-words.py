def findPerfectWords(fileName):
  testCount = 0
  perfectCount = 0
  with open(fileName,'r') as file:
    with open('perfect-words.txt', 'w') as outFile:
      for line in file:
        value = 0
        word = line[0:-1] # remove newline character
        for char in word:
          val = getValue(char)
          if val == 0:
            value = 0
            break
          else:
            value += val
        if value == 100:
          perfectCount += 1
          printPerfect(word)
          outFile.write(line)
        testCount += 1

  
  
  print()
  print("WORDS TESTED: " + str(testCount))
  print("PERFECT WORDS FOUND: " + str(perfectCount))
  print()

def getValue(char):
  ascii = ord(char)
  if (ascii > 96 and ascii < 123):
    return ascii - 96
  elif (ascii > 64 and ascii < 91):
    return ascii - 64
  else:
    return 0

def printPerfect(word):
  print(word, end=': ')
  i = 0
  for char in word:
    i += 1
    value = str(getValue(char))
    if i != len(word):
      print(value, end=' + ')
    else:
      print(value)

if __name__ == "__main__":
  findPerfectWords("words.txt")