#include <imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <imguizmo/ImSequencer.h>
#include <imguizmo/GraphEditor.h>
#include <imguizmo/ImZoomSlider.h>
#include <imguizmo/ImLightRig.h>

#undef NDEBUG
#include <cassert>

int main ()
{
  IMGUI_CHECKVERSION ();
  ImGui::CreateContext ();

  ImGuizmo::Enable (true);
  ImGuizmo::Style& style (ImGuizmo::GetStyle ());
  assert (style.TranslationLineThickness > 0.0f);

  float matrix[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  };
  float translation[3], rotation[3], scale[3];
  ImGuizmo::DecomposeMatrixToComponents (matrix, translation, rotation, scale);
  assert (scale[0] > 0.0f);
  ImGuizmo::RecomposeMatrixFromComponents (translation, rotation, scale, matrix);

  ImGui::DestroyContext ();
}
