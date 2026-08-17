# imguizmo - Immediate-mode 3D gizmo widgets for Dear ImGui

This is a `build2` package repository for [`ImGuizmo`](https://github.com/CedricGuillemet/ImGuizmo),
a collection of Dear ImGui widgets for 3D manipulation and more.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`imguizmo` in your `build2`-based project, then instead see the accompanying
[`PACKAGE-README.md`](libimguizmo/PACKAGE-README.md) file.

The development setup for `imguizmo` uses the standard `bdep`-based workflow.
For example:

```
git clone --recursive .../imguizmo.git
cd imguizmo

bdep init -C @gcc cc config.cxx=g++
bdep update
bdep test
```
