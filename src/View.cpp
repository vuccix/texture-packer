#include "View.h"
#include <IO/IO.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <utility>

View::View() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void View::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // -----------------------------------------------------------------------------------------------------------------

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove   | ImGuiWindowFlags_NoTitleBar
                                     | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("Texture Packer", nullptr, flags)) {
        constexpr const char* labels[] = { "Metallic", "Occlusion", "Roughness" };

        for (std::size_t i = 0; i < m_paths.size(); ++i) {
            ImGui::TextUnformatted(labels[i]);

            if (m_paths[i].empty()) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            }

            ImGui::PushID(static_cast<int>(i));
            if (ImGui::Button(m_paths[i].empty() ? "Select..." : "Selected", ImVec2(180, 32))) {
                if (auto path = IO::getPath(IO::Path::Load))
                    m_paths[i] = std::move(*path);
            }
            ImGui::PopID();

            ImGui::PopStyleColor(2);
            ImGui::SameLine();
            ImGui::TextUnformatted(m_paths[i].empty() ? "No file selected" : m_paths[i].filename().string().c_str());
            ImGui::Spacing();
        }

        ImGui::Separator();
        ImGui::Spacing();

        constexpr const char* formats[] = { "Unity", "ORM" };
        int32_t formatIndex             = (m_format == Pack::Format::ORM) ? 1 : 0;

        ImGui::SetNextItemWidth(180);
        if (ImGui::Combo("Format", &formatIndex, formats, IM_ARRAYSIZE(formats)))
            m_format = (formatIndex == 0) ? Pack::Format::Unity : Pack::Format::ORM;

        ImGui::Spacing();

        if (ImGui::Button("Reset", ImVec2(100, 32)))
            m_paths.fill(fs::path{});

        ImGui::SameLine();

        const bool hasAnyPath = std::ranges::any_of(m_paths,
            [](const fs::path& path) {
                return !path.empty();
            });

        ImGui::BeginDisabled(!hasAnyPath);
        if (ImGui::Button("Process", ImVec2(100, 32))) {
            if (const auto outPath = IO::getPath(IO::Path::Save)) {
                Pack::process(m_paths, *outPath, m_format);
                m_paths.fill(fs::path{});
            }
        }

        ImGui::EndDisabled();
    }

    ImGui::End();

    // -----------------------------------------------------------------------------------------------------------------
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
