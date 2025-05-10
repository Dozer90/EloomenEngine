#pragma once

namespace eloo::input {
    enum class keyboard_key {
        unknown = -1,

        modifier_begin,
        command_left = modifier_begin, ctrl_left, shift_left, alt_left, command_right, ctrl_right, shift_right, alt_right,
        modifier_end = alt_right,

        system_begin,
        print_screen = system_begin, pause_break, context_menu, application_key, sleep,
        system_end = sleep,

        lock_begin,
        caps_lock = lock_begin, num_lock, scroll_lock,
        lock_end = scroll_lock,

        control_begin,
        space = control_begin, enter, tab, backspace, escape, insert, del,
        control_end = del,

        alpha_begin,
        a = alpha_begin, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
        alpha_end = z,

        numeric_begin,
        num_0 = numeric_begin, num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9,
        numeric_end = num_9,

        numpad_begin,
        numpad_0 = numpad_begin, numpad_1, numpad_2, numpad_3, numpad_4, numpad_5, numpad_6, numpad_7, numpad_8, numpad_9,
        numpad_end = numpad_9,

        numpad_op_begin,
        numpad_add = numpad_op_begin, numpad_subtract, numpad_multiply, numpad_divide, numpad_decimal,
        numpad_op_end = numpad_decimal,

        navigation_begin,
        arrow_up = navigation_begin, arrow_down, arrow_left, arrow_right, home, end, page_up, page_down,
        navigation_end = page_down,

        punctuation_begin,
        subtract = punctuation_begin, equal, bracket_left, bracket_right, slash, backslash, semicolon, apostrophe, grave, comma, period,
        punctuation_end = period,

        function_begin,
        f1 = function_begin, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
        function_end = f12,

        media_begin,
        media_play = media_begin, media_stop, media_next, media_previous, media_volume_up, media_volume_down, media_mute,
        media_end = media_mute,

        brightness_begin,
        brightness_up = brightness_begin, brightness_down,
        brightness_end = brightness_down,

        oem_begin,
        oem_1 = oem_begin, oem_2, oem_3, oem_4, oem_5, oem_6, oem_7, oem_8, oem_plus, oem_comma, oem_minus, oem_period, oem_102,
        ime_convert, ime_nonconvert, ime_accept, ime_modechange, kana_mode, oem_clear,
        oem_end = oem_clear,

        count
    };

    enum class mouse_button {
        unknown = -1,

        left,
        right,
        middle,
        variable,

        count
    };

    enum class mouse_wheel {
        unknown = -1,

        up,
        down,
        left,
        right,

        count
    };
}