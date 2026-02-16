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

#ifndef _C_MODULE_PARSER_H_
#define _C_MODULE_PARSER_H_

#include "rainman/module/CModuleMetadata.h"
#include <string>
#include <vector>

//! A parsed (number, name) pair from a .module file directive.
struct ParsedEntry
{
    signed long iNumber = 0;
    std::string sName;
};

//! A parsed CoH data source section from a .module file.
struct ParsedDataSource
{
    std::string sToc;
    std::string sOption;
    signed long iNumber = 0;
    std::string sFolder;
    std::vector<ParsedEntry> archives;
};

//! Result of parsing a .module file.
struct CModuleParseResult
{
    CModuleMetadata metadata;
    int iModuleType = 0; // Maps to CModuleFile::eModuleType (0=DoW, 1=CoHEarly, 2=CoH)
    std::vector<ParsedEntry> folders;
    std::vector<ParsedEntry> archives;
    std::vector<ParsedEntry> requireds;
    std::vector<ParsedEntry> compatibles;
    std::vector<ParsedDataSource> dataSources;
};

//! Stateless utility that extracts INI parsing logic from CModuleFile::LoadModuleFile().
class CModuleParser
{
  public:
    //! Parse a .module file and return the structured result.
    /*!
        \param sFileName Path to the .module file to parse.
        \return The parse result containing metadata, module type, and all parsed entries.
        \throws CRainmanException on I/O errors or if the file is not a valid .module file.
    */
    static CModuleParseResult Parse(const char *sFileName);
};

#endif
