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

#include "views/interfaces/IImageView.h"
#include <rainman/formats/CRgtFile.h>
#include <rainman/io/IFileStore.h>
#include <wx/string.h>

//! Presenter for image viewing and save-copy operations.
/*!
    Owns the image transcoding and save logic extracted from frmImageViewer.
    All heavy Rainman I/O (RGT/DDS/TGA conversion) lives here, testable
    without a running GUI via a mock IImageView.
*/
class CImagePresenter
{
  public:
    //! Image save formats matching the radio button indices.
    enum class SaveFormat
    {
        RGT = 0,
        DDS = 1,
        TGA = 2
    };

    explicit CImagePresenter(IImageView &view);

    //! Compute the output file path by replacing the extension.
    /*!
        \param sFilename       Original filename (e.g. "foo.rgt").
        \param eTargetFormat   Target save format.
        \return Path with the extension replaced (e.g. "foo.dds").
    */
    [[nodiscard]] static wxString ComputeOutputPath(const wxString &sFilename, SaveFormat eTargetFormat);

    //! Convert a compression radio-button index to the DXT level used by CRgtFile.
    /*!
        Index mapping:
        - 0 → -1 (RGB, no alpha — not valid for DXT, used as sentinel)
        - 1 → -1 (RGBA — not valid for DXT, used as sentinel)
        - 2 →  1 (DXT1)
        - 3 →  3 (DXT3)
        - 4 →  5 (DXT5)

        \param iCompressionIndex  Radio button selection (0–4).
        \return DXT compression level, or -1 if not a DXT format.
    */
    [[nodiscard]] static int CompressionToDxtLevel(int iCompressionIndex);

    //! Check whether saving would overwrite the original file.
    /*!
        \param iCurrentFormat   Current format radio index (0=RGT, 1=DDS, 2=TGA).
        \param eTargetFormat    Target save format.
        \return true if formats match (overwrite risk).
    */
    [[nodiscard]] static bool WouldOverwrite(int iCurrentFormat, SaveFormat eTargetFormat);

    //! Perform image transcoding and write to the given output stream.
    /*!
        \param pRgtFile           Source image (not null).
        \param pStream            Output stream to write to (not null).
        \param eTargetFormat      Target format (RGT, DDS, TGA).
        \param iCompressionIndex  Compression radio selection (0–4).
        \param bMipLevels         Whether to generate mip levels.
        \return true on success, false on error (error reported via view).
    */
    bool SaveToStream(CRgtFile *pRgtFile, IFileStore::IOutputStream *pStream, SaveFormat eTargetFormat,
                      int iCompressionIndex, bool bMipLevels);

  private:
    IImageView &m_view;
};
