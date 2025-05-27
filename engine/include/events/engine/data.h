#pragma once

namespace eloo::events::engine {
    // ==================================================================
    // ==================================================================
    //
    // Window events
    //
    // ==================================================================
    // ==================================================================

    struct window_resized_data {
        int width;         // New width of the window
        int height;        // New height of the window
    };

    struct window_moved_data {
        int positionX;     // New X position of the window
        int positionY;     // New Y position of the window
    };

    struct window_focus_changed_data {
        bool focused;      // True if the window is focused, false otherwise
    };


    // ==================================================================
    // ==================================================================
    //
    // Input events
    //
    // ==================================================================
    // ==================================================================

    struct key_pressed_data {
        int key;           // Key code of the pressed key
        int scancode;      // Scancode of the pressed key
        int mods;          // Modifier keys (e.g., Shift, Ctrl)
    };

    struct key_released_data {
        int key;           // Key code of the released key
        int scancode;      // Scancode of the released key
        int mods;          // Modifier keys (e.g., Shift, Ctrl)
    };

    struct mouse_button_pressed_data {
        int button;        // Mouse button code
        float positionX;   // X position of the mouse when the button was pressed
        float positionY;   // Y position of the mouse when the button was pressed
    };

    struct mouse_button_released_data {
        int button;        // Mouse button code
        float positionX;   // X position of the mouse when the button was released
        float positionY;   // Y position of the mouse when the button was released
    };

    struct mouse_moved_data {
        float positionX;   // New X position of the mouse
        float positionY;   // New Y position of the mouse
    };

    struct mouse_scrolled_data {
        float offsetX;     // Scroll offset in the X direction
        float offsetY;     // Scroll offset in the Y direction
    };
}