//
// Created by Luca on 22/02/2023.
//

#include <string_view>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <iterator>

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
#include "core/aes_api.hpp"

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
        if (disabled) {
            //ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            //ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            ImGui::BeginDisabled(disabled);
            if (ImGui::Button("OK", ImVec2(120, 0)))  { disabled = false; }
            ImGui::EndDisabled();
        } else {
            if (ImGui::Button("OK", ImVec2(120, 0)))  { disabled = true; }
        }

        /*if (disabled)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }*/

        const char* overlay = "Una progress bar";
        ImGui::ProgressBar(0.35, ImVec2(0, 0), overlay);

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("Encrypt_Decrypt_Tab", tab_bar_flags)) {
            if (ImGui::BeginTabItem("Encrypt")) { //TODO: metterlo in una funzione generica per creare TabItem?
                ImGui::Text("Encryption");
                ImGui::Text("This is the Encryption tab!\n");

                static const std::array<std::string, 3>& aes_types = { "AES 128", "AES 192", "AES 256" }; //TODO: recuperarli dalla map.
                static std::string aes_selected_item = aes_types.at(0);
                generic_combo<std::array<std::string, 3>, 3>(aes_types, aes_selected_item, "##aes_combo");

                static char text[128] = ""; // se lo metto static il testo rimane nel text field. //TODO: u8%s; u8"" (per utf-8)
                ImGui::InputTextMultiline("Input", text, IM_ARRAYSIZE(text));
                //ImGui::TextUnformatted();

                //aes_types_combo(); //TODO: ho bisogno di recuperare il valore della combo.

                //const std::array<char*, 3>& modes = { "ECB", "CBC", "CFB" }; //TODO: remove
                //static const std::array<std::string, 3>& modes = { "ECB", "CBC", "CFB" };

                //std::vector<std::string_view> modes = aes::cvt::from_map_to_vector<std::string_view>(aes::mod::MODES_NAMES, false); //TODO: uncomment when fixed

                std::vector<std::string> modes; //TODO: rename in modes
                for(const auto& m : aes::mod::MODES_NAMES) {
                    modes.emplace_back(m.second);
                }

                //static char* selected_item = "ECB"; //TODO: remove
                static std::string selected_item_mode = modes.at(0);
                //generic_combo<std::array<std::string, 3>, 3>(modes, selected_item_mode); //TODO: uncomment/remove
                generic_combo<std::vector<std::string>>(modes, selected_item_mode, "##modes_combo");

                static char iv[128] = "";
                ImGui::InputTextWithHint("iv:", "iv", iv, IM_ARRAYSIZE(iv));

                static char password[64] = ""; //TODO: rename in key
                ImGui::InputTextWithHint("encryption_key", "Key", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

                std::vector<std::string> paddings;
                for(const auto& p : aes::pad::PADDING_NAMES) {
                    paddings.emplace_back(p.second);
                }

                static std::string selected_item_padding = paddings.at(0);
                generic_combo<std::vector<std::string>>(paddings, selected_item_padding, "##padding_combo");

                static char ciphertext[256] = "";

                if(ImGui::Button("Encrypt")) {
                    //char* save_text = text;
                    //TODO: AES_DEBUG volendo
                    std::cout << "Encrypt" << std::endl;
                    std::cout << "aes: " << aes_selected_item << std::endl;
                    std::cout << "mode: " << selected_item_mode << std::endl;
                    std::cout << "padding: " << selected_item_padding << std::endl;
                    std::cout << "input text: " << text << std::endl;
                    std::cout << "iv: " << iv << std::endl;
                    std::cout << "key: " << password << std::endl;
                    ImGui::LogText("clicked");

                    std::vector<uint8_t> encryption_iv(std::begin(iv), std::end(iv)); //TODO: remove

                    std::strcpy(ciphertext, aes::gui::retrieve_output(aes::ops::Operations::ENCRYPT, text, password, encryption_iv, aes_selected_item, selected_item_mode, selected_item_padding));
                    AES_DEBUG("output: {}", ciphertext)
                }

                ImGui::InputTextMultiline("ciphertext", ciphertext, IM_ARRAYSIZE(ciphertext), ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly);

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Decrypt")) {
                ImGui::Text("Decryption");
                ImGui::Text("This is the decryption tab!\n");

                static char text[128] = ""; // se lo metto static il testo rimane nel text field.
                ImGui::InputTextMultiline("Input", text, IM_ARRAYSIZE(text));

                static const std::array<std::string, 3>& aes_types = { "AES 128", "AES 192", "AES 256" };
                static std::string aes_selected_item = aes_types.at(0);
                generic_combo<std::array<std::string, 3>, 3>(aes_types, aes_selected_item, "##aes_combo");


                static char password[64] = "";
                ImGui::InputTextWithHint("decryption_key", "Key", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

                std::vector<std::string> modes; //TODO: rename in modes
                for(const auto& m : aes::mod::MODES_NAMES) {
                    modes.emplace_back(m.second);
                }

                static char iv[128] = "";
                ImGui::InputTextWithHint("iv:", "iv", iv, IM_ARRAYSIZE(iv));

                static std::string selected_item_mode = modes.at(0);
                generic_combo<std::vector<std::string>>(modes, selected_item_mode, "##modes_combo");

                std::vector<std::string> paddings; //= aes::cvt::from_map_to_vector_views<std::string>(aes::pad::PADDING_NAMES, false); //TODO: uncomment when fixed
                for(const auto& p : aes::pad::PADDING_NAMES) {
                    paddings.emplace_back(p.second);
                }

                static std::string selected_item_padding = paddings.at(0);
                generic_combo<std::vector<std::string>>(paddings, selected_item_padding, "##padding_combo");

                static char output[256] = "";

                if(ImGui::Button("Decrypt")) {
                    //TODO: usare LOGGER
                    std::cout << "Decrypt" << std::endl; //TODO: AES_INFO
                    std::cout << "aes: " << aes_selected_item << std::endl; //TODO: AES_DEBUG
                    std::cout << "mode: " << selected_item_mode << std::endl;
                    std::cout << "padding:  " << selected_item_padding << std::endl;
                    std::cout << "key: " << password << std::endl;
                    std::cout << "iv: " << iv << std::endl;
                    std::cout << "input text: " << text << std::endl;
                    ImGui::LogText("clicked");

                    std::vector<uint8_t> decrcyption_iv(std::begin(iv), std::end(iv));

                    //output = ;
                    std::strcpy(output, aes::gui::retrieve_output(aes::ops::Operations::DECRYPT, text, password, decrcyption_iv, aes_selected_item, selected_item_mode, selected_item_padding));
                    //std::memcpy(output, aes::gui::retrieve_output(aes::ops::Operations::DECRYPT, text, password, iv, aes_selected_item, selected_item_mode, selected_item_padding), sizeof(output));
                    AES_DEBUG("output: {}", output)
                    //output = aes::gui::retrieve_output("decryption_button_label", aes::ops::Operations::DECRYPT, text, password, aes_selected_item, selected_item_mode, selected_item_padding);
                }

                //ImGui::InputText("deciphered_plaintext", output, IM_ARRAYSIZE(output), ImGuiInputTextFlags_ReadOnly);
                ImGui::InputTextMultiline("deciphered_plaintext", output, IM_ARRAYSIZE(output), ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly);

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

//TODO: iv
char* retrieve_output(const aes::ops::Operations& operation, const std::string& input, const std::string& key, const std::optional<std::vector<uint8_t>>& iv, const std::string& aes_type, const std::string& mode_string, const std::string& padding_string)
{
    //const aes::AES& aes = ALL_AES_TYPES_NAMES.find("AES 128"); //TODO: remove

    std::string message = input; //TODO: std::remove_const<std::string>::type(input), but it's unnecessary.
    std::string key_not_const = key;

    const aes::AES aes = aes::cvt::retrieve_key_from_map(aes::ALL_AES_TYPES_NAMES, std::string_view(aes_type)).value();
    AES_DEBUG("aes: {}", aes::ALL_AES_TYPES_NAMES.at(aes))

    const aes::mod::Modes mode = aes::cvt::retrieve_key_from_map(aes::mod::MODES_NAMES, std::string_view(mode_string)).value();
    AES_DEBUG("mode: {}", aes::mod::MODES_NAMES.at(mode))

    const aes::pad::Paddings padding = aes::cvt::retrieve_key_from_map(aes::pad::PADDING_NAMES, std::string_view(padding_string)).value();
    AES_DEBUG("padding: {}", aes::pad::PADDING_NAMES.at(padding))

    AES_DEBUG("iv: {}", std::string(iv.value().cbegin(), iv.value().cend()))

    std::optional<std::vector<std::uint8_t>> iv2;

    /*for(const auto& [k, v] : aes::ALL_AES_TYPES_NAMES) { //TODO: remove
        if(v == aes_type) {
            aes = k;
        }
    }*/

    /*if(ImGui::Button(button_label.c_str())) {
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
    }*/

    /*switch(operation) { //TODO: remove
        case aes::ops::Operations::ENCRYPT:
            if(ImGui::Button(button_label.c_str())) {

            }
            break;
        case aes::ops::Operations::DECRYPT:
            break;
    }*/

    switch(operation) {
        case aes::ops::Operations::ENCRYPT:
        {
            const std::vector<uint8_t>& ciphertext = aes::api::encrypt(message, key_not_const, iv, padding, mode, aes); //TODO: mettere iv al posto di iv2
            const unsigned char* ciphertext2 = &ciphertext[0];
            //const unsigned char* ciphertext4 = &aes::api::encrypt(message, key_not_const, iv, padding, mode, aes)[0]; //TODO: remove
            unsigned char ciphertext3[256];
            std::copy(ciphertext.begin(), ciphertext.end(), ciphertext3);
            //TODO: return char* or char[]?
            return (char*) ciphertext2; //reinterpret_cast<char *>(&ciphertext3[0])
            //break;
        }
        case aes::ops::Operations::DECRYPT:
        {
            const std::vector<uint8_t>& decrypted_plaintext = aes::api::decrypt(message, key_not_const, iv, padding, mode, aes); //TODO: mettere iv al posto di iv2
            return (char*) &decrypted_plaintext[0];
            //break;
        }
    }

    //const char* temp = ""; //TODO: remove
    return const_cast<char*>("");
}

void show()
{
    init();

    window();
}

} // namespace aes::gui
