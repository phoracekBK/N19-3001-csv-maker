module PLC where

import HSnap7.Common


data PcInterface =
  PcInterface
    {
      success  :: Bool
      , failed :: Bool
    } deriving (Show)


data Glass =
  Glass
    {
      jobNr                       :: String
      , vehicleNumber             :: String
      , rearWindow                :: String
      , vehicleModel              :: Int
      , windowID                  :: Int
      , primerAppEnable           :: Bool
      , primerInspectionEnbale    :: Bool
      , primerApplicationTime     :: DTL
      , primerInspectionResult    :: Bool
      , primerDetectionZones      :: [Bool]
      , drawerIndex               :: Int
      , primerFlashoffTime        :: DTL
      , timeSinceLastDispense     :: DTL
      , glueStartApplicationTime  :: DTL
      , glueEndApplicationTime    :: DTL
      , metralightEn              :: Bool
      , glueApplicationResult     :: Bool
      , glueInspectionBypass      :: Bool
      , metralightZone            :: [Int]
      , assemblyTime              :: DTL
      , aBarrelExpiration         :: Bool
      , aBarrelBatchNumber        :: String
      , aBarrelSerialNumber       :: String
      , aAppliedGlueAmount        :: Float
      , pistolTemperatureMin      :: Int
      , pistolTempDuringApp       :: Float
      , pistolTemperatureMax      :: Int
      , aPotTemperatureMin        :: Int
      , aPotTempDuringApp         :: Float
      , aPotTemperatureMax        :: Int
      , bBarrelExpiration         :: Bool
      , bBarrelBatchNumber        :: String
      , bBarrelSerialNumber       :: String
      , bAppliedGlueAmount        :: Float
      , aApplicationRatio         :: Float
      , bApplicationRatio         :: Float
      , mixerTubeLife             :: Int
      , robotCompleteSuccess      :: Bool
      , dispenseCompleteSuccess   :: Bool
      , rotaryUniteCompleteSucces :: Bool
      , addhesiveProcessComplete  :: Bool
      , ambientHumidity           :: Float
      , ambientTemperature        :: Float
    } deriving Show

data PlcInterface =
  PlcInterface
    {
      storeRequest :: Bool
    } deriving (Show)


instance S7DB Glass where
      readFromDB db =
            Glass
            <$> readByteArray readChar 2 10
            <*> readByteArray readChar 28 13
            <*> readString db 42
            <*> readByte db 62
            <*> readUDInt db 64
            <*> readBool db 163 3
            <*> readBool db 163 2
            <*> readDTL db 74
            <*> readBool db 162 1
            <*> readPrimerDetektionZones 146
            <*> readInt db 148
            <*> readDTL db 86
            <*> readDTL db 134
            <*> readDTL db 98
            <*> readDTL db 110
            <*> readBool db 162 3
            <*> readBool db 162 2
            <*> readBool db 163 4
            <*> readByteArray readByte 196 12
            <*> readDTL db 122
            <*> readBool db 247 0
            <*> readString db 208
            <*> readString db 226
            <*> readReal db 180
            <*> readInt db 152
            <*> readReal db 172
            <*> readInt db 150
            <*> readInt db 156
            <*> readReal db 176
            <*> readInt db 156
            <*> readBool db 287 0
            <*> readString db 248
            <*> readString db 266
            <*> readReal db 184
            <*> readReal db 164
            <*> readReal db 168
            <*> readDInt db 158
            <*> readBool db 162 5
            <*> readBool db 162 6
            <*> readBool db 162 7
            <*> readBool db 163 0
            <*> readReal db 188
            <*> readReal db 192
        where

          readByteArray :: (DB -> Int -> IO a) -> Int -> Int -> IO [a]
          readByteArray _ _ 0      = return []
          readByteArray f offset n =
            (:) <$> f db offset <*> readByteArray f (offset + 1) (n-1)

          readPrimerDetektionZones :: Int -> IO [Bool]
          readPrimerDetektionZones offset = do
            bit1 <- readBool db offset 0
            bit2 <- readBool db offset 1
            bit3 <- readBool db offset 2
            bit4 <- readBool db offset 3

            return [bit1, bit2, bit3, bit4]


      writeToDB = undefined


instance S7DB PcInterface where
  readFromDB = undefined

  writeToDB interface db = do
    writeBool db 0 0 (success interface)
    writeBool db 0 1 (failed interface)

instance S7DB PlcInterface where
  readFromDB db =
    PlcInterface <$> readBool db 0 0

  writeToDB = undefined
