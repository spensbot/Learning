module HW3 where
import Data.List

skips :: [a] -> [[a]]
skips list = [list]
--
--myIndexed :: [a] -> [(Int, a)]
--myIndexed [] = 

hasIndexMultiple :: Int -> (Int, a) -> Bool
hasIndexMultiple multiple (index, _) = index `mod` multiple == 0

--skipNum :: [a] -> Int -> [a]
--skipNum list num =  map snd (filter (hasIndexMultiple num) (indexed list))
