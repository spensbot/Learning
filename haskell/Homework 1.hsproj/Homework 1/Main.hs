toDigits :: Integer -> [Integer]
toDigits n
  | n < 1 = []
  | otherwise = toDigits (n `div` 10) ++ [n `mod` 10]
  
reverse2 :: [Integer] -> [Integer]
reverse2 [] = []
reverse2 (a:[]) = [a]
reverse2 (first : rest) = reverse rest ++ [first]

toDigitsRev :: Integer -> [Integer]
toDigitsRev n = reverse2 (toDigits n)

doubleEveryOther :: [Integer] -> [Integer]
doubleEveryOther [] = []
doubleEveryOther (x:[]) = [x]
doubleEveryOther (x : (y : rest)) = [x, y*2] ++ doubleEveryOther rest

doubleEveryOtherFromRight :: [Integer] -> [Integer]
doubleEveryOtherFromRight = reverse2 . doubleEveryOther . reverse2

sumList :: [Integer] -> Integer
sumList [] = 0
sumList (first : []) = first
sumList (first : rest) = first + sumList rest

sumDigits :: [Integer] -> Integer
sumDigits [] = 0
sumDigits (first : rest) = sumList (toDigits first) + sumDigits rest

validate :: Integer -> Bool
validate n = (sumDigits(doubleEveryOtherFromRight (toDigits n)) `mod` 10) == 0

--------------------------------    HANOI    ------------------------------

type Peg = String
type Move = (Peg, Peg)

-- List moves required to move stack of n discs from peg a to peg b'

hanoi :: Integer -> Peg -> Peg -> Peg -> [Move]
hanoi n a b c
  | n == 0 = []
  | otherwise = (hanoi (n-1) a c b) ++ [(a, b)] ++ (hanoi (n-1) c b a)
  
getSize :: [Move] -> Integer
getSize [] = 0
getSize (x:[]) = 1
getSize (x: rest) = 1 + getSize(rest)


-------------------------------    HANOI 4-PEG    --------------------------------

hanoi4 :: Integer -> Peg -> Peg -> Peg -> Peg -> [Move]
hanoi4 n a b c d
  | n == 0 = []
  | n == 1 = [(a, b)]
  | n == 2 = [(a, c), (a, b), (c, b)]
  | n == 3 = [(a, c), (a, d), (a, b), (d, b), (c, b)]
  | n == 4 = [(a, c), (a, b), (a, d), (b, d), (a, b), (d, a), (d, b), (a, b), (c, b)]
  | otherwise = (hanoi4 (n-2) a d b c) ++ [(a, c), (a, b), (c, b)] ++ (hanoi4 (n-2) d b a c)
  

















