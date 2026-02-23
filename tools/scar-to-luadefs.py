#!/usr/bin/env python3
"""
Generate consolidated LuaLS @meta stubs for Dawn of War SCAR scripting.

Merges two sources into a single definition file:
  1. function_list.htm — Relic's ScarDoc HTML (complete function reference)
  2. scardoc.dat — binary with engine constants + a few extra functions

Usage:
    python tools/scar-to-luadefs.py <game_root> [--scardoc <path>]

Example:
    python tools/scar-to-luadefs.py "C:/Program Files (x86)/Steam/steamapps/common/Dawn of War Definitive Edition"
"""

import os
import re
import struct
import sys
from pathlib import Path

# Map scardoc type names to LuaLS annotation types.
TYPE_MAP = {
    "Void":           None,
    "Boolean":        "boolean",
    "Bool":           "boolean",
    "Integer":        "integer",
    "Int":            "integer",
    "Real":           "number",
    "Float":          "number",
    "Number":         "number",
    "String":         "string",
    "Table":          "table",
    "Function":       "function",
    "LuaFunction":    "function",
    "Any":            "any",
    "SGroupID":       "SGroupID",
    "EGroupID":       "EGroupID",
    "PlayerID":       "PlayerID",
    "Position":       "Position",
    "ScarPosition":   "Position",
    "ScarPos":        "Position",
    "SquadID":        "SquadID",
    "EntityID":       "EntityID",
    "MarkerID":       "MarkerID",
    "ActorTable":     "ActorTable",
    "AbilityID":      "AbilityID",
    "UpgradeID":      "UpgradeID",
    "CriticalID":     "CriticalID",
    "LocString":      "LocString",
    "Blueprint":      "Blueprint",
    "PropertyBagGroup": "PropertyBagGroup",
    "LuaTable":       "table",
    "StringTable":    "table",
    "ButtonID":       "ButtonID",
    "SoundChannel":   "SoundChannel",
    "Modifier":       "Modifier",
    "ProxType":       "ProxType",
    "PingID":         "PingID",
    # Group name strings are passed as plain strings in SCAR.
    "SGroup":         "string",
    "EGroup":         "string",
}

# Typos found in game files — map to corrected types.
TYPO_MAP = {
    "Integrer":  "integer",
    "PlaierID":  "PlayerID",
    "locID":     "integer",
    "beaconID":  "integer",
}

# Import tier for each SCAR library file.
# Tier 1: loaded by ScarUtil.scar — available with import("ScarUtil.scar")
# Tier 2: loaded by WXPScarUtil.scar — available with import("WXPScarUtil.scar")
FILE_TIER: dict[str, int] = {
    "scarutil.scar": 1, "setup.scar": 1, "proximity.scar": 1,
    "timer.scar": 1, "player.scar": 1, "entity.scar": 1,
    "squad.scar": 1, "command.scar": 1, "groupcallers.scar": 1,
    "groups.scar": 1, "camera.scar": 1, "restrict.scar": 1,
    "actor.scar": 1, "modifiers.scar": 1, "cpuplayerutil.scar": 1,
    "difficultylevel.scar": 1, "ui.scar": 1, "production.scar": 1,
    "wxpscarutil.scar": 2, "wxpactor.scar": 2, "wxpbutton.scar": 2,
    "wxpcommand.scar": 2, "wxpdifficultylevel.scar": 2,
    "wxpentityprecach.scar": 2, "wxpgroups.scar": 2,
    "wxpmetamap.scar": 2, "wxprestrict.scar": 2, "wxpui.scar": 2,
}

# Standalone files not imported by ScarUtil or WXPScarUtil.
STANDALONE_IMPORT: dict[str, str] = {
    "wcutil.scar": 'import("WCUtil.scar")',
    "balancetool.scar": 'import("BalanceTool.scar")',
    "camerashake.scar": 'import("CameraShake.scar")',
    "debug.scar": 'import("Debug.scar")',
}


def map_type(raw_type: str) -> str | None:
    """Map a scardoc type name to a LuaLS annotation type."""
    raw_type = raw_type.strip()
    if raw_type in TYPE_MAP:
        return TYPE_MAP[raw_type]
    if raw_type in TYPO_MAP:
        return TYPO_MAP[raw_type]
    # Strip LuaConfig:: prefix used in some annotations.
    cleaned = re.sub(r"^LuaConfig::RefTable$", "table", raw_type)
    if cleaned != raw_type:
        return cleaned
    # Unknown — return as-is.
    return raw_type


def _read_binary_string(f) -> str:
    """Read a length-prefixed string from a binary scardoc.dat file."""
    raw = f.read(4)
    if len(raw) < 4:
        return ""
    length = struct.unpack("<I", raw)[0]
    if length == 0:
        return ""
    return f.read(length).decode("ascii", errors="replace")


def parse_scardoc_dat(filepath: str) -> tuple[list[dict], list[str]]:
    """Parse a scardoc.dat binary file into (functions, constants).

    Returns a tuple of (functions_list, constants_list) where each function
    dict has the same shape as parse_scar_file output.
    """
    functions = []
    constants: list[str] = []

    with open(filepath, "rb") as f:
        raw = f.read(4)
        if len(raw) < 4:
            return [], []
        count = struct.unpack("<I", raw)[0]

        for _ in range(count):
            return_type_str = _read_binary_string(f)
            name = _read_binary_string(f)

            if not name:
                # Constant: return_type holds the constant name.
                constants.append(return_type_str)
            else:
                raw_argc = f.read(4)
                if len(raw_argc) < 4:
                    break
                argc = struct.unpack("<I", raw_argc)[0]
                raw_args = [_read_binary_string(f) for _ in range(argc)]
                desc = _read_binary_string(f)

                # Parse args into (type, name, optional) tuples.
                params: list[tuple] = []
                for arg_str in raw_args:
                    arg_str = arg_str.strip()
                    optional = arg_str.startswith("[") and arg_str.endswith("]")
                    if optional:
                        arg_str = arg_str[1:-1].strip()
                    parts = arg_str.split(None, 1)
                    if len(parts) == 2:
                        params.append((map_type(parts[0]) or "any", parts[1], optional))
                    elif len(parts) == 1:
                        params.append(("any", parts[0], optional))

                ret = map_type(return_type_str)

                functions.append({
                    "name": name,
                    "params": params,
                    "return_type": ret,
                    "shortdesc": desc,
                    "extdesc": "",
                    "source": "engine",
                })

    return functions, constants


def _clean_html(text: str) -> str:
    """Strip HTML tags and convert whitespace to plain text."""
    # Convert <BR> tags to newlines.
    text = re.sub(r"<[Bb][Rr]\s*/?>", "\n", text)
    # Strip remaining HTML tags.
    text = re.sub(r"<[^>]+>", "", text)
    # Replace non-breaking spaces with regular spaces.
    text = text.replace("\xa0", " ")
    # Replace tabs with spaces.
    text = text.replace("\t", "    ")
    # Collapse runs of blank lines into single blank lines.
    text = re.sub(r"\n{3,}", "\n\n", text)
    return text.strip()


def parse_scardoc_html(filepath: str) -> list[dict]:
    """Parse Relic's ScarDoc function_list.htm into function dicts.

    The HTML is generated by Relic's internal ScarDoc build tool from both
    C++ engine source (//? annotations in LuaLibs/*.cpp) and SCAR library
    files (--? annotations in *.scar).  It contains the complete function
    reference including functions not present in scardoc.dat.
    """
    with open(filepath, "r", encoding="utf-16") as f:
        html = f.read()

    functions = []

    func_pattern = re.compile(
        r'<a name="([^"]+)"></a><p>\s*'
        r'<table[^>]*>\s*'
        r'<tr><td[^>]*><span class="function">(.*?)</span></td></tr>\s*'
        r'<tr>\s*<td[^>]*>\s*</td>\s*<td>\s*(.*?)\s*</td>\s*</tr>\s*'
        r'<tr>\s*<td[^>]*>\s*</td>\s*<td><em>Source: </em>'
        r'<a href="([^"]*)"',
        re.DOTALL
    )

    for m in func_pattern.finditer(html):
        anchor = m.group(1)
        sig_html = m.group(2)
        desc_html = m.group(3)
        source_path = m.group(4)

        if anchor.startswith("Group_"):
            continue

        # Parse return type from: <span class="arg">RetType </span> FuncName(...)
        ret_match = re.match(
            r'<span class="arg">(\w+)\s*</span>\s*(.*)', sig_html, re.DOTALL
        )
        if not ret_match:
            continue

        ret_type_raw = ret_match.group(1).strip()
        rest = re.sub(r'\s+', ' ', ret_match.group(2).strip())

        name_match = re.match(r'(\*?\w+)\s*\(\s*(.*?)\s*\)', rest)
        if not name_match:
            continue

        func_name = name_match.group(1)
        args_html = name_match.group(2)

        # Parse parameters from: <span class="arg">Type</span> name, ...
        params: list[tuple] = []
        if args_html.strip():
            for part in re.split(r',\s*(?=(?:\[?\s*)?<span|$)', args_html):
                part = part.strip()
                if not part:
                    continue
                arg_match = re.match(
                    r'(?:\[?\s*)?<span class="arg">(\w+)</span>\s+(\w+)\s*\]?\s*$',
                    part,
                )
                if arg_match:
                    ptype = arg_match.group(1)
                    pname = arg_match.group(2)
                    optional = part.strip().startswith("[")
                    params.append((map_type(ptype) or "any", pname, optional))

        ret = map_type(ret_type_raw)

        # Parse description paragraphs.
        desc_parts = re.findall(r'<p>(.*?)</p>', desc_html, re.DOTALL)
        shortdesc = _clean_html(desc_parts[0]) if desc_parts else ""
        extdesc = _clean_html(" ".join(p.strip() for p in desc_parts[1:])) if len(desc_parts) > 1 else ""

        source_file = source_path.rsplit("/", 1)[-1] if "/" in source_path else source_path

        functions.append({
            "name": func_name,
            "params": params,
            "return_type": ret,
            "shortdesc": shortdesc,
            "extdesc": extdesc,
            "source_file": source_file,
        })

    return functions


def _write_func(out, func):
    """Write a single function stub to the output file."""
    if func["shortdesc"]:
        for i, line in enumerate(func["shortdesc"].split("\n")):
            out.write(f"--- {line}\n")
    if func.get("extdesc"):
        out.write("---\n")
        for ext_line in func["extdesc"].split("\n"):
            ext_line = ext_line.rstrip()
            if not ext_line:
                out.write("---\n")
            else:
                out.write(f"--- {ext_line}\n")
    source = func.get("source_file") or func.get("source")
    if source:
        out.write(f"---\n--- *Source: {source}*\n")

    for param in func["params"]:
        ptype, pname = param[0], param[1]
        opt = "?" if len(param) > 2 and param[2] else ""
        out.write(f"---@param {pname}{opt} {ptype}\n")

    if func["return_type"]:
        out.write(f"---@return {func['return_type']}\n")

    param_names = [p[1] for p in func["params"]]
    out.write(f"function {func['name']}({', '.join(param_names)}) end\n\n")


def generate(scardoc_path: str | None,
             scardoc_html_path: str | None, output_path: str):
    """Merge HTML + scardoc.dat sources and generate consolidated LuaLS stubs.

    Sources:
      1. scardoc HTML (function_list.htm) — complete function reference
      2. scardoc.dat binary — constants + functions missing from HTML
    """
    # 1. Parse scardoc HTML (primary source).
    html_funcs: list[dict] = []
    html_by_name: dict[str, dict] = {}

    if scardoc_html_path and os.path.isfile(scardoc_html_path):
        html_funcs = parse_scardoc_html(scardoc_html_path)
        html_by_name = {f["name"]: f for f in html_funcs}
        print(f"Read {len(html_funcs)} functions from "
              f"{os.path.basename(scardoc_html_path)}")

    # 2. Parse scardoc.dat for constants and extra functions.
    constants: list[str] = []
    dat_funcs: list[dict] = []
    if scardoc_path and os.path.isfile(scardoc_path):
        dat_funcs, constants = parse_scardoc_dat(scardoc_path)
        print(f"Read {len(dat_funcs)} functions + {len(constants)} constants from "
              f"{os.path.basename(scardoc_path)}")

    # Add scardoc.dat functions not already in HTML.
    dat_only_funcs = [f for f in dat_funcs if f["name"] not in html_by_name]
    if dat_only_funcs:
        print(f"  {len(dat_only_funcs)} functions only in scardoc.dat: "
              f"{', '.join(f['name'] for f in dat_only_funcs)}")

    if not html_funcs and not dat_funcs:
        print("Error: no functions found from any source", file=sys.stderr)
        return False

    # 3. Classify functions by source file.
    # Functions with a .scar source → library (grouped by file and tier).
    # Functions with a .cpp source → engine-only.
    library_funcs: list[dict] = []
    engine_only_funcs: list[dict] = []

    for func in html_funcs:
        sf = func.get("source_file", "")
        if sf.endswith(".scar"):
            library_funcs.append(func)
        else:
            engine_only_funcs.append(func)

    # scardoc.dat-only functions go into engine-only.
    for func in dat_only_funcs:
        func["source_file"] = "scardoc.dat"
        engine_only_funcs.append(func)

    # Group library functions by source file.
    by_file: dict[str, list[dict]] = {}
    for func in library_funcs:
        key = func["source_file"].lower()
        by_file.setdefault(key, []).append(func)

    for funcs in by_file.values():
        funcs.sort(key=lambda f: f["name"])

    # Group engine-only by C++ source file.
    engine_by_cpp: dict[str, list[dict]] = {}
    for func in engine_only_funcs:
        key = func.get("source_file", "unknown")
        engine_by_cpp.setdefault(key, []).append(func)

    for funcs in engine_by_cpp.values():
        funcs.sort(key=lambda f: f["name"])

    # Partition .scar files into tiers.
    tier1_files, tier2_files, standalone_files, other_files = [], [], [], []
    for sf in sorted(by_file):
        if sf in FILE_TIER:
            (tier1_files if FILE_TIER[sf] == 1 else tier2_files).append(sf)
        elif sf in STANDALONE_IMPORT:
            standalone_files.append(sf)
        else:
            other_files.append(sf)

    total_funcs = len(engine_only_funcs) + len(library_funcs)

    # 4. Write consolidated output.
    with open(output_path, "w", encoding="utf-8", newline="\n") as out:
        out.write("---@meta scar-dow\n\n")
        out.write("-- Auto-generated SCAR stubs for Dawn of War by scar-to-luadefs.py\n")
        out.write(f"-- Sources: function_list.htm + scardoc.dat\n")
        out.write(f"-- {total_funcs} functions, {len(constants)} constants\n")
        out.write("-- Do not edit manually — regenerate with: python tools/scar-to-luadefs.py\n")
        out.write("--\n")
        out.write("-- Import tiers:\n")
        out.write('--   Tier 1: available after import("ScarUtil.scar")\n')
        out.write('--   Tier 2: available after import("WXPScarUtil.scar")  (includes Tier 1)\n')
        out.write("--   Standalone: requires explicit import (noted per-section)\n")
        out.write("--   Engine-only: C++ built-ins with no .scar wrapper\n")
        out.write("--\n")
        out.write('-- All mod scripts should import("WXPScarUtil.scar") to get Tier 1 + Tier 2.\n')
        out.write("\n")

        # Constants.
        if constants:
            out.write(f"{'--' * 40}\n")
            out.write("-- Constants\n")
            out.write(f"{'--' * 40}\n\n")
            for c in sorted(constants):
                out.write(f"---@type any\n{c} = nil\n\n")

        # Library functions by tier.
        def _write_tier(files, tier_label):
            if not files:
                return
            out.write(f"{'--' * 40}\n")
            out.write(f"-- {tier_label}\n")
            out.write(f"{'--' * 40}\n\n")
            for sf in files:
                note = STANDALONE_IMPORT.get(sf)
                if note:
                    out.write(f"-- from {sf} — requires {note}\n\n")
                else:
                    out.write(f"-- from {sf}\n\n")
                for func in by_file[sf]:
                    _write_func(out, func)

        _write_tier(tier1_files, 'Tier 1: Available via import("ScarUtil.scar")')
        _write_tier(tier2_files, 'Tier 2: Available via import("WXPScarUtil.scar")')
        _write_tier(standalone_files, "Standalone: Requires explicit import()")
        _write_tier(other_files, "Other")

        # Engine-only functions grouped by C++ source file.
        if engine_only_funcs:
            out.write(f"{'--' * 40}\n")
            out.write("-- Engine-only: C++ built-ins (no .scar wrapper)\n")
            out.write(f"{'--' * 40}\n\n")
            for cpp_file in sorted(engine_by_cpp):
                out.write(f"-- from {cpp_file}\n\n")
                for func in engine_by_cpp[cpp_file]:
                    _write_func(out, func)

    print(f"Wrote {total_funcs} functions + {len(constants)} constants to {output_path}")
    print(f"  ({len(library_funcs)} with .scar source, "
          f"{len(engine_only_funcs)} engine-only C++ built-ins)")
    return True


def main():
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    msf_dir = project_root / "Mod_Studio_Files"
    lsp_dir = msf_dir / "lsp"

    # Default paths.
    scardoc_path = str(msf_dir / "scardoc.dat")
    output_path = str(lsp_dir / "dow" / "scar-dow.lua")
    scardoc_html_path: str | None = None

    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <game_root> [--scardoc <path>]")
        print()
        print("Generates consolidated SCAR stubs for LuaLS from:")
        print("  - Tools/ScarDoc/function_list.htm (engine + library functions)")
        print("  - Mod_Studio_Files/scardoc.dat (constants)")
        print()
        print("The <game_root> should be the game installation directory, e.g.:")
        print(f'  python {sys.argv[0]} "C:/Program Files (x86)/Steam/steamapps/'
              f'common/Dawn of War Definitive Edition"')
        sys.exit(1)

    game_path = Path(sys.argv[1])

    # Optional: override scardoc.dat path.
    if "--scardoc" in sys.argv:
        idx = sys.argv.index("--scardoc")
        if idx + 1 < len(sys.argv):
            scardoc_path = sys.argv[idx + 1]

    # Look for ScarDoc HTML.
    html_candidates = [
        game_path / "Tools" / "ScarDoc" / "function_list.htm",
    ]
    for candidate in html_candidates:
        if candidate.is_file():
            scardoc_html_path = str(candidate)
            break

    if not scardoc_html_path:
        print(f"Error: function_list.htm not found under {game_path / 'Tools' / 'ScarDoc'}",
              file=sys.stderr)
        sys.exit(1)

    if not os.path.isfile(scardoc_path):
        print(f"Warning: {scardoc_path} not found, no constants will be emitted",
              file=sys.stderr)
        scardoc_path = None

    if not generate(scardoc_path, scardoc_html_path, output_path):
        sys.exit(1)


if __name__ == "__main__":
    main()
