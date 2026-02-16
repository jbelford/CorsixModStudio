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

#include <wx/string.h>
#include "Result.h"
#include <rainman/module/CModuleFile.h>

class CFileMap;
class CUcsFile;

//! Service adapter wrapping CModuleFile for the CDMS GUI layer.
/*!
    Translates between wxString/Result and Rainman char/CRainmanException
    conventions. Does NOT own the CModuleFile; ConstructFrame retains ownership.

    All methods that can throw CRainmanException return Result instead.
    String getters convert char to wxString at the boundary.
*/
class ModuleService
{
  public:
    ModuleService() : m_pModule(nullptr) {}
    explicit ModuleService(CModuleFile *pModule) : m_pModule(pModule) {}

    void SetModule(CModuleFile *pModule) { m_pModule = pModule; }
    CModuleFile *GetModule() const { return m_pModule; }
    bool HasModule() const { return m_pModule != nullptr; }

    // --- Lifecycle ---

    //! Create a new CModuleFile, load a .module file, and return it.
    static Result<CModuleFile *> LoadModuleFile(const wxString &sPath, pfnStatusCallback pfnCallback = nullptr,
                                                void *pCallbackTag = nullptr);

    //! Create a new CModuleFile, load an SGA as a faux module, and return it.
    static Result<CModuleFile *> LoadSgaAsMod(const wxString &sPath, pfnStatusCallback pfnCallback = nullptr,
                                              void *pCallbackTag = nullptr);

    // --- Module operations (require m_pModule set) ---

    Result<void> SetLocale(const wxString &sLocale);

    Result<void> ReloadResources(unsigned long iReloadWhat = CModuleFile::RR_All,
                                 unsigned long iReloadWhatRequired = CModuleFile::RR_All,
                                 unsigned long iReloadWhatEngines = CModuleFile::RR_All,
                                 pfnStatusCallback pfnCallback = nullptr, void *pCallbackTag = nullptr);

    Result<void> SetMapPackRootFolder(const wxString &sFolder);

    // --- Module type ---

    CModuleFile::eModuleType GetModuleType() const;
    bool IsFauxModule() const;

    // --- String directive getters (char* → wxString) ---

    wxString GetName() const;
    wxString GetModFolder() const;
    wxString GetLocaleFolder() const;
    wxString GetDescription() const;
    wxString GetTextureFe() const;
    wxString GetTextureIcon() const;
    wxString GetUiName() const;
    wxString GetDllName() const;
    wxString GetLocale() const;
    wxString GetFileMapName() const;
    wxString GetApplicationPath() const;

    // --- Numeric directive getters ---

    long GetVersionMajor() const;
    long GetVersionMinor() const;
    long GetVersionRevision() const;
    long GetSgaOutputVersion() const;

    // --- String directive setters (wxString → char*) ---

    void SetDescription(const wxString &sValue);
    void SetTextureFe(const wxString &sValue);
    void SetTextureIcon(const wxString &sValue);
    void SetUiName(const wxString &sValue);

    // --- Sub-object accessors (thin pass-through) ---

    CFileMap *GetFileMap() const;
    CFileSystemStore *GetFileSystem() const;

    // --- UCS accessors ---

    size_t GetUcsCount() const;
    CModuleFile::CUcsHandler *GetUcs(size_t iId) const;
    const wchar_t *ResolveUCS(unsigned long iStringID) const;
    Result<void> NewUCS(const wxString &sName);

    //! Create a new UCS entry with an existing CUcsFile object.
    Result<void> NewUCS(const wxString &sName, CUcsFile *pUcs);

    //! Check whether a directory exists in the module's virtual filesystem.
    Result<bool> DirectoryExists(const wxString &sPath);

    // --- Required/Engine accessors ---

    size_t GetRequiredCount() const;
    const CModuleFile::CRequiredHandler *GetRequired(size_t iId) const;
    size_t GetEngineCount() const;
    const CModuleFile *GetEngine(size_t iId) const;

    // --- Archive accessors ---

    size_t GetArchiveCount() const;
    CModuleFile::CArchiveHandler *GetArchive(size_t iId) const;

  private:
    CModuleFile *m_pModule;
};
