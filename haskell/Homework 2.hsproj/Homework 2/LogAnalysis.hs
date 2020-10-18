{-# OPTIONS_GHC -Wall #-}
module LogAnalysis where

import Log

parseList :: [String] -> LogMessage
parseList ("I" : timestamp: rest) = LogMessage Info (read timestamp) (unwords rest)  
parseList ("W" : timestamp: rest) = LogMessage Warning (read timestamp) (unwords rest)
parseList ("E" : severity: timestamp: rest) = LogMessage (Error (read severity)) (read timestamp) (unwords rest)
parseList stringList = Unknown (unwords stringList)

parseMessage :: String -> LogMessage
parseMessage s = parseList ( words s )


parse :: String -> [LogMessage]
parse fileContents = map parseMessage (lines fileContents)


getTimeStamp :: LogMessage -> TimeStamp
getTimeStamp (Unknown _) = undefined
getTimeStamp (LogMessage _ timeStamp _) = timeStamp


getMessageString :: LogMessage -> String
getMessageString (Unknown _) = undefined
getMessageString (LogMessage _ _ string) = string


insert :: LogMessage -> MessageTree -> MessageTree
insert (Unknown _) tree = tree
insert message Leaf = Node Leaf message Leaf
insert newMessage (Node leftTree nodeMessage rightTree)
  | (getTimeStamp newMessage) < (getTimeStamp nodeMessage) = Node (insert newMessage leftTree) nodeMessage rightTree
  | otherwise = Node leftTree nodeMessage (insert newMessage rightTree)
  

buildHelper :: [LogMessage] -> MessageTree -> MessageTree
buildHelper [] tree = tree
buildHelper (firstMessage: rest) tree = buildHelper rest (insert firstMessage tree)


build :: [LogMessage] -> MessageTree
--build messageList = buildHelper messageList Leaf
build messageList = foldr insert Leaf messageList


inOrder :: MessageTree -> [LogMessage]
inOrder Leaf = []
inOrder (Node leftTree message rightTree) = (inOrder leftTree)  ++ ( message : (inOrder rightTree) )


isSevere :: LogMessage -> Bool
isSevere (LogMessage (Error severity) _ _) = severity >= 50
isSevere _ = False


filterSevere :: [LogMessage] -> [LogMessage]
filterSevere = filter isSevere
--filterSevere [] = []
--filterSevere (firstMessage : rest)
--  | isSevere firstMessage = firstMessage : (filterSevere rest)
--  | otherwise = filterSevere rest


whatWentWrong :: [LogMessage] -> [String]
whatWentWrong messages = getMessageString `map` (inOrder (build (filterSevere messages)))



































