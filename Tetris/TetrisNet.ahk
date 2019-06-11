#NoEnv  ;Aploium
#SingleInstance Force
;#Warn
#Include AHKlib.ahk
#NoTrayIcon
SendMode Input
SetWorkingDir %A_ScriptDir%
SetBatchLines -1


argc = %0%
arga = %1%
argb = %2%



IfEqual,argc,0
{
   MsgBox 请不要直接运行本程序，请运行Tetris.exe
     ExitApp
}



if(not (arga<=100000 && arga>=0) )
  ExitApp

if(arga==0)
{
   FileAppend,a,doing.txt
   FileDelete, ScoreOnline.txt
   argpass = %2%
}
else
   argpass = %3%

if(arga==1)
{
   FileDelete,ScoreOnline.txt
   ExitApp
}


Random,rndnum,1,100000
cachefile := A_Temp . "\AploiumCache.txt"
countcache := A_Temp . "\AploiumCountCache.txt"
;cachefileB :=A_Temp . "\AploiumCache-" .  A_ComputerName .  "-" . rndnum . "txt"
FileDelete,%cachefile%



FTP_Init() 
FTP_Open("45.62.112.65","Tetris", argpass . "NY6h3725dNFzPun5")
FTP_GetFile("score.txt",cachefile,1)
FTP_GetFile("count.txt",countcache,1)
if(arga==0)
{
   FileMove,%countcache%,ScoreOnline.txt,1
   FileRead,data,%cachefile%
   FileAppend,`n%data%,ScoreOnline.txt 
   FileDelete,doing.txt
   ExitApp
  }
FTP_PutFile(cachefile, "score." . A_Mon . A_DD . A_Hour . A_Min .  A_Sec . ".txt", Flags=1) 

FileRead,cnt,%countcache%
cnt++
FormatTime,tme,%A_Now%, yyyy-MM-dd_HH:mm:ss
FileAppend,`n%arga%  %argb%  %tme%, %cachefile%
FileDelete,%countcache%
FileAppend,%cnt%,%countcache%
;FileMove,cachefile,cachefileB
FTP_PutFile(cachefile, "score.txt", Flags=1) 
FTP_PutFile(countcache, "count.txt", Flags=1) 
FileDelete,doing.txt
FTP_Close()

ExitApp
