#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <imguizmo/ImGradient.h>
#include <imguizmo/ImCurveEdit.h>
#include <imguizmo/ImSequencer.h>
#include <imguizmo/GraphEditor.h>
#include <imguizmo/ImVectorEditor.h>
#include <imguizmo/ImZoomSlider.h>
#include <imguizmo/ImLightRig.h>

#include <vector>

#undef NDEBUG
#include <cassert>

// Call each compiled widget once in a headless frame without input. No
// widget may report a change.

struct gradient: ImGradient::Delegate
{
  ImVec4 points[2] = {{1, 0, 0, 0}, {0, 0, 1, 1}};

  size_t GetPointCount () override {return 2;}
  ImVec4* GetPoints () override {return points;}
  int EditPoint (int i, ImVec4 v) override {points[i] = v; return i;}
  ImVec4 GetPoint (float t) override {return points[t < 0.5f ? 0 : 1];}
  void AddPoint (ImVec4) override {}
};

struct curves: ImCurveEdit::Delegate
{
  ImVec2 points[3] = {{0, 0}, {0.5f, 1}, {1, 0.5f}};
  ImVec2 min {0, 0};
  ImVec2 max {1, 1};

  size_t GetCurveCount () override {return 1;}
  ImVec2& GetMin () override {return min;}
  ImVec2& GetMax () override {return max;}
  size_t GetPointCount (size_t) override {return 3;}
  uint32_t GetCurveColor (size_t) override {return 0xFF0000FF;}
  ImVec2* GetPoints (size_t) override {return points;}
  int EditPoint (size_t, int i, ImVec2 v) override {points[i] = v; return i;}
  void AddPoint (size_t, ImVec2) override {}
};

struct sequence: ImSequencer::SequenceInterface
{
  int start = 0;
  int end = 10;

  int GetFrameMin () const override {return 0;}
  int GetFrameMax () const override {return 50;}
  int GetItemCount () const override {return 1;}

  void
  Get (int, int** s, int** e, int* t, unsigned int* c) override
  {
    if (s) *s = &start;
    if (e) *e = &end;
    if (t) *t = 0;
    if (c) *c = 0xFFAA8080;
  }
};

struct graph: GraphEditor::Delegate
{
  std::vector<GraphEditor::Node> nodes {
    {"a", 0, ImRect (ImVec2 (0, 0), ImVec2 (200, 200)), false},
    {"b", 0, ImRect (ImVec2 (400, 0), ImVec2 (600, 200)), true}};
  GraphEditor::Link link {0, 0, 1, 0};

  bool AllowedLink (GraphEditor::NodeIndex, GraphEditor::NodeIndex) override {return true;}
  void SelectNode (GraphEditor::NodeIndex i, bool s) override {nodes[i].mSelected = s;}
  void MoveSelectedNodes (const ImVec2) override {}
  void AddLink (GraphEditor::NodeIndex, GraphEditor::SlotIndex, GraphEditor::NodeIndex, GraphEditor::SlotIndex) override {}
  void DelLink (GraphEditor::LinkIndex) override {}
  void CustomDraw (ImDrawList*, ImRect, GraphEditor::NodeIndex) override {}
  void RightClick (GraphEditor::NodeIndex, GraphEditor::SlotIndex, GraphEditor::SlotIndex) override {}

  const size_t GetTemplateCount () override {return 1;}

  const GraphEditor::Template
  GetTemplate (GraphEditor::TemplateIndex) override
  {
    return {IM_COL32 (160, 160, 180, 255),
            IM_COL32 (100, 100, 140, 255),
            IM_COL32 (110, 110, 150, 255),
            1, nullptr, nullptr,
            1, nullptr, nullptr};
  }

  const size_t GetNodeCount () override {return nodes.size ();}
  const GraphEditor::Node GetNode (GraphEditor::NodeIndex i) override {return nodes[i];}
  const size_t GetLinkCount () override {return 1;}
  const GraphEditor::Link GetLink (GraphEditor::LinkIndex) override {return link;}
};

template <typename F>
static void
frame (F&& widget)
{
  ImGui::NewFrame ();
  ImGuizmo::BeginFrame ();
  ImGui::SetNextWindowSize (ImVec2 (1024, 768));
  ImGui::Begin ("Test");
  widget ();
  ImGui::End ();
  ImGui::Render ();
  assert (ImGui::GetDrawData () != nullptr);
}

int main ()
{
  IMGUI_CHECKVERSION ();
  ImGui::CreateContext ();

  ImGuiIO& io (ImGui::GetIO ());
  io.IniFilename = nullptr;
  io.DisplaySize = ImVec2 (1024, 768);
  io.DeltaTime = 1.0f / 60.0f;
  io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;

  {
    float view[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -5, 1};
    float proj[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, -1, 0, 0, -0.2f, 0};
    float model[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    bool r (true);
    frame ([&] {
      ImGuizmo::SetDrawlist ();
      ImGuizmo::SetRect (0, 0, 1024, 768);
      r = ImGuizmo::Manipulate (view, proj,
                                ImGuizmo::TRANSLATE, ImGuizmo::WORLD,
                                model);
    });
    assert (!r);
  }

  {
    gradient g;
    int sel (0);
    bool r (true);
    frame ([&] {r = ImGradient::Edit (g, ImVec2 (300, 30), sel);});
    assert (!r);
  }

  {
    curves c;
    int r (-1);
    frame ([&] {r = ImCurveEdit::Edit (c, ImVec2 (300, 200), 1);});
    assert (r == 0);
  }

  {
    sequence s;
    int cur (5), sel (-1), first (0);
    bool exp (true), r (true);
    frame ([&] {
      r = ImSequencer::Sequencer (&s, &cur, &exp, &sel, &first,
                                  ImSequencer::SEQUENCER_EDIT_ALL);
    });
    assert (!r);
  }

  // Straight links and the focus frame of GraphEditor are drawn only with
  // these settings.
  //
  {
    graph g;
    GraphEditor::Options o;
    GraphEditor::ViewState v;
    o.mDisplayLinksAsCurves = false;
    frame ([&] {
      ImGui::SetNextWindowFocus ();
      GraphEditor::Show (g, o, v, true);
    });
  }

  {
    ImVectorEditor::Editor e;
    ImVectorEditor::Path p;
    p.anchors.resize (2);
    p.anchors[1].position = ImVec2 (100, 50);
    ImVectorEditor::Result r;
    frame ([&] {r = e.Draw ("##vector", p);});
    assert (!r.changed);
  }

  {
    float lo (0.2f), hi (0.8f);
    bool r (true);
    frame ([&] {r = ImZoomSlider::ImZoomSlider (0.f, 1.f, lo, hi);});
    assert (!r);
  }

  ImGui::DestroyContext ();
}
