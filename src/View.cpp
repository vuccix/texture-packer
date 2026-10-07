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

namespace {

void openURL(const std::string& url) {
#ifdef _WIN32
    const std::string command = R"(start "" ")" + url + "\"";
#elif __APPLE__
    const std::string command = "open \"" + url + "\"";
#else
    const std::string command = "xdg-open \"" + url + "\"";
#endif

    std::system(command.c_str());
}

}

void View::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // -----------------------------------------------------------------------------------------------------------------

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);

    constexpr auto dimensionPopup = "Image dimensions do not match";
    constexpr auto successPopup   = "Success";
    bool openDimensionPopup       = false;
    bool openSuccessPopup         = false;

    auto modal = [](const char* popup, const char* message) {
        if (ImGui::BeginPopupModal(popup, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::TextUnformatted(message);
            ImGui::Spacing();

            if (ImGui::Button("OK", ImVec2(100, 0)))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
    };

    constexpr const char* labels[]   = { "Metallic", "Occlusion", "Roughness" };
    constexpr const char* formats[]  = { "Unity", "ORM" };

    constexpr ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove   | ImGuiWindowFlags_NoTitleBar
                                     | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    if (ImGui::Begin("Texture Packer", nullptr, flags)) {
        const auto imageForSlot = [this](const size_t i) -> Image& {
            switch (i) {
                case 0:  return m_bundle.metallic;
                case 1:  return m_bundle.occlusion;
                default: return m_bundle.roughness;
            }
        };

        for (size_t i = 0; i < m_paths.size(); ++i) {
            ImGui::TextUnformatted(labels[i]);

            if (m_paths[i].empty()) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered));
            }
            else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Button));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));
            }

            ImGui::PushID(static_cast<int32_t>(i));
            if (ImGui::Button(m_paths[i].empty() ? "Select..." : "Replace...", ImVec2(180, 32))) {

                if (const auto selectedPath = IO::getPath(IO::Path::Load)) {
                    const auto [newWidth, newHeight] = IO::imgInfo(*selectedPath);

                    bool dimensionsMatch = true;
                    for (size_t j = 0; j < m_paths.size(); ++j) {
                        if (j == i || m_paths[j].empty()) continue;

                        const Image& other = imageForSlot(j);
                        if (other.hasTex() && (other.width != newWidth || other.height != newHeight)) {
                            dimensionsMatch = false;
                            break;
                        }
                    }

                    if (!dimensionsMatch)
                        openDimensionPopup = true;
                    else {
                        Image image     = IO::load(*selectedPath);
                        imageForSlot(i) = std::move(image);
                        m_paths[i]      = *selectedPath;
                        m_bundle.width  = imageForSlot(i).width;
                        m_bundle.height = imageForSlot(i).height;
                    }
                }
            }
            ImGui::PopID();
            ImGui::PopStyleColor(2);

            ImGui::SameLine();

            const std::string filename = m_paths[i].empty() ? "No file selected" : m_paths[i].filename().string();
            ImGui::TextUnformatted(filename.c_str());

            ImGui::Spacing();
        }

        if (openDimensionPopup)
            ImGui::OpenPopup(dimensionPopup);

        ImGui::Separator();
        ImGui::Spacing();

        int32_t formatIndex = (m_format == Pack::Format::ORM) ? 1 : 0;

        ImGui::SetNextItemWidth(180);
        if (ImGui::Combo("Format", &formatIndex, formats, IM_ARRAYSIZE(formats)))
            m_format = (formatIndex == 0) ? Pack::Format::Unity : Pack::Format::ORM;

        ImGui::Spacing();

        if (ImGui::Button("Reset", ImVec2(100, 32))) {
            m_paths.fill(fs::path{});
            m_bundle.clear();
        }

        ImGui::SameLine();

        const bool hasAnyImage = std::ranges::any_of(
            m_paths, [](const fs::path& path) {
                return !path.empty();
            });

        ImGui::BeginDisabled(!hasAnyImage);
        if (ImGui::Button("Process", ImVec2(100, 32))) {
            if (const auto outPath = IO::getPath(IO::Path::Save)) {
                Pack::process(m_bundle, *outPath, m_format);
                m_paths.fill(fs::path{});
                m_bundle.clear();
                openSuccessPopup = true;
            }
        }
        ImGui::EndDisabled();

        constexpr float buttonWidth = 100.f;
        const     float rightEdge   = ImGui::GetWindowContentRegionMax().x;
        ImGui::SameLine(rightEdge - buttonWidth);
        if (ImGui::Button("Report a Bug", ImVec2(buttonWidth, 32)))
            ::openURL("https://github.com/vuccix/texture-packer/issues/new");

        if (openSuccessPopup)
            ImGui::OpenPopup(successPopup);

        modal(dimensionPopup, "You cannot load images with different dimensions");
        modal(successPopup, "Textures successfully packed");
    }

    ImGui::End();

    // -----------------------------------------------------------------------------------------------------------------
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
