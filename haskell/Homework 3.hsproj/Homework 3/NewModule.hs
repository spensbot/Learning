module HW3 where
import Data.List
import Data.List.Split
import Data.Foldable

-- FREEEEBEEEE
myIndexed :: [a] -> [(Int, a)]
myIndexed list = zip [0..] list

hasIndexMultiple :: Int -> (Int, a) -> Bool
hasIndexMultiple multiple (index, _) = (index + 1) `mod` multiple == 0

skipNum :: [a] -> Int -> [a]
--skipNum list 0 = list
skipNum list num =  map snd (filter (hasIndexMultiple num) (myIndexed list))

skips :: [a] -> [[a]]
skips list = map (skipNum list) [1..(length list)]
--skips list = map  (myIndexed list)

-- =============   PART II   ======================

subGroups3 :: [a] -> [(a, a, a)]
subGroups3 list = zip3 list (drop 1 list) (drop 2 list)

isLocalMaxima :: (Integer, Integer, Integer) -> Bool
isLocalMaxima (a, b, c) = (b > a) && b > c

takeMiddle :: (Integer, Integer, Integer) -> Integer
takeMiddle (a, b, c) = b

localMaxima :: [Integer] -> [Integer]
localMaxima list = map takeMiddle (filter isLocalMaxima (subGroups3 list))

-- =============   PART III   ======================

-- FREEEEEEEEEEEEE
countElem :: Integer -> [Integer] -> Int
countElem element list = length (filter (== element) list)

getCounts :: [Integer] -> [Int]
getCounts list = map (flip countElem list) [0..9] 


getChar2 :: Int -> Int -> Char
getChar2 count row
  | count > row = '*'
  | otherwise = ' '

getRow :: [Int] -> Int -> String
getRow countList row = (map ((flip getChar2) row) countList)

histogram :: [Integer] -> String
histogram list = intercalate "\n" (reverse (map ( getRow (getCounts list)) [0..(maximum (getCounts list)-1)])) ++ "\n==========\n0123456789\n"
 








xor :: [Bool] -> Bool
xor list = odd (foldr (\value accumulator -> accumulator + if value then 1 else 0) 0 list)

map' :: (a -> b) -> [a] -> [b]
map' f list = foldl (\accumulator value -> accumulator ++ [f value]) [] list

--myFoldl :: (a -> b -> a) -> a -> [b] -> a
--myFoldl f base list = foldr (/value accumulator -> 



cartProd :: [a] -> [b] -> [(a, b)]
cartProd xs ys = [(x,y) | x <- xs, y <- ys]

sieveBase :: Int -> [Int]
sieveBase max = map (\val -> val*2 + 1) ([1..max] \\ (map (\(i, j) -> i + j + 2*i*j) (cartProd [1..max] [1..max])))

sieve :: Int -> [Int]
sieve max = sieveBase ((max - 2) `div` 2)



data Tree a = Leaf
            | Node Integer (Tree a) a (Tree a)
  deriving (Show, Eq)

getChildren :: Tree a -> [a]
getChildren Leaf = []
getChildren (Node height left elem right) = getChildren left ++ [elem] ++ getChildren right

countChildren = length . getChildren

isBalanced :: Tree a -> Bool
isBalanced Leaf = True
isBalanced (Node height left elem right) = abs (getHeight left - getHeight right) <= 1 && isBalanced left && isBalanced right

insertNode :: a -> Tree a -> Tree a
insertNode newElem Leaf = Node 0 Leaf newElem Leaf
insertNode newElem (Node height left elem right)
  | (isFull left && isFull right && getHeight left == getHeight right) = Node (height + 1) left elem (insertNode newElem right)
  | not (isFull right) = Node height left elem (insertNode newElem right)
  | otherwise = Node height (insertNode newElem left) elem right
  
foldTree :: [a] -> Tree a
foldTree list = foldr insertNode Leaf list 
getHeight :: Tree a -> Integer
getHeight Leaf = (-1)
getHeight (Node height _ _ _) = height

isFull :: Tree a -> Bool
isFull Leaf = True
isFull (Node _ left _ right)
  | getHeight left == getHeight right = isFull left && isFull right
  | otherwise = False


fun1 :: [Integer] -> Integer
fun1 [] = 1
fun1 (x:xs)
  | even x = (x - 2) * fun1 xs
  | otherwise = fun1 xs
  
fun1' :: [Integer] -> Integer
--fun1' = foldl (\accum val -> (val-2) * accum) 1 . filter even
fun1' = product . map (flip (-) 2) . filter even


fun2 :: Integer -> Integer
fun2 1 = 0
fun2 n 
  | even n = n + fun2 (n `div` 2)
  | otherwise = fun2 (3 * n + 1)
 
weirdPattern = iterate (\n -> if even n then div n 2 else 3*n+1)
  
fun2' :: Integer -> Integer
fun2' = sum . filter even . takeWhile (/=1) . weirdPattern





















































