#include <gtkmm.h>
#include <iostream>
#include <class/Button.hpp>

struct ButtonData
{
    std::string label;
    const char *icon;
    std::function<void()> onClick;
};

class KyreneWindow : public Gtk::Window
{
public:
    KyreneWindow()
    {
        set_title("Kyrene");
        set_default_size(800, 500);

        m_box.set_orientation(Gtk::Orientation::HORIZONTAL);

        m_sidebar.set_size_request(200, -1);

        for (const auto &si : sidebar_items)
        {
            Button btn(si.label, si.icon, si.onClick);
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

    /*
     *   Sidebar item
     */
    std::vector<ButtonData> sidebar_items{
        ButtonData{
            .label = "Library",
            .icon = "/org/kyrene/assets/icons/game-icon.svg",
            .onClick = [this]()
            {
                //
            },
        },
        ButtonData{
            .label = "Performance",
            .icon = "/org/kyrene/assets/icons/perf-icon.svg",
            .onClick = [this]()
            {
                //
            },
        },
        ButtonData{
            .label = "Settings",
            .icon = "/org/kyrene/assets/icons/setting-icon.svg",
            .onClick = [this]()
            {
                //
            },
        },
    };
};

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.kyrene.linuz");
    return app->make_window_and_run<KyreneWindow>(argc, argv);
}
