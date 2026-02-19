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

#pragma once

#include "rainman/core/gnuc_defines.h"
#include "rainman/io/IDirectoryTraverser.h"
#include "rainman/core/Api.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

class RAINMAN_API CDoWFileView : public IFileStore, public IDirectoryTraverser
{
  protected:
    struct _VirtFolder;
    struct _VirtFile;
    static bool _SortFolds(const std::unique_ptr<_VirtFolder> &a, const std::unique_ptr<_VirtFolder> &b);
    static bool _SortFiles(const std::unique_ptr<_VirtFile> &a, const std::unique_ptr<_VirtFile> &b);

  public:
    //! Basic constructor
    CDoWFileView(void);

    //! Basic destructor
    ~CDoWFileView(void);

    // IFileStore Interface
    //! Initialize the IFileStore interface
    /*!
        \param[in] pUnused Unused
    */
    virtual void VInit(void *pUnused = 0);

    //! Open a readable stream
    virtual IStream *VOpenStream(const char *sFile);

    //! Open a readable and writable stream
    virtual IOutputStream *VOpenOutputStream(const char *sFile, bool bEraseIfPresent);

    // IDirectoryTraverser Interface
    class CIterator : public IDirectoryTraverser::IIterator
    {
      protected:
        friend class CDoWFileView;
        CIterator(_VirtFolder *pFolder, CDoWFileView *pStore);

        std::string m_sParentPath;
        std::string m_sFullPath;
        CDoWFileView *m_pStore;

        int m_iWhat;

        _VirtFolder *m_pDirectory;
        std::vector<std::unique_ptr<_VirtFolder>>::iterator m_FoldIter;
        std::vector<std::unique_ptr<_VirtFile>>::iterator m_FileIter;

      public:
        ~CIterator();

        virtual IDirectoryTraverser::IIterator::eTypes VGetType();
        virtual IDirectoryTraverser::IIterator *VOpenSubDir();
        virtual IFileStore::IStream *VOpenFile();
        virtual const char *VGetName();
        virtual const char *VGetFullPath();
        virtual const char *VGetDirectoryPath();
        virtual IDirectoryTraverser::IIterator::eErrors VNextItem();
        virtual tLastWriteTime VGetLastWriteTime();

        virtual void *VGetTag(long iTag); // 0 = ModName | 1 = Source Name
    };

    tLastWriteTime VGetLastWriteTime(const char *sPath);
    virtual IDirectoryTraverser::IIterator *VIterate(const char *sPath);
    virtual unsigned long VGetEntryPointCount();
    virtual const char *VGetEntryPoint(unsigned long iID);
    virtual void VCreateFolderIn(const char *sPath, const char *sNewFolderName);

    // Other stuff
    void AddFileSource(IDirectoryTraverser *pTrav, IDirectoryTraverser::IIterator *pDirectory, IFileStore *pIOStore,
                       const char *sMod, const char *sSourceType, bool bIsReqMod = false, bool bCanWrite = false,
                       bool bIsOutput = false);
    void Reset();

  protected:
    std::vector<std::string> m_vModNames;
    std::vector<std::string> m_vSourceNames;
    std::vector<IFileStore *> m_vSourceStores;
    std::vector<IDirectoryTraverser *> m_vSourceDirItrs;
    std::vector<std::pair<bool, bool>> m_vSourceFlags;

    struct _VirtFile
    {
        _VirtFile();

        std::string sName;
        unsigned long iModID;
        unsigned long iSourceID;
        bool bInReqMod;
        unsigned long iReqModID;
        unsigned long iReqSourceID;
        _VirtFolder *pParent;
        bool bGotWriteTime;
        tLastWriteTime oWriteTime;
    };

    struct _VirtFolder
    {
        _VirtFolder();

        _VirtFolder *pParent;
        std::string sName;
        std::string sFullName;
        std::vector<std::unique_ptr<_VirtFolder>> vChildFolders;
        std::vector<std::unique_ptr<_VirtFile>> vChildFiles;

        std::map<unsigned long, std::string> mapSourceFolderNames;
    };

    _VirtFolder m_RootFolder;

    void _Clean();
    void _RawMapFolder(unsigned long iModID, unsigned long iSourceID, IDirectoryTraverser::IIterator *pSourceDirectory,
                       _VirtFolder *pDestination, bool bIsReqMod = false);
    _VirtFile *_FindFile(const char *sPath, CDoWFileView::_VirtFolder **pFolder = 0);
    void _EnsureOutputFolder(_VirtFolder *pFolder, unsigned long *pSourceID);
};
