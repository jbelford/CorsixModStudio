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

#include "ModuleService.h"
#include "common/strconv.h"
#include <rainman/core/Exception.h>
#include <rainman/module/CFileMap.h>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/localization/CUcsFile.h>

// --- Lifecycle (static) ---

Result<CModuleFile *> ModuleService::LoadModuleFile(const wxString &sPath, pfnStatusCallback pfnCallback,
                                                    void *pCallbackTag)
{
    auto sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<CModuleFile *>::Err(wxT("Memory allocation error"));

    auto *pMod = new CModuleFile;
    if (!pMod)
    {
        return Result<CModuleFile *>::Err(wxT("Memory allocation error"));
    }

    try
    {
        pMod->LoadModuleFile(sFile.get(), pfnCallback, pCallbackTag);
    }
    catch (CRainmanException *pE)
    {
        delete pMod;
        return ResultFromExceptionT<CModuleFile *>(pE);
    }

    return Result<CModuleFile *>::Ok(pMod);
}

Result<CModuleFile *> ModuleService::LoadSgaAsMod(const wxString &sPath, pfnStatusCallback pfnCallback,
                                                  void *pCallbackTag)
{
    auto sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<CModuleFile *>::Err(wxT("Memory allocation error"));

    auto *pMod = new CModuleFile;
    if (!pMod)
    {
        return Result<CModuleFile *>::Err(wxT("Memory allocation error"));
    }

    try
    {
        pMod->LoadSgaAsMod(sFile.get(), pfnCallback, pCallbackTag);
    }
    catch (CRainmanException *pE)
    {
        delete pMod;
        return ResultFromExceptionT<CModuleFile *>(pE);
    }

    return Result<CModuleFile *>::Ok(pMod);
}

// --- Module operations ---

Result<void> ModuleService::SetLocale(const wxString &sLocale)
{
    auto sVal = wxStringToAscii(sLocale);
    if (!sVal)
        return Result<void>::Err(wxT("Memory allocation error"));

    try
    {
        m_pModule->SetLocale(sVal.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }

    return Result<void>::Ok();
}

Result<void> ModuleService::ReloadResources(unsigned long iReloadWhat, unsigned long iReloadWhatRequired,
                                            unsigned long iReloadWhatEngines, pfnStatusCallback pfnCallback,
                                            void *pCallbackTag)
{
    try
    {
        m_pModule->ReloadResources(iReloadWhat, iReloadWhatRequired, iReloadWhatEngines, pfnCallback, pCallbackTag);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }

    return Result<void>::Ok();
}

Result<void> ModuleService::SetMapPackRootFolder(const wxString &sFolder)
{
    m_pModule->SetMapPackRootFolder(sFolder.wc_str());
    return Result<void>::Ok();
}

// --- Module type ---

CModuleFile::eModuleType ModuleService::GetModuleType() const { return m_pModule->GetModuleType(); }

bool ModuleService::IsFauxModule() const { return m_pModule->IsFauxModule(); }

// --- String directive getters ---

static wxString SafeAsciiToWx(const char *s) { return s ? wxString::FromUTF8(s) : wxString(); }

wxString ModuleService::GetName() const { return SafeAsciiToWx(m_pModule->GetName()); }
wxString ModuleService::GetModFolder() const { return SafeAsciiToWx(m_pModule->GetModFolder()); }
wxString ModuleService::GetLocaleFolder() const { return SafeAsciiToWx(m_pModule->GetLocaleFolder()); }
wxString ModuleService::GetDescription() const { return SafeAsciiToWx(m_pModule->GetDescription()); }
wxString ModuleService::GetTextureFe() const { return SafeAsciiToWx(m_pModule->GetTextureFe()); }
wxString ModuleService::GetTextureIcon() const { return SafeAsciiToWx(m_pModule->GetTextureIcon()); }
wxString ModuleService::GetUiName() const { return SafeAsciiToWx(m_pModule->GetUiName()); }
wxString ModuleService::GetDllName() const { return SafeAsciiToWx(m_pModule->GetDllName()); }
wxString ModuleService::GetLocale() const { return SafeAsciiToWx(m_pModule->GetLocale()); }
wxString ModuleService::GetFileMapName() const { return SafeAsciiToWx(m_pModule->GetFileMapName()); }
wxString ModuleService::GetApplicationPath() const { return SafeAsciiToWx(m_pModule->GetApplicationPath()); }

// --- Numeric directive getters ---

long ModuleService::GetVersionMajor() const { return m_pModule->GetVersionMajor(); }
long ModuleService::GetVersionMinor() const { return m_pModule->GetVersionMinor(); }
long ModuleService::GetVersionRevision() const { return m_pModule->GetVersionRevision(); }
long ModuleService::GetSgaOutputVersion() const { return m_pModule->GetSgaOutputVersion(); }

// --- String directive setters ---

void ModuleService::SetDescription(const wxString &sValue)
{
    auto s = wxStringToAscii(sValue);
    if (s)
    {
        m_pModule->SetDescription(s.get());
    }
}

void ModuleService::SetTextureFe(const wxString &sValue)
{
    auto s = wxStringToAscii(sValue);
    if (s)
    {
        m_pModule->SetTextureFe(s.get());
    }
}

void ModuleService::SetTextureIcon(const wxString &sValue)
{
    auto s = wxStringToAscii(sValue);
    if (s)
    {
        m_pModule->SetTextureIcon(s.get());
    }
}

void ModuleService::SetUiName(const wxString &sValue)
{
    auto s = wxStringToAscii(sValue);
    if (s)
    {
        m_pModule->SetUiName(s.get());
    }
}

// --- Sub-object accessors ---

CFileMap *ModuleService::GetFileMap() const { return m_pModule->GetFileMap(); }

CFileSystemStore *ModuleService::GetFileSystem() const { return m_pModule->GetFileSystem(); }

// --- UCS accessors ---

size_t ModuleService::GetUcsCount() const { return m_pModule->GetUcsCount(); }

CModuleFile::CUcsHandler *ModuleService::GetUcs(size_t iId) const { return m_pModule->GetUcs(iId); }

const wchar_t *ModuleService::ResolveUCS(unsigned long iStringID) const { return m_pModule->ResolveUCS(iStringID); }

Result<void> ModuleService::NewUCS(const wxString &sName)
{
    auto s = wxStringToAscii(sName);
    if (!s)
        return Result<void>::Err(wxT("Memory allocation error"));

    try
    {
        m_pModule->NewUCS(s.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }

    return Result<void>::Ok();
}

Result<void> ModuleService::NewUCS(const wxString &sName, std::shared_ptr<CUcsFile> pUcs)
{
    auto s = wxStringToAscii(sName);
    if (!s)
        return Result<void>::Err(wxT("Memory allocation error"));

    try
    {
        m_pModule->NewUCS(s.get(), std::move(pUcs));
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }

    return Result<void>::Ok();
}

Result<bool> ModuleService::DirectoryExists(const wxString &sPath)
{
    auto s = wxStringToAscii(sPath);
    if (!s)
        return Result<bool>::Err(wxT("Memory allocation error"));

    bool bExists = m_pModule->VDirectoryExists(s.get());
    return Result<bool>::Ok(bExists);
}

// --- Required/Engine accessors ---

size_t ModuleService::GetRequiredCount() const { return m_pModule->GetRequiredCount(); }

const CModuleFile::CRequiredHandler *ModuleService::GetRequired(size_t iId) const
{
    return m_pModule->GetRequired(iId);
}

size_t ModuleService::GetEngineCount() const { return m_pModule->GetEngineCount(); }

const CModuleFile *ModuleService::GetEngine(size_t iId) const { return m_pModule->GetEngine(iId); }

// --- Archive accessors ---

size_t ModuleService::GetArchiveCount() const { return m_pModule->GetArchiveCount(); }

CModuleFile::CArchiveHandler *ModuleService::GetArchive(size_t iId) const { return m_pModule->GetArchive(iId); }
