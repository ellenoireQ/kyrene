#include <optional>
#include <string>

class Card
{
protected:
    Gtk::Frame *frame;
    Gtk::Box *m_root;
    Gtk::Box *m_header;
    Gtk::Picture *m_image;
    Gtk::Label *m_title;
    Gtk::Label *m_desc;
    Gtk::Box *m_content;
    Gtk::Button *m_action_button;

public:
    Card(const char *imagePath, std::string titleText, std::optional<std::string> bodyText = std::nullopt, std::optional<std::string> buttonLabel = std::nullopt, std::optional<std::string> style = std::nullopt)
    {
        frame = Gtk::make_managed<Gtk::Frame>();

        auto m_outer = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 0);

        m_root = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 8);
        m_header = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 6);
        m_content = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 6);
        m_image = nullptr;
        m_title = nullptr;
        m_desc = nullptr;
        m_action_button = nullptr;

        frame->set_child(*m_outer);
        frame->add_css_class("ky-card");
        frame->set_halign(Gtk::Align::FILL);
        frame->set_hexpand(true);
        frame->set_margin_start(8);
        frame->set_margin_end(8);
        frame->set_margin_top(8);
        frame->set_margin_bottom(8);
        m_header->add_css_class("ky-card-header");
        m_content->add_css_class("ky-card-content");

        if (imagePath != nullptr)
        {
            m_image = Gtk::make_managed<Gtk::Picture>();
            m_image->set_resource(imagePath);
            m_image->set_hexpand(true);
            m_image->set_halign(Gtk::Align::FILL);
            m_image->set_content_fit(Gtk::ContentFit::COVER);
            m_image->set_can_shrink(true);
            m_image->set_size_request(-1, 180);
            m_image->add_css_class("ky-card-image");

            m_outer->append(*m_image);
        }

        m_root->add_css_class("ky-card-body");
        m_outer->append(*m_root);

        m_title = Gtk::make_managed<Gtk::Label>(titleText);
        m_title->set_halign(Gtk::Align::START);
        m_title->add_css_class("ky-card-title");
        m_root->append(*m_title);

        if (bodyText.has_value())
        {
            m_desc = Gtk::make_managed<Gtk::Label>(bodyText.value());
            m_desc->set_wrap(true);
            m_desc->set_halign(Gtk::Align::START);
            m_desc->add_css_class("ky-card-desc");
            m_root->append(*m_desc);
        }

        m_root->append(*m_content);
        m_root->set_hexpand(true);

        if (buttonLabel.has_value())
        {
            m_action_button = Gtk::make_managed<Gtk::Button>(buttonLabel.value());
            m_action_button->add_css_class("ky-card-action");
            auto btn_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
            btn_box->set_halign(Gtk::Align::END);
            btn_box->append(*m_action_button);
            m_root->append(*btn_box);
        }

        if (style.has_value())
        {
            frame->add_css_class(style.value());
        }
    }

    void addContent(Gtk::Widget &widget)
    {
        m_content->append(widget);
    }

    Gtk::Box *getContentWidget() const
    {
        return m_content;
    }

    Gtk::Button *getActionButton() const
    {
        return m_action_button;
    }

    Gtk::Frame *getWidget() const
    {
        return frame;
    }
};
