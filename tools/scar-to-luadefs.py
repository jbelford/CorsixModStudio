#!/usr/bin/env python3
"""
Generate consolidated LuaLS @meta stubs for Dawn of War SCAR scripting.

Merges two sources into a single definition file:
  1. scardoc.dat — binary file containing engine-registered C++ functions
  2. .scar files — Lua library functions with --? scardoc annotations

Usage:
    python tools/scar-to-luadefs.py <scar_dir> [--scardoc <path>]

Example:
    python tools/scar-to-luadefs.py "C:/Program Files (x86)/Steam/steamapps/common/Dawn of War Definitive Edition/DoWDE/Data/scar"
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
        for i, line in enumerate(func["shortdesc"].split("\n")):
            out.write(f"---{line}\n") if i > 0 else out.write(f"--- {line}\n")
    if func.get("extdesc"):
        ext = func["extdesc"].replace("\\n", "\n--- ").replace("\\t", "  ")
        for ext_line in ext.split("\n"):
            ext_line = ext_line.rstrip()
            if not ext_line or ext_line == "---":
                continue
            if ext_line.startswith("--- "):
                out.write(f"{ext_line}\n")
            elif ext_line:
                out.write(f"--- {ext_line}\n")

    for param in func["params"]:
        ptype, pname = param[0], param[1]
        opt = "?" if len(param) > 2 and param[2] else ""
        out.write(f"---@param {pname}{opt} {ptype}\n")

    if func["return_type"]:
        out.write(f"---@return {func['return_type']}\n")

    param_names = [p[1] for p in func["params"]]
    out.write(f"function {func['name']}({', '.join(param_names)}) end\n\n")


def generate(scar_dir: str, scardoc_path: str | None, output_path: str):
    """Scan SCAR directory and scardoc.dat, generate consolidated LuaLS stubs."""
    # 1. Parse scardoc.dat if available.
    engine_funcs: list[dict] = []
    constants: list[str] = []
    engine_by_name: dict[str, dict] = {}

    if scardoc_path and os.path.isfile(scardoc_path):
        engine_funcs, constants = parse_scardoc_dat(scardoc_path)
        engine_by_name = {f["name"]: f for f in engine_funcs}
        print(f"Read {len(engine_funcs)} engine functions + "
              f"{len(constants)} constants from {os.path.basename(scardoc_path)}")

    # 2. Parse .scar files.
    scar_files = sorted(Path(scar_dir).glob("*.scar"))
    if not scar_files:
        print(f"Error: no .scar files found in {scar_dir}", file=sys.stderr)
        return False

    all_scar_funcs: list[dict] = []
    for sf in scar_files:
        all_scar_funcs.extend(parse_scar_file(str(sf)))

    # Build map of SCAR functions (first occurrence wins for duplicates).
    scar_by_name: dict[str, dict] = {}
    for func in all_scar_funcs:
        name = func["name"]
        if name not in scar_by_name and not name.startswith("_"):
            scar_by_name[name] = func

    # 3. Merge: for functions in both sources, use the engine annotations
    # (they have full param types from the C++ registration) but place them
    # under their .scar source file for grouping.
    # For functions only in scardoc.dat, they go in the "Engine-only" section.
    merged_funcs: list[dict] = []
    engine_only_funcs: list[dict] = []

    # Process all engine functions first.
    for func in engine_funcs:
        name = func["name"]
        if name in scar_by_name:
            # Present in both — use engine data but tag with .scar source file.
            merged = dict(func)
            merged["source_file"] = scar_by_name[name]["source_file"]
            merged_funcs.append(merged)
        else:
            engine_only_funcs.append(func)

    # Add SCAR-only functions (not in scardoc.dat).
    for name, func in scar_by_name.items():
        if name not in engine_by_name:
            merged_funcs.append(func)

    # Group merged functions by source file.
    by_file: dict[str, list[dict]] = {}
    for func in merged_funcs:
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

    total_funcs = len(engine_only_funcs) + len(merged_funcs)
    overlap_count = sum(1 for f in merged_funcs if f["name"] in engine_by_name)

    # 4. Write consolidated output.
    with open(output_path, "w", encoding="utf-8", newline="\n") as out:
        out.write("---@meta scar-dow\n\n")
        out.write("-- Auto-generated SCAR stubs for Dawn of War by scar-to-luadefs.py\n")
        out.write(f"-- Sources: scardoc.dat + .scar library files\n")
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

        # Library functions by tier (includes both engine+scar overlap and scar-only).
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

        # Engine-only functions (no .scar file counterpart).
        if engine_only_funcs:
            out.write(f"{'--' * 40}\n")
            out.write("-- Engine-only: C++ built-ins (no .scar wrapper)\n")
            out.write(f"{'--' * 40}\n\n")
            for func in sorted(engine_only_funcs, key=lambda f: f["name"]):
                _write_func(out, func)

    print(f"Wrote {total_funcs} functions + {len(constants)} constants to {output_path}")
    if engine_funcs:
        print(f"  ({len(engine_only_funcs)} engine-only, "
              f"{overlap_count} from both sources grouped by .scar file, "
              f"{len(merged_funcs) - overlap_count} library-only)")
    return True


def main():
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    msf_dir = project_root / "Mod_Studio_Files"
    lsp_dir = msf_dir / "lsp"

    # Default paths.
    scardoc_path = str(msf_dir / "scardoc.dat")
    output_path = str(lsp_dir / "dow" / "scar-dow.lua")

    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <scar_directory> [--scardoc <path>]")
        print()
        print("Generates consolidated SCAR stubs for LuaLS from both")
        print("scardoc.dat (engine functions) and .scar library files.")
        print()
        print(f"Example:")
        print(f'  python {sys.argv[0]} "C:/Program Files (x86)/Steam/steamapps/'
              f'common/Dawn of War Definitive Edition/DoWDE/Data/scar"')
        sys.exit(1)

    scar_dir = sys.argv[1]

    # Optional: override scardoc.dat path.
    if "--scardoc" in sys.argv:
        idx = sys.argv.index("--scardoc")
        if idx + 1 < len(sys.argv):
            scardoc_path = sys.argv[idx + 1]

    if not os.path.isdir(scar_dir):
        print(f"Error: {scar_dir} is not a directory", file=sys.stderr)
        sys.exit(1)

    if not os.path.isfile(scardoc_path):
        print(f"Warning: {scardoc_path} not found, generating library stubs only",
              file=sys.stderr)
        scardoc_path = None

    if not generate(scar_dir, scardoc_path, output_path):
        sys.exit(1)


if __name__ == "__main__":
    main()
