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
        load_css();

        m_box.set_orientation(Gtk::Orientation::HORIZONTAL);

        m_sidebar.set_size_request(200, -1);
        m_sidebar.add_css_class("ky-sidebar");

        for (const auto &si : sidebar_items)
        {
            Button btn(si.label, si.icon, si.onClick, "ky-sidebar-btn");
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

    void load_css()
    {
        auto css_provider = Gtk::CssProvider::create();

        try
        {
            css_provider->load_from_resource("/org/kyrene/assets/style/style.css");
        }
        catch (const Gtk::CssParserError &ex)
        {
            std::cerr << "CSS parsing error: " << ex.what() << std::endl;
            return;
        }

        Gtk::StyleContext::add_provider_for_display(
            Gdk::Display::get_default(),
            css_provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
};

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.kyrene.linuz");
    return app->make_window_and_run<KyreneWindow>(argc, argv);
}
