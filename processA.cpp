#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <bits/stdc++.h>
#include <limits>
using namespace std;

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\MyFileMappingObject");
TCHAR szMsg[]=TEXT("Hi, this is message from A: ");
bool continueProc;

int _tmain()
{
   HANDLE hMapFile, hSemaphore;
   LPCTSTR pBuf;
   continueProc = true;

   // Create the semaphore
    hSemaphore = CreateSemaphore(NULL, 1, 1, TEXT("processSemaphore"));
    if (hSemaphore == NULL) {
        _tprintf(TEXT("Could not create semaphore (%d).\n"), GetLastError());
        return 1;
    }

   hMapFile = CreateFileMapping(
                 INVALID_HANDLE_VALUE,    // use paging file
                 NULL,                    // default security
                 PAGE_READWRITE,          // read/write access
                 0,                       // maximum object size (high-order DWORD)
                 BUF_SIZE,                // maximum object size (low-order DWORD)
                 szName);                 // name of mapping object

   if (hMapFile == NULL)
   {
      _tprintf(TEXT("Could not create file mapping object (%d).\n"),
             GetLastError());
      return 1;
   }
   pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                        FILE_MAP_ALL_ACCESS, // read/write permission
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
      // Acquire the semaphore before accessing shared memory
      printf("Waiting...\n");
      DWORD dwWaitResult = WaitForSingleObject(hSemaphore, INFINITE);
      // discards the input buffer
       if(cin.fail()){
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max());
      }

      printf("Input your message to B: ");
      scanf("%255[^\n]",szMsg);
      CopyMemory((PVOID)pBuf, szMsg, (max(_tcslen(pBuf), _tcslen(szMsg)) * sizeof(TCHAR)));
        
      printf("Message has sent! Wait for B...\n");
      // Release the semaphore
      ReleaseSemaphore(hSemaphore, 1, NULL);

       _getch();

      printf("Waiting...\n");
      WaitForSingleObject(hSemaphore, INFINITE);
      // discards the input buffer
       if(cin.fail()){
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max());
      }

      printf("Process B's message: %s\n",pBuf);
      ReleaseSemaphore(hSemaphore, 1, NULL);
       _getch();
      printf("Do you want to continue? 1.Yes; Any to cancel\n");
      int choice;
      cin >> choice;
      getchar();
      if (choice != 1) continueProc = false;
   } while (continueProc);
   
   UnmapViewOfFile(pBuf);

   CloseHandle(hMapFile);

   return 0;
}
