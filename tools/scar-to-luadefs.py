#!/usr/bin/env python3
"""
Parse SCAR library files and generate LuaLS @meta definition stubs.

Reads .scar files from a game data directory, extracts function definitions
and their --? scardoc annotations, and produces a LuaLS-compatible Lua file
with @param/@return annotations.

Functions already defined in scar-dow.lua (from scardoc.dat) are skipped
to avoid duplicates.  Internal functions (names starting with '_') are
also excluded.

Usage:
    python tools/scar-to-luadefs.py <scar_dir> [--engine-stubs <path>]

Example:
    python tools/scar-to-luadefs.py "C:/Program Files (x86)/Steam/steamapps/common/Dawn of War Definitive Edition/DoWDE/Data/scar"
"""

import os
import re
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


def parse_scardoc_args(args_str: str) -> list[tuple[str, str]]:
    """Parse a --? @args line into [(type, name), ...].

    The format is either:
      'Type1 name1, Type2 name2'      (comma-separated)
      'Type1 name1 Type2 name2'       (space-separated)
    """
    args_str = args_str.strip().rstrip(",")
    if not args_str or args_str.lower() == "void":
        return []

    # Try comma-separated first.
    parts = [p.strip() for p in args_str.split(",") if p.strip()]

    # Handle space-separated "Type Name Type Name ..." (no commas).
    if len(parts) == 1:
        tokens = parts[0].split()
        if len(tokens) >= 4 and len(tokens) % 2 == 0:
            return [(tokens[k], tokens[k + 1]) for k in range(0, len(tokens), 2)]

    result = []
    for part in parts:
        tokens = part.split(None, 1)
        if len(tokens) == 2:
            result.append((tokens[0], tokens[1]))
        elif len(tokens) == 1:
            # Skip bare type names with no parameter name (malformed annotations).
            if "::" in tokens[0]:
                continue
            result.append(("any", tokens[0]))

    if result:
        return result

    return []


def load_engine_function_names(stubs_path: str) -> set[str]:
    """Load function names from the engine stubs file (scar-dow.lua)."""
    names: set[str] = set()
    if not os.path.isfile(stubs_path):
        return names
    with open(stubs_path, "r", encoding="utf-8") as f:
        for line in f:
            m = re.match(r"^function\s+(\w+)\(", line)
            if m:
                names.add(m.group(1))
    return names


def parse_scar_file(filepath: str) -> list[dict]:
    """Parse a single .scar file and extract function metadata."""
    functions = []
    with open(filepath, "r", encoding="utf-8", errors="replace") as f:
        lines = f.readlines()

    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r"^\s*function\s+(\w[\w]*)\s*\(([^)]*)\)", line)
        if not m:
            i += 1
            continue

        func_name = m.group(1)
        param_names = [p.strip() for p in m.group(2).split(",") if p.strip()]

        # Collect preceding --? comment lines.
        comment_lines = []
        j = i - 1
        while j >= 0 and re.match(r"^\s*--", lines[j]):
            comment_lines.insert(0, lines[j].rstrip())
            j -= 1

        # Parse annotations from comments.
        shortdesc = ""
        extdesc = ""
        args_str = ""
        result_str = ""
        group = ""

        for cline in comment_lines:
            # Strip the --? prefix.
            cm = re.match(r"^\s*--\?\s*(.*)", cline)
            if not cm:
                # Plain -- comment, try to use as description.
                cm2 = re.match(r"^\s*--\s*(.*)", cline)
                if cm2 and not shortdesc:
                    text = cm2.group(1).strip()
                    # Skip internal markers and block comment delimiters.
                    if (text and not text.startswith("creator:")
                            and not text.startswith("TODO")
                            and not text.startswith("[[")
                            and not text.startswith("]]")):
                        shortdesc = text
                continue
            text = cm.group(1)

            if text.startswith("@shortdesc"):
                shortdesc = text[len("@shortdesc"):].strip()
            elif text.startswith("@shordesc"):
                shortdesc = text[len("@shordesc"):].strip()
            elif text.startswith("@extdesc"):
                extdesc = text[len("@extdesc"):].strip()
            elif text.startswith("@args"):
                args_str = text[len("@args"):].strip()
            elif text.startswith("@result"):
                result_str = text[len("@result"):].strip()
            elif text.startswith("@group"):
                group = text[len("@group"):].strip()

        # Parse typed arguments from @args.
        typed_args = parse_scardoc_args(args_str)

        # Build final parameter list: prefer @args types, fall back to param names.
        params = []
        if typed_args and len(typed_args) == len(param_names):
            for (atype, _aname), pname in zip(typed_args, param_names):
                params.append((map_type(atype) or "any", pname))
        elif typed_args:
            # Annotation count differs from signature — use annotations.
            for atype, aname in typed_args:
                params.append((map_type(atype) or "any", aname))
        else:
            # No @args — all params are 'any'.
            for pname in param_names:
                params.append(("any", pname))

        # Parse return type.
        ret_type = None
        if result_str:
            ret_type = map_type(result_str)

        functions.append({
            "name": func_name,
            "params": params,
            "return_type": ret_type,
            "shortdesc": shortdesc,
            "extdesc": extdesc,
            "group": group,
            "source_file": os.path.basename(filepath),
        })

        i += 1

    return functions


def _write_func(out, func):
    """Write a single function stub to the output file."""
    if func["shortdesc"]:
        out.write(f"--- {func['shortdesc']}\n")
    if func["extdesc"]:
        ext = func["extdesc"].replace("\\n", "\n--- ").replace("\\t", "  ")
        for ext_line in ext.split("\n"):
            ext_line = ext_line.rstrip()
            if not ext_line or ext_line == "---":
                continue
            if ext_line.startswith("--- "):
                out.write(f"{ext_line}\n")
            elif ext_line:
                out.write(f"--- {ext_line}\n")

    for ptype, pname in func["params"]:
        out.write(f"---@param {pname} {ptype}\n")

    if func["return_type"]:
        out.write(f"---@return {func['return_type']}\n")

    param_names = [p[1] for p in func["params"]]
    out.write(f"function {func['name']}({', '.join(param_names)}) end\n\n")


def generate(scar_dir: str, engine_stubs: str, output_path: str):
    """Scan SCAR directory and generate LuaLS stubs."""
    engine_names = load_engine_function_names(engine_stubs)

    scar_files = sorted(Path(scar_dir).glob("*.scar"))
    if not scar_files:
        print(f"Error: no .scar files found in {scar_dir}", file=sys.stderr)
        return False

    all_functions = []
    for sf in scar_files:
        all_functions.extend(parse_scar_file(str(sf)))

    # Filter: skip engine-defined, internal (_-prefixed), and duplicates.
    seen: set[str] = set()
    filtered = []
    for func in all_functions:
        name = func["name"]
        if name in engine_names or name.startswith("_") or name in seen:
            continue
        seen.add(name)
        filtered.append(func)

    # Group by source file (lowercase).
    by_file: dict[str, list[dict]] = {}
    for func in filtered:
        key = func["source_file"].lower()
        by_file.setdefault(key, []).append(func)

    for funcs in by_file.values():
        funcs.sort(key=lambda f: f["name"])

    # Partition files into tiers.
    tier1_files, tier2_files, standalone_files, other_files = [], [], [], []
    for sf in sorted(by_file):
        if sf in FILE_TIER:
            (tier1_files if FILE_TIER[sf] == 1 else tier2_files).append(sf)
        elif sf in STANDALONE_IMPORT:
            standalone_files.append(sf)
        else:
            other_files.append(sf)

    # Write output.
    with open(output_path, "w", encoding="utf-8", newline="\n") as out:
        out.write("---@meta scar-dow-lib\n\n")
        out.write("-- Auto-generated from DoWDE .scar library files by scar-to-luadefs.py\n")
        out.write(f"-- {len(filtered)} functions\n")
        out.write("-- Do not edit manually — regenerate with: python tools/scar-to-luadefs.py\n")
        out.write("--\n")
        out.write("-- Import tiers:\n")
        out.write('--   Tier 1: available after import("ScarUtil.scar")\n')
        out.write('--   Tier 2: available after import("WXPScarUtil.scar")  (includes Tier 1)\n')
        out.write("--   Standalone: requires explicit import (noted per-section)\n")
        out.write("--\n")
        out.write('-- All mod scripts should import("WXPScarUtil.scar") to get Tier 1 + Tier 2.\n\n')

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

    print(f"Wrote {len(filtered)} functions to {output_path}")
    print(f"  (skipped {len(engine_names)} engine functions, "
          f"{sum(1 for f in all_functions if f['name'].startswith('_'))} internal)")
    return True


def main():
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    lsp_dir = project_root / "Mod_Studio_Files" / "lsp"

    # Default paths.
    engine_stubs = str(lsp_dir / "dow" / "scar-dow.lua")
    output_path = str(lsp_dir / "dow" / "scar-dow-lib.lua")

    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <scar_directory> [--engine-stubs <path>]")
        print(f"\nExample:")
        print(f'  python {sys.argv[0]} "C:/Program Files (x86)/Steam/steamapps/'
              f'common/Dawn of War Definitive Edition/DoWDE/Data/scar"')
        sys.exit(1)

    scar_dir = sys.argv[1]

    # Optional: override engine stubs path.
    if "--engine-stubs" in sys.argv:
        idx = sys.argv.index("--engine-stubs")
        if idx + 1 < len(sys.argv):
            engine_stubs = sys.argv[idx + 1]

    if not os.path.isdir(scar_dir):
        print(f"Error: {scar_dir} is not a directory", file=sys.stderr)
        sys.exit(1)

    if not generate(scar_dir, engine_stubs, output_path):
        sys.exit(1)


if __name__ == "__main__":
    main()
