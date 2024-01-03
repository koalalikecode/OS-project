#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <bits/stdc++.h>
#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
using namespace std;
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");
bool continueProc;

int _tmain()
{
   HANDLE hMapFile;
   LPCTSTR pBuf;
   char Msg[255];
   continueProc = true;

   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   szName);               // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not open file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }

   pBuf = (LPTSTR) MapViewOfFile(hMapFile, // handle to map object
               FILE_MAP_ALL_ACCESS,  // read/write permission
               0,
               0,
               BUF_SIZE);

   if (pBuf == NULL)
   {
      _tprintf(TEXT("Could not map view of file (%d).\n"),
             GetLastError());

      CloseHandle(hMapFile);

      return 1;
   }
   do {
      printf("Process A's message: %s\n",pBuf);
      getch();
      printf("Input your message to A: ");
      scanf("%255[^\n]",Msg);
      CopyMemory((PVOID)pBuf, Msg, (max(_tcslen(pBuf), _tcslen(Msg)) * sizeof(char)));
      printf("Message has sent!\n");
      getch();
      printf("Do you want to continue? 1.Yes; Any to cancel\n");
      int choice;
      cin >> choice;
      getchar();
      if (choice != 1) continueProc = false;
   } while(continueProc);

   UnmapViewOfFile(pBuf);
   CloseHandle(hMapFile);
   return 0;
}
