module Main where


import Data.Time.Clock
import Data.Time.Calendar
import System.Directory
import Control.Concurrent
import System.Environment
import HSnap7.Common
import qualified HSnap7.Client as Cli
import PLC
import CSV
--sdeka,srajče,sesqintan,sestrom,sklobása,spousta slavných slov se s snasa sps: sesqintan je spušten kolmo k zemi

data CycleState =
  InterfaceInit
  | ReadInterface
  | CheckFile
  | StoreCsvRecord String CSVRecord
  | StoreCsvRecordNewFile String CSVRecord
  | WriteResponse
  | LogAccident String
  | Reconnect
  deriving (Show)


type FSM s = s -> IO s


glassToCSVRecord :: Glass -> CSVRecord
glassToCSVRecord glass =
  CSVRecord [CSVItem (jobNr glass) "JobNummer" ""
            , CSVItem (vehicleNumber glass) "AuftragsNr" ""
            , CSVItem (rearWindow glass) "ScheibenType" ""
            , CSVItem (parseVehicleModel $ vehicleModel glass) "FahrzeugModell" ""
            , CSVItem (show $ windowID glass) "ScheibenNr" ""
            , CSVItem (showConditional (primerAppEnable glass) (primerApplicationTime glass) showDTL) "TS_PrimerAuftrag" "Datum / Uhrzeit"
            , CSVItem (showConditional (primerInspectionEnbale glass) (primerInspectionResult glass) show) "PrimerDetektiert" "true/false/NaN"
            , CSVItem (showConditional (primerInspectionEnbale glass) (primerDetectionZones glass) show) "PrimerDetektiertZones" "true/false/NaN"
            , CSVItem (showConditional (primerAppEnable glass) (primerFlashoffTime glass) showDTL) "TS_PrimerAbgetrocknet" "Datum / Uhrzeit"
            , CSVItem (showConditional (primerAppEnable glass) fromPrimerToGlueAppInterval show) "TI_PrimerAufgebrachtBisKleberaupe" "s"
            , CSVItem (show $ drawerIndex glass) "Lagerfach" ""
            , CSVItem (showDTL $ timeSinceLastDispense glass) "TS_LetzteSpuelungMischer" "Datum / Uhrzeit"
            , CSVItem (showDTL $ glueStartApplicationTime glass) "TS_KleberaupeStart" "Datum / Uhrzeit"
            , CSVItem (showDTL $ glueEndApplicationTime glass) "TS_KleberaupeFertig" "Datum / Uhrzeit"
            , CSVItem (showConditional (metralightEn glass) (glueApplicationResult glass) show) "Kleberaubenerkennung" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) (glueInspectionBypass glass) show) "Metralight-Ergebnis umgangen" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 0) showMetralightStatus) "MetralightZone1" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 1) showMetralightStatus) "MetralightZone2" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 2) showMetralightStatus) "MetralightZone3" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 3) showMetralightStatus) "MetralightZone4" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 4) showMetralightStatus) "MetralightZone5" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 5) showMetralightStatus) "MetralightZone6" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 6) showMetralightStatus) "MetralightZone7" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 7) showMetralightStatus) "MetralightZone8" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 8) showMetralightStatus) "MetralightZone9" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 9) showMetralightStatus) "MetralightZone10" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 10) showMetralightStatus) "MetralightZone11" "true/false/NaN"
            , CSVItem (showConditional (metralightEn glass) ((metralightZone glass) !! 11) showMetralightStatus) "MetralightZone12" "true/false/NaN"
            , CSVItem (show $ fromGlueAppToAssemblingInterval) "TI_KleberaupeFertigBisScheibeEndnommen" "s"
            , CSVItem (show $ aBarrelExpiration glass) "KomponenteA_Unabgelaufen" "true/false"
            , CSVItem (aBarrelBatchNumber glass) "KomponenteA_BatchId" ""
            , CSVItem (aBarrelSerialNumber glass) "KomponenteA_SerienNr" ""
            , CSVItem (show $ aAppliedGlueAmount glass) "KomponenteA_Menge" "ml"
            , CSVItem (show $ pistolTemperatureMin glass) "AppizierdueseTempMin" "°C"
            , CSVItem (show $ pistolTempDuringApp glass) "AppizierdueseTempAktuell" "°C"
            , CSVItem (show $ pistolTemperatureMax glass) "AppizierdueseTempMax" "°C"
            , CSVItem (show $ aPotTemperatureMin glass) "KomponenteA_TempMin" "°C"
            , CSVItem (show $ aPotTempDuringApp glass) "KomponenteA_TempAktuell" "°C"
            , CSVItem (show $ aPotTemperatureMax glass) "KomponenteA_TempMax" "°C"
            , CSVItem (show $ bBarrelExpiration glass) "KomponenteB_Unabgelaufen" "true/false"
            , CSVItem (bBarrelBatchNumber glass) "KomponenteB_BatchId" ""
            , CSVItem (bBarrelSerialNumber glass) "KomponenteB_SerienNr" ""
            , CSVItem (show $ bAppliedGlueAmount glass) "KomponenteB_Menge" "ml"
            , CSVItem ((show $ aApplicationRatio glass) ++ ":"++ (show $ bApplicationRatio glass)) "MischungsverhaeltnisKomponenten" ""
            , CSVItem (show $ mixerTubeLife glass) "MischerrohrLebensdauerVerbleibend" "s"
            , CSVItem (show $ robotCompleteSuccess glass) "RoboterZyklusOhneFehler" "true/false"
            , CSVItem (show $ dispenseCompleteSuccess glass) "DosiereinheitOhneFehler" "true/false"
            , CSVItem (show $ rotaryUniteCompleteSucces glass) "DrehtischOhneFehler" "true/false"
            , CSVItem (show $ addhesiveProcessComplete glass) "KleberaupenauftragOhneFehler" "true/false"]
    where
      fromPrimerToGlueAppInterval = (showTimeInterval (primerApplicationTime glass) (glueStartApplicationTime glass))

      fromGlueAppToAssemblingInterval = (showTimeInterval (glueEndApplicationTime glass) (assemblyTime glass))

      parseVehicleModel :: Int -> String
      parseVehicleModel 1 = "ID.BUZZ"
      parseVehicleModel 7 = "T7"
      parseVehicleModel _ = "NaN"

      showMetralightStatus :: Int -> String
      showMetralightStatus 0x10 = "OK"
      showMetralightStatus 0x20 = "NOK"
      showMetralightStatus _    = "NaN"

      showDTL :: DTL -> String
      showDTL dtl =
        show  (year dtl)
        ++ "-"
        ++ show (month dtl)
        ++ "-"
        ++ show (day dtl)
        ++ " "
        ++ show (hour dtl)
        ++ ":"
        ++ show (minute dtl)
        ++ ":"
        ++ show (second dtl)

      showTimeInterval :: DTL -> DTL -> Integer
      showTimeInterval f t = ((dtlToSeconds f) - (dtlToSeconds t))

      showConditional :: Bool -> a -> (a -> String) -> String
      showConditional True a f = f a
      showConditional False _ _ = "NaN"


fsmHandler :: String -> S7 -> FSM CycleState
fsmHandler _ client ReadInterface                 = callbackReadInterface client
fsmHandler diskPath client CheckFile                     = callbackCheckFile diskPath client
fsmHandler _ _ (StoreCsvRecord path glass)        = callbackStoreCsvRecord path glass
fsmHandler _ _ (StoreCsvRecordNewFile path glass) = callbackStoreCsvRecordNewFile path glass
fsmHandler _ client (WriteResponse)               = callbackWriteResponse client
fsmHandler _ _ (LogAccident msg)                  = callbackLogAccident msg
fsmHandler _ client InterfaceInit                 = callbackInterfaceInit client
fsmHandler _ _ Reconnect                          = return Reconnect


callbackReadInterface :: S7 -> IO CycleState
callbackReadInterface client =
  threadDelay 500000
  >> Cli.writeData responseInit 18 288 1 client
  >> Cli.readData 18 0 1 client
  >>= transition
  where
    transition (Just (PlcInterface True))  = return CheckFile
    transition (Just (PlcInterface False)) = return ReadInterface
    transition Nothing                     = return (LogAccident "PLC interface read Error")

    responseInit = PcInterface False False


callbackCheckFile :: String -> S7 -> IO CycleState
callbackCheckFile diskPath client = do
  path   <- fileName
  exists <- doesFileExist path
  glass  <- Cli.readData 18 0 288 client

  transition glass path exists
  where
    transition Nothing _ _             = return (LogAccident "Glass record read Error")
    transition (Just glass) path True  = return (StoreCsvRecord path (glassToCSVRecord glass))
    transition (Just glass) path False = return (StoreCsvRecordNewFile path (glassToCSVRecord glass))

    fileName :: IO String
    fileName = getCurrentTime >>= return . toGregorian . utctDay >>= buildFileName
      where
        buildFileName (y, m, d) = return (diskPath ++ "Klebezelle-" ++ show y ++ "-" ++ show m ++ "-"++ show d ++ ".csv")


callbackStoreCsvRecord :: String -> CSVRecord -> IO CycleState
callbackStoreCsvRecord path record =
  appendFile path ("\n" ++ (buildCSVLine record value ";"))
  >> putStrLn ("CSV record stored to -> " ++ path)
  >> return WriteResponse


callbackStoreCsvRecordNewFile :: String -> CSVRecord -> IO CycleState
callbackStoreCsvRecordNewFile path record =
  appendFile path ((buildCSVLine record label ";") ++ "\n")
  >> appendFile path ((buildCSVLine record unit ";") ++ "\n")
  >> appendFile path (buildCSVLine record value ";")
  >> putStrLn ("CSV record stored to -> " ++ path)
  >> return WriteResponse


callbackWriteResponse :: S7 -> IO CycleState
callbackWriteResponse client =
  Cli.writeData responseSuccess 18 288 1 client >> return InterfaceInit
  where
    responseSuccess = PcInterface True False


callbackLogAccident :: String -> IO CycleState
callbackLogAccident msg = putStrLn msg >> return InterfaceInit


callbackInterfaceInit :: S7 -> IO CycleState
callbackInterfaceInit client =
  threadDelay 500000
  >> Cli.readData 18 0 1 client
  >>= transition
    where
      transition (Just (PlcInterface False)) = return ReadInterface
      transition (Just (PlcInterface True))  = return InterfaceInit
      transition _                           = return Reconnect


runFSM :: String -> S7 -> FSM CycleState
runFSM diskPath client state = do
  fsmHandler diskPath client state
  >>= nextCycle
  where
    nextCycle Reconnect = return Reconnect
    nextCycle nextState = runFSM diskPath client nextState



mainLoop :: String -> S7 -> IO ()
mainLoop diskPath client =
  runFSM diskPath client ReadInterface
  >> return ()


program :: String -> IO ()
program diskPath =
  Cli.withS7Client "192.168.2.1" 0 1 (mainLoop diskPath)
  >> program diskPath

main :: IO ()
main = do
  putStrLn "Program running: "
  args <- getArgs

  if args == [] then
    program ""
  else
    program (args !! 0)
