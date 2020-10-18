{-# LANGUAGE FlexibleInstances #-}

import ExprT
import Parser
--import StackVM
import qualified Data.Map as M
import Control.Applicative

eval :: ExprT -> Integer
eval (Lit x) = x
eval (Add left right) = eval left + eval right
eval (Mul left right) = eval left * eval right

evalStr :: String -> Maybe Integer
evalStr str = fmap eval (parseExp Lit Add Mul str)

class Expr a where
  lit :: Integer -> a
  add :: a -> a -> a
  mul :: a -> a -> a
  
instance Expr ExprT where
  lit = Lit
  add = Add
  mul = Mul
  
reify :: ExprT -> ExprT
reify = id

instance Expr Integer where
  lit = id
  add = (+)
  mul = (*)
  
instance Expr Bool where
  lit = (> 0)
  add = (||)
  mul = (&&)
  
newtype MinMax = MinMax Integer deriving (Eq, Show)
newtype Mod7 = Mod7 Integer deriving (Eq, Show)

instance Expr MinMax where
  lit = MinMax 
  add (MinMax x) (MinMax y) = lit $ max x y
  mul (MinMax x) (MinMax y) = lit $ min x y
  
instance Expr Mod7 where
  lit x = Mod7 $ x `mod` 7
  add (Mod7 x) (Mod7 y) = lit $ x + y
  mul (Mod7 x) (Mod7 y) = lit $ x * y
  
testExp :: Expr a => Maybe a
testExp = parseExp lit add mul "(3 * -4) + 5"

--TODO: implement Ord on MinMax



class HasVars a where
  var :: String -> a
  
instance HasVars (M.Map String Integer -> Maybe Integer) where
  var = M.lookup

instance Expr (M.Map String Integer -> Maybe Integer) where
  lit = const . Just   
  add f1 f2 = (\x -> liftA2 (+) (f1 x) (f2 x))
  mul f1 f2 = (\x -> liftA2 (*) (f1 x) (f2 x)) 
  
withVars :: [(String, Integer)] -> (M.Map String Integer -> Maybe Integer) -> Maybe Integer
withVars vs exp = exp $ M.fromList vs
































