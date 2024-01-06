#include "menu.h"

#include "../config.h"
#include "../hacks/hacks.h"
// #include "custom_action.h"
#include "animation.h"
#include "gui.h"

namespace menu
{
    bool menu_open = false;
    // bool opening = true;
    int wait_one_frame = 1;

    void animation_is_done()
    {
        if (globals::animation == 1)
        {
            menu_open = false;
        }
    }

    void set_transition()
    {
        // cocos2d::CCEaseRateAction *ease;
        EaseInAnimation *ease;
        globals::animation_done = false;
        globals::random_direction = rand() % 4 + 1;

        if (menu_open)
        {
            // float timescale = cocos2d::CCDirector::sharedDirector()->getScheduler()->getTimeScale();
            // float duration = config::menu_animation_length * timescale;
            // ease = cocos2d::CCEaseIn::create(CustomAction::create(duration, 1, 0, &globals::animation, nullptr), 0.5f);
            ease = new EaseInAnimation(config::menu_animation_length, 1, 0, &globals::animation, animation_is_done);

            cocos2d::CCEGLView::sharedOpenGLView()->showCursor(true);
            // opening = true;
        }
        else
        {
            // float timescale = cocos2d::CCDirector::sharedDirector()->getScheduler()->getTimeScale();
            // float duration = config::menu_animation_length * timescale;
            // ease = cocos2d::CCEaseOut::create(CustomAction::create(duration, 0, 1, &globals::animation, animation_is_done), 0.5f);
            ease = new EaseInAnimation(config::menu_animation_length, 0, 1, &globals::animation, animation_is_done);

            menu_open = true;
            // opening = false;
        }

        // if (ease)
        // {
        //     auto scene = cocos2d::CCDirector::sharedDirector()->getRunningScene();
        //     scene->stopActionByTag(4000);
        //     ease->setTag(4000);
        //     scene->runAction(ease);
        //     globals::animation_action = ease;
        // }

        if (ease)
        {
            if (globals::animation_action)
                delete globals::animation_action;

            globals::animation_action = ease;
        }
    }

    void toggle()
    {
        menu_open = !menu_open;
        if (!menu_open)
            config::save();
        set_transition();
    }

    void set_styles()
    {
        auto &style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(4, 4);
        style.WindowRounding = config::window_rounding;
        style.FramePadding = ImVec2(4, 2);
        style.FrameRounding = 0;
        style.ItemSpacing = ImVec2(12, 2);
        style.ItemInnerSpacing = ImVec2(8, 6);
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.IndentSpacing = 25.0f;
        style.ScrollbarSize = 15.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 5.0f;
        style.GrabRounding = 3.0f;
        style.WindowBorderSize = config::border_size;
        style.WindowMinSize = ImVec2(32, 32);
        style.ScaleAllSizes(globals::screen_size.x * config::menu_size);

        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(
            (float)ImGui::GetTime() * config::menu_rainbow_speed,
            config::menu_rainbow_brightness,
            config::menu_rainbow_brightness,
            r, g, b);

        auto rainbow_color = ImVec4(r, g, b, 1);
        globals::current_color = config::menu_color_rainbow && config::text_color_rainbow
                                     ? rainbow_color
                                     : config::text_color.to_imvec4();

        auto colors = style.Colors;
        colors[ImGuiCol_WindowBg] = config::bg_color.to_imvec4();
        colors[ImGuiCol_FrameBg] = config::frame_color.to_imvec4();
        colors[ImGuiCol_TitleBg] =
            colors[ImGuiCol_TitleBgActive] =
                colors[ImGuiCol_TitleBgCollapsed] =
                    config::menu_color_rainbow ? rainbow_color : config::menu_color.to_imvec4();
    }

    void reset_windows()
    {
        globals::reset_windows = true;

        // const int game_width = (int)cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width;
        const int game_width = (int)ImGui::GetIO().DisplaySize.x;

        // pack windows left to right
        const float step = (float)config::window_snap;
        float x = step;
        float win_w = WINDOW_WIDTH * globals::screen_size.x * config::menu_size;
        std::vector<std::vector<float>> rows;
        std::vector<float> row;
        int column_index = 0;

        // check if we have window positions
        if (globals::window_positions.is_null())
        {
            globals::reset_windows = false;
            return;
        }

        // iterate over all keys in globals::window_positions
        for (auto &window : globals::window_positions.items())
        {
            // get window name
            std::string name = window.key();

            if (column_index != 0)
                x += win_w + step;

            // check if we have space to the right
            if (x + win_w > game_width - step)
            {
                // we don't have space, so move to the next row
                rows.push_back(row);
                row.clear();
                x = step;
                column_index = 0;
            }

            // calculate y
            float y = step;
            if (rows.size() > 0)
            {
                // we have previous rows, so take y from the last row
                y = rows[rows.size() - 1][column_index] + step;
            }

            // add window position to the row
            row.push_back(y + globals::window_positions[name]["h"].get<float>());

            // set window position
            globals::window_positions[name]["x"] = x;
            globals::window_positions[name]["y"] = y;

            column_index++;
        }
    }

    void draw()
    {
        // float s = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width / 1920.0f;
        // if (s != globals::screen_size.x)
        // {
        //     globals::screen_size.x = s;
        //     globals::reset_windows = true;
        // }
        // s = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().height / 1080.0f;
        // if (s != globals::screen_size.y)
        // {
        //     globals::screen_size.y = s;
        //     globals::reset_windows = true;
        // }
        float s = ImGui::GetIO().DisplaySize.x / 1920.0f;
        if (s != globals::screen_size.x)
        {
            globals::screen_size.x = s;
            globals::reset_windows = true;
        }
        s = ImGui::GetIO().DisplaySize.y / 1080.0f;
        if (s != globals::screen_size.y)
        {
            globals::screen_size.y = s;
            globals::reset_windows = true;
        }

        if (globals::animation_action && !globals::animation_action->is_done())
        {
            float dt = cocos2d::CCDirector::sharedDirector()->getDeltaTime();
            globals::animation_action->update(dt);
        }

        hacks::update();

        if (!menu_open)
            return;

        // Set styles
        set_styles();

        gui::Begin(PROJECT_NAME);

        gui::ImText("Version: " PROJECT_VERSION);
        gui::ImText("Build date: " __DATE__ " " __TIME__);
        if (gui::ImButton("Open GitHub page"))
            cocos2d::CCApplication::sharedApplication()->openURL(PROJECT_HOMEPAGE_URL);

        gui::End();

        hacks::draw();

        gui::Begin("Interface");

        gui::PushWidth(70);
        gui::ImInputFloat("UI Scale", &config::menu_size);
        if (config::menu_size < 0.5)
            config::menu_size = 0.5;
        else if (config::menu_size > 3)
            config::menu_size = 1;
        gui::ImInputFloat("Border Size", &config::border_size);
        gui::ImInputFloat("Window Rounding", &config::window_rounding);
        gui::ImInputInt("Window Snap", &config::window_snap, 0);
        gui::PopWidth();

        gui::ImPopupSetting("Colors", []()
                            {
            gui::ImColorEdit4("Text Color", (float *)&config::text_color);
            gui::ImColorEdit4("Background Color", (float *)&config::bg_color);
            gui::ImColorEdit4("Frame Color", (float *)&config::frame_color);
            gui::ImColorEdit4("Menu Color", (float *)&config::menu_color);
            gui::ImColorEdit4("Disabled Color", (float *)&config::disabled_color); });

        gui::ImToggleButton("Rainbow", &config::menu_color_rainbow, []()
                            {
            gui::ImInputFloat("Rainbow Speed", &config::menu_rainbow_speed);
            gui::ImInputFloat("Rainbow Brightness", &config::menu_rainbow_brightness);
            gui::ImCheckbox("Rainbow Text", &config::text_color_rainbow); });

        gui::PushWidth(100);
        gui::ImInputFloat("Open Speed", &config::menu_animation_length);
        gui::PopWidth();

        if (globals::reset_windows)
            globals::reset_windows = false;

        if (gui::ImButton("Stack Windows"))
            reset_windows();

        gui::End();

        // Animation stuff
        // if (!globals::animation_done && (opening && globals::animation == 0 || !opening && globals::animation == 1))
        // {
        //     globals::animation_done = true;
        //     globals::animation_action = nullptr;
        // }
        if (!globals::animation_done && globals::animation_action && globals::animation_action->is_done())
        {
            globals::animation_done = true;
            delete globals::animation_action;
            globals::animation_action = nullptr;
        }

        // set window positions on the first run, but only after the first frame
        // to allow ImGui to calculate window heights
        if (config::reposition_windows && wait_one_frame-- == 0)
        {
            reset_windows();
            config::reposition_windows = false;
        }
    }

    void init()
    {
        srand((uint32_t)time(NULL));

        // Initialize ImGui
        ImGuiIO &io = ImGui::GetIO();
        io.IniFilename = NULL; // we are saving window positions manually

        // Load font
        ImFontConfig font_cfg;
        font_cfg.OversampleH = 3;
        font_cfg.OversampleV = 3;
        globals::main_font = io.Fonts->AddFontFromFileTTF("openhack\\Inter.ttf", 16.0f, &font_cfg);
        globals::title_font = io.Fonts->AddFontFromFileTTF("openhack\\Inter.ttf", 18.0f, &font_cfg);
        io.Fonts->Build();
        ImGui_ImplOpenGL3_CreateFontsTexture();
        io.FontDefault = globals::main_font;

        // globals::screen_size.x = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().width / 1920.0f;
        // globals::screen_size.y = cocos2d::CCDirector::sharedDirector()->getOpenGLView()->getFrameSize().height / 1080.0f;
        globals::screen_size.x = io.DisplaySize.x / 1920.0f;
        globals::screen_size.y = io.DisplaySize.y / 1080.0f;

        set_styles();
    }
}