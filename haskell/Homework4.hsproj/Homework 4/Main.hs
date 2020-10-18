module HW4 where
import Data.List
import Data.List.Split
import Data.Foldable


fun1 :: [Integer] -> Integer
fun1 [] = 1
fun1 (x:xs)
  | even x = (x - 2) * fun1 xs
  | otherwise = fun1 xs
  
fun1' :: [Integer] -> Integer
fun1' = product . map (flip (-) 2) . (filter even)

fun2 :: Integer -> Integer
fun2 1 = 0
fun2 n 
  | even n = n + fun2 (n `div` 2)
  | otherwise = fun2 (3 * n + 1)
  
grossMath :: Integer -> Integer 
grossMath x = if even x then x `div` 2 else 3 * x + 1

fun2' :: Integer -> Integer
fun2' = sum . filter even . takeWhile (>1) . iterate grossMath


data Tree a = Leaf
            | Node Integer (Tree a) a (Tree a)
  deriving (Show, Eq)
  
treeHeight :: Tree a -> Integer
treeHeight Leaf = (-1)
treeHeight (Node h _ _ _) = h

shorter :: Tree a -> Tree a -> Bool
shorter Leaf Leaf = False
shorter Leaf (Node _ _ _ _) = True
shorter (Node _ _ _ _) Leaf = False
shorter (Node h1 _ _ _) (Node h2 _ _ _) = h1 < h2

insertNode :: Tree a -> a -> Tree a
insertNode Leaf elem = Node 0 Leaf elem Leaf
insertNode (Node height left myElem right) elem
  | shorter left right = Node height (insertNode left elem) myElem right
  | otherwise = Node (treeHeight (insertNode right elem) + 1) left myElem (insertNode right elem)
  
foldTree :: [a] -> Tree a
foldTree lst = foldr (\elem tree -> insertNode tree elem) Leaf lst

isBalanced :: Tree a -> Bool
isBalanced Leaf = True
isBalanced (Node _ left _ right) = (abs (treeHeight left - treeHeight right) <= 1) && (isBalanced left) && (isBalanced right)






















xor :: [Bool] -> Bool
xor list = odd (foldr (\value accumulator -> accumulator + if value then 1 else 0) 0 list)

map' :: (a -> b) -> [a] -> [b]
map' f list = foldl (\accumulator value -> accumulator ++ [f value]) [] list

cartProd :: [a] -> [b] -> [(a, b)]
cartProd xs ys = [(x,y) | x <- xs, y <- ys]

sieveBase :: Int -> [Int]
sieveBase max = map (\val -> val*2 + 1) ([1..max] \\ (map (\(i, j) -> i + j + 2*i*j) (cartProd [1..max] [1..max])))

sieve :: Int -> [Int]
sieve max = sieveBase ((max - 2) `div` 2)






























getChildren :: Tree a -> [a]
getChildren Leaf = []
getChildren (Node height left elem right) = getChildren left ++ [elem] ++ getChildren right

countChildren = length . getChildren

--isBalanced :: Tree a -> Bool
--isBalanced Leaf = True
--isBalanced (Node height left elem right) = abs (getHeight left - getHeight right) <= 1 && isBalanced left && isBalanced right
--
--insertNode :: a -> Tree a -> Tree a
--insertNode newElem Leaf = Node 0 Leaf newElem Leaf
--insertNode newElem (Node height left elem right)
--  | (isFull left && isFull right && getHeight left == getHeight right) = Node (height + 1) left elem (insertNode newElem right)
--  | not (isFull right) = Node height left elem (insertNode newElem right)
--  | otherwise = Node height (insertNode newElem left) elem right
--  
--foldTree :: [a] -> Tree a
--foldTree list = foldr insertNode Leaf list 
--getHeight :: Tree a -> Integer
--getHeight Leaf = (-1)
--getHeight (Node height _ _ _) = height
--
--isFull :: Tree a -> Bool
--isFull Leaf = True
--isFull (Node _ left _ right)
--  | getHeight left == getHeight right = isFull left && isFull right
--  | otherwise = False
--
--
--fun1 :: [Integer] -> Integer
--fun1 [] = 1
--fun1 (x:xs)
--  | even x = (x - 2) * fun1 xs
--  | otherwise = fun1 xs
--  
--fun1' :: [Integer] -> Integer
----fun1' = foldl (\accum val -> (val-2) * accum) 1 . filter even
--fun1' = product . map (flip (-) 2) . filter even
--
--
--fun2 :: Integer -> Integer
--fun2 1 = 0
--fun2 n 
--  | even n = n + fun2 (n `div` 2)
--  | otherwise = fun2 (3 * n + 1)
-- 
--weirdPattern = iterate (\n -> if even n then div n 2 else 3*n+1)
--  
--fun2' :: Integer -> Integer
--fun2' = sum . filter even . takeWhile (/=1) . weirdPattern
