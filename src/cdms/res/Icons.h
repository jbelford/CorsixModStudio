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

// SVG icon constants for CDMS UI.
// Action icons sourced from Lucide (https://lucide.dev) — MIT license.
// File-type icons use Lucide file variants with per-format accent colors.
//
// All action icons use the CDMS accent color #d5873e (orange).
// wxBitmapBundle::FromSVG() renders these at any DPI via nanosvg.

#pragma once

namespace cdms::icons
{

// ---------------------------------------------------------------------------
// Accent color used across all action icons (matches frmRgdEditor "View Code")
// ---------------------------------------------------------------------------
inline constexpr char kAccentColor[] = "#d5873e";

// ---------------------------------------------------------------------------
// Toolbar action icons (Lucide, accent #d5873e)
// ---------------------------------------------------------------------------

// plus — "New entry" / "Add" (frmUCSEditor toolbar)
inline constexpr char kPlus[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M5 12h14"/>)"
    R"(<path d="M12 5v14"/>)"
    R"(</svg>)";

// arrow-right-to-line — "Send to RGD" / "Apply" (frmUCSEditor toolbar)
inline constexpr char kArrowRightToLine[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M17 12H3"/>)"
    R"(<path d="m11 18 6-6-6-6"/>)"
    R"(<path d="M21 5v14"/>)"
    R"(</svg>)";

// arrow-left-to-line — "Back to RGD" / "Cancel" (frmUCSEditor toolbar)
inline constexpr char kArrowLeftToLine[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M3 19V5"/>)"
    R"(<path d="m13 6-6 6 6 6"/>)"
    R"(<path d="M7 12h14"/>)"
    R"(</svg>)";

// check — "Check Lua" / "Validate" (frmScarEditor toolbar)
inline constexpr char kCheck[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M20 6 9 17l-5-5"/>)"
    R"(</svg>)";

// ---------------------------------------------------------------------------
// Tool panel icons (Lucide, accent #d5873e)
// ---------------------------------------------------------------------------

// globe — "Locale" tool
inline constexpr char kGlobe[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<circle cx="12" cy="12" r="10"/>)"
    R"(<path d="M12 2a14.5 14.5 0 0 0 0 20 14.5 14.5 0 0 0 0-20"/>)"
    R"(<path d="M2 12h20"/>)"
    R"(</svg>)";

// file-text — "UCS Editor" tool
inline constexpr char kFileText[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M10 9H8"/>)"
    R"(<path d="M16 13H8"/>)"
    R"(<path d="M16 17H8"/>)"
    R"(</svg>)";

// package — "SGA Packer" tool
inline constexpr char kPackage[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M11 21.73a2 2 0 0 0 2 0l7-4A2 2 0 0 0 21 16V8a2 2 0 0 0-1-1.73l-7-4a2 2 0 0 0-2 0l-7 4A2 2 0 0 0 3 8v8a2 2 0 0 0 1 1.73z"/>)"
    R"(<path d="M12 22V12"/>)"
    R"(<polyline points="3.29 7 12 12 20.71 7"/>)"
    R"(<path d="m7.5 4.27 9 5.15"/>)"
    R"(</svg>)";

// folder-output — "Extract All" tool
inline constexpr char kFolderOutput[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M2 7.5V5a2 2 0 0 1 2-2h3.9a2 2 0 0 1 1.69.9l.81 1.2a2 2 0 0 0 1.67.9H20a2 2 0 0 1 2 2v10a2 2 0 0 1-2 2H4a2 2 0 0 1-2-1.5"/>)"
    R"(<path d="M2 13h10"/>)"
    R"(<path d="m5 10-3 3 3 3"/>)"
    R"(</svg>)";

// calculator — "DPS Calculator" tool
inline constexpr char kCalculator[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<rect width="16" height="20" x="4" y="2" rx="2"/>)"
    R"(<line x1="8" x2="16" y1="6" y2="6"/>)"
    R"(<line x1="16" x2="16" y1="14" y2="18"/>)"
    R"(<path d="M16 10h.01"/>)"
    R"(<path d="M12 10h.01"/>)"
    R"(<path d="M8 10h.01"/>)"
    R"(<path d="M12 14h.01"/>)"
    R"(<path d="M8 14h.01"/>)"
    R"(<path d="M12 18h.01"/>)"
    R"(<path d="M8 18h.01"/>)"
    R"(</svg>)";

// git-branch — "Lua Inheritance Tree" tool
inline constexpr char kGitBranch[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M15 6a9 9 0 0 0-9 9V3"/>)"
    R"(<circle cx="18" cy="6" r="3"/>)"
    R"(<circle cx="6" cy="18" r="3"/>)"
    R"(</svg>)";

// refresh-cw — "Refresh file list" tool
inline constexpr char kRefreshCw[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M3 12a9 9 0 0 1 9-9 9.75 9.75 0 0 1 6.74 2.74L21 8"/>)"
    R"(<path d="M21 3v5h-5"/>)"
    R"(<path d="M21 12a9 9 0 0 1-9 9 9.75 9.75 0 0 1-6.74-2.74L3 16"/>)"
    R"(<path d="M8 16H3v5"/>)"
    R"(</svg>)";

// settings — "AE Setup" tool
inline constexpr char kSettings[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M9.671 4.136a2.34 2.34 0 0 1 4.659 0 2.34 2.34 0 0 0 3.319 1.915 2.34 2.34 0 0 1 2.33 4.033 2.34 2.34 0 0 0 0 3.831 2.34 2.34 0 0 1-2.33 4.033 2.34 2.34 0 0 0-3.319 1.915 2.34 2.34 0 0 1-4.659 0 2.34 2.34 0 0 0-3.32-1.915 2.34 2.34 0 0 1-2.33-4.033 2.34 2.34 0 0 0 0-3.831A2.34 2.34 0 0 1 6.35 6.051a2.34 2.34 0 0 0 3.319-1.915"/>)"
    R"(<circle cx="12" cy="12" r="3"/>)"
    R"(</svg>)";

// ---------------------------------------------------------------------------
// Navigation / tree icons (Lucide, accent #d5873e)
// ---------------------------------------------------------------------------

// list-tree — "Table of Contents" tree node
inline constexpr char kListTree[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M8 5h13"/>)"
    R"(<path d="M13 12h8"/>)"
    R"(<path d="M13 19h8"/>)"
    R"(<path d="M3 10a2 2 0 0 0 2 2h3"/>)"
    R"(<path d="M3 5v12a2 2 0 0 0 2 2h3"/>)"
    R"(</svg>)";

// chevron-up — "Move up" button (frmModule)
inline constexpr char kChevronUp[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="m18 15-6-6-6 6"/>)"
    R"(</svg>)";

// chevron-down — "Move down" button (frmModule)
inline constexpr char kChevronDown[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="m6 9 6 6 6-6"/>)"
    R"(</svg>)";

// ---------------------------------------------------------------------------
// File-type icons (Lucide file variants with per-format accent colors)
//
// Mapping:
//   file-code  → script files (AI, LUA, SCAR)
//   file-cog   → config/data files (RGD)
//   file-image → image files (TGA, DDS)
//   file-box   → model/3D files (RGM, BFX)
//   file-text  → text/attrib files (ABP)
//   file-sliders → texture ref files (RGT)
//   file (plain) → unknown/null files (NIL)
// ---------------------------------------------------------------------------

// AI — script file (blue #5b9bd5)
inline constexpr char kFileAi[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#5b9bd5" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M10 12.5 8 15l2 2.5"/>)"
    R"(<path d="m14 12.5 2 2.5-2 2.5"/>)"
    R"(</svg>)";

// LUA — script file (indigo #6b71c7)
inline constexpr char kFileLua[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#6b71c7" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M10 12.5 8 15l2 2.5"/>)"
    R"(<path d="m14 12.5 2 2.5-2 2.5"/>)"
    R"(</svg>)";

// NIL — unknown/null file (gray #999999)
inline constexpr char kFileNil[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#999999" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(</svg>)";

// RGD — config/data file (orange #d5873e)
inline constexpr char kFileRgd[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#d5873e" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M15 8a1 1 0 0 1-1-1V2a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8z"/>)"
    R"(<path d="M20 8v12a2 2 0 0 1-2 2h-4.182"/>)"
    R"(<path d="m3.305 19.53.923-.382"/>)"
    R"(<path d="M4 10.592V4a2 2 0 0 1 2-2h8"/>)"
    R"(<path d="m4.228 16.852-.924-.383"/>)"
    R"(<path d="m5.852 15.228-.383-.923"/>)"
    R"(<path d="m5.852 20.772-.383.924"/>)"
    R"(<path d="m8.148 15.228.383-.923"/>)"
    R"(<path d="m8.53 21.696-.382-.924"/>)"
    R"(<path d="m9.773 16.852.922-.383"/>)"
    R"(<path d="m9.773 19.148.922.383"/>)"
    R"(<circle cx="7" cy="18" r="3"/>)"
    R"(</svg>)";

// SCAR — script file (red #c75050)
inline constexpr char kFileScar[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#c75050" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M10 12.5 8 15l2 2.5"/>)"
    R"(<path d="m14 12.5 2 2.5-2 2.5"/>)"
    R"(</svg>)";

// TGA — image file (green #50a050)
inline constexpr char kFileTga[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#50a050" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<circle cx="10" cy="12" r="2"/>)"
    R"(<path d="m20 17-1.296-1.296a2.41 2.41 0 0 0-3.408 0L9 22"/>)"
    R"(</svg>)";

// RGT — texture ref file (gold #c7a035)
inline constexpr char kFileRgt[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#c7a035" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M8 12h8"/>)"
    R"(<path d="M10 11v2"/>)"
    R"(<path d="M8 17h8"/>)"
    R"(<path d="M14 16v2"/>)"
    R"(</svg>)";

// DDS — image file (teal #35a0c7)
inline constexpr char kFileDds[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#35a0c7" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<circle cx="10" cy="12" r="2"/>)"
    R"(<path d="m20 17-1.296-1.296a2.41 2.41 0 0 0-3.408 0L9 22"/>)"
    R"(</svg>)";

// BFX — effects file (purple #a050c7)
inline constexpr char kFileBfx[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#a050c7" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M14.5 22H18a2 2 0 0 0 2-2V8a2.4 2.4 0 0 0-.706-1.706l-3.588-3.588A2.4 2.4 0 0 0 14 2H6a2 2 0 0 0-2 2v3.8"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M11.7 14.2 7 17l-4.7-2.8"/>)"
    R"(<path d="M3 13.1a2 2 0 0 0-.999 1.76v3.24a2 2 0 0 0 .969 1.78L6 21.7a2 2 0 0 0 2.03.01L11 19.9a2 2 0 0 0 1-1.76V14.9a2 2 0 0 0-.97-1.78L8 11.3a2 2 0 0 0-2.03-.01z"/>)"
    R"(<path d="M7 17v5"/>)"
    R"(</svg>)";

// ABP — attrib/text file (mint #50c790)
inline constexpr char kFileAbp[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#50c790" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M6 22a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h8a2.4 2.4 0 0 1 1.704.706l3.588 3.588A2.4 2.4 0 0 1 20 8v12a2 2 0 0 1-2 2z"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M10 9H8"/>)"
    R"(<path d="M16 13H8"/>)"
    R"(<path d="M16 17H8"/>)"
    R"(</svg>)";

// RGM — model file (rust #c77050)
inline constexpr char kFileRgm[] =
    R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="#c77050" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">)"
    R"(<path d="M14.5 22H18a2 2 0 0 0 2-2V8a2.4 2.4 0 0 0-.706-1.706l-3.588-3.588A2.4 2.4 0 0 0 14 2H6a2 2 0 0 0-2 2v3.8"/>)"
    R"(<path d="M14 2v5a1 1 0 0 0 1 1h5"/>)"
    R"(<path d="M11.7 14.2 7 17l-4.7-2.8"/>)"
    R"(<path d="M3 13.1a2 2 0 0 0-.999 1.76v3.24a2 2 0 0 0 .969 1.78L6 21.7a2 2 0 0 0 2.03.01L11 19.9a2 2 0 0 0 1-1.76V14.9a2 2 0 0 0-.97-1.78L8 11.3a2 2 0 0 0-2.03-.01z"/>)"
    R"(<path d="M7 17v5"/>)"
    R"(</svg>)";

// ---------------------------------------------------------------------------
// Helper: render an SVG string constant to a wxBitmap at a given size.
// Useful for adding SVG icons to wxImageList (which doesn't support bundles).
// ---------------------------------------------------------------------------
// Usage:
//   #include <wx/bmpbndl.h>
//   pImageList->Add(cdms::icons::SvgToBitmap(kFileLua, wxSize(16, 16)));

inline wxBitmap SvgToBitmap(const char *svgData, const wxSize &size)
{
    return wxBitmapBundle::FromSVG(svgData, size).GetBitmap(size);
}

} // namespace cdms::icons
