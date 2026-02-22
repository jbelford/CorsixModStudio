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
#include "frame/Construct.h"
#include "views/frmUCSSelector.h"
#include "presenters/CDpsCalculatorPresenter.h"
#include "presenters/CRefreshFilesPresenter.h"
#include "views/interfaces/IDpsCalculatorView.h"
#include "views/interfaces/IRefreshFilesView.h"
#include <memory>

class CLocaleTool : public ConstructFrame::ITool
{
  public:
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;
};

class CAESetupTool : public ConstructFrame::ITool
{
  public:
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;
};

class CUcsTool : public ConstructFrame::ITool
{
  public:
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;
    static void HandleSelectorResponse(frmUCSSelector *pSelector, wxAuiNotebook *pTabsDestination,
                                       unsigned long *pResult = 0, bool bRegisterTabStrip = false);
};

class CSgaPackerTool : public ConstructFrame::ITool
{
  public:
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;
};

class CExtractAllTool : public ConstructFrame::ITool
{
  public:
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;
};

class CDpsCalculatorTool : public ConstructFrame::ITool, private IDpsCalculatorView
{
  public:
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;

  private:
    // IDpsCalculatorView implementation
    void ShowProgress(const wxString &sMessage) override;
    void HideProgress() override;
    void ShowError(const wxString &sMessage) override;
    void OnDpsComplete() override;
    void DisableControls() override;
    void EnableControls() override;

    std::unique_ptr<CDpsCalculatorPresenter> m_pPresenter;
};

class CMakeLuaInheritTree : public ConstructFrame::ITool
{
  protected:
    static void _ForEach(IDirectoryTraverser::IIterator *pItr, void *pTag);
    void _DoLua(IDirectoryTraverser::IIterator *pItr);

  public:
    CMakeLuaInheritTree();
    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    static bool _DoesExist(const char *sFol);
    void Do(const char *sAttrib);
    void DoAction() override;
    CInheritTable *pTable;
    size_t iAttribL;
};

class CRefreshFilesTool : public ConstructFrame::ITool, private IRefreshFilesView
{
  public:
    ~CRefreshFilesTool() override;

    wxString GetName() override;
    wxString GetHelpString() override;
    wxBitmapBundle GetBitmapBundle(const wxSize &size) override;

    void DoAction() override;

  private:
    // IRefreshFilesView implementation
    void ShowLoading(const wxString &sMessage) override;
    void HideLoading() override;
    void UpdateProgress(const wxString &sMessage) override;
    void RefreshFileTree() override;
    void ShowError(const wxString &sMessage) override;

    std::unique_ptr<CRefreshFilesPresenter> m_pPresenter;
    frmLoading *m_pLoadingForm = nullptr;
};
