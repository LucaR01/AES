//
// Created by Luca on 09/12/2022.
//

#ifndef AES_GUI_H
#define AES_GUI_H

#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <type_traits>
#include <optional>

#include "glfw/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "core/operations/operations.hpp"
#include "core/aes.hpp"
#include "defaults/defaults.hpp"

namespace aes::gui {

static constexpr unsigned short WINDOW_WIDTH = 1280;
static constexpr unsigned short WINDOW_HEIGHT = 720;
static constexpr std::string_view WINDOW_TITLE = "AES";

static bool set_files_path = true;

/**
 * @brief This function prints the glfw error.
 * @param error: the error code.
 * @param description: the description of the error.
 */
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Questo concept permette di verificare che il parametro passato sia un container.
//TODO: da mettere tipo in un folder: generic_templates o templates o generics.
template <class ContainerType>
concept Container = requires(ContainerType a, const ContainerType b)
{
    requires std::regular<ContainerType>;
    requires std::swappable<ContainerType>;
    requires std::destructible<typename ContainerType::value_type>;
    requires std::same_as<typename ContainerType::reference, typename ContainerType::value_type &>;
    requires std::same_as<typename ContainerType::const_reference, const typename ContainerType::value_type &>;
    requires std::forward_iterator<typename ContainerType::iterator>;
    requires std::forward_iterator<typename ContainerType::const_iterator>;
    requires std::signed_integral<typename ContainerType::difference_type>;
    requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::iterator>::difference_type>;
    requires std::same_as<typename ContainerType::difference_type, typename std::iterator_traits<typename ContainerType::const_iterator>::difference_type>;
    { a.begin() } -> std::same_as<typename ContainerType::iterator>;
    { a.end() } -> std::same_as<typename ContainerType::iterator>;
    { b.begin() } -> std::same_as<typename ContainerType::const_iterator>;
    { b.end() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.cbegin() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.cend() } -> std::same_as<typename ContainerType::const_iterator>;
    { a.size() } -> std::same_as<typename ContainerType::size_type>;
    { a.max_size() } -> std::same_as<typename ContainerType::size_type>;
    { a.empty() } -> std::same_as<bool>;
};

//TODO: static constexpr?
//TODO: farlo con un Allocator opzionale?

/**
 * @version: AES v1.0.0
 * @brief: This function permits to create an ImGui::Combo by passing it a container with strings (std::string, char* [], etc.) and the current selected item of the combo.
 * Usage: aes::gui::generic_combo<std::vector, std::string>(std::vector<std::string>{ "Hello", "World!" }, "Hello");
 *
 * @tparam Container
 * @tparam Types
 * @param combo_items
 * @param selected_item
 */
template<template<typename... Args> class Container, typename... Types>
void generic_combo(const Container<Types...>& combo_items, const char* selected_item = nullptr)
{
    char* items[combo_items.size()];
    static const char* current_item = selected_item;

    for(unsigned short i = 0; i < combo_items.size(); i++) {
        items[i] = combo_items.at(i).data();
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
}

//TODO: farlo con un Allocator opzionale?
//TODO: usare il concept Container qui sopra definito.
/**
 * @version: AES v1.0.0
 * @brief: This function permits to create an ImGui::Combo by passing it a container with strings (std::string, char* [], etc). and the current selected item of the combo.
 * Usage: aes::gui::generic_combo<std::array<std::string, 3>>(std::array<std::string, 3>{"Hello", "World!"}, "Hello"};
 *
 * @tparam CONTAINER
 * @param combo_items
 * @param selected_item
 */
template<typename CONTAINER, std::size_t SIZE = 0> // prima , std::size_t SIZE
void generic_combo(const CONTAINER& combo_items, const char* selected_item = nullptr, const std::string_view& combo_label = "") requires std::is_same_v<CONTAINER, std::vector<std::string>> || std::is_same_v<CONTAINER, std::array<std::string, SIZE>> || std::is_same_v<CONTAINER, std::array<char*, SIZE>>
{
    /*char* items; //TODO: uncomment

    if(std::is_same<CONTAINER, std::vector<std::string>>()) { //TODO: remove?
        items[combo_items.size()];
    } else {
        items = combo_items;
    }*/

    char* items[combo_items.size()];
    static const char* current_item = selected_item;

    for(unsigned short i = 0; i < combo_items.size(); i++) {
        items[i] = combo_items.at(i); //TODO: se fosse vector<std::string> mi serve combo_items.at(i).data();
    }

    if (ImGui::BeginCombo(combo_label.data(), current_item)) {
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
}

/**
 * @version: AES v1.0.0
 * @brief: This function permits to create an ImGui::Combo by passing it a container with strings (std::string, char* [], etc). and the current selected item of the combo.
 * Usage: aes::gui::generic_combo<std::array<std::string, 3>>(std::array<std::string, 3>{"Hello", "World!"}, "Hello"};
 *
 * @tparam CONTAINER: the container type which has to be a std::vector<std::string> or a std::array<std::string, SIZE>
 * @param combo_items: the parameters of the combo.
 * @param selected_item: the item of the combo that is selected.
 * @param combo_label: It's the name of the combo.
 * @param SIZE: it's a std::size_t aka unsigned long long int.
 */
template<typename CONTAINER, std::size_t SIZE = 0>
void generic_combo(const CONTAINER& combo_items, std::string& selected_item, const std::string_view& combo_label) requires std::is_same_v<CONTAINER, std::vector<std::string>> || std::is_same_v<CONTAINER, std::array<std::string, SIZE>>
{
    /*char* items; //TODO: uncomment

    if(std::is_same<CONTAINER, std::vector<std::string>>()) { //TODO: remove
        items[combo_items.size()];
    } else {
        items = combo_items;
    }*/

    //char* items[combo_items.size()];
    //std::vector<std::string> items{}; //TODO: remove
    //static const char* current_item = selected_item.c_str();

    /*for(unsigned short i = 0; i < combo_items.size(); i++) { //TODO: remove
        items[i] = combo_items.at(i); //TODO: se fosse vector<std::string> mi serve combo_items.at(i).data();
    }*/

    // Il ## doppio cancelletto dice a ImGui di non mostrare il label.
    if (ImGui::BeginCombo(combo_label.data(), selected_item.c_str())) {
        for (const auto& item : combo_items) {
            bool is_selected = (selected_item == item);
            if (ImGui::Selectable(item.c_str(), is_selected)) {
                //current_item = item;
                //selected_item.assign(current_item, current_item + 3); //TODO: remove
                selected_item = item;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}
/**
 *
 * @param operation : ENCRYPT or DECRYPT.
 * @param encryption_object : whether you want to run your operation in a MESSAGE or a FILE.
 * @param input : The input message.
 * @param key : The key that we want to use for the encryption/decryption.
 * @param aes_type : The type of @enum AES: 128, 192 or 256.
 * @param mode : @enum Modes indicates which mode we want to use for encryption/decryption.
 * @param padding : @enum Paddings indicates which padding we want to use.
 * @param iv : the initialization vector.
 * @param input_file_path : the file from which we want to retrieve the plaintext or encrypted text.
 * @param output_file_path : the file where we want to store the encrypted text or deciphered plaintext.
 * @return the encrypted/decrypted text or an empty string if we encrypt/decrypt a file.
 */
char* retrieve_output(const aes::ops::Operations& operation, const aes::ops::EncryptionOperations& encryption_object, const std::string& input, const std::string& key, const std::optional<std::vector<uint8_t>>& iv, const std::string& aes_type, const std::string& mode, const std::string& padding,
                      const std::string& input_file_path = "", const std::string& output_file_path = "");

/**
 * @brief This function inits the window.
 */
void init();

/**
 * @brief This function shows the window with the parameters.
 * @param message : the message that we want to encrypt/decrypt.
 * @param key : The key that we want to use for the encryption/decryption.
 * @param aes : The type of @enum AES: 128, 192 or 256.
 * @param mode : @enum Modes indicates which mode we want to use for encryption/decryption.
 * @param padding : @enum Paddings indicates which padding we want to use.
 * @param iv : the initialization vector.
 * @param input_file_path : the file from which we want to retrieve the plaintext or encrypted text.
 * @param output_file_path : the file where we want to store the encrypted text or deciphered plaintext.
 */
void window(const aes::AES& aes = aes::def::DEFAULT_AES, const aes::mod::Modes& mode = aes::def::DEFAULT_MODE, const aes::pad::Paddings& padding = aes::def::DEFAULT_PADDING, const std::string& message = std::string(def::EMPTY_STRING), const std::string& key = std::string(def::EMPTY_STRING), const std::string& iv = std::string(def::EMPTY_STRING), const std::string& input_file_path = std::string(def::EMPTY_STRING), const std::string& output_file_path = std::string(aes::def::DEFAULT_OUTPUT_FILE_PATH));

/**
 * @brief This function setup the window with the @fn init() and calls @fn window();
 * @param message : the message that we want to encrypt/decrypt.
 * @param key : The key that we want to use for the encryption/decryption.
 * @param aes : The type of @enum AES: 128, 192 or 256.
 * @param mode : @enum Modes indicates which mode we want to use for encryption/decryption.
 * @param padding : @enum Paddings indicates which padding we want to use.
 * @param iv : the initialization vector.
 * @param input_file_path : the file from which we want to retrieve the plaintext or encrypted text.
 * @param output_file_path : the file where we want to store the encrypted text or deciphered plaintext.
 */
void show(const aes::AES& aes = aes::def::DEFAULT_AES, const aes::mod::Modes& mode = aes::def::DEFAULT_MODE, const aes::pad::Paddings& padding = aes::def::DEFAULT_PADDING, const std::string& message = std::string(def::EMPTY_STRING), const std::string& key = std::string(def::EMPTY_STRING), const std::string& iv = std::string(def::EMPTY_STRING), const std::string& input_file_path = std::string(def::EMPTY_STRING), const std::string& output_file_path = std::string(aes::def::DEFAULT_OUTPUT_FILE_PATH));

} // namespace aes::gui

#endif //AES_GUI_H
