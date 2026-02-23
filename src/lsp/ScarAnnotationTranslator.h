/*
LSP Client Library
Copyright (C) 2026 CorsixModStudio Contributors

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

#include "lsp/Api.h"
#include "lsp/Protocol.h"
#include <string>
#include <vector>

namespace lsp
{

/// Result of translating a SCAR document's --? annotations to LuaLS ---@ format.
struct TranslationResult
{
    /// The translated document text with ---@ annotations.
    std::string text;

    /// Maps each original line number to the first corresponding translated line number.
    /// Size == number of original lines. If the original has no expansion, the values
    /// form a monotonically increasing sequence with possible gaps (from @args expansion).
    std::vector<int> originalToTranslated;

    /// Maps each translated line number back to the original line number.
    /// Size == number of translated lines.
    std::vector<int> translatedToOriginal;

    /// Marks each original line as being inside a --? annotation block (true) or
    /// a passthrough line (false). Used by MapToOriginal to reset character positions
    /// for block lines, since translated annotation text has no column correspondence
    /// with the original --? source line.
    std::vector<bool> isBlockLine;
};

/// Translates Relic's --? SCAR documentation comments into LuaLS ---@ annotations.
///
/// Relic format example:
/// @code
///   --? @shortdesc Assign an sgroup to a actor.
///   --? @result Void
///   --? @args ActorTable actor, String sgroupname
///   --? @extdesc
///   --?  Actor_SetFromSGroup( ACT.Gabriel, "sg_Gabriel" )
///   function Actor_SetFromSGroup( actor, groupname )
/// @endcode
///
/// Translated to:
/// @code
///   ---Assign an sgroup to a actor.
///   ---@return void
///   ---@param actor ActorTable
///   ---@param sgroupname string
///   ---
///   --- Actor_SetFromSGroup( ACT.Gabriel, "sg_Gabriel" )
///   function Actor_SetFromSGroup( actor, groupname )
/// @endcode
class LSP_API CScarAnnotationTranslator
{
  public:
    /// Translate all --? annotation blocks in @p source to ---@ annotations.
    /// Returns the transformed text and bidirectional line mapping.
    static TranslationResult Translate(const std::string &source);

    /// Map a position from original document coordinates to translated coordinates.
    static Position MapToTranslated(const TranslationResult &result, Position original);

    /// Map a position from translated document coordinates back to original coordinates.
    static Position MapToOriginal(const TranslationResult &result, Position translated);

    /// Map a Relic scardoc type name to a LuaLS annotation type.
    /// Unknown types pass through as-is (assumed to be custom @class types).
    static std::string MapType(const std::string &scarType);
};

} // namespace lsp
