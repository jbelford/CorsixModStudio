/*
    This file is part of Corsix's Mod Studio.

    Corsix's Mod Studio is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Corsix's Mod Studio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corsix's Mod Studio; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <list>
#include <string>
#include <vector>

/// A SCAR API function or constant loaded from a binary reference file.
struct ScarFunctionDef
{
    std::string sReturn;
    std::string sName;
    std::list<std::string> sArguments;
    std::string sDesc;
    int iType = 0; // 0 = function, 1 = constant
};

/// Load SCAR API function/constant definitions from a binary reference file.
/// @param pFilePath  Path to the .ref file (e.g. scar.ref or coh_scar.ref).
/// @returns Loaded definitions, or empty vector if the file cannot be opened.
std::vector<ScarFunctionDef> LoadScarReference(const wchar_t *pFilePath);
