#pragma once
#include <gtkmm.h>
#include <functional>
#include <optional>

class Toggle
{
protected:
    Gtk::Box *m_container;
    Gtk::Label *m_label;
    Gtk::Switch *m_switch;

public:
    Toggle(std::string labelText, bool active = false, std::function<void(bool)> onToggle = nullptr, std::optional<std::string> style = std::nullopt)
    {
        m_container = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
        m_container->set_hexpand(true);
        m_container->set_margin_start(12);
        m_container->set_margin_end(12);
        m_container->set_margin_top(8);
        m_container->set_margin_bottom(8);

        m_label = Gtk::make_managed<Gtk::Label>(labelText);
        m_label->set_halign(Gtk::Align::START);
        m_label->set_xalign(0.0);
        m_label->set_hexpand(true);

        m_switch = Gtk::make_managed<Gtk::Switch>();
        m_switch->set_active(active);
        m_switch->set_hexpand(true);
        m_switch->set_halign(Gtk::Align::END);

        if (onToggle)
        {
            m_switch->property_active().signal_changed().connect([this, onToggle]()
                                                                 { onToggle(m_switch->get_active()); });
        }

        m_container->append(*m_label);
        m_container->append(*m_switch);

        if (style.has_value())
        {
            m_container->add_css_class(style.value());
        }
    }

    Gtk::Box *getWidget() const
    {
        return m_container;
    }

    bool getActive() const
    {
        return m_switch->get_active();
    }

    void setActive(bool active)
    {
        m_switch->set_active(active);
    }
};
