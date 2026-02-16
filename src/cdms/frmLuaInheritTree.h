#pragma once
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
// ----------------------------
#include <rainman/lua/CInheritTable.h>
#include <wx/treectrl.h>

class frmLuaInheritTree : public wxWindow
{
  protected:
    wxTreeCtrl *m_pTree;
    CInheritTable *m_pInheritTable;
    bool bFirstActivate;

    void _AddChildren(const wxTreeItemId &oParent, CInheritTable::CNode *pParent);

  public:
    frmLuaInheritTree(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition,
                      const wxSize &size = wxDefaultSize);
    ~frmLuaInheritTree();

    void OnSize(wxSizeEvent &event);
    void OnTreeActivate(wxTreeEvent &event);
    void OnTreeTooltip(wxTreeEvent &event);
    void OnTreeExpanding(wxTreeEvent &event);
    void OnTreeRightClick(wxTreeEvent &event);

    void OnActivated();

    enum
    {
        IDC_LuaTree = wxID_HIGHEST + 1,
    };

    DECLARE_EVENT_TABLE()
};
