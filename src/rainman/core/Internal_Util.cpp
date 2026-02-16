/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "rainman/core/Internal_Util.h"
#include "rainman/core/Exception.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void Util_strtolower(char *sStr)
{
    while (*sStr)
    {
        *sStr = tolower(*sStr);
        ++sStr;
    }
}

char *Util_fgetline(FILE *f, unsigned int iInitSize)
{
    unsigned int iTotalLen;
    if (f == nullptr)
        return nullptr;
    if (iInitSize < 4)
        iInitSize = 4;
    iTotalLen = iInitSize;
    char *sBuffer = new char[iInitSize];
    if (sBuffer == nullptr)
        return nullptr;
    sBuffer[0] = '\0';
    char *sReadTo = sBuffer;

    do
    {
        if (fgets(sReadTo, iInitSize, f) == nullptr)
        {
            if (feof(f))
            {
                size_t len = strlen(sBuffer);
                if (len > 0 && sBuffer[len - 1] == '\n')
                    sBuffer[len - 1] = 0;
                return sBuffer;
            }
            delete[] sBuffer;
            return nullptr;
        }
        if (sReadTo[strlen(sReadTo) - 1] == '\n')
        {
            sReadTo[strlen(sReadTo) - 1] = 0;
            return sBuffer;
        }
        iTotalLen += iInitSize;
        char *sTmp = new char[iTotalLen];
        if (sTmp == nullptr)
        {
            delete[] sBuffer;
            return nullptr;
        }
        strcpy(sTmp, sBuffer);
        delete[] sBuffer;
        sBuffer = sTmp;
        sReadTo = sBuffer + strlen(sBuffer);
    } while (1);
}

char *Util_mystrdup(const char *sStr)
{
    char *s = new char[strlen(sStr) + 1];
    if (s)
        strcpy(s, sStr);
    return s;
}

void Util_TrimWhitespace(char **pStringPointer)
{
    char *sBegin = *pStringPointer;
    while (*sBegin == ' ' || *sBegin == '\t' || *sBegin == 10 || *sBegin == 13)
        ++sBegin;
    *pStringPointer = sBegin;
    size_t iL = strlen(sBegin);
    if (iL)
    {
        --iL;
        while (sBegin[iL] == ' ' || sBegin[iL] == '\t' || sBegin[iL] == 10 || sBegin[iL] == 13)
        {
            sBegin[iL] = 0;
            --iL;
        }
    }
}

void Util_ForEach(IDirectoryTraverser::IIterator *pDirectory, Util_ForEachFunction pFn, void *pTag, bool bRecursive)
{
    try
    {
        // Check if directory has anything in it (blank directories can either be null pointers or T_Nothing so accodate
        // for both)
        if (pDirectory && pDirectory->VGetType() != IDirectoryTraverser::IIterator::T_Nothing)
        {
            do
            {
                if (pDirectory->VGetType() == IDirectoryTraverser::IIterator::T_File)
                {
                    pFn(pDirectory, pTag);
                }
                else if (bRecursive)
                {
                    IDirectoryTraverser::IIterator *pSub = nullptr;
                    try
                    {
                        pSub = pDirectory->VOpenSubDir();
                        Util_ForEach(pSub, pFn, pTag, true);
                    }
                    catch (CRainmanException *pE)
                    {
                        delete pSub;
                        throw new CRainmanException(pE, __FILE__, __LINE__, "Error iterating sub-directory");
                    }
                    delete pSub;
                }
            } while (pDirectory->VNextItem() == IDirectoryTraverser::IIterator::E_OK);
        }
    }
    CATCH_THROW("Error") // Error could have been many causes, so only a generic message can be given
}

void Util_EnsureEndsWith(char *sStr, char cChar)
{
    size_t iL = strlen(sStr) - 1;
    if (sStr[iL] != cChar)
    {
        sStr[iL] = cChar;
        sStr[++iL] = 0;
    }
}

void Util_EnsureEndsWith(char *sStr, char cChar, char cChar2)
{
    size_t iL = strlen(sStr) - 1;
    if (sStr[iL] != cChar && sStr[iL] != cChar2)
    {
        sStr[iL] = cChar;
        sStr[++iL] = 0;
    }
}
