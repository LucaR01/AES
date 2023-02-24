//
// Created by Luca on 22/02/2023.
//

#include <string_view>
#include <cstdint>

#include "graphics/gui/gui.hpp"

#include "glfw/glfw3.h" //TODO: se lo tengo nel .hpp, rimuoverli da qui.
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/aes.hpp"
#include "core/modes/modes.hpp"
#include "core/padding/padding.hpp"
#include "convert/aes_convert.hpp"
#include "logger/logger.hpp"

//TODO: Guida su ImGui: https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

namespace aes::gui {

void init()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        //return 1;
        std::exit(1);
    }
}

/*void generic_combo(std::array<std::string, 5>& combo_items, char* selected_item)
{
    char* items[combo_items.size()];
    static const char* current_item = selected_item;

    for(unsigned short i = 0; i < combo_items.size(); i++) {
        items[i] = combo_items.at(i);
    }

    if (ImGui::BeginCombo("##combo", current_item)) {
        for (const auto& item : items) {
            bool is_selected = (current_item == item);
            if (ImGui::Selectable(item, is_selected)) {
                current_item = item;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}*/

// Questo è un di esempio che andrò a modificare.
void window()
{
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    //TODO: nullptr?
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL) {
        //return 1;
        std::exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("AES v1.0.0");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)


        //char* buf = (char*)malloc(50 * sizeof(char*)); // così se lo metto static non va; devo usare il char array //TODO: remove
        //ImGui::SameLine();
        //ImGui::InputTextWithHint("Input ", "Plaintext", buf, IM_ARRAYSIZE(buf)); //TODO: remove

        /*if (ImGui::IsItemHovered() || (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))) { //ImGui::IsRootWindowOrAnyChildFocused()
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }*/

        /*char* preview_value;
        ImGui::BeginCombo("AES TYPE", preview_value);*/

        /*const char* items[] = { "AES 128", "AES 192", "AES 256" }; //TODO: rename
        //std::array<std::string, aes::NUM_OF_AES_TYPES> aes_types = { "AES 128", "AES 192", "AES 256" }; //TODO: uncomment
        static const char* current_item = nullptr;
        //static const std::string current_item = "";

        if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            for (const auto& item : items)
            {
                bool is_selected = (current_item == item); // You can store your selection however you want, outside or inside your objects
                if (ImGui::Selectable(item, is_selected)) {
                    current_item = item;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                }
            }
            ImGui::EndCombo();
        }*/

        static bool disabled = true;
        ImGui::Checkbox("Disable", &disabled);
        if (disabled)
        {
            //ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            //ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            ImGui::BeginDisabled(disabled);
            if (ImGui::Button("OK", ImVec2(120, 0)))  { disabled = false; }
            ImGui::EndDisabled();
        }

        /*if (disabled)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }*/

        const char* overlay = "Una progress bar";
        ImGui::ProgressBar(0.35, ImVec2(0, 0), overlay);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("Encrypt_Decrypt_Tab", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Encrypt"))
            {
                ImGui::Text("Encryption");
                ImGui::Text("This is the Encryption tab!\n");

                static char text[128] = ""; // se lo metto static il testo rimane nel text field.
                ImGui::InputTextMultiline("Input", text, IM_ARRAYSIZE(text));

                //aes_types_combo(); //TODO: ho bisogno di recuperare il valore della combo.

                //const std::array<char*, 3>& modes = { "ECB", "CBC", "CFB" }; //TODO: remove
                //static const std::array<std::string, 3>& modes = { "ECB", "CBC", "CFB" };

                //std::vector<std::string_view> vec = aes::cvt::from_map_to_vector<std::string_view>(aes::mod::MODES_NAMES, false); //TODO: uncomment when fixed

                std::vector<std::string> modes; //TODO: rename in modes
                for(const auto& m : aes::mod::MODES_NAMES) {
                    modes.emplace_back(m.second);
                }

                //static char* selected_item = "ECB"; //TODO: remove
                static std::string selected_item_mode = modes.at(0);
                //generic_combo<std::array<std::string, 3>, 3>(modes, selected_item_mode); //TODO: uncomment/remove
                generic_combo<std::vector<std::string>>(modes, selected_item_mode);

                static char password[64] = "";
                ImGui::InputTextWithHint("password (w/ hint)", "Key", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

                if(ImGui::Button("Encrypt"))
                {
                    //char* save_text = text;
                    std::cout << "button clicked " << selected_item_mode << std::endl; //TODO: uncomment
                    std::cout << "input text: " << text << std::endl;
                    ImGui::LogText("clicked");
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Decrypt"))
            {
                ImGui::Text("Decryption");
                ImGui::Text("This is the decryption tab!\n");

                static char text[128] = ""; // se lo metto static il testo rimane nel text field.
                ImGui::InputTextMultiline("Input", text, IM_ARRAYSIZE(text));

                static const std::array<std::string, 3>& aes_types = { "AES 128", "AES 192", "AES 256" };
                static std::string aes_selected_item = aes_types.at(0);
                generic_combo<std::array<std::string, 3>, 3>(aes_types, aes_selected_item);


                static char password[64] = "";
                ImGui::InputTextWithHint("password (w/ hint)", "Key", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

                if(ImGui::Button("Decrypt"))
                {
                    char* save_text = text;
                    std::cout << "button clicked " << aes_selected_item << std::endl;
                    std::cout << "input text: " << save_text << std::endl;
                    ImGui::LogText("clicked");
                }

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h); //TODO: commentare questo perché dà errore, aggiungere -lopengl32
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w); //TODO: commentare questo perché dà errore, aggiungere -lopengl32
        glClear(GL_COLOR_BUFFER_BIT); //TODO: commentare questo perché dà errore, aggiungere -lopengl32
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void button_function(const std::string& button_label, const aes::ops::Operations& operation, const std::string& input, const std::string& key, const std::string& aes_type, const std::string& mode_string, const std::string& padding_string)
{
    //const aes::AES& aes = ALL_AES_TYPES_NAMES.find("AES 128"); //TODO: remove

    const aes::AES aes = aes::cvt::retrieve_key_from_map(aes::ALL_AES_TYPES_NAMES, std::string_view(aes_type)).value();
    AES_DEBUG("aes: {}", aes::ALL_AES_TYPES_NAMES.at(aes))

    const aes::mod::Modes mode = aes::cvt::retrieve_key_from_map(aes::mod::MODES_NAMES, std::string_view(mode_string)).value();
    AES_DEBUG("mode: {}", aes::mod::MODES_NAMES.at(mode))

    const aes::pad::Paddings padding = aes::cvt::retrieve_key_from_map(aes::pad::PADDING_NAMES, std::string_view(padding_string)).value();
    AES_DEBUG("padding: {}", aes::pad::PADDING_NAMES.at(padding))

    /*for(const auto& [k, v] : aes::ALL_AES_TYPES_NAMES) { //TODO: remove
        if(v == aes_type) {
            aes = k;
        }
    }*/

    if(ImGui::Button(button_label.c_str())) {
        switch(operation) {
            case aes::ops::Operations::ENCRYPT:
                //std::vector<uint8_t> encrypted_text = ; //TODO: uncomment
                //TODO: show in a read only text.
                break;
            case aes::ops::Operations::DECRYPT:
                //std::vector<uint8_t> decrypted_plaintext = ; //TODO: uncomment
                //TODO: show in a read only text.
                break;
        }
    }

    /*switch(operation) { //TODO: remove
        case aes::ops::Operations::ENCRYPT:
            if(ImGui::Button(button_label.c_str())) {

            }
            break;
        case aes::ops::Operations::DECRYPT:
            break;
    }*/
}

void show()
{
    init();

    window();
}

} // namespace aes::gui
