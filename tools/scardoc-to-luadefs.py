#!/usr/bin/env python3
"""
Convert scardoc.dat / scardoc_coh.dat binary files to LuaLS @meta definition files.

Binary format (per the design doc):
    [uint32] functionCount
    For each function:
        [uint32 len][chars] returnType
        [uint32 len][chars] functionName
        if functionName is not empty:   (empty name = constant)
            [uint32] argCount
            for each arg:
                [uint32 len][chars] argument   — e.g. "PlayerID player"
            [uint32 len][chars] description
        else:
            returnType actually contains the constant name
"""

import struct
import sys
import os
from pathlib import Path

# scardoc type -> LuaLS annotation
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
}


def read_string(f):
    """Read a length-prefixed string from the binary file."""
    raw = f.read(4)
    if len(raw) < 4:
        return ""
    length = struct.unpack("<I", raw)[0]
    if length == 0:
        return ""
    data = f.read(length)
    return data.decode("ascii", errors="replace")


def map_type(scardoc_type):
    """Map a scardoc type name to a LuaLS annotation type."""
    if scardoc_type in TYPE_MAP:
        return TYPE_MAP[scardoc_type]
    # Unknown types — use as-is (custom @class assumed in scar-types.lua)
    return scardoc_type


def parse_argument(arg_str):
    """Parse 'TypeName paramName' or '[TypeName paramName]' into (type, name, optional)."""
    arg_str = arg_str.strip()
    optional = arg_str.startswith("[") and arg_str.endswith("]")
    if optional:
        arg_str = arg_str[1:-1].strip()

    parts = arg_str.split(None, 1)
    if len(parts) == 2:
        return map_type(parts[0]) or "any", parts[1], optional
    elif len(parts) == 1:
        return "any", parts[0], optional
    else:
        return "any", "arg", optional


def convert(input_path, output_path, meta_name):
    """Convert a scardoc.dat binary file to a LuaLS @meta Lua file."""
    functions = []
    constants = []

    with open(input_path, "rb") as f:
        raw = f.read(4)
        if len(raw) < 4:
            print(f"Error: {input_path} is too small", file=sys.stderr)
            return False

        count = struct.unpack("<I", raw)[0]
        print(f"Reading {count} entries from {input_path}")

        for i in range(count):
            return_type = read_string(f)
            name = read_string(f)

            if not name:
                # Constant: return_type holds the constant name
                constants.append(return_type)
            else:
                # Function
                raw_argc = f.read(4)
                if len(raw_argc) < 4:
                    break
                argc = struct.unpack("<I", raw_argc)[0]
                args = [read_string(f) for _ in range(argc)]
                desc = read_string(f)
                functions.append({
                    "name": name,
                    "return_type": return_type,
                    "args": args,
                    "desc": desc,
                })

    # Write output
    with open(output_path, "w", encoding="utf-8", newline="\n") as out:
        out.write(f"---@meta {meta_name}\n\n")
        out.write(f"-- Auto-generated from {os.path.basename(input_path)} by scardoc-to-luadefs.py\n")
        out.write(f"-- {len(functions)} functions, {len(constants)} constants\n")
        out.write("-- Do not edit manually — regenerate with: python tools/scardoc-to-luadefs.py\n\n")

        # Constants
        if constants:
            out.write("-- Constants\n")
            for c in sorted(constants):
                out.write(f"---@type any\n{c} = nil\n\n")

        # Functions
        for func in sorted(functions, key=lambda f: f["name"]):
            # Description
            if func["desc"]:
                for line in func["desc"].split("\n"):
                    out.write(f"---{line}\n")

            # Parameters
            for arg_str in func["args"]:
                arg_type, arg_name, optional = parse_argument(arg_str)
                opt_mark = "?" if optional else ""
                out.write(f"---@param {arg_name}{opt_mark} {arg_type}\n")

            # Return type
            ret = map_type(func["return_type"])
            if ret:
                out.write(f"---@return {ret}\n")

            # Function signature
            param_names = []
            for arg_str in func["args"]:
                _, arg_name, _ = parse_argument(arg_str)
                param_names.append(arg_name)
            out.write(f"function {func['name']}({', '.join(param_names)}) end\n\n")

    print(f"Wrote {len(functions)} functions + {len(constants)} constants to {output_path}")
    return True


def main():
    script_dir = Path(__file__).parent
    project_root = script_dir.parent
    msf_dir = project_root / "Mod_Studio_Files"
    lsp_dir = msf_dir / "lsp"

    # Dawn of War
    dow_input = msf_dir / "scardoc.dat"
    if dow_input.exists():
        convert(str(dow_input), str(lsp_dir / "scar-dow.lua"), "scar-dow")
    else:
        print(f"Warning: {dow_input} not found, skipping DoW stubs", file=sys.stderr)

    # Company of Heroes
    coh_input = msf_dir / "scardoc_coh.dat"
    if coh_input.exists():
        convert(str(coh_input), str(lsp_dir / "scar-coh.lua"), "scar-coh")
    else:
        print(f"Warning: {coh_input} not found, skipping CoH stubs", file=sys.stderr)


if __name__ == "__main__":
    main()
