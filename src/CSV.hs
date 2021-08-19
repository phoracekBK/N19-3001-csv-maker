module CSV where

import Data.List

data CSVItem =
  CSVItem
  {
    value :: String
    , label :: String
    , unit :: String
  } deriving (Show)


newtype CSVRecord =
  CSVRecord [CSVItem]
    deriving (Show)



buildCSVLine :: CSVRecord -> (CSVItem -> String) -> String -> String
buildCSVLine (CSVRecord record) f delimiter =
  mconcat
  $ intersperse delimiter (map (\item -> f item) record)
