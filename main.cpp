#include <gtkmm.h>
#include <iostream>
#include <class/Button.hpp>

class MyWindow : public Gtk::Window
{
public:
    MyWindow()
    {
        set_title("Kyrene");
        set_default_size(800, 500);

        m_box.set_orientation(Gtk::Orientation::HORIZONTAL);

        m_sidebar.set_size_request(200, -1);

        std::vector<std::string> items = {
            "Library",
            "Performance",
            "Settings",
        };

        for (const auto &text : items)
        {
            Button btn(text, "/org/kyrene/assets/icons/game-icon.svg", []() {});
            m_sidebar.append(*btn.getWidget());
        }

        m_content.set_label("Content Area");

        m_box.append(m_sidebar);
        m_box.append(m_content);

        set_child(m_box);
    }

private:
    Gtk::Box m_box{Gtk::Orientation::HORIZONTAL};
    Gtk::ListBox m_sidebar;
    Gtk::Label m_content;

    void on_button_clicked(const std::string &label)
    {
        std::cout << label << " clicked\n";
        m_content.set_text(label);
    }
};

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.kyrene.linuz");
    return app->make_window_and_run<MyWindow>(argc, argv);
}
