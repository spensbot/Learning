from random import randint

def getRandomList(n):
  list = []
  while (n > 0):
    list.append(randint(0, 9))
    n-=1
  return list

def printList(list):
  for item in list:
    print(f"{item}", end=" ")
  print("")


def quickSort(list):
  if (len(list) < 2):
    return list, 0

  pivotValue = list.pop()

  less = []
  more = []
  iterations = 0
  
  for item in list:
    iterations += 1
    if(item < pivotValue):
      less.append(item)
    else:
      more.append(item)

  if (len(more) == 0):
    more.append(pivotValue)
  else:
    less.append(pivotValue)

  sortedLess, iLess = quickSort(less)
  sortedMore, iMore = quickSort(more)
  
  return sortedLess + sortedMore , iLess + iMore + iterations

def bubbleSort(list):
  swapped = True
  iterations = 0
  while swapped:
    swapped = False
    i = 0
    while i < len(list) - 1:
      iterations += 1
      current = list[i]
      next = list[i+1]
      if (current > next):
        swapped = True
        list[i+1] = current
        list[i] = next
      i+=1
  print(f"Bubble Sort Iterations: {iterations}")
  return list

quickSorted, iterations = quickSort(getRandomList(20))

print(f"Quick Sort Iterations: {iterations}")
printList(quickSorted)
printList(bubbleSort(getRandomList(20)))




