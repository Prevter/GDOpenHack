#pragma once
#include "../pch.h"

namespace hacks
{
    void init();
    void draw();
    void update();

    // methods for saving and loading settings
    void save(nlohmann::json *data);
    void load(nlohmann::json *data);

    class Hack
    {
    public:
        // called during hooks::init()
        virtual void init() = 0;
        // called during menu rendering
        // (embedded = true if the hack is being rendered from "EmbeddedHackComponent")
        virtual void draw(bool embedded = false) = 0;
        // called every frame
        virtual void update() = 0;

        // methods for saving and loading settings for this hack
        virtual void load(nlohmann::json *data) = 0;
        virtual void save(nlohmann::json *data) = 0;

        // used to identify hacks
        virtual std::string get_id() = 0;
    };

    extern std::vector<Hack *> hacks;

    class Component
    {
    public:
        // called inside the window's draw method
        virtual void draw() = 0;

        // methods for saving and loading settings for this component
        virtual void load(nlohmann::json *data) = 0;
        virtual void save(nlohmann::json *data) = 0;

    private:
        std::string m_type;
    };

    struct opcode_t
    {
        void *address;
        std::string library;
        std::vector<uint8_t> on_bytes;
        std::vector<uint8_t> off_bytes;
    };

    // Controls a single toggleable hack
    class ToggleComponent : public Component
    {
    public:
        ToggleComponent(std::string &title, std::string id, std::function<void()> callback, std::vector<opcode_t> opcodes);
        virtual void draw() override;

        // save and load toggle setting
        virtual void load(nlohmann::json *data) override;
        virtual void save(nlohmann::json *data) override;

        bool apply_patch();
        bool is_enabled();
        std::string get_id();
        std::string get_title();

        void set_warnings(bool has_warnings) { m_has_warnings = has_warnings; }

    private:
        bool m_enabled;
        std::string m_title;
        std::string m_id;
        std::function<void()> m_callback;
        std::vector<opcode_t> m_opcodes;
        bool m_has_warnings = false;
    };

    // A component that only displays text
    class TextComponent : public Component
    {
    public:
        TextComponent(std::string text);
        virtual void draw() override;

        // text components don't have any settings to save or load
        virtual void load(nlohmann::json *data) override {}
        virtual void save(nlohmann::json *data) override {}

    private:
        std::string m_text;
    };

    class EmbeddedHackComponent : public Component
    {
    public:
        EmbeddedHackComponent(Hack *hack);
        virtual void draw() override;

        // saving is handled by the hack itself
        virtual void load(nlohmann::json *data) override {}
        virtual void save(nlohmann::json *data) override {}

    private:
        Hack *m_hack;
    };

    class Window
    {
    public:
        Window(std::string title);
        void draw();
        void add_component(Component *component);
        std::string get_title();

        // methods for saving and loading settings for this window
        void load(nlohmann::json *data);
        void save(nlohmann::json *data);

    private:
        std::string m_title;
        std::vector<Component *> m_components;

        friend void init();
    };

    extern std::vector<Window> windows;
}