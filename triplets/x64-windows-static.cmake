# Custom overlay triplet for CorsixModStudio
# Overrides the built-in x64-windows-static triplet to enable wxUSE_STD_CONTAINERS,
# fixing wxAuiNotebook tab-close crashes caused by iterator invalidation.
# See: https://forums.wxwidgets.org/viewtopic.php?t=52366

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)

# wxWidgets: use STL-backed containers to fix wxAuiNotebook iterator bugs
set(WXWIDGETS_USE_STD_CONTAINERS ON)
