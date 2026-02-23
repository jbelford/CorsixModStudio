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

#include "lsp/ScarAnnotationTranslator.h"
#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace lsp
{

// ---------------------------------------------------------------------------
// Type mapping — mirrors tools/scardoc-to-luadefs.py TYPE_MAP
// ---------------------------------------------------------------------------

static const std::unordered_map<std::string, std::string> &GetTypeMap()
{
    static const std::unordered_map<std::string, std::string> sTypeMap = {
        {"Void", ""},
        {"Boolean", "boolean"},
        {"Bool", "boolean"},
        {"Integer", "integer"},
        {"Int", "integer"},
        {"Real", "number"},
        {"Float", "number"},
        {"Number", "number"},
        {"String", "string"},
        {"Table", "table"},
        {"Function", "function"},
        {"LuaFunction", "function"},
        {"Any", "any"},
        {"SGroupID", "SGroupID"},
        {"EGroupID", "EGroupID"},
        {"PlayerID", "PlayerID"},
        {"Position", "Position"},
        {"ScarPosition", "Position"},
        {"SquadID", "SquadID"},
        {"EntityID", "EntityID"},
        {"MarkerID", "MarkerID"},
        {"ActorTable", "ActorTable"},
        {"AbilityID", "AbilityID"},
        {"UpgradeID", "UpgradeID"},
        {"CriticalID", "CriticalID"},
        {"LocString", "LocString"},
        {"Blueprint", "Blueprint"},
        {"PropertyBagGroup", "PropertyBagGroup"},
    };
    return sTypeMap;
}

std::string CScarAnnotationTranslator::MapType(const std::string &scarType)
{
    const auto &typeMap = GetTypeMap();
    auto it = typeMap.find(scarType);
    if (it != typeMap.end())
    {
        return it->second;
    }
    // Unknown types pass through as-is
    return scarType;
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static std::string TrimLeft(const std::string &s)
{
    auto it = std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isspace(c); });
    return std::string(it, s.end());
}

static std::string Trim(const std::string &s)
{
    auto start = std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isspace(c); });
    if (start == s.end())
    {
        return "";
    }
    auto end = std::find_if(s.rbegin(), s.rend(), [](unsigned char c) { return !std::isspace(c); });
    return std::string(start, end.base());
}

/// Split a line into (indent, content_after_--?) if it starts with --?.
/// Returns false if the line is not a --? comment.
static bool ParseScarComment(const std::string &line, std::string &indent, std::string &content)
{
    auto pos = line.find("--?");
    if (pos == std::string::npos)
    {
        return false;
    }
    indent = line.substr(0, pos);
    content = (pos + 3 < line.size()) ? line.substr(pos + 3) : "";
    return true;
}

/// Parse "@args Type1 name1, Type2 name2, ..." into a vector of (type, name) pairs.
struct ArgPair
{
    std::string type;
    std::string name;
    bool optional = false;
};

static std::vector<ArgPair> ParseArgs(const std::string &argsStr)
{
    std::vector<ArgPair> result;
    if (argsStr.empty())
    {
        return result;
    }

    // Split by comma
    std::istringstream stream(argsStr);
    std::string token;
    while (std::getline(stream, token, ','))
    {
        std::string trimmed = Trim(token);
        if (trimmed.empty())
        {
            continue;
        }

        // Check for optional parameter: [Type name]
        bool optional = false;
        if (trimmed.front() == '[' && trimmed.back() == ']')
        {
            optional = true;
            trimmed = trimmed.substr(1, trimmed.size() - 2);
            trimmed = Trim(trimmed);
        }

        // Split into "Type name"
        auto spacePos = trimmed.find(' ');
        ArgPair pair;
        pair.optional = optional;
        if (spacePos != std::string::npos)
        {
            pair.type = trimmed.substr(0, spacePos);
            pair.name = Trim(trimmed.substr(spacePos + 1));
        }
        else
        {
            pair.type = "any";
            pair.name = trimmed;
        }
        result.push_back(std::move(pair));
    }
    return result;
}

// ---------------------------------------------------------------------------
// Core translation
// ---------------------------------------------------------------------------

/// Parsed annotation block — all the info extracted from consecutive --? lines.
struct AnnotationBlock
{
    std::string shortdesc;
    std::string resultType;
    std::vector<ArgPair> args;
    std::vector<std::string> extraLines; // @extdesc text, examples, bare --? lines
    std::string indent;                  // Indentation of the block
    int startLine = 0;                   // Original line index where block starts
    int lineCount = 0;                   // Number of original --? lines consumed
};

/// Parse consecutive --? lines starting at lines[startIdx].
/// Returns the parsed block; advances pastEndIdx to the first non-comment line.
static AnnotationBlock ParseBlock(const std::vector<std::string> &lines, int startIdx, int &pastEndIdx)
{
    AnnotationBlock block;
    block.startLine = startIdx;

    bool inExtdesc = false;
    int i = startIdx;
    for (; i < static_cast<int>(lines.size()); ++i)
    {
        std::string indent, content;
        if (!ParseScarComment(lines[i], indent, content))
        {
            break;
        }

        if (block.lineCount == 0)
        {
            block.indent = indent;
        }
        block.lineCount++;

        std::string trimmedContent = TrimLeft(content);

        if (trimmedContent.rfind("@shortdesc", 0) == 0)
        {
            block.shortdesc = Trim(trimmedContent.substr(10));
            inExtdesc = false;
        }
        else if (trimmedContent.rfind("@result", 0) == 0)
        {
            block.resultType = Trim(trimmedContent.substr(7));
            inExtdesc = false;
        }
        else if (trimmedContent.rfind("@args", 0) == 0)
        {
            block.args = ParseArgs(Trim(trimmedContent.substr(5)));
            inExtdesc = false;
        }
        else if (trimmedContent.rfind("@extdesc", 0) == 0)
        {
            inExtdesc = true;
            // If there's text after @extdesc on the same line, capture it
            std::string rest = Trim(trimmedContent.substr(8));
            if (!rest.empty())
            {
                block.extraLines.push_back(rest);
            }
        }
        else
        {
            // Bare --? line (example code, extended description continuation, etc.)
            block.extraLines.push_back(content);
        }
    }

    pastEndIdx = i;
    return block;
}

/// Extract parameter names from a function declaration line.
/// e.g. "function Actor_SetFromSGroup( actor, groupname )" → {"actor", "groupname"}
static std::vector<std::string> ParseFunctionParams(const std::string &line)
{
    std::vector<std::string> params;
    auto parenOpen = line.find('(');
    if (parenOpen == std::string::npos)
    {
        return params;
    }
    auto parenClose = line.find(')', parenOpen);
    if (parenClose == std::string::npos)
    {
        return params;
    }

    std::string paramStr = line.substr(parenOpen + 1, parenClose - parenOpen - 1);
    std::istringstream stream(paramStr);
    std::string token;
    while (std::getline(stream, token, ','))
    {
        std::string trimmed = Trim(token);
        if (!trimmed.empty())
        {
            params.push_back(trimmed);
        }
    }
    return params;
}

/// Check if a line starts a function declaration (ignoring leading whitespace).
static bool IsFunctionDeclaration(const std::string &line)
{
    std::string trimmed = TrimLeft(line);
    return trimmed.rfind("function ", 0) == 0 || trimmed.rfind("function(", 0) == 0 ||
           trimmed.rfind("local function ", 0) == 0;
}

/// Emit the translated annotation lines from a parsed block.
/// If @p funcParamNames is non-empty, use those names instead of @args names
/// (positional mapping) so that @param annotations match the actual function signature.
static std::vector<std::string> EmitBlock(const AnnotationBlock &block,
                                          const std::vector<std::string> &funcParamNames = {})
{
    std::vector<std::string> output;

    // Description
    if (!block.shortdesc.empty())
    {
        output.push_back(block.indent + "---" + block.shortdesc);
    }

    // Parameters — use function signature names when available to ensure
    // @param annotations match the actual parameter names in the function declaration.
    for (size_t idx = 0; idx < block.args.size(); ++idx)
    {
        const auto &arg = block.args[idx];
        std::string mapped = CScarAnnotationTranslator::MapType(arg.type);
        if (mapped.empty())
        {
            mapped = "any";
        }
        // Prefer function param name over @args name for LuaLS matching
        std::string paramName = (idx < funcParamNames.size()) ? funcParamNames[idx] : arg.name;
        std::string optMark = arg.optional ? "?" : "";
        output.push_back(block.indent + "---@param " + paramName + optMark + " " + mapped);
    }

    // Return type
    if (!block.resultType.empty())
    {
        std::string mapped = CScarAnnotationTranslator::MapType(block.resultType);
        if (!mapped.empty())
        {
            output.push_back(block.indent + "---@return " + mapped);
        }
    }

    // Extra lines (extended description, examples)
    for (const auto &extra : block.extraLines)
    {
        output.push_back(block.indent + "---" + extra);
    }

    return output;
}

TranslationResult CScarAnnotationTranslator::Translate(const std::string &source)
{
    TranslationResult result;

    // Split source into lines (preserving empty lines, handling \r\n and \n)
    std::vector<std::string> lines;
    {
        std::istringstream stream(source);
        std::string line;
        while (std::getline(stream, line))
        {
            // Strip trailing \r if present
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }
            lines.push_back(std::move(line));
        }
        // Handle trailing newline: if source ends with \n, getline produces an extra empty read
        // that we want. If source is empty, lines is empty, which is fine.
    }

    std::vector<std::string> outputLines;
    result.originalToTranslated.resize(lines.size(), 0);

    int i = 0;
    while (i < static_cast<int>(lines.size()))
    {
        std::string indent, content;
        if (ParseScarComment(lines[i], indent, content))
        {
            // Parse the full --? block
            int pastEnd = 0;
            AnnotationBlock block = ParseBlock(lines, i, pastEnd);

            // Record mapping: all original lines in this block map to the current output position
            int translatedStart = static_cast<int>(outputLines.size());
            for (int origLine = i; origLine < pastEnd; ++origLine)
            {
                result.originalToTranslated[origLine] = translatedStart;
            }

            // Look ahead for the function declaration to get actual parameter names
            std::vector<std::string> funcParams;
            if (!block.args.empty() && pastEnd < static_cast<int>(lines.size()) &&
                IsFunctionDeclaration(lines[pastEnd]))
            {
                funcParams = ParseFunctionParams(lines[pastEnd]);
            }

            // Emit translated block
            auto emitted = EmitBlock(block, funcParams);
            for (const auto &eline : emitted)
            {
                result.translatedToOriginal.push_back(i); // All emitted lines trace back to block start
                outputLines.push_back(eline);
            }

            i = pastEnd;
        }
        else
        {
            // Non-comment line — pass through unchanged
            result.originalToTranslated[i] = static_cast<int>(outputLines.size());
            result.translatedToOriginal.push_back(i);
            outputLines.push_back(lines[i]);
            ++i;
        }
    }

    // Join output lines
    std::ostringstream oss;
    for (int idx = 0; idx < static_cast<int>(outputLines.size()); ++idx)
    {
        if (idx > 0)
        {
            oss << '\n';
        }
        oss << outputLines[idx];
    }
    // Preserve trailing newline if original had one
    if (!source.empty() && source.back() == '\n')
    {
        oss << '\n';
    }
    result.text = oss.str();

    return result;
}

// ---------------------------------------------------------------------------
// Position mapping
// ---------------------------------------------------------------------------

Position CScarAnnotationTranslator::MapToTranslated(const TranslationResult &result, Position original)
{
    Position out = original;
    if (original.line >= 0 && original.line < static_cast<int>(result.originalToTranslated.size()))
    {
        out.line = result.originalToTranslated[original.line];
    }
    return out;
}

Position CScarAnnotationTranslator::MapToOriginal(const TranslationResult &result, Position translated)
{
    Position out = translated;
    if (translated.line >= 0 && translated.line < static_cast<int>(result.translatedToOriginal.size()))
    {
        out.line = result.translatedToOriginal[translated.line];
    }
    return out;
}

} // namespace lsp
