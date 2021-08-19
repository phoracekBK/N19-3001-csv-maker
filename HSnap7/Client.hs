{--
-- TODO:
-- finish binding to low level c api functions
-- finish higher level interface function
-- unify api names
-- reverse endianity for flow point data types
-- finish reading and writing function for string from byte array
-- create documentation
-- treat against memory range error during reading and writing data
-- treat thread safety
--}

module Client where

import Foreign.Ptr
import Foreign.C.Types
import Foreign.C.String
import Foreign.Storable
import Foreign.Marshal.Alloc

import Common

-- snap7. high level functions --


-- TODO: measure against connection error
withS7Client :: String -> Int -> Int -> (S7 -> IO ()) -> IO ()
withS7Client address rack slot connSuccessAction = do
    client <- create
    connStatus <- withCString address $ (\cstr -> connectTo client cstr (fromIntegral rack) (fromIntegral slot))
    withConnection connStatus client
    where
      withConnection 0 client = do
          rv <- connSuccessAction client
          _ <- disconnect client
          destroyS7Ptr client
          return rv
      withConnection _ client = destroyS7Ptr client


destroyS7Ptr :: S7 -> IO ()
destroyS7Ptr client = alloca (\ptr -> do
                                poke ptr client
                                destroy ptr)


readData :: (Show a, S7DB a) => Int -> Int -> Int -> S7 -> IO (Maybe a)
readData dbNumber start dbSize client = allocaBytes dbSize readBytes
  where
    readBytes bytearray = do
      ret <- dbRead client (fromIntegral dbNumber) (fromIntegral start) (fromIntegral dbSize) bytearray
      if ret == 0
      then
        do
        self <- readFromDB bytearray
        return (Just self)
      else
        return Nothing


writeData :: (S7DB a) => a -> Int -> Int -> Int -> S7 -> IO (Bool)
writeData self dbNumber start dbSize client = allocaBytes dbSize writeBytes
  where
    writeBytes bytearray = do
      writeToDB self bytearray
      ret <- dbWrite client (fromIntegral dbNumber) (fromIntegral start) (fromIntegral dbSize) bytearray
      if ret == 0
      then
        return True
      else
        return False


---------------------- snap7.h c api binding  -------------------

foreign import ccall "snap7.h Cli_Create"
    create :: IO S7


foreign import ccall "snap7.h Cli_Destroy"
    destroy :: Ptr S7 -> IO ()


foreign import ccall "snap7.h Cli_ConnectTo"
    connectTo :: S7 -> CString -> CInt -> CInt -> IO CInt


foreign import ccall "snap7.h Cli_Disconnect"
    disconnect :: S7 -> IO CInt


foreign import ccall "snap7.h Cli_SetConnectionParams"
    setConnectionParams :: S7 -> CString -> Word -> Word -> IO CInt


foreign import ccall "snap7.h Cli_SetConnectionType"
    setConnectionType :: S7 -> Word -> IO CInt


foreign import ccall "snap7.h Cli_Connect"
    connect :: S7 -> IO CInt


foreign import ccall "snap7.h Cli_GetParam"
    getParam :: S7 -> CInt -> Ptr a -> IO CInt


foreign import ccall "snap7.h Cli_SetParam"
    setParam :: S7 -> CInt -> Ptr a -> IO CInt



-- Data I/O main functions

foreign import ccall "snap7.h Cli_ReadArea"
    readArea :: S7 -> CInt -> CInt -> CInt -> CInt -> CInt -> DB -> IO CInt


foreign import ccall "snap7.h Cli_WriteArea"
    writeArea :: S7 -> CInt -> CInt -> CInt -> CInt -> CInt -> DB -> IO CInt


-- Data I/O Lean functions

foreign import ccall "snap7.h Cli_DBRead"
    dbRead :: S7 -> CInt -> CInt -> CInt -> DB -> IO CInt


foreign import ccall "snap7.h Cli_DBWrite"
    dbWrite :: S7 -> CInt -> CInt -> CInt -> DB -> IO CInt




{-- TODO
int S7API Cli_SetAsCallback(S7Object Client, pfn_CliCompletion pCompletion, void *usrPtr);
// Data I/O main functions
int S7API Cli_ReadMultiVars(S7Object Client, PS7DataItem Item, int ItemsCount);
int S7API Cli_WriteMultiVars(S7Object Client, PS7DataItem Item, int ItemsCount);
// Data I/O Lean functions
int S7API Cli_MBRead(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_MBWrite(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_EBRead(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_EBWrite(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_ABRead(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_ABWrite(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_TMRead(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_TMWrite(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_CTRead(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_CTWrite(S7Object Client, int Start, int Amount, void *pUsrData);
// Directory functions
int S7API Cli_ListBlocks(S7Object Client, TS7BlocksList *pUsrData);
int S7API Cli_GetAgBlockInfo(S7Object Client, int BlockType, int BlockNum, TS7BlockInfo *pUsrData);
int S7API Cli_GetPgBlockInfo(S7Object Client, void *pBlock, TS7BlockInfo *pUsrData, int Size);
int S7API Cli_ListBlocksOfType(S7Object Client, int BlockType, TS7BlocksOfType *pUsrData, int *ItemsCount);
// Blocks functions
int S7API Cli_Upload(S7Object Client, int BlockType, int BlockNum, void *pUsrData, int *Size);
int S7API Cli_FullUpload(S7Object Client, int BlockType, int BlockNum, void *pUsrData, int *Size);
int S7API Cli_Download(S7Object Client, int BlockNum, void *pUsrData, int Size);
int S7API Cli_Delete(S7Object Client, int BlockType, int BlockNum);
int S7API Cli_DBGet(S7Object Client, int DBNumber, void *pUsrData, int *Size);
int S7API Cli_DBFill(S7Object Client, int DBNumber, int FillChar);
// Date/Time functions
int S7API Cli_GetPlcDateTime(S7Object Client, tm *DateTime);
int S7API Cli_SetPlcDateTime(S7Object Client, tm *DateTime);
int S7API Cli_SetPlcSystemDateTime(S7Object Client);
// System Info functions
int S7API Cli_GetOrderCode(S7Object Client, TS7OrderCode *pUsrData);
int S7API Cli_GetCpuInfo(S7Object Client, TS7CpuInfo *pUsrData);
int S7API Cli_GetCpInfo(S7Object Client, TS7CpInfo *pUsrData);
int S7API Cli_ReadSZL(S7Object Client, int ID, int Index, TS7SZL *pUsrData, int *Size);
int S7API Cli_ReadSZLList(S7Object Client, TS7SZLList *pUsrData, int *ItemsCount);
// Control functions
int S7API Cli_PlcHotStart(S7Object Client);
int S7API Cli_PlcColdStart(S7Object Client);
int S7API Cli_PlcStop(S7Object Client);
int S7API Cli_CopyRamToRom(S7Object Client, int Timeout);
int S7API Cli_Compress(S7Object Client, int Timeout);
int S7API Cli_GetPlcStatus(S7Object Client, int *Status);
// Security functions
int S7API Cli_GetProtection(S7Object Client, TS7Protection *pUsrData);
int S7API Cli_SetSessionPassword(S7Object Client, char *Password);
int S7API Cli_ClearSessionPassword(S7Object Client);
// Low level
int S7API Cli_IsoExchangeBuffer(S7Object Client, void *pUsrData, int *Size);
// Misc
int S7API Cli_GetExecTime(S7Object Client, int *Time);
int S7API Cli_GetLastError(S7Object Client, int *LastError);
int S7API Cli_GetPduLength(S7Object Client, int *Requested, int *Negotiated);
int S7API Cli_ErrorText(int Error, char *Text, int TextLen);
// 1.1.0
int S7API Cli_GetConnected(S7Object Client, int *Connected);
//------------------------------------------------------------------------------
//  Async functions
//------------------------------------------------------------------------------
int S7API Cli_AsReadArea(S7Object Client, int Area, int DBNumber, int Start, int Amount, int WordLen, void *pUsrData);
int S7API Cli_AsWriteArea(S7Object Client, int Area, int DBNumber, int Start, int Amount, int WordLen, void *pUsrData);
int S7API Cli_AsDBRead(S7Object Client, int DBNumber, int Start, int Size, void *pUsrData);
int S7API Cli_AsDBWrite(S7Object Client, int DBNumber, int Start, int Size, void *pUsrData);
int S7API Cli_AsMBRead(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_AsMBWrite(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_AsEBRead(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_AsEBWrite(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_AsABRead(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_AsABWrite(S7Object Client, int Start, int Size, void *pUsrData);
int S7API Cli_AsTMRead(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_AsTMWrite(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_AsCTRead(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_AsCTWrite(S7Object Client, int Start, int Amount, void *pUsrData);
int S7API Cli_AsListBlocksOfType(S7Object Client, int BlockType, TS7BlocksOfType *pUsrData, int *ItemsCount);
int S7API Cli_AsReadSZL(S7Object Client, int ID, int Index, TS7SZL *pUsrData, int *Size);
int S7API Cli_AsReadSZLList(S7Object Client, TS7SZLList *pUsrData, int *ItemsCount);
int S7API Cli_AsUpload(S7Object Client, int BlockType, int BlockNum, void *pUsrData, int *Size);
int S7API Cli_AsFullUpload(S7Object Client, int BlockType, int BlockNum, void *pUsrData, int *Size);
int S7API Cli_AsDownload(S7Object Client, int BlockNum, void *pUsrData, int Size);
int S7API Cli_AsCopyRamToRom(S7Object Client, int Timeout);
int S7API Cli_AsCompress(S7Object Client, int Timeout);
int S7API Cli_AsDBGet(S7Object Client, int DBNumber, void *pUsrData, int *Size);
int S7API Cli_AsDBFill(S7Object Client, int DBNumber, int FillChar);
int S7API Cli_CheckAsCompletion(S7Object Client, int *opResult);
int S7API Cli_WaitAsCompletion(S7Object Client, int Timeout);
--}
