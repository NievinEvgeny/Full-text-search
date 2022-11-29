#include <fts/replxx-wrapper.hpp>
#include <replxx.hxx>
#include <string>

namespace replxx::wrapper {

std::string getline()
{
    std::string prompt = "\x1b[1;32mquery\x1b[0m> ";
    std::string input;

    replxx::Replxx rx;

    rx.install_window_change_handler();

    rx.set_word_break_characters(" \n\t.,-%!;:=*~^'\"/?<>|[](){}");
    rx.set_no_color(false);
    rx.bind_key_internal(replxx::Replxx::KEY::BACKSPACE, "delete_character_left_of_cursor");
    rx.bind_key_internal(replxx::Replxx::KEY::DELETE, "delete_character_under_cursor");
    rx.bind_key_internal(replxx::Replxx::KEY::LEFT, "move_cursor_left");
    rx.bind_key_internal(replxx::Replxx::KEY::RIGHT, "move_cursor_right");
    rx.bind_key_internal(replxx::Replxx::KEY::HOME, "move_cursor_to_begining_of_line");
    rx.bind_key_internal(replxx::Replxx::KEY::END, "move_cursor_to_end_of_line");
    rx.bind_key_internal(replxx::Replxx::KEY::control(replxx::Replxx::KEY::LEFT), "move_cursor_one_word_left");
    rx.bind_key_internal(replxx::Replxx::KEY::control(replxx::Replxx::KEY::RIGHT), "move_cursor_one_word_right");
    rx.bind_key_internal(replxx::Replxx::KEY::control(replxx::Replxx::KEY::ENTER), "commit_line");
    rx.bind_key_internal(replxx::Replxx::KEY::meta(replxx::Replxx::KEY::BACKSPACE), "kill_to_whitespace_on_left");
    rx.bind_key_internal(replxx::Replxx::KEY::INSERT, "toggle_overwrite_mode");

    for (;;)
    {
        char const* cinput{nullptr};

        do
        {
            cinput = rx.input(prompt);
        } while ((cinput == nullptr) && (errno == EAGAIN));

        if (cinput == nullptr)
        {
            break;
        }

        input = cinput;

        if (!input.empty())
        {
            break;
        }
    }

    return input;
}

}  // namespace replxx::wrapper