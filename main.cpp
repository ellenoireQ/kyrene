#include <gtkmm.h>
#include <iostream>
#include <memory>
#include <variant>
#include <class/Button.hpp>
#include <class/Card.hpp>
#include <class/Toggle.hpp>
#include <utils/JsonParser.hpp>
#include <format>
#include <monitor/cpu_monitor.hpp>
#include <future>

struct ButtonData
{
    std::string label;
    const char *icon;
    std::function<void()> onClick;
};

class KyreneWindow : public Gtk::Window
{
public:
    ~KyreneWindow()
    {
        cpumon.unregister_cpu_mon();
    }

    KyreneWindow()
    {
        cfg.set<int>(ConfigKey::LifecycleInterval, 2);

        cpumon.register_cpu_mon();

        set_title("Kyrene");
        set_default_size(1000, 650);
        load_css();

        auto header = Gtk::make_managed<Gtk::HeaderBar>();

        auto menu_btn = Gtk::make_managed<Gtk::Button>();
        auto menu_img = Gtk::make_managed<Gtk::Image>();
        menu_img->set_from_icon_name("open-menu-symbolic");
        menu_btn->set_child(*menu_img);
        menu_btn->signal_clicked().connect([this]()
                                           { m_sidebar.set_visible(!m_sidebar.is_visible()); });

        auto settings_btn = Gtk::make_managed<Gtk::Button>();
        auto settings_img = Gtk::make_managed<Gtk::Image>();
        settings_img->set_from_icon_name("preferences-system-symbolic");
        settings_btn->set_child(*settings_img);
        settings_btn->signal_clicked().connect([this]()
                                               { m_stack.set_visible_child("settings"); });

        auto title_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 10);
        auto title_lbl = Gtk::make_managed<Gtk::Label>("Kyrene");
        title_lbl->set_hexpand(true);
        title_lbl->set_halign(Gtk::Align::START);
        settings_btn->set_halign(Gtk::Align::END);
        menu_btn->set_halign(Gtk::Align::END);

        title_box->append(*title_lbl);
        title_box->append(*settings_btn);
        title_box->append(*menu_btn);

        header->set_title_widget(*title_box);
        header->set_show_title_buttons(true);
        set_titlebar(*header);

        m_box.set_orientation(Gtk::Orientation::HORIZONTAL);

        setup_library_page();
        setup_performance_page();
        setup_settings_page();

        m_stack.add(m_library_page, "library", "Library");
        m_stack.add(m_perf_page, "performance", "Performance");
        m_stack.add(m_settings_page, "settings", "Settings");

        auto cpumon_data = cpumon.get_data();

        if (cpumon_data.empty())
        {
            std::cout << "Data is empty" << std::endl;
        }
        std::cout << "" << cpumon_data[0].cpu_name << std::endl;

        m_scrolled_window.set_child(m_stack);
        m_scrolled_window.set_policy(
            Gtk::PolicyType::NEVER,
            Gtk::PolicyType::AUTOMATIC);

        m_sidebar.set_size_request(220, -1);
        m_sidebar.add_css_class("ky-sidebar");
        m_sidebar.set_selection_mode(Gtk::SelectionMode::SINGLE);

        std::vector<ButtonData> sidebar_items{
            {"Library", "/org/kyrene/assets/icons/game-icon.svg",
             [this]()
             { m_stack.set_visible_child("library"); }},
            {"Performance", "/org/kyrene/assets/icons/perf-icon.svg",
             [this]()
             { m_stack.set_visible_child("performance"); }},
            {"Settings", "/org/kyrene/assets/icons/setting-icon.svg",
             [this]()
             { m_stack.set_visible_child("settings"); }},
        };

        for (const auto &si : sidebar_items)
        {
            Button btn(si.label, si.icon, si.onClick, "ky-sidebar-btn");
            m_sidebar.append(*btn.getWidget());
        }

        m_box.append(m_sidebar);
        m_box.append(m_scrolled_window);
        set_child(m_box);

        m_stack.set_visible_child("library");
    }

private:
    AppConfig cfg;
    CPUMon cpumon{cfg};

    struct CardData
    {
        std::string title;
        std::string description;
        const char *imagePath;
        std::optional<std::string> style;
    };

    struct SettingToggle
    {
        std::string label;
        bool defaultValue;
        std::function<void(bool)> onChange;
    };

    struct SettingButton
    {
        std::string label;
        std::string buttonText;
        std::function<void()> onClick;
    };

    struct SettingInput
    {
        std::string label;
        std::string placeholder;
        std::string defaultValue;
        std::function<void(const std::string &)> onChange;
    };

    using SettingItem = std::variant<SettingToggle, SettingButton, SettingInput>;

    Gtk::Box m_box{Gtk::Orientation::HORIZONTAL};
    Gtk::ListBox m_sidebar;
    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::Stack m_stack;

    Gtk::Box m_library_page{Gtk::Orientation::VERTICAL};
    Gtk::Box m_perf_page{Gtk::Orientation::VERTICAL};
    Gtk::Box m_settings_page{Gtk::Orientation::VERTICAL};

    std::vector<std::unique_ptr<Toggle>> m_toggles;
    std::vector<std::unique_ptr<Toggle>> m_settings_toggles;

    void setup_library_page()
    {
        auto &grid_box = *Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        grid_box.add_css_class("ky-m-content");
        grid_box.set_spacing(16);

        const auto data = load_cards_from_json();
        const int cols = 2;
        Gtk::Box *current_row = nullptr;

        for (size_t i = 0; i < data.size(); ++i)
        {
            if (i % cols == 0)
            {
                current_row = Gtk::make_managed<Gtk::Box>(
                    Gtk::Orientation::HORIZONTAL, 16);
                current_row->set_homogeneous(true);
            }

            Card card(data[i].imagePath, data[i].title,
                      data[i].description,
                      std::optional<std::string>("Open"),
                      data[i].style);

            if (auto btn = card.getActionButton())
                btn->signal_clicked().connect(
                    [title = data[i].title]()
                    { std::cout << "Clicked: " << title << std::endl; });

            current_row->append(*card.getWidget());

            if (i % cols == cols - 1 || i + 1 == data.size())
            {
                grid_box.append(*current_row);
                current_row = nullptr;
            }
        }

        m_library_page.append(grid_box);
    }

    void setup_performance_page()
    {
        m_perf_page.set_spacing(16);
        m_perf_page.add_css_class("ky-m-content");

        auto *header = Gtk::make_managed<Gtk::Label>("Performance Page");
        header->add_css_class("title");
        header->set_halign(Gtk::Align::START);
        m_perf_page.append(*header);

        auto *container = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        container->set_spacing(0);
        container->set_homogeneous(false);
        container->add_css_class("ky-card-toggles-container");

        for (int i = 0; i <= 10; i++)
        {
            auto num = std::format("Enable overlay {}", i);
            m_toggles.push_back(std::make_unique<Toggle>(num, false, [](bool active)
                                                         { std::cout << "Overlay: " << (active ? "ON" : "OFF") << std::endl; }, "ky-card-toggle"));

            container->append(*m_toggles.back()->getWidget());

            if (i < 10)
            {
                auto sep = Gtk::make_managed<Gtk::Separator>(
                    Gtk::Orientation::HORIZONTAL);

                sep->add_css_class("ky-card-toggle-separator");
                sep->set_hexpand(true);
                sep->set_halign(Gtk::Align::FILL);
                container->append(*sep);
            }
        }

        m_perf_page.append(*container);
    }

    void setup_settings_page()
    {
        m_settings_page.set_spacing(16);
        m_settings_page.add_css_class("ky-m-content");

        auto *header = Gtk::make_managed<Gtk::Label>("Settings");
        header->add_css_class("title");
        header->set_halign(Gtk::Align::START);
        m_settings_page.append(*header);

        append_settings_section("General", {SettingToggle{"Launch at startup", false, [](bool v)
                                                          { std::cout << "Startup: " << v << "\n"; }},
                                            SettingToggle{"Minimize to tray on close", true, [](bool v)
                                                          { std::cout << "Tray: " << v << "\n"; }},
                                            SettingToggle{"Check for updates automatically", true, [](bool v)
                                                          { std::cout << "Updates: " << v << "\n"; }},
                                            SettingInput{"Custom Username", "Enter your username", "Player1", [](const std::string &v)
                                                         { std::cout << "Username changed to: " << v << "\n"; }},
                                            SettingButton{"Clear Cache", "Clear Now", []()
                                                          { std::cout << "Cache has been cleared!\n"; }}});
    }

    void append_settings_section(const std::string &title,
                                 const std::vector<SettingItem> &items)
    {
        auto *sec_lbl = Gtk::make_managed<Gtk::Label>(title);
        sec_lbl->add_css_class("ky-settings-section-label");
        sec_lbl->set_halign(Gtk::Align::START);
        m_settings_page.append(*sec_lbl);

        auto *container = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
        container->set_spacing(0);
        container->set_homogeneous(false);
        container->add_css_class("ky-card-toggles-container");

        for (std::size_t i = 0; i < items.size(); ++i)
        {
            const auto &item = items[i];

            if (std::holds_alternative<SettingToggle>(item))
            {
                const auto &toggle_item = std::get<SettingToggle>(item);
                m_settings_toggles.push_back(
                    std::make_unique<Toggle>(toggle_item.label, toggle_item.defaultValue,
                                             toggle_item.onChange, "ky-card-toggle"));
                container->append(*m_settings_toggles.back()->getWidget());
            }
            else if (std::holds_alternative<SettingButton>(item))
            {
                const auto &button_item = std::get<SettingButton>(item);

                auto *row = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 16);
                row->add_css_class("ky-card-toggle");

                row->set_margin_start(16);
                row->set_margin_end(16);
                row->set_margin_top(12);
                row->set_margin_bottom(12);

                auto *label = Gtk::make_managed<Gtk::Label>(button_item.label);
                label->set_hexpand(true);
                label->set_halign(Gtk::Align::START);

                auto *btn = Gtk::make_managed<Gtk::Button>(button_item.buttonText);
                btn->set_halign(Gtk::Align::END);
                btn->set_valign(Gtk::Align::CENTER);
                btn->signal_clicked().connect(button_item.onClick);

                row->append(*label);
                row->append(*btn);
                container->append(*row);
            }
            else if (std::holds_alternative<SettingInput>(item))
            {
                const auto &input_item = std::get<SettingInput>(item);

                auto *row = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 16);
                row->add_css_class("ky-card-toggle");

                row->set_margin_start(16);
                row->set_margin_end(16);
                row->set_margin_top(12);
                row->set_margin_bottom(12);

                auto *label = Gtk::make_managed<Gtk::Label>(input_item.label);
                label->set_hexpand(true);
                label->set_halign(Gtk::Align::START);

                auto *entry = Gtk::make_managed<Gtk::Entry>();
                entry->set_placeholder_text(input_item.placeholder);
                entry->set_text(input_item.defaultValue);
                entry->set_halign(Gtk::Align::END);
                entry->set_valign(Gtk::Align::CENTER);

                entry->signal_changed().connect([entry, onChange = input_item.onChange]()
                                                { onChange(entry->get_text()); });

                row->append(*label);
                row->append(*entry);
                container->append(*row);
            }

            if (i + 1 < items.size())
            {
                auto *sep = Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::HORIZONTAL);
                sep->add_css_class("ky-card-toggle-separator");
                sep->set_hexpand(true);
                sep->set_halign(Gtk::Align::FILL);
                container->append(*sep);
            }
        }

        m_settings_page.append(*container);
    }

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

    std::vector<CardData> load_cards_from_json()
    {
        const std::vector<std::string> candidatePaths{
            "assets/allgame.json",
            "../assets/allgame.json",
        };

        std::vector<std::future<std::optional<kyrene::utils::JsonParser::Json>>> futures;

        for (const auto &candidate : candidatePaths)
        {
            futures.push_back(std::async(std::launch::async, [candidate]()
                                         {
            std::string localError;
            return kyrene::utils::JsonParser::tryParseFile(candidate, &localError); }));
        }

        std::optional<kyrene::utils::JsonParser::Json> finalJson = std::nullopt;

        for (auto &fut : futures)
        {
            auto jsonResult = fut.get();

            if (jsonResult.has_value() && !jsonResult.value().is_null())
            {
                finalJson = std::move(jsonResult.value());
                break;
            }
        }

        if (!finalJson.has_value())
        {
            std::cerr << "Failed to read allgame.json from all candidate paths." << std::endl;
            return {
                {"Fallback Game", "JSON file could not be loaded.", "/org/kyrene/assets/bg/image.png", std::optional<std::string>("ky-card")},
            };
        }

        if (!finalJson->is_object() ||
            !finalJson->contains("applist") ||
            !(*finalJson)["applist"].is_object() ||
            !(*finalJson)["applist"].contains("apps") ||
            !(*finalJson)["applist"]["apps"].is_array())
        {
            std::cerr << "Invalid JSON structure in allgame.json: expected applist.apps array" << std::endl;
            return {
                {"Fallback Game", "JSON structure is invalid.", "/org/kyrene/assets/bg/image.png", std::optional<std::string>("ky-card")},
            };
        }

        std::vector<CardData> cards;
        const auto &apps = (*finalJson)["applist"]["apps"];
        constexpr std::size_t maxInitialCards = 200;

        if (apps.size() > maxInitialCards)
        {
            std::cerr << "allgame.json contains " << apps.size()
                      << " entries; loading only the first " << maxInitialCards
                      << " to avoid high memory usage at startup" << std::endl;
        }

        cards.reserve(std::min(apps.size(), maxInitialCards));

        for (std::size_t i = 0; i < apps.size() && i < maxInitialCards; ++i)
        {
            const auto &app = apps[i];
            const auto appId = app.value("appid", 0);
            const auto name = app.value("name", std::string{});
            const auto title = name.empty() ? ("App " + std::to_string(appId)) : name;

            cards.push_back(CardData{
                .title = title,
                .description = std::to_string(appId),
                .imagePath = "/org/kyrene/assets/bg/image.png",
                .style = "ky-card",
            });
        }

        return cards;
    }
};

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.kyrene.linuz");
    return app->make_window_and_run<KyreneWindow>(argc, argv);
}