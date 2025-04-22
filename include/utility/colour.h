#pragma once

#include <Maths/CoreMath.h>

namespace eloo {
    using colour_t = uint32_t;

    namespace Colour {
#if !defined IS_BIG_ENDIAN
        static constexpr uint32_t R_COMPONENT_MASK = 0xFF000000;
        static constexpr uint32_t G_COMPONENT_MASK = 0x00FF0000;
        static constexpr uint32_t B_COMPONENT_MASK = 0x0000FF00;
        static constexpr uint32_t A_COMPONENT_MASK = 0x000000FF;
#else
        static constexpr uint32_t R_COMPONENT_MASK = 0x000000FF;
        static constexpr uint32_t G_COMPONENT_MASK = 0x0000FF00;
        static constexpr uint32_t B_COMPONENT_MASK = 0x00FF0000;
        static constexpr uint32_t A_COMPONENT_MASK = 0xFF000000;
#endif

        FORCE_INLINE constexpr colour_t set_r(colour_t colour, uint8_t r) {
            return (colour & ~R_COMPONENT_MASK) | (r << R_COMPONENT_MASK);
        }
        FORCE_INLINE constexpr colour_t set_g(colour_t colour, uint8_t g) {
            return (colour & ~G_COMPONENT_MASK) | (g << G_COMPONENT_MASK);
        }
        FORCE_INLINE constexpr colour_t set_b(colour_t colour, uint8_t b) {
            return (colour & ~B_COMPONENT_MASK) | (b << B_COMPONENT_MASK);
        }
        FORCE_INLINE constexpr colour_t set_a(colour_t colour, uint8_t a) {
            return (colour & ~A_COMPONENT_MASK) | (a << A_COMPONENT_MASK);
        }

        FORCE_INLINE constexpr colour_t set_r(colour_t colour, float r) {
            return set_r(colour, static_cast<uint8_t>(r * 0xFF));
        }
        FORCE_INLINE constexpr colour_t set_g(colour_t colour, float g) {
            return set_g(colour, static_cast<uint8_t>(g * 0xFF));
        }
        FORCE_INLINE constexpr colour_t set_b(colour_t colour, float b) {
            return set_b(colour, static_cast<uint8_t>(b * 0xFF));
        }
        FORCE_INLINE constexpr colour_t set_a(colour_t colour, float a) {
            return set_a(colour, static_cast<uint8_t>(a * 0xFF));
        }

        FORCE_INLINE constexpr uint8_t get_r(colour_t colour) {
            return (colour & R_COMPONENT_MASK) >> R_COMPONENT_MASK;
        }
        FORCE_INLINE constexpr uint8_t get_g(colour_t colour) {
            return (colour & G_COMPONENT_MASK) >> G_COMPONENT_MASK;
        }
        FORCE_INLINE constexpr uint8_t get_b(colour_t colour) {
            return (colour & B_COMPONENT_MASK) >> B_COMPONENT_MASK;
        }
        FORCE_INLINE constexpr uint8_t get_a(colour_t colour) {
            return (colour & A_COMPONENT_MASK) >> A_COMPONENT_MASK;
        }

        FORCE_INLINE constexpr colour_t make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
            return
                (r << R_COMPONENT_MASK) |
                (g << G_COMPONENT_MASK) |
                (b << B_COMPONENT_MASK) |
                (a << A_COMPONENT_MASK);
        }

        FORCE_INLINE constexpr colour_t make_rgba(float r, float g, float b, float a) {
            return make_rgba(
                static_cast<uint8_t>(r * 0xFF),
                static_cast<uint8_t>(g * 0xFF),
                static_cast<uint8_t>(b * 0xFF),
                static_cast<uint8_t>(a * 0xFF));
        }

        FORCE_INLINE constexpr colour_t make_rgb(uint8_t r, uint8_t g, uint8_t b) {
            return make_rgba(r, g, b, 0xFF);
        }

        FORCE_INLINE constexpr colour_t make_rgb(float r, float g, float b) {
            return make_rgba(r, g, b, 1.0f);
        }

        FORCE_INLINE constexpr colour_t make_hsla(float h, float s, float l, float a) {
            float r, g, b;
            if (s == 0.0f) {
                r = g = b = l; // Achromatic
            } else {
                auto constexpr hue_to_rgb = [](float p, float q, float t) {
                    if (t < 0.0f) t += 1.0f;
                    if (t > 1.0f) t -= 1.0f;
                    if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
                    if (t < 1.0f / 2.0f) return q;
                    if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
                    return p;
                };

                float q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - l * s);
                float p = 2.0f * l - q;
                r = hue_to_rgb(p, q, h + 1.0f / 3.0f);
                g = hue_to_rgb(p, q, h);
                b = hue_to_rgb(p, q, h - 1.0f / 3.0f);
            }
            return make_rgba(r, g, b, a);
        }

        FORCE_INLINE constexpr colour_t make_hsl(float h, float s, float l) {
            return make_hsla(h, s, l, 1.0f);
        }

        FORCE_INLINE constexpr colour_t make_hsva(float h, float s, float v, float a) {
            float r, g, b;
            int i = static_cast<int>(h * 6.0f);
            const float f = h * 6.0f - i;
            const float p = v * (1.0f - s);
            const float q = v * (1.0f - f * s);
            const float t = v * (1.0f - (1.0f - f) * s);
            i %= 6;
            switch (i) {
                case 0: r = v; g = t; b = p; break;
                case 1: r = q; g = v; b = p; break;
                case 2: r = p; g = v; b = t; break;
                case 3: r = p; g = q; b = v; break;
                case 4: r = t; g = p; b = v; break;
                case 5: r = v; g = p; b = q; break;
            }
            return make_rgba(r, g, b, a);
        }

        FORCE_INLINE constexpr colour_t make_hsv(float h, float s, float v) {
            return make_hsva(h, s, v, 1.0f);
        }

        FORCE_INLINE constexpr colour_t make_kelvin(uint32_t kelvin) {
            float r, g, b;
            constexpr float scalar = 1.0f / 255.0f;
            if (kelvin <= 6600) {
                r = 1.0f;
                g = (99.470f * math::log(kelvin) - 161.12f) * scalar;
                b = kelvin <= 1900 ? 0.0f : (138.52 * math::log(kelvin - 100) - 305.04f) * scalar;
            } else {
                r = 329.698f * pow(kelvin / 100.0f - 60.0f, -0.1332f) * scalar;
                g = 288.122f * pow(kelvin / 100.0f - 60.0f, -0.0755f) * scalar;
                b = 1.0f;
            }

            return make_rgb(r, g, b);
        }

        ////////////////////////////////////////////////////////////////////////////

        // Named colours
        static constexpr colour_t C_ALICEBLUE            = make_rgb(0xF0, 0xF8, 0xFF);
        static constexpr colour_t C_ANTIQUEWHITE         = make_rgb(0xFA, 0xEB, 0xD7);
        static constexpr colour_t C_AQUA                 = make_rgb(0x00, 0xFF, 0xFF);
        static constexpr colour_t C_AQUAMARINE           = make_rgb(0x7F, 0xFF, 0xD4);
        static constexpr colour_t C_AZURE                = make_rgb(0xF0, 0xFF, 0xFF);
        static constexpr colour_t C_BEIGE                = make_rgb(0xF5, 0xF5, 0xDC);
        static constexpr colour_t C_BISQUE               = make_rgb(0xFF, 0xE4, 0xC4);
        static constexpr colour_t C_BLACK                = make_rgb(0x00, 0x00, 0x00);
        static constexpr colour_t C_BLANCHEDALMOND       = make_rgb(0xFF, 0xEB, 0xCD);
        static constexpr colour_t C_BLUE                 = make_rgb(0x00, 0x00, 0xFF);
        static constexpr colour_t C_BLUEVIOLET           = make_rgb(0x8A, 0x2B, 0xE2);
        static constexpr colour_t C_BROWN                = make_rgb(0xA5, 0x2A, 0x2A);
        static constexpr colour_t C_BURLYWOOD            = make_rgb(0xDE, 0xB8, 0x87);
        static constexpr colour_t C_CADETBLUE            = make_rgb(0x5F, 0x9E, 0xA0);
        static constexpr colour_t C_CHARTREUSE           = make_rgb(0x7F, 0xFF, 0x00);
        static constexpr colour_t C_CHOCOLATE            = make_rgb(0xD2, 0x69, 0x1E);
        static constexpr colour_t C_CORAL                = make_rgb(0xFF, 0x7F, 0x50);
        static constexpr colour_t C_CORNFLOWERBLUE       = make_rgb(0x64, 0x95, 0xED);
        static constexpr colour_t C_CORNSILK             = make_rgb(0xFF, 0xF8, 0xDC);
        static constexpr colour_t C_CRIMSON              = make_rgb(0xDC, 0x14, 0x3C);
        static constexpr colour_t C_CYAN                 = make_rgb(0x00, 0xFF, 0xFF);
        static constexpr colour_t C_DARKBLUE             = make_rgb(0x00, 0x00, 0x8B);
        static constexpr colour_t C_DARKCYAN             = make_rgb(0x00, 0x8B, 0x8B);
        static constexpr colour_t C_DARKGOLDENROD        = make_rgb(0xB8, 0x86, 0x0B);
        static constexpr colour_t C_DARKGRAY             = make_rgb(0xA9, 0xA9, 0xA9);
        static constexpr colour_t C_DARKGREEN            = make_rgb(0x00, 0x64, 0x00);
        static constexpr colour_t C_DARKGREY             = make_rgb(0xA9, 0xA9, 0xA9);
        static constexpr colour_t C_DARKKHAKI            = make_rgb(0xBD, 0xB7, 0x6B);
        static constexpr colour_t C_DARKMAGENTA          = make_rgb(0x8B, 0x00, 0x8B);
        static constexpr colour_t C_DARKOLIVEGREEN       = make_rgb(0x55, 0x6B, 0x2F);
        static constexpr colour_t C_DARKORANGE           = make_rgb(0xFF, 0x8C, 0x00);
        static constexpr colour_t C_DARKORCHID           = make_rgb(0x99, 0x32, 0xCC);
        static constexpr colour_t C_DARKRED              = make_rgb(0x8B, 0x00, 0x00);
        static constexpr colour_t C_DARKSALMON           = make_rgb(0xE9, 0x96, 0x7A);
        static constexpr colour_t C_DARKSEAGREEN         = make_rgb(0x8F, 0xBC, 0x8F);
        static constexpr colour_t C_DARKSLATEBLUE        = make_rgb(0x48, 0x3D, 0x8B);
        static constexpr colour_t C_DARKSLATEGRAY        = make_rgb(0x2F, 0x4F, 0x4F);
        static constexpr colour_t C_DARKSLATEGREY        = make_rgb(0x2F, 0x4F, 0x4F);
        static constexpr colour_t C_DARKTURQUOISE        = make_rgb(0x00, 0xCE, 0xD1);
        static constexpr colour_t C_DARKVIOLET           = make_rgb(0x94, 0x00, 0xD3);
        static constexpr colour_t C_DEEPPINK             = make_rgb(0xFF, 0x14, 0x93);
        static constexpr colour_t C_DEEPSKYBLUE          = make_rgb(0x00, 0xBF, 0xFF);
        static constexpr colour_t C_DIMGRAY              = make_rgb(0x69, 0x69, 0x69);
        static constexpr colour_t C_DIMGREY              = make_rgb(0x69, 0x69, 0x69);
        static constexpr colour_t C_DODGERBLUE           = make_rgb(0x1E, 0x90, 0xFF);
        static constexpr colour_t C_FIREBRICK            = make_rgb(0xB2, 0x22, 0x22);
        static constexpr colour_t C_FLORALWHITE          = make_rgb(0xFF, 0xFA, 0xF0);
        static constexpr colour_t C_FORESTGREEN          = make_rgb(0x22, 0x8B, 0x22);
        static constexpr colour_t C_FUCHSIA              = make_rgb(0xFF, 0x00, 0xFF);
        static constexpr colour_t C_GAINSBORO            = make_rgb(0xDC, 0xDC, 0xDC);
        static constexpr colour_t C_GHOSTWHITE           = make_rgb(0xF8, 0xF8, 0xFF);
        static constexpr colour_t C_GOLD                 = make_rgb(0xFF, 0xD7, 0x00);
        static constexpr colour_t C_GOLDENROD            = make_rgb(0xDA, 0xA5, 0x20);
        static constexpr colour_t C_GRAY                 = make_rgb(0x80, 0x80, 0x80);
        static constexpr colour_t C_GREY                 = make_rgb(0x80, 0x80, 0x80);
        static constexpr colour_t C_GREEN                = make_rgb(0x00, 0x80, 0x00);
        static constexpr colour_t C_GREENYELLOW          = make_rgb(0xAD, 0xFF, 0x2F);
        static constexpr colour_t C_HONEYDEW             = make_rgb(0xF0, 0xFF, 0xF0);
        static constexpr colour_t C_HOTPINK              = make_rgb(0xFF, 0x69, 0xB4);
        static constexpr colour_t C_INDIANRED            = make_rgb(0xCD, 0x5C, 0x5C);
        static constexpr colour_t C_INDIGO               = make_rgb(0x4B, 0x00, 0x82);
        static constexpr colour_t C_IVORY                = make_rgb(0xFF, 0xFF, 0xF0);
        static constexpr colour_t C_KHAKI                = make_rgb(0xF0, 0xE6, 0x8C);
        static constexpr colour_t C_LAVENDER             = make_rgb(0xE6, 0xE6, 0xFA);
        static constexpr colour_t C_LAVENDERBLUSH        = make_rgb(0xFF, 0xF0, 0xF5);
        static constexpr colour_t C_LAWNGREEN            = make_rgb(0x7C, 0xFC, 0x00);
        static constexpr colour_t C_LEMONCHIFFON         = make_rgb(0xFF, 0xFA, 0xCD);
        static constexpr colour_t C_LIGHTBLUE            = make_rgb(0xAD, 0xD8, 0xE6);
        static constexpr colour_t C_LIGHTCORAL           = make_rgb(0xF0, 0x80, 0x80);
        static constexpr colour_t C_LIGHTCYAN            = make_rgb(0xE0, 0xFF, 0xFF);
        static constexpr colour_t C_LIGHTGOLDENRODYELLOW = make_rgb(0xFA, 0xFA, 0xD2);
        static constexpr colour_t C_LIGHTGRAY            = make_rgb(0xD3, 0xD3, 0xD3);
        static constexpr colour_t C_LIGHTGREEN           = make_rgb(0x90, 0xEE, 0x90);
        static constexpr colour_t C_LIGHTGREY            = make_rgb(0xD3, 0xD3, 0xD3);
        static constexpr colour_t C_LIGHTPINK            = make_rgb(0xFF, 0xB6, 0xC1);
        static constexpr colour_t C_LIGHTSALMON          = make_rgb(0xFF, 0xA0, 0x7A);
        static constexpr colour_t C_LIGHTSEAGREEN        = make_rgb(0x20, 0xB2, 0xAA);
        static constexpr colour_t C_LIGHTSKYBLUE         = make_rgb(0x87, 0xCE, 0xFA);
        static constexpr colour_t C_LIGHTSLATEGRAY       = make_rgb(0x77, 0x88, 0x99);
        static constexpr colour_t C_LIGHTSLATEGREY       = make_rgb(0x77, 0x88, 0x99);
        static constexpr colour_t C_LIGHTSTEELBLUE       = make_rgb(0xB0, 0xC4, 0xDE);
        static constexpr colour_t C_LIGHTYELLOW          = make_rgb(0xFF, 0xFF, 0xE0);
        static constexpr colour_t C_LIME                 = make_rgb(0x00, 0xFF, 0x00);
        static constexpr colour_t C_LIMEGREEN            = make_rgb(0x32, 0xCD, 0x32);
        static constexpr colour_t C_LINEN                = make_rgb(0xFA, 0xF0, 0xE6);
        static constexpr colour_t C_MAGENTA              = make_rgb(0xFF, 0x00, 0xFF);
        static constexpr colour_t C_MAROON               = make_rgb(0x80, 0x00, 0x00);
        static constexpr colour_t C_MEDIUMAQUAMARINE     = make_rgb(0x66, 0xCD, 0xAA);
        static constexpr colour_t C_MEDIUMBLUE           = make_rgb(0x00, 0x00, 0xCD);
        static constexpr colour_t C_MEDIUMORCHID         = make_rgb(0xBA, 0x55, 0xD3);
        static constexpr colour_t C_MEDIUMPURPLE         = make_rgb(0x93, 0x70, 0xDB);
        static constexpr colour_t C_MEDIUMSEAGREEN       = make_rgb(0x3C, 0xB3, 0x71);
        static constexpr colour_t C_MEDIUMSLATEBLUE      = make_rgb(0x7B, 0x68, 0xEE);
        static constexpr colour_t C_MEDIUMSPRINGGREEN    = make_rgb(0x00, 0xFA, 0x9A);
        static constexpr colour_t C_MEDIUMTURQUOISE      = make_rgb(0x48, 0xD1, 0xCC);
        static constexpr colour_t C_MEDIUMVIOLETRED      = make_rgb(0xC7, 0x15, 0x85);
        static constexpr colour_t C_MIDNIGHTBLUE         = make_rgb(0x19, 0x19, 0x70);
        static constexpr colour_t C_MINTCREAM            = make_rgb(0xF5, 0xFF, 0xFA);
        static constexpr colour_t C_MISTYROSE            = make_rgb(0xFF, 0xE4, 0xE1);
        static constexpr colour_t C_MOCCASIN             = make_rgb(0xFF, 0xE4, 0xB5);
        static constexpr colour_t C_NAVAJOWHITE          = make_rgb(0xFF, 0xDE, 0xAD);
        static constexpr colour_t C_NAVY                 = make_rgb(0x00, 0x00, 0x80);
        static constexpr colour_t C_OLDLACE              = make_rgb(0xFD, 0xF5, 0xE6);
        static constexpr colour_t C_OLIVE                = make_rgb(0x80, 0x80, 0x00);
        static constexpr colour_t C_OLIVEDRAB            = make_rgb(0x6B, 0x8E, 0x23);
        static constexpr colour_t C_ORANGE               = make_rgb(0xFF, 0xA5, 0x00);
        static constexpr colour_t C_ORANGERED            = make_rgb(0xFF, 0x45, 0x00);
        static constexpr colour_t C_ORCHID               = make_rgb(0xDA, 0x70, 0xD6);
        static constexpr colour_t C_PALEGOLDENROD        = make_rgb(0xEE, 0xE8, 0xAA);
        static constexpr colour_t C_PALEGREEN            = make_rgb(0x98, 0xFB, 0x98);
        static constexpr colour_t C_PALETURQUOISE        = make_rgb(0xAF, 0xEE, 0xEE);
        static constexpr colour_t C_PALEVIOLETRED        = make_rgb(0xDB, 0x70, 0x93);
        static constexpr colour_t C_PAPAYAWHIP           = make_rgb(0xFF, 0xEF, 0xD5);
        static constexpr colour_t C_PEACHPUFF            = make_rgb(0xFF, 0xDA, 0xB9);
        static constexpr colour_t C_PERU                 = make_rgb(0xCD, 0x85, 0x3F);
        static constexpr colour_t C_PINK                 = make_rgb(0xFF, 0xC0, 0xCB);
        static constexpr colour_t C_PLUM                 = make_rgb(0xDD, 0xA0, 0xDD);
        static constexpr colour_t C_POWDERBLUE           = make_rgb(0xB0, 0xE0, 0xE6);
        static constexpr colour_t C_PURPLE               = make_rgb(0x80, 0x00, 0x80);
        static constexpr colour_t C_RED                  = make_rgb(0xFF, 0x00, 0x00);
        static constexpr colour_t C_ROSYBROWN            = make_rgb(0xBC, 0x8F, 0x8F);
        static constexpr colour_t C_ROYALBLUE            = make_rgb(0x41, 0x69, 0xE1);
        static constexpr colour_t C_SADDLEBROWN          = make_rgb(0x8B, 0x45, 0x13);
        static constexpr colour_t C_SALMON               = make_rgb(0xFA, 0x80, 0x72);
        static constexpr colour_t C_SANDYBROWN           = make_rgb(0xF4, 0xA4, 0x60);
        static constexpr colour_t C_SEAGREEN             = make_rgb(0x2E, 0x8B, 0x57);
        static constexpr colour_t C_SEASHELL             = make_rgb(0xFF, 0xF5, 0xEE);
        static constexpr colour_t C_SIENNA               = make_rgb(0xA0, 0x52, 0x2D);
        static constexpr colour_t C_SILVER               = make_rgb(0xC0, 0xC0, 0xC0);
        static constexpr colour_t C_SKYBLUE              = make_rgb(0x87, 0xCE, 0xEB);
        static constexpr colour_t C_SLATEBLUE            = make_rgb(0x6A, 0x5A, 0xCD);
        static constexpr colour_t C_SLATEGRAY            = make_rgb(0x70, 0x80, 0x90);
        static constexpr colour_t C_SLATEGREY            = make_rgb(0x70, 0x80, 0x90);
        static constexpr colour_t C_SNOW                 = make_rgb(0xFF, 0xFA, 0xFA);
        static constexpr colour_t C_SPRINGGREEN          = make_rgb(0x00, 0xFF, 0x7F);
        static constexpr colour_t C_STEELBLUE            = make_rgb(0x46, 0x82, 0xB4);
        static constexpr colour_t C_TAN                  = make_rgb(0xD2, 0xB4, 0x8C);
        static constexpr colour_t C_TEAL                 = make_rgb(0x00, 0x80, 0x80);
        static constexpr colour_t C_THISTLE              = make_rgb(0xD8, 0xBF, 0xD8);
        static constexpr colour_t C_TOMATO               = make_rgb(0xFF, 0x63, 0x47);
        static constexpr colour_t C_TRANSPARENT          = make_rgba(0x00, 0x00, 0x00, 0x00);
        static constexpr colour_t C_TURQUOISE            = make_rgb(0x40, 0xE0, 0xD0);
        static constexpr colour_t C_VIOLET               = make_rgb(0xEE, 0x82, 0xEE);
        static constexpr colour_t C_WHEAT                = make_rgb(0xF5, 0xDE, 0xB3);
        static constexpr colour_t C_WHITE                = make_rgb(0xFF, 0xFF, 0xFF);
        static constexpr colour_t C_WHITESMOKE           = make_rgb(0xF5, 0xF5, 0xF5);
        static constexpr colour_t C_YELLOW               = make_rgb(0xFF, 0xFF, 0x00);
        static constexpr colour_t C_YELLOWGREEN          = make_rgb(0x9A, 0xCD, 0x32);
    }
}