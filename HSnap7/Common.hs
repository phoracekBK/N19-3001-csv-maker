module Common where

import Foreign.Ptr
import Foreign.C.Types
import Foreign.C.String
import qualified Data.Text as T
import Data.Int
import Data.Word
import GHC.Float
import Data.Endian
import Data.Bits
import Foreign.Storable
import Data.Text.Encoding
import qualified Data.ByteString as BS



class S7DB a where
      readFromDB :: DB -> IO a
      writeToDB :: a -> DB -> IO ()


data S7Object = S7Object
type S7       = Ptr S7Object
type DB       = Ptr CChar


newtype TOD = TOD Int
  deriving (Eq, Ord)


newtype Time = Time Int
  deriving (Eq, Ord)


data DTL   = DTL
        {year        :: Int
        , month      :: Int
        , day        :: Int
        , weekday    :: Int
        , hour       :: Int
        , minute     :: Int
        , second     :: Int
        , nanosecond :: Int}
          deriving (Show, Eq, Ord)


instance Show TOD where
    show (TOD a) =  "TOD#" ++ (showDigits toHour )++ ":" ++ (showDigits toMinute) ++ ":" ++ (showDigits toSecond)
      where
        toHour = div a (60*60*1000)
        toMinute = div (mod a (60*60*1000)) (60*1000)
        toSecond = mod (mod a (60*60*1000)) (60*1000)
        showDigits time | time >= 10 = show time
                        | otherwise  = "0" ++ show time


instance Show Time where
  show (Time a) = "T#" ++ show a ++ "ms"



dtlToSeconds :: DTL -> Integer
dtlToSeconds dtl =
  fromIntegral
  $ yearToSec
  + monthToSec
  + dayToSec
  + hourToSec
  + minutesToSec
  + (toInteger (second dtl))
  where
    yearToSec = round ((fromIntegral (year dtl) - 1970)  * (31556925.216 :: Double))
    monthToSec = round ((fromIntegral ((month dtl) * 86400)) * (30.4368499 :: Double))
    dayToSec = toInteger ((day dtl) * 86400)
    hourToSec = toInteger ((hour dtl) * 3600)
    minutesToSec = toInteger ((minute dtl) * 60)




readBool :: DB -> Int -> Int -> IO Bool
readBool db offset index =
  (\b -> testBit b index)
  <$> (peekByteOff db offset :: IO Word8)


writeBool :: DB -> Int -> Int -> Bool -> IO ()
writeBool db offset index value = do
  b <- peekByteOff db offset :: IO Word8
  pokeByteOff db offset (updateByte value b)
  where
    updateByte True b = setBit b index
    updateByte False b = clearBit b index


readInt :: DB -> Int -> IO Int
readInt db offset =
  fromIntegral
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Int16)


writeInt :: DB -> Int -> Int -> IO ()
writeInt db offset value = pokeByteOff db offset (fromBigEndian $ fromIntegral value :: Int16)


readUInt :: DB -> Int -> IO Int
readUInt db offset =
  fromIntegral
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Int16)


writeUInt :: DB -> Int -> Int -> IO ()
writeUInt db offset value = pokeByteOff db offset (fromBigEndian $ fromIntegral value :: Int16)


readDInt :: DB -> Int -> IO Int
readDInt db offset =
  fromIntegral
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Int32)


writeDInt :: DB -> Int -> Int -> IO ()
writeDInt db offset value = pokeByteOff db offset (fromBigEndian $ fromIntegral value :: Int32)


readUDInt :: DB -> Int -> IO Int
readUDInt db offset =
  fromIntegral
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Int32)


writeUDInt :: DB -> Int -> Int -> IO ()
writeUDInt db offset value = pokeByteOff db offset (fromBigEndian $ fromIntegral value :: Int32)


readByte :: DB -> Int -> IO Int
readByte db offset =
  fromIntegral
  <$> (peekByteOff db offset :: IO Word8)


writeByte :: DB -> Int -> Int -> IO ()
writeByte db offset value = pokeByteOff db offset (fromIntegral value :: Word8)


readChar :: DB -> Int -> IO Char
readChar db offset =
  toEnum
  <$> fromIntegral
  <$> (peekByteOff db offset :: IO CChar)


writeChar :: DB -> Int -> Char -> IO ()
writeChar db offset value = pokeByteOff db offset (fromIntegral $ fromEnum $ value :: CChar)


readWord :: DB -> Int -> IO Int
readWord db offset =
  fromIntegral
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Word16)


writeWord :: DB -> Int -> Int -> IO ()
writeWord db offset value = pokeByteOff db offset (fromBigEndian $ fromIntegral value :: Word16)


readDWord :: DB -> Int -> IO Int
readDWord db offset =
  fromIntegral
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Word32)


writeDWord :: DB -> Int -> Int -> IO ()
writeDWord db offset value = pokeByteOff db offset (fromBigEndian $ fromIntegral value :: Word32)


readUSInt :: DB -> Int -> IO Int
readUSInt db offset = fromIntegral <$> (peekByteOff db offset :: IO Word8)


writeUSInt :: DB -> Int -> Int -> IO ()
writeUSInt db offset value = pokeByteOff db offset (fromIntegral value :: Word8)


readReal :: DB -> Int -> IO Float
readReal db offset =
  castWord32ToFloat
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Word32)


writeReal :: DB -> Int -> Float -> IO ()
writeReal db offset value = pokeByteOff db offset (fromBigEndian $ castFloatToWord32 value)


readLReal :: DB -> Int -> IO Double
readLReal db offset =
  castWord64ToDouble
  <$> toBigEndian
  <$> (peekByteOff db offset :: IO Word64)


writeLRead :: DB -> Int -> Double -> IO ()
writeLRead db offset value = pokeByteOff db offset (fromBigEndian $ castDoubleToWord64 value)


readTOD :: DB -> Int -> IO TOD
readTOD db offset = TOD <$> (readDInt db offset)


writeTOD :: DB -> Int -> TOD -> IO ()
writeTOD db offset (TOD a) = writeDInt db offset a


readTime :: DB -> Int -> IO Time
readTime db offset = Time <$> readInt db offset


writeTime :: DB -> Int -> Time -> IO ()
writeTime db offset (Time a) = writeInt db offset a


readString :: DB -> Int -> IO String
readString db offset = peekCString (plusPtr db (offset + 2))


writeString :: DB -> Int -> Int -> String -> IO ()
writeString db offset range value = do
  writeByte db offset range
  writeByte db (offset + 1) (maxLength $ length value)
  pokeString 2 value
  where
    pokeString _ []     = return ()
    pokeString i (x:xs) = do
        writeChar db (offset+i) x
        pokeString (i+1) xs
    maxLength len | len >= range = range
                  | otherwise = len


readStringAsText :: DB -> Int -> IO T.Text
readStringAsText db offset =
  decodeUtf8
  <$> BS.packCString (plusPtr db (offset + 2))


writeStringAsText :: DB -> Int -> Int -> T.Text -> IO ()
writeStringAsText db offset range value = do
  writeByte db offset range
  writeByte db (offset + 1) (maxLength (T.length value))
  BS.useAsCString (encodeUtf8 (T.take range value)) (\str -> pokeByteOff db (offset + 2) str)
  where
    maxLength len | len >= range = range
                  | otherwise = len


readWStringAsText :: DB -> Int -> IO T.Text
readWStringAsText db offset =
  decodeUtf16BE
  <$> BS.packCString (plusPtr db (offset + 2))


writeWStringAsText :: DB -> Int -> Int -> T.Text -> IO ()
writeWStringAsText db offset range value = do
  writeByte db offset range
  writeByte db (offset + 1) (maxLength (T.length value))
  BS.useAsCString (encodeUtf16LE (T.take range value)) (\str -> pokeByteOff db (offset + 2) str)
  where
    maxLength len | len >= range = range
                  | otherwise = len


readDTL :: DB -> Int -> IO DTL
readDTL db offset = do
  yearValue       <- readUInt db offset
  monthValue      <- readUSInt db (offset + 2)
  dayValue        <- readUSInt db (offset + 3)
  weekdayValue    <- readUSInt db (offset + 4)
  hourValue       <- readUSInt db (offset + 5)
  minuteValue     <- readUSInt db (offset + 6)
  secondValue     <- readUSInt db (offset + 7)
  nanosecondValue <- readUDInt db (offset + 8)

  return DTL {year = yearValue
            , month = monthValue
            , day = dayValue
            , weekday = weekdayValue
            , hour = hourValue
            , minute = minuteValue
            , second = secondValue
            , nanosecond = nanosecondValue}


writeDTL :: DB -> Int -> DTL -> IO ()
writeDTL db offset value = do
  writeUInt db offset (year value)
  writeUSInt db (offset + 2) (month value)
  writeUSInt db (offset + 3) (day value)
  writeUSInt db (offset + 4) (weekday value)
  writeUSInt db (offset + 5) (hour value)
  writeUSInt db (offset + 6) (minute value)
  writeUSInt db (offset + 7) (second value)
  writeUDInt db (offset + 8) (nanosecond value)
