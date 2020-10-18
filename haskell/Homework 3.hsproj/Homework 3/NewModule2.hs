import ExprT
import Parser

eval :: ExprT -> Integer
eval (Lit n) = n
eval (Add first second) = eval first + eval second
eval (Mul first second) = eval first * eval second

evalStr :: String -> Maybe Integer
evalStr = maybe Nothing (Just . eval) . parseExp Lit Add Mul


