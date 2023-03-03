//
// Created by Luca on 22/02/2023.
//

#include <string_view>
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <cstring> // per strlen()

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
#include "version/version.hpp"
#include "core/operations/operations.hpp"

#include "imfilebrowser.h"

//TODO: Guida su ImGui: https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html

//TODO: imgui-notify: https://github.com/patrickcjk/imgui-notify

// ImGui i text devono essere static così rimane la stringa.
// ImGui in un label se c'è ## vuol dire che non sarà visibile.

namespace aes::gui {

void init()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        //return 1;
        std::exit(1);
    }
}

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

    ImGui::FileBrowser encryption_input_file_dialog;
    ImGui::FileBrowser encryption_output_file_dialog;
    ImGui::FileBrowser decryption_input_file_dialog;
    ImGui::FileBrowser decryption_output_file_dialog;

    encryption_input_file_dialog.SetTitle("Encryption Input File Dialog");
    encryption_output_file_dialog.SetTitle("Encryption Output File Dialog");
    decryption_input_file_dialog.SetTitle("Decryption Input File Dialog");
    decryption_output_file_dialog.SetTitle("Decryption Output File Dialog");

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

        ImGui::Begin(aes::vrs::aes.version.c_str());

        static bool disabled = true; //TODO: remove?
        ImGui::Checkbox("Disable", &disabled);
        if (disabled) {
            ImGui::BeginDisabled(disabled);
            if (ImGui::Button("OK", ImVec2(120, 0)))  { disabled = false; }
            ImGui::EndDisabled();
        } else {
            if (ImGui::Button("OK", ImVec2(120, 0)))  { disabled = true; }
        }

        std::vector<std::string> modes; //TODO: usare aes::cvt::from_map_to_vector_views when fixed
        for(const auto& m : aes::mod::MODES_NAMES) {
            modes.emplace_back(m.second);
        }

        std::vector<std::string> paddings; //= aes::cvt::from_map_to_vector_views<std::string>(aes::pad::PADDING_NAMES, false); //TODO: uncomment when fixed
        for(const auto& p : aes::pad::PADDING_NAMES) {
            paddings.emplace_back(p.second);
        }

        std::vector<std::string> aes_types; //TODO: usare aes::cvt::from_map_to_vector_views when fixed
        for(const auto& a: aes::ALL_AES_TYPES_NAMES) {
            aes_types.emplace_back(a.second);
        }

        static char encryption_input_file_path[128] = "";
        static char encryption_output_file_path[128] = "";
        static char decryption_input_file_path[128] = "";
        static char decryption_output_file_path[128] = "";

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("Encrypt_Decrypt_Tab", tab_bar_flags)) {
            if (ImGui::BeginTabItem("Encrypt")) { //TODO: metterlo in una funzione generica per creare TabItem?
                ImGui::Text("Encryption");
                ImGui::Text("This is the Encryption tab!\n");

                static int encrypt_operation = 0;
                //TODO: Volendo usare l'enum Encryption Operations.
                ImGui::RadioButton("Message", &encrypt_operation, 0); ImGui::SameLine();
                ImGui::RadioButton("File", &encrypt_operation, 1);

                switch(encrypt_operation) {
                    case 0:
                        static std::string encrypt_message_aes_selected_item = aes_types.at(0);
                        generic_combo<std::vector<std::string>>(aes_types, encrypt_message_aes_selected_item, "##aes_combo");

                        static char text[128] = ""; //TODO: u8%s; u8"" (per utf-8)
                        ImGui::InputTextMultiline("Input", text, IM_ARRAYSIZE(text));

                        static std::string encrypt_message_selected_item_mode = modes.at(0);
                        generic_combo<std::vector<std::string>>(modes, encrypt_message_selected_item_mode, "##modes_combo");

                        static char encrypt_message_iv[128] = "";
                        ImGui::InputTextWithHint("iv:", "iv", encrypt_message_iv, IM_ARRAYSIZE(encrypt_message_iv));

                        static char encrypt_message_key[64] = ""; //TODO: rename in key
                        ImGui::InputTextWithHint("encryption_key", "Key", encrypt_message_key, IM_ARRAYSIZE(encrypt_message_key), ImGuiInputTextFlags_Password);

                        static std::string encrypt_message_selected_item_padding = paddings.at(0);
                        generic_combo<std::vector<std::string>>(paddings, encrypt_message_selected_item_padding, "##padding_combo");

                        static char ciphertext[256] = "";

                        if(ImGui::Button("Encrypt")) {
                            //TODO: AES_DEBUG volendo
                            std::cout << "Encrypt" << std::endl;
                            std::cout << "aes: " << encrypt_message_aes_selected_item << std::endl;
                            std::cout << "mode: " << encrypt_message_selected_item_mode << std::endl;
                            std::cout << "padding: " << encrypt_message_selected_item_padding << std::endl;
                            std::cout << "input text: " << text << std::endl;
                            std::cout << "iv: " << encrypt_message_iv << std::endl;
                            std::cout << "key: " << encrypt_message_key << std::endl;
                            ImGui::LogText("clicked");

                            std::vector<uint8_t> encryption_iv(std::begin(encrypt_message_iv), std::end(encrypt_message_iv)); //TODO: remove

                            if(std::strlen(text) > 0 && std::strlen(encrypt_message_key) > 0) {
                                std::strcpy(ciphertext, aes::gui::retrieve_output(aes::ops::Operations::ENCRYPT, text, encrypt_message_key, encryption_iv, encrypt_message_aes_selected_item, encrypt_message_selected_item_mode, encrypt_message_selected_item_padding));
                                AES_DEBUG("ciphertext: {}", ciphertext)
                            } /*else { //TODO: uncomment/remove
                        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Input size <= 0");
                    }*/
                        }
                        ImGui::InputTextMultiline("ciphertext", ciphertext, IM_ARRAYSIZE(ciphertext), ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly);
                        break;
                    case 1:
                        static std::string encrypt_file_aes_selected_item = aes_types.at(0);
                        generic_combo<std::vector<std::string>>(aes_types, encrypt_file_aes_selected_item, "##aes_combo");

                        static std::string encrypt_file_selected_item_mode = modes.at(0);
                        generic_combo<std::vector<std::string>>(modes, encrypt_file_selected_item_mode, "##modes_combo");

                        static char encrypt_file_iv[128] = "";
                        ImGui::InputTextWithHint("iv:", "iv", encrypt_file_iv, IM_ARRAYSIZE(encrypt_file_iv));

                        static char encrypt_file_key[64] = "";
                        ImGui::InputTextWithHint("encryption_key", "Key", encrypt_file_key, IM_ARRAYSIZE(encrypt_file_key), ImGuiInputTextFlags_Password);

                        static std::string encrypt_file_selected_item_padding = paddings.at(0);
                        generic_combo<std::vector<std::string>>(paddings, encrypt_file_selected_item_padding, "##padding_combo");

                        if(ImGui::Button("encryption_input_file_dialog")) { //TODO: rename label.
                            encryption_input_file_dialog.Open();
                        }

                        if(ImGui::Button("encryption_output_file_dialog")) {
                            encryption_output_file_dialog.Open();
                        }

                        ImGui::SameLine();
                        ImGui::InputText("input file ", encryption_input_file_path, IM_ARRAYSIZE(encryption_input_file_path), ImGuiInputTextFlags_ReadOnly);
                        //ImGui::NewLine();

                        //ImGui::SameLine();
                        ImGui::InputText("output file ", encryption_output_file_path, IM_ARRAYSIZE(encryption_output_file_path), ImGuiInputTextFlags_ReadOnly);

                        if(ImGui::Button("Encrypt File")) {
                            AES_INFO("Encrypt File")
                            AES_DEBUG("aes: {}", encrypt_file_aes_selected_item)
                            AES_DEBUG("mode: {}", encrypt_file_selected_item_mode)
                            AES_DEBUG("padding: {}", encrypt_file_selected_item_padding)
                            AES_DEBUG("iv: {}", encrypt_file_iv)
                            AES_DEBUG("key: {}", encrypt_file_key)
                            AES_DEBUG("encryption_input_file_path: {}", encryption_input_file_path)
                            AES_DEBUG("encryption_output_file_path: {}", encryption_output_file_path)

                            if(std::strlen(encryption_input_file_path) > 0 && std::strlen(encryption_output_file_path) > 0) {
                                //TODO: chiamare funzione per cifrare il file.
                            }
                        }
                        break;
                    default:
                        break;
                }

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Decrypt")) {
                ImGui::Text("Decryption");
                ImGui::Text("This is the Decryption tab!\n");

                static int decrypt_operation = 0;
                ImGui::RadioButton("Message", &decrypt_operation, 0); ImGui::SameLine();
                ImGui::RadioButton("File", &decrypt_operation, 1);

                switch(decrypt_operation) {
                    case 0:
                        static char text[128] = ""; // se lo metto static il testo rimane nel text field.
                        ImGui::InputTextMultiline("Input", text, IM_ARRAYSIZE(text));

                        static std::string decrypt_message_aes_selected_item = aes_types.at(0);
                        generic_combo<std::vector<std::string>>(aes_types, decrypt_message_aes_selected_item, "##aes_combo");

                        static char decrypt_message_key[64] = "";
                        ImGui::InputTextWithHint("decryption_key", "Key", decrypt_message_key, IM_ARRAYSIZE(decrypt_message_key), ImGuiInputTextFlags_Password);

                        static char decrypt_message_iv[128] = "";
                        ImGui::InputTextWithHint("iv", "iv", decrypt_message_iv, IM_ARRAYSIZE(decrypt_message_iv));

                        static std::string decrypt_message_selected_item_mode = modes.at(0);
                        generic_combo<std::vector<std::string>>(modes, decrypt_message_selected_item_mode, "##modes_combo");

                        static std::string decrypt_message_selected_item_padding = paddings.at(0);
                        generic_combo<std::vector<std::string>>(paddings, decrypt_message_selected_item_padding, "##padding_combo");

                        static char output[256] = "";

                        if(ImGui::Button("Decrypt")) {
                            //TODO: usare LOGGER
                            std::cout << "Decrypt" << std::endl; //TODO: AES_INFO
                            std::cout << "aes: " << decrypt_message_aes_selected_item << std::endl; //TODO: AES_DEBUG
                            std::cout << "mode: " << decrypt_message_selected_item_mode << std::endl;
                            std::cout << "padding:  " << decrypt_message_selected_item_padding << std::endl;
                            std::cout << "key: " << decrypt_message_key << std::endl;
                            std::cout << "iv: " << decrypt_message_iv << std::endl;
                            std::cout << "input text: " << text << std::endl;
                            ImGui::LogText("clicked");

                            std::vector<uint8_t> decrcyption_iv(std::begin(decrypt_message_iv), std::end(decrypt_message_iv));

                            if(std::strlen(text) > 0 && std::strlen(decrypt_message_key) > 0) {
                                std::strcpy(output, aes::gui::retrieve_output(aes::ops::Operations::DECRYPT, text, decrypt_message_key, decrcyption_iv, decrypt_message_aes_selected_item, decrypt_message_selected_item_mode, decrypt_message_selected_item_padding));
                                //std::memcpy(output, aes::gui::retrieve_output(aes::ops::Operations::DECRYPT, text, password, iv, aes_selected_item, selected_item_mode, selected_item_padding), sizeof(output)); //TODO: remove
                                AES_DEBUG("output: {}", output)
                            } /*else { //TODO: remove/uncomment
                        ImGui::TextColored(ImVec4(.7f, .2f, .3f, 1.f), "Input text size <= 0");
                    }*/
                            /*if (ImGui::BeginPopupModal("Speed Adjustment")) { //TODO: remove/uncomment
                                std::string speed_text = "You're adjusting the speed";
                                speed_text += "\n";
                                ImGui::Text(speed_text.c_str());
                                //list the current speed
                                std::string currSpeed= "This is the current speed: ";
                                ImGui::Text(currSpeed.c_str());


                                ImGui::Spacing();
                                ImGui::NextColumn();

                                ImGui::Columns(1);
                                ImGui::Separator();

                                ImGui::NewLine();

                                //ImGui::SameLine(GetWindowWidth() - 270);
                                //click ok when finished adjusting
                                if (ImGui::Button("OK finished adjusting", ImVec2(200, 0))) {
                                    //speedpopup= false;
                                    ImGui::CloseCurrentPopup();
                                }

                                ImGui::EndPopup();
                            }*/
                        }

                        //ImGui::InputText("deciphered_plaintext", output, IM_ARRAYSIZE(output), ImGuiInputTextFlags_ReadOnly);
                        ImGui::InputTextMultiline("deciphered_plaintext", output, IM_ARRAYSIZE(output), ImVec2(0, 0), ImGuiInputTextFlags_ReadOnly);
                        break;
                    case 1:
                        static std::string decrypt_file_aes_selected_item = aes_types.at(0);
                        generic_combo<std::vector<std::string>>(aes_types, decrypt_file_aes_selected_item, "##aes_combo");


                        static char decrypt_file_key[64] = "";
                        ImGui::InputTextWithHint("decryption_key", "Key", decrypt_file_key, IM_ARRAYSIZE(decrypt_file_key), ImGuiInputTextFlags_Password);

                        static char decrypt_file_iv[128] = "";
                        ImGui::InputTextWithHint("iv", "iv", decrypt_file_iv, IM_ARRAYSIZE(decrypt_file_iv));

                        static std::string decrypt_file_selected_item_mode = modes.at(0);
                        generic_combo<std::vector<std::string>>(modes, decrypt_file_selected_item_mode, "##modes_combo");

                        static std::string decrypt_file_selected_item_padding = paddings.at(0);
                        generic_combo<std::vector<std::string>>(paddings, decrypt_file_selected_item_padding, "##padding_combo");

                        if(ImGui::Button("decryption_input_file_dialog")) {
                            decryption_input_file_dialog.Open();
                        }

                        if(ImGui::Button("decryption_output_file_dialog")) {
                            decryption_output_file_dialog.Open();
                        } ImGui::SameLine();

                        ImGui::InputText("decryption input file ", decryption_input_file_path, IM_ARRAYSIZE(decryption_input_file_path), ImGuiInputTextFlags_ReadOnly);
                        ImGui::NewLine();

                        //ImGui::SameLine();
                        ImGui::InputText("decryption output file ", decryption_output_file_path, IM_ARRAYSIZE(decryption_output_file_path), ImGuiInputTextFlags_ReadOnly);

                        if(ImGui::Button("Decrypt File")) {
                            AES_DEBUG("decryption_input_file_path: {}", decryption_input_file_path)
                            AES_DEBUG("decryption_output_file_path: {}", decryption_output_file_path)

                            if(std::strlen(decryption_input_file_path) > 0 && std::strlen(decryption_output_file_path) > 0) {
                                //TODO: chiamare funzione per decifrare il file.
                                //aes::api::decrypt_file();
                            }
                        }
                        break;
                    default:
                        break;
                }

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();

        encryption_input_file_dialog.Display();
        encryption_output_file_dialog.Display();
        decryption_input_file_dialog.Display();
        decryption_output_file_dialog.Display();

        if(encryption_input_file_dialog.HasSelected())
        {
            std::strcpy(encryption_input_file_path, encryption_input_file_dialog.GetSelected().string().data());
            std::cout << "encryption_input_file_path: " << encryption_input_file_path << std::endl;
            encryption_input_file_dialog.ClearSelected();
        }

        if(encryption_output_file_dialog.HasSelected())
        {
            std::strcpy(encryption_output_file_path, encryption_output_file_dialog.GetSelected().string().data());
            std::cout << "encryption_output_file_path: " << encryption_output_file_path << std::endl;
            encryption_output_file_dialog.ClearSelected();
        }

        if(decryption_input_file_dialog.HasSelected())
        {
            std::strcpy(decryption_input_file_path, decryption_input_file_dialog.GetSelected().string().data());
            std::cout << "decryption_input_file_path: " << decryption_input_file_path << std::endl;
            decryption_input_file_dialog.ClearSelected();
        }

        if(decryption_output_file_dialog.HasSelected())
        {
            std::strcpy(decryption_output_file_path, decryption_output_file_dialog.GetSelected().string().data());
            std::cout << "decryption_output_file_path: " << decryption_output_file_path << std::endl;
            decryption_output_file_dialog.ClearSelected();
        }

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
