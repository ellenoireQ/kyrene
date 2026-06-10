#include <functional>

class Button
{
protected:
    Gtk::Button *button;
    Gtk::ListBoxRow *row;

public:
    Button(std::string labelText, const char *iconPath, std::function<void()> onClick, std::optional<std::string> style)
    {
        row = Gtk::make_managed<Gtk::ListBoxRow>();
        row->set_activatable(true);
        button = Gtk::make_managed<Gtk::Button>();
        button->set_hexpand(true);
        button->set_halign(Gtk::Align::FILL);
        button->set_has_frame(false);

        if (onClick)
        {
            button->signal_clicked().connect(onClick);
        }

        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
        box->set_hexpand(true);
        box->set_halign(Gtk::Align::FILL);
        box->set_margin_start(4);
        box->set_margin_end(4);

        auto iconWidget = Gtk::make_managed<Gtk::Image>();
        iconWidget->set_from_resource(iconPath);
        iconWidget->set_pixel_size(20);

        auto labelWidget = Gtk::make_managed<Gtk::Label>(labelText);
        labelWidget->set_hexpand(true);
        labelWidget->set_halign(Gtk::Align::START);
        labelWidget->set_xalign(0.0);

        box->append(*iconWidget);
        box->append(*labelWidget);

        button->set_child(*box);
        if (style.has_value())
        {
            row->add_css_class(style.value());
        }
        row->set_child(*button);
    }

    Gtk::ListBoxRow *getWidget() const
    {
        return row;
    }
};