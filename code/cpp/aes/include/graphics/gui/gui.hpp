//
// Created by Luca on 09/12/2022.
//

#ifndef AES_GUI_H
#define AES_GUI_H

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <type_traits>

#include "glfw/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace aes::gui {

//TODO: const int& error, char[]?
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
/*static_assert(Container<std::vector<unsigned char>>); //TODO: remove
static_assert(Container<std::string>);*/

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
void generic_combo(const CONTAINER& combo_items, const char* selected_item = nullptr) requires std::is_same_v<CONTAINER, std::vector<std::string>> || std::is_same_v<CONTAINER, std::array<std::string, SIZE>> || std::is_same_v<CONTAINER, std::array<char*, SIZE>>
{
    /*char* items; //TODO: uncomment

    if(std::is_same<CONTAINER, std::vector<std::string>>()) {
        items[combo_items.size()];
    } else {
        items = combo_items;
    }*/

    char* items[combo_items.size()];
    static const char* current_item = selected_item;

    for(unsigned short i = 0; i < combo_items.size(); i++) {
        items[i] = combo_items.at(i); //TODO: se fosse vector<std::string> mi serve combo_items.at(i).data();
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

//void generic_combo(const char* items[], char* selected_item);

void aes_types_combo();

void init();

void window();

void show();

} // namespace aes::gui

#endif //AES_GUI_H
