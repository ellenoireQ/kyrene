#include <functional>

class Button
{
protected:
    Gtk::Button *button;
    Gtk::ListBoxRow *row;

public:
    Button(std::string labelText, const char *iconPath, std::function<void()> onClick)
    {
        row = Gtk::make_managed<Gtk::ListBoxRow>();
        button = Gtk::make_managed<Gtk::Button>();

        if (onClick)
        {
            button->signal_clicked().connect(onClick);
        }

        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 8);

        auto iconWidget = Gtk::make_managed<Gtk::Image>();
        iconWidget->set_from_resource(iconPath);

        auto labelWidget = Gtk::make_managed<Gtk::Label>(labelText);

        box->append(*iconWidget);
        box->append(*labelWidget);

        button->set_child(*box);
        row->set_child(*button);
    }

    Gtk::ListBoxRow *getWidget() const
    {
        return row;
    }
};