#include "gui.h"

#include <GLFW/glfw3.h>  // Will drag system OpenGL headers
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <math.h>

#include "log.h"
#include "scene/components/meshComp.h"
#include "scene/entity.h"
#include "window/window.h"

namespace neith {

bool Gui::mRenderGui = false;

void Gui::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    const char* glsl_version = "#version 330";
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Window::GetGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiStyle& style = ImGui::GetStyle();
    float highDPIscaleFactor = 1.8f;
    style.ScaleAllSizes(highDPIscaleFactor);

    io.Fonts->AddFontFromFileTTF("assets/fonts/Hack-Regular.ttf", floor(12.0f * highDPIscaleFactor), NULL, NULL);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use
    // ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application
    // (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double
    // backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL,
    // io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

    // Our state
    // bool show_demo_window = true;
}

void Gui::Update()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your
    // inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or
    // clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or
    // clear/overwrite your copy of the keyboard data. Generally you may always pass all inputs to dear imgui, and
    // hide them from your application based on those two flags.
    if (!mRenderGui) {
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code
    // to learn more about Dear ImGui!).
    static bool show_demo_window = true;
    static bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        // static float f = 0.0f;
        // static int counter = 0;

        // ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!" and append into it.

        // ImGui::Text("This is some useful text.");           // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        // ImGui::ColorEdit3("clear color", (float*)&clear_color);  // Edit 3 floats representing a color

        //// Entity::GetChildCount(0)

        // CreateTree(0);

        // if (ImGui::Button(
        //"Button"))  // Buttons return true when clicked (most widgets return true when edited/activated)
        // counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
        // ImGui::GetIO().Framerate);
        // ImGui::End();
    }
    static unsigned int selectEntityID = 0;
    {
        ImGui::Begin("Entities");  // Create a window called "Hello, world!" and append into it.

        if (ImGui::TreeNode("Scene")) {
            selectEntityID = CreateTree(0, selectEntityID);
            ImGui::TreePop();
        }

        ImGui::End();
    }
    {
        ImGui::Begin("Details");  // Create a window called "Hello, world!" and append into it.

        DisplayEntityDetails(selectEntityID);

        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window) {
        ImGui::Begin("Another Window",
                     &show_another_window);  // Pass a pointer to our bool variable (the window will have a closing
                                             // button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    // int display_w, display_h;
    // glfwGetFramebufferSize(window, &display_w, &display_h);
    // glViewport(0, 0, display_w, display_h);
    // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
    // clear_color.w);
    // glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glfwSwapBuffers(window);
}

unsigned int Gui::CreateTree(unsigned int entityID, unsigned int selectEntityID)
{
    for (int i = 0; i < Entity::GetChildCount(entityID); i++) {
        unsigned int childEntityID = Entity::GetChild(entityID, i);

        if (Entity::GetChildCount(childEntityID) == 0) {
            bool isSelected = (selectEntityID == childEntityID);
            if (ImGui::Selectable(("   " + Entity::GetName(childEntityID)).data(), isSelected)) {
                selectEntityID = childEntityID;
            }
        }
        else {
            if (ImGui::TreeNode(Entity::GetName(childEntityID).data())) {
                selectEntityID = CreateTree(childEntityID, selectEntityID);
                ImGui::TreePop();
            }
        }
    }
    return selectEntityID;
}

void Gui::DisplayEntityDetails(unsigned int entityID)
{
    for (Component* comp : Entity::GetComponents(entityID)) {
        DebugInfo* debugInfo = comp->GetDebugInfo(entityID);
        if (ImGui::CollapsingHeader(debugInfo->message.data())) {
            DisplayDebugInfo(debugInfo->child.at(0));
        }
        delete debugInfo;
    }
}
void Gui::DisplayDebugInfo(DebugInfo* debugInfo)
{
    if (debugInfo->child.size() == 0) {
        ImGui::Text(debugInfo->message.data());
    }
    else {
        if (ImGui::TreeNode(debugInfo->message.data())) {
            for (DebugInfo* child : debugInfo->child) {
                DisplayDebugInfo(child);
            }
            ImGui::TreePop();
        }
    }
}

void Gui::Clean()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
}  // namespace neith
