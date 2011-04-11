//***********************************************************************************************
//
//    Copyright (c) 1993-1997 Axon Instruments.
//    All rights reserved.
//    Permission is granted to freely to use, modify and copy the code in this file.
//
//***********************************************************************************************
//
// MODULE:  DOSPRNT.CPP
// PURPOSE: Provide page-pause and key-pause functionality for a DOS command
//          line interface.
//
#include "wincpp.hpp"
#include <dos.h>
#include <bios.h>
#include <signal.h>
#include <io.h>
#include <stdarg.h>

// Constants used to access BIOS functions.

#define VIDEO       0x10
#define GETMODE     0x0F
#define SETCUR      0x02
#define READCUR     0x03
#define WRITETTY    0x0E

// Global variables.
static BOOL g_bInitialized = FALSE;
static BOOL g_bRedirected  = FALSE;
static BOOL g_bPause       = FALSE;
static int  g_nScreenLines = 25;
static int  g_nScreenCols  = 80;
static char g_szBlanks[]   = "                                        ";
static char g_szContinue[] = "Press a key to continue (<Esc> to quit).";

static int g_nLines = 0;

static unsigned GetCursorPos(void)
{
   union REGS regs;
   regs.h.ah = GETMODE;            // get current video page in BH
   int86(VIDEO, &regs, &regs);
   regs.h.ah = READCUR;            // get current cursor location in DX
   int86(VIDEO, &regs, &regs);
   return(regs.x.dx);
}

static void SetCursorPos(int row, int col)
{
   union REGS regs;
   regs.h.ah = GETMODE;            // get current video page in BH
   int86(VIDEO, &regs, &regs);
   regs.h.ah = SETCUR;           
   regs.x.dx = col;                // set current cursor location in DX
   if (row != -1)                  // if row = -1, col = row<<8+col
      regs.h.dh = (char)row;
   int86(VIDEO, &regs, &regs);
}

static void OutText(char *s)
{
   union REGS regs;
   regs.h.ah = GETMODE;            // get current video page in BH
   int86(VIDEO, &regs, &regs);
   regs.h.ah = WRITETTY;          
   regs.h.bl = 7;                 
   while (*s)
   {
      regs.h.al = *s++;
      int86(VIDEO, &regs, &regs);
      if (regs.h.al == '\n')
      {
         regs.h.al = '\r';
         int86(VIDEO, &regs, &regs);
      }
   }
}

static void OutChar(char ch)
{
   union REGS regs;
   regs.h.ah = GETMODE;            // get current video page in BH
   int86(VIDEO, &regs, &regs);
   regs.h.ah = WRITETTY;          
   regs.h.bl = 7;                 
   regs.h.al = ch;
   int86(VIDEO, &regs, &regs);
}

static void GetScreenRowCol(int *pnScreenLines, int *pnScreenCols)
{
   char far *fpCols;
   char far *fpRows;
   fpCols = (char far *)(MK_FP(0x0040, 0x004A));
   fpRows = (char far *)(MK_FP(0x0040, 0x0084));

   *pnScreenCols  = *fpCols;
   *pnScreenLines = *fpRows;
}

static int inkey(void)     // Inputs a character using system level calls.
{
   return (_bios_keybrd( _KEYBRD_READ ) & 0xff );
}

static int khit(void)     // Inputs a character using system level calls.
{
   return (_bios_keybrd( _KEYBRD_READY ));
}

void DOS_Initialize(void)
{
   // disable default <Ctrl-C> processing, treat <Ctrl-C> as a normal key sequence.
   signal( SIGINT, SIG_IGN );

   // work out if the output is redirected to a file or an other device
   UINT rc1 = GetCursorPos();
   fputc(0xFF, stdout);
   UINT rc2 = GetCursorPos();
   g_bRedirected = (rc1 == rc2);

   if (isatty(fileno(stdout)))
      fputc('\b', stdout);
   else
      fseek(stdout, -1L, SEEK_CUR);

   GetScreenRowCol(&g_nScreenLines, &g_nScreenCols);
   g_bInitialized = TRUE;
}

static int ShowTextAndPause(char *szMsg)
{
   int ch;
   short sCol;
   unsigned rcSave;

   if (g_bRedirected)
      return(0);

   rcSave = GetCursorPos();
   sCol = (short)((g_nScreenCols - strlen(szMsg))/2);
   SetCursorPos(g_nScreenLines, sCol);
   OutText(szMsg);
   while (!khit());
   ch = inkey();
   SetCursorPos(g_nScreenLines, sCol);
   OutText(g_szBlanks);
   SetCursorPos(-1, rcSave);
   return(ch);
}

static void CheckForPause(void)
{
   int ch = 0;

   if (khit())
      ch = inkey();

   if (!g_bRedirected && ((ch == 19) || (ch == ' ') ||     // <Ctrl-S> or <Space>
       (g_bPause && (g_nLines == g_nScreenLines-1))) )       // full screen
   {
      ch = ShowTextAndPause(g_szContinue);
      g_nLines = 0;
   }
   if ((ch == 27) ||       // <Esc>
       (ch == 3))          // <Ctrl-C>
   {
      fputs("\n", stdout);
      exit(1);
   }
}

static void PutString(char *pszStr, int bCR)
{
   int rval;
   char  *pszBS;

   if (g_bRedirected)
   {
      rval = (bCR ? puts(pszStr) : fputs(pszStr, stdout));
      if (rval)
      {
         fputs("\nCannot write to stdout.\n", stderr);
         exit(-1);
      }
   }
   else
   {
      pszBS = strchr(pszStr, '\b');
      while (pszBS)
      {
         *pszBS = '\0';
         OutText(pszStr);
         fputchar('\b');
         pszStr = pszBS + 1;
         pszBS = strchr(pszStr, '\b');
      }
      OutText(pszStr);
      if (bCR)
         OutText("\r\n");
   }
}

int DOS_fputs(char *pszStart, FILE *pf)
{
   char *pszEnd;
   while ((pszEnd = strchr(pszStart, '\n')) != NULL)
   {
      *pszEnd = '\0';
      PutString(pszStart, TRUE);
      g_nLines++;
      CheckForPause();
      pszStart = pszEnd + 1;
   }
   PutString(pszStart, FALSE);
   return TRUE;
}

int cdecl DOS_fprintf(FILE *pf, char *fmt, ...)         /* printf with page pause */
{
   int rval;
   va_list args;
   char  buf[160];
   
   if (!g_bInitialized)
      return 0;
      
   va_start(args, fmt);
   rval = vsprintf(buf, fmt, args);
   DOS_fputs(buf, pf);
   return rval;
}

void DOS_SetPauseFlag(BOOL bPause)
{
   g_bPause = bPause;
   g_nLines = 0;
}
