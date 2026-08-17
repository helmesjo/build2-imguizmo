# libimguizmo - Immediate-mode 3D gizmo C++ library for Dear ImGui

This is a `build2` package for the [`ImGuizmo`](https://github.com/CedricGuillemet/ImGuizmo)
C++ library. It provides a collection of Dear ImGui widgets for 3D
manipulation and more, including a matrix gizmo, view cube, sequencer,
graph editor, and vector path editor.


## Usage

To start using `libimguizmo` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libimguizmo ^1.10.0
```

Then import the library in your `buildfile`:

```
import libs = libimguizmo%lib{imguizmo}
```

Include Dear ImGui first, then the widget headers. Both the prefixed and
unprefixed forms work. `ImZoomSlider.h` and `ImLightRig.h` use `ImVec2`
operators, so define `IMGUI_DEFINE_MATH_OPERATORS` before including
`imgui.h`, as the upstream example does:

```
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <ImGuizmo.h>
```


## Importable targets

This package provides the following importable targets:

```
lib{imguizmo}
```

The compiled library of all ImGuizmo widgets. Header-only widgets
(`ImZoomSlider.h`, `ImLightRig.h`) are installed with the rest of the public
headers.


## Configuration variables

This package provides no configuration variables.
