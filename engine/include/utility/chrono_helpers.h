#pragma once

// #define ELOO_USE_EXTRA_CHRONO_TYPES

#include "utility/defines.h"

#include <EASTL/chrono.h>
#include <EASTL/string.h>
#include <chrono>

#if defined(ELOO_USE_EXTRA_CHRONO_TYPES)
#include <compare>

namespace eastl {
    using strong_ordering = std::strong_ordering;
    using weak_ordering = std::weak_ordering;
    using partial_ordering = std::partial_ordering;
}

// 
namespace eastl::chrono {
    
    ///////////////////////////////////////////////////////////////////////////////
    // C++20 standard: days, weeks, months, and years
    ///////////////////////////////////////////////////////////////////////////////
    using days = duration<int, ratio_multiply<ratio<24>, hours::period>>;
    using weeks = duration<int, ratio_multiply<ratio<7>, days::period>>;
    using years = duration<int, ratio_multiply<ratio<146097, 400>, days::period>>;
    using months = duration<int, ratio_divide<years::period, ratio<12>>>;


    ///////////////////////////////////////////////////////////////////////////////
    // system clock types
    ///////////////////////////////////////////////////////////////////////////////
    template <class _Duration>
    using sys_time = time_point<system_clock, _Duration>;
    using sys_seconds = sys_time<seconds>;
    using sys_days = sys_time<days>;


    ///////////////////////////////////////////////////////////////////////////////
    // local clock types
    ///////////////////////////////////////////////////////////////////////////////
    struct local_t {};
    template <class _Duration>
    using local_time = time_point<local_t, _Duration>;
    using local_seconds = local_time<seconds>;
    using local_days = local_time<days>;


    ///////////////////////////////////////////////////////////////////////////////
    // last_spec
    ///////////////////////////////////////////////////////////////////////////////
    struct last_spec {
        explicit last_spec() = default;
    };
    inline constexpr last_spec last{};


    ///////////////////////////////////////////////////////////////////////////////
    // day
    ///////////////////////////////////////////////////////////////////////////////
    class day {
    public:
        day() = default;
        constexpr explicit day(unsigned int d) ELOO_NOEXCEPT : mDay(static_cast<unsigned char>(d)) {}

        constexpr day& operator ++ () ELOO_NOEXCEPT { *this += days{1}; return *this; }
        constexpr day& operator -- () ELOO_NOEXCEPT { *this -= days{1}; return *this; }
        constexpr day operator ++ (int) ELOO_NOEXCEPT { day temp{*this}; ++(*this); return temp; }
        constexpr day operator -- (int) ELOO_NOEXCEPT { day temp{*this}; --(*this); return temp; }

        constexpr day& operator += (const days& d) ELOO_NOEXCEPT {
            mDay += static_cast<unsigned char>(d.count());
            return *this;
        }
        constexpr day& operator -= (const days& d) ELOO_NOEXCEPT {
            mDay -= static_cast<unsigned char>(d.count());
            return *this;
        }

        ELOO_NODISCARD constexpr explicit operator unsigned int() const ELOO_NOEXCEPT {
            return mDay;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mDay >= 1 && mDay <= 31; // Valid days are from 1 to 31.
        }

    private:
        unsigned char mDay = 1;
    };

    ELOO_NODISCARD constexpr bool operator == (const day& lhs, const day& rhs) ELOO_NOEXCEPT {
        return static_cast<unsigned int>(lhs) == static_cast<unsigned int>(rhs);
    }
    ELOO_NODISCARD constexpr strong_ordering operator <=> (const day& lhs, const day& rhs) ELOO_NOEXCEPT {
        return static_cast<unsigned int>(lhs) <=> static_cast<unsigned int>(rhs);
    }

    ELOO_NODISCARD constexpr day operator + (const day& lhs, const days& rhs) ELOO_NOEXCEPT {
        return day{ static_cast<unsigned int>(lhs) + rhs.count() };
    }
    ELOO_NODISCARD constexpr day operator + (const days& lhs, const day& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_NODISCARD constexpr day operator - (const day& lhs, const days& rhs) ELOO_NOEXCEPT {
        return day{ static_cast<unsigned int>(lhs) - rhs.count() };
    }
    ELOO_NODISCARD constexpr days operator - (const day& lhs, const day& rhs) ELOO_NOEXCEPT {
        return days{ static_cast<unsigned int>(lhs) - static_cast<unsigned int>(rhs) };
    }


    ///////////////////////////////////////////////////////////////////////////////
    // month
    ///////////////////////////////////////////////////////////////////////////////
    class month {
    public:
        month() = default;
        constexpr explicit month(unsigned int m) ELOO_NOEXCEPT : mMonth(static_cast<unsigned char>(m)) {}

        constexpr month& operator ++ () ELOO_NOEXCEPT { *this += months{1}; return *this; }
        constexpr month& operator -- () ELOO_NOEXCEPT { *this -= months{1}; return *this; }
        constexpr month operator ++ (int) ELOO_NOEXCEPT { month temp{*this}; ++(*this); return temp; }
        constexpr month operator -- (int) ELOO_NOEXCEPT { month temp{*this}; --(*this); return temp; }

        constexpr month& operator += (const months& m) ELOO_NOEXCEPT;
        constexpr month& operator -= (const months& m) ELOO_NOEXCEPT;

        ELOO_NODISCARD constexpr explicit operator unsigned int() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mMonth >= 1 && mMonth <= 12; // Valid months are from 1 to 12.
        }

    private:
        unsigned char mMonth = 1;
    };

    ELOO_NODISCARD constexpr bool operator == (const month& lhs, const month& rhs) ELOO_NOEXCEPT {
        return static_cast<unsigned int>(lhs) == static_cast<unsigned int>(rhs);
    }
    ELOO_NODISCARD constexpr strong_ordering operator <=> (const month& lhs, const month& rhs) ELOO_NOEXCEPT {
        return static_cast<unsigned int>(lhs) <=> static_cast<unsigned int>(rhs);
    }

    ELOO_NODISCARD constexpr month operator + (const month& lhs, const months& rhs) ELOO_NOEXCEPT {
        const auto _Mo = static_cast<long long>(static_cast<unsigned int>(lhs)) + (rhs.count() - 1);
        const auto _Div = (_Mo >= 0 ? _Mo : _Mo - 11) / 12;
        return month{ static_cast<unsigned int>(_Mo - _Div * 12 + 1) };
    }
    ELOO_NODISCARD constexpr month operator + (const months& lhs, const month& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_NODISCARD constexpr months operator - (const month& lhs, const month& rhs) ELOO_NOEXCEPT {
        const auto _Mo = static_cast<unsigned int>(lhs) - static_cast<unsigned int>(rhs);
        return months{ _Mo <= 11 ? _Mo : _Mo + 12 };
    }
    ELOO_NODISCARD constexpr month operator - (const month& lhs, const months& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    constexpr month& month::operator += (const months& m) ELOO_NOEXCEPT {
        *this = *this + m;
        return *this;
    }
    constexpr month& month::operator -= (const months& m) ELOO_NOEXCEPT {
        *this = *this - m;
        return *this;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // year
    ///////////////////////////////////////////////////////////////////////////////
    class year {
    public:
        year() = default;
        constexpr explicit year(int y) ELOO_NOEXCEPT : mYear(static_cast<short>(y)) {}

        constexpr year& operator ++ () ELOO_NOEXCEPT { *this += years{1}; return *this; }
        constexpr year& operator -- () ELOO_NOEXCEPT { *this -= years{1}; return *this; }
        constexpr year operator ++ (int) ELOO_NOEXCEPT { year temp{*this}; ++(*this); return temp; }
        constexpr year operator -- (int) ELOO_NOEXCEPT { year temp{*this}; --(*this); return temp; }

        constexpr year& operator += (const years& y) ELOO_NOEXCEPT {
#ifdef __EDG__ // TRANSITION, VSO-1271098
            mYear = static_cast<short>(mYear + y.count());
#else // ^^^ workaround / no workaround vvv
            mYear += static_cast<short>(y.count());
#endif // ^^^ no workaround ^^^
            return *this;
        }

        constexpr year& operator -= (const years& y) ELOO_NOEXCEPT {
#ifdef __EDG__ // TRANSITION, VSO-1271098
            mYear = static_cast<short>(mYear - y.count());
#else // ^^^ workaround / no workaround vvv
            mYear -= static_cast<short>(y.count());
#endif // ^^^ no workaround ^^^
            return *this;
        }

        ELOO_NODISCARD constexpr year operator + () const ELOO_NOEXCEPT {
            return *this;
        }
        ELOO_NODISCARD constexpr year operator - () const ELOO_NOEXCEPT {
            return year{ -mYear };
        }

        ELOO_NODISCARD constexpr bool is_leap() const ELOO_NOEXCEPT {
            // A year is a leap year if it is divisible by 4, except for end-of-century years, which must be divisible by 400.
            return (mYear % 4 == 0 && mYear % 100 != 0) || (mYear % 400 == 0);
        }

        ELOO_NODISCARD constexpr explicit operator int() const ELOO_NOEXCEPT {
            return mYear;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mYear >= __min_year && mYear <= __max_year; // Valid years are from -32767 to 32767.
        }

        ELOO_NODISCARD static constexpr year(min)() ELOO_NOEXCEPT {
            return year{ __min_year };
        }
        ELOO_NODISCARD static constexpr year(max)() ELOO_NOEXCEPT {
            return year{ __max_year };
        }

    private:
        short mYear = 0;
        static constexpr int __min_year = -32767; // Minimum year value, inclusive.
        static constexpr int __max_year = 32767; // Maximum year value, inclusive.
    };

    ELOO_NODISCARD constexpr bool operator == (const year& lhs, const year& rhs) ELOO_NOEXCEPT {
        return static_cast<int>(lhs) == static_cast<int>(rhs);
    }
    ELOO_NODISCARD constexpr strong_ordering operator <=> (const year& lhs, const year& rhs) ELOO_NOEXCEPT {
        return static_cast<int>(lhs) <=> static_cast<int>(rhs);
    }

    ELOO_NODISCARD constexpr year operator + (const year& lhs, const years& rhs) ELOO_NOEXCEPT {
        return year{ static_cast<int>(lhs) + rhs.count() };
    }
    ELOO_NODISCARD constexpr year operator + (const years& lhs, const year& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_NODISCARD constexpr year operator - (const year& lhs, const years& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }
    ELOO_NODISCARD constexpr years operator - (const year& lhs, const year& rhs) ELOO_NOEXCEPT {
        return years{ static_cast<int>(lhs) - static_cast<int>(rhs) };
    }


    ///////////////////////////////////////////////////////////////////////////////
    // weekday
    ///////////////////////////////////////////////////////////////////////////////
    class weekday_indexed;
    class weekday_last;

    class weekday {
    public:
        constexpr weekday() = default;
        constexpr explicit weekday(unsigned int wd) ELOO_NOEXCEPT : mWeekday(static_cast<unsigned char>(wd % 7)) {}
        constexpr weekday(const sys_days& d) ELOO_NOEXCEPT
            : mWeekday(static_cast<unsigned char>(_Weekday_from_days(d.time_since_epoch().count()))) {}
        constexpr explicit weekday(const local_days& d) ELOO_NOEXCEPT
            : mWeekday(static_cast<unsigned char>(_Weekday_from_days(d.time_since_epoch().count()))) {}

        constexpr weekday& operator ++ () ELOO_NOEXCEPT {
            *this += days{ 1 }; return *this;
        }
        constexpr weekday& operator -- () ELOO_NOEXCEPT {
            *this -= days{ 1 }; return *this;
        }
        constexpr weekday operator ++ (int) ELOO_NOEXCEPT {
            weekday temp{ *this }; ++(*this); return temp;
        }
        constexpr weekday operator -- (int) ELOO_NOEXCEPT {
            weekday temp{ *this }; --(*this); return temp;
        }

        constexpr weekday& operator += (const days& d) ELOO_NOEXCEPT;
        constexpr weekday& operator -= (const days& d) ELOO_NOEXCEPT;

        ELOO_NODISCARD constexpr unsigned int c_encoding() const ELOO_NOEXCEPT {
            return mWeekday;
        }

        ELOO_NODISCARD constexpr unsigned int iso_encoding() const ELOO_NOEXCEPT {
            return (mWeekday + 6) % 7;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mWeekday <= 6;
        }

        ELOO_NODISCARD constexpr weekday_indexed operator [] (unsigned int index) const ELOO_NOEXCEPT;
        ELOO_NODISCARD constexpr weekday_last operator [] (last_spec) const ELOO_NOEXCEPT;

    private:
        // courtesy of Howard Hinnant
        // https://howardhinnant.github.io/date_algorithms.html#weekday_from_days
        _NODISCARD static constexpr unsigned int _Weekday_from_days(int _Tp) noexcept {
            return static_cast<unsigned int>(_Tp >= -4 ? (_Tp + 4) % 7 : (_Tp + 5) % 7 + 6);
        }

    private:
        unsigned char mWeekday = 0;
    };

    ELOO_NODISCARD constexpr bool operator == (const weekday& lhs, const weekday& rhs) ELOO_NOEXCEPT {
        return lhs.c_encoding() == rhs.c_encoding();
    }
    ELOO_NODISCARD constexpr strong_ordering operator <=> (const weekday& lhs, const weekday& rhs) ELOO_NOEXCEPT {
        return lhs.c_encoding() <=> rhs.c_encoding();
    }

    ELOO_NODISCARD constexpr weekday operator + (const weekday& lhs, const days& rhs) ELOO_NOEXCEPT {
        const auto _Wd = static_cast<long long>(lhs.c_encoding()) + rhs.count();
        const auto _Div = (_Wd >= 0 ? _Wd : _Wd - 6) / 7;
        return weekday{ static_cast<unsigned int>(_Wd - _Div * 7) };
    }
    ELOO_NODISCARD constexpr weekday operator + (const days& lhs, const weekday& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_NODISCARD constexpr weekday operator - (const weekday& lhs, const days& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }
    ELOO_NODISCARD constexpr days operator - (const weekday& lhs, const weekday& rhs) ELOO_NOEXCEPT {
        const auto _Wd = lhs.c_encoding() - rhs.c_encoding();
        const auto _Wk = _Wd <= 6 ? _Wd : _Wd + 7;
        return days{ _Wk };
    }

    inline constexpr weekday& weekday::operator += (const days& d) ELOO_NOEXCEPT {
        *this = *this + d;
        return *this;
    }
    inline constexpr weekday& weekday::operator -= (const days& d) ELOO_NOEXCEPT {
        *this = *this - d;
        return *this;
    }

    // weekday_indexed
    class weekday_indexed {
    public:
        weekday_indexed() = default;
        constexpr weekday_indexed(const chrono::weekday& wd, unsigned int idx) ELOO_NOEXCEPT
            : mWeekday(wd), mIndex(static_cast<unsigned char>(idx)) {}

        ELOO_NODISCARD constexpr chrono::weekday weekday() const ELOO_NOEXCEPT {
            return mWeekday;
        }
        ELOO_NODISCARD constexpr unsigned int index() const ELOO_NOEXCEPT {
            return mIndex;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mWeekday.ok() && mIndex >= 1 && mIndex <= 5; // Valid indices are from 1 to 5.
        }

    private:
        chrono::weekday mWeekday;
        unsigned char mIndex = 1; // The index of the weekday, 1-based (1 = first occurrence, 2 = second occurrence, etc.).
    };

    ELOO_NODISCARD constexpr bool operator == (const weekday_indexed& lhs, const weekday_indexed& rhs) ELOO_NOEXCEPT {
        return lhs.weekday() == rhs.weekday() && lhs.index() == rhs.index();
    }
    ELOO_NODISCARD constexpr bool operator != (const weekday_indexed& lhs, const weekday_indexed& rhs) ELOO_NOEXCEPT {
        return !(lhs == rhs);
    }

    // weekday_last
    class weekday_last {
    public:
        constexpr explicit weekday_last(const chrono::weekday& wd) ELOO_NOEXCEPT : mWeekday(wd) {}

        ELOO_NODISCARD constexpr eastl::chrono::weekday weekday() const ELOO_NOEXCEPT {
            return mWeekday;
        }
        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mWeekday.ok();
        }

    private:
        chrono::weekday mWeekday; // The weekday for the last occurrence in the month.
    };

    ELOO_NODISCARD constexpr bool operator == (const weekday_last& lhs, const weekday_last& rhs) ELOO_NOEXCEPT {
        return lhs.weekday() == rhs.weekday();
    }
    ELOO_NODISCARD constexpr bool operator != (const weekday_last& lhs, const weekday_last& rhs) ELOO_NOEXCEPT {
        return !(lhs == rhs);
    }

    inline constexpr weekday_indexed weekday::operator [] (unsigned int index) const ELOO_NOEXCEPT {
        return weekday_indexed{ *this, index };
    }
    inline constexpr weekday_last weekday::operator [] (last_spec) const ELOO_NOEXCEPT {
        return weekday_last{ *this };
    }


    ///////////////////////////////////////////////////////////////////////////////
    // mixed date types
    ///////////////////////////////////////////////////////////////////////////////

    class month_day {
    public:
        month_day() = default;
        constexpr month_day(const month& m, const day& d) ELOO_NOEXCEPT : mMonth(m), mDay(d) {}

        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr day day() const ELOO_NOEXCEPT {
            return mDay;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            if (!mMonth.ok() || !mDay.ok()) {
                return false; // If either month or day is invalid, the month_day is invalid.
            }

            const auto _Da = static_cast<unsigned int>(mDay);
            const auto _Mo = static_cast<unsigned int>(mMonth);
            if (_Mo == 2) {
                return _Da <= 29;
            }

            if (_Mo == 4 || _Mo == 6 || _Mo == 9 || _Mo == 11) {
                return _Da <= 30;
            }

            return true;
        }

    private:
        chrono::month mMonth; // The month of the year.
        chrono::day mDay; // The day of the month.
    };

    ELOO_NODISCARD constexpr bool operator == (const month_day& lhs, const month_day& rhs) ELOO_NOEXCEPT {
        return lhs.month() == rhs.month() && lhs.day() == rhs.day();
    }

    ELOO_NODISCARD constexpr strong_ordering operator <=> (const month_day& lhs, const month_day& rhs) ELOO_NOEXCEPT {
        const auto _Comp = lhs.month() <=> rhs.month();
        if (_Comp != 0) {
            return _Comp;
        }

        return lhs.day() <=> rhs.day();
    }

    ///////////////////////////////////////////////////////////////////////////////

    class month_day_last {
    public:
        constexpr explicit month_day_last(const month& m) ELOO_NOEXCEPT : mMonth(m) {}

        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mMonth.ok(); // Valid if the month is valid.
        }

    private:
        chrono::month mMonth; // The month of the year.
    };

    ELOO_NODISCARD constexpr bool operator == (const month_day_last& lhs, const month_day_last& rhs) ELOO_NOEXCEPT {
        return lhs.month() == rhs.month();
    }
    ELOO_NODISCARD constexpr strong_ordering operator <=> (const month_day_last& lhs, const month_day_last& rhs) ELOO_NOEXCEPT {
        return lhs.month() <=> rhs.month();
    }

    ///////////////////////////////////////////////////////////////////////////////

    class month_weekday {
    public:
        constexpr month_weekday(const month& m, const weekday_indexed& wdi) ELOO_NOEXCEPT
            : mMonth(m), mWeekdayIndexed(wdi) {}

        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr weekday_indexed weekday_indexed() const ELOO_NOEXCEPT {
            return mWeekdayIndexed;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mMonth.ok() && mWeekdayIndexed.ok();
        }

    private:
        chrono::month mMonth;
        chrono::weekday_indexed mWeekdayIndexed;
    };

    ELOO_NODISCARD constexpr bool operator == (const month_weekday& lhs, const month_weekday& rhs) ELOO_NOEXCEPT {
        return lhs.month() == rhs.month() && lhs.weekday_indexed() == rhs.weekday_indexed();
    }

    ///////////////////////////////////////////////////////////////////////////////

    class month_weekday_last {
    public:
        constexpr month_weekday_last(const month& m, const weekday_last& wdl) ELOO_NOEXCEPT
            : mMonth(m), mWeekdayLast(wdl) {}

        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr weekday_last weekday_last() const ELOO_NOEXCEPT {
            return mWeekdayLast;
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mMonth.ok() && mWeekdayLast.ok();
        }

    private:
        chrono::month mMonth;
        chrono::weekday_last mWeekdayLast;
    };

    ELOO_NODISCARD constexpr bool operator == (const month_weekday_last& lhs, const month_weekday_last& rhs) ELOO_NOEXCEPT {
        return lhs.month() == rhs.month() && lhs.weekday_last() == rhs.weekday_last();
    }

    ///////////////////////////////////////////////////////////////////////////////

    class year_month {
    public:
        year_month() = default;
        constexpr year_month(const year& y, const month& m) ELOO_NOEXCEPT : mYear(y), mMonth(m) {}

        ELOO_NODISCARD constexpr year year() const ELOO_NOEXCEPT {
            return mYear;
        }
        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }

        template <int = 0>
        constexpr year_month& operator += (const months& m) ELOO_NOEXCEPT;
        template <int = 0>
        constexpr year_month& operator -= (const months& m) ELOO_NOEXCEPT;
        constexpr year_month& operator += (const years& y) ELOO_NOEXCEPT;
        constexpr year_month& operator -= (const years& y) ELOO_NOEXCEPT;

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mYear.ok() && mMonth.ok(); // Valid if both year and month are valid.
        }

    private:
        chrono::year mYear;
        chrono::month mMonth;
    };

    ELOO_NODISCARD constexpr bool operator == (const year_month& lhs, const year_month& rhs) ELOO_NOEXCEPT {
        return lhs.year() == rhs.year() && lhs.month() == rhs.month();
    }
    ELOO_NODISCARD constexpr strong_ordering operator <=> (const year_month& lhs, const year_month& rhs) ELOO_NOEXCEPT {
        const auto _Comp = lhs.year() <=> rhs.year();
        if (_Comp != 0) {
            return _Comp;
        }
        return lhs.month() <=> rhs.month();
    }

    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month operator + (const year_month& lhs, const months& rhs) ELOO_NOEXCEPT {
        const auto _Mo = static_cast<long long>(static_cast<unsigned int>(lhs.month())) + (rhs.count() - 1);
        const auto _Div = (_Mo >= 0 ? _Mo : _Mo - 11) / 12;
        return year_month{ lhs.year() + years{_Div}, month{static_cast<unsigned int>(_Mo - _Div * 12 + 1)} };
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month operator + (const months& lhs, const year_month& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month operator - (const year_month& lhs, const months& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr months operator - (const year_month& lhs, const year_month& rhs) ELOO_NOEXCEPT {
        return lhs.year() - rhs.year()
            + months{ static_cast<int>(static_cast<unsigned int>(lhs.month()))
                     - static_cast<int>(static_cast<unsigned int>(rhs.month())) };
    }

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month operator + (const year_month& lhs, const years& rhs) ELOO_NOEXCEPT {
        return year_month{ lhs.year() + rhs, lhs.month() };
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month operator + (const years& lhs, const year_month& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month operator - (const year_month& lhs, const years& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    template <int>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month& year_month::operator += (const months& m) ELOO_NOEXCEPT {
        *this = *this + m;
        return *this;
    }
    template <int>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month& year_month::operator -= (const months& m) ELOO_NOEXCEPT {
        *this = *this - m;
        return *this;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month& year_month::operator += (const years& y) ELOO_NOEXCEPT {
        *this = *this + y;
        return *this;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month& year_month::operator -= (const years& y) ELOO_NOEXCEPT {
        *this = *this - y;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////

    inline constexpr day _Last_day_table[] = {
        day{ 31 }, day{ 28 }, day{ 31 }, day{ 30 }, day{ 31 }, day{ 30 },
        day{ 31 }, day{ 31 }, day{ 30 }, day{ 31 }, day{ 30 }, day{ 31 },
        day{ 255 }, day{ 255 }, day{ 255 }, day{ 255 } // Extra invalid days to prevent UB.
    };

    ELOO_FORCE_INLINE ELOO_NODISCARD day _Last_day(const year& y, const month& m) ELOO_NOEXCEPT {
        // Returns the last day of the month, accounting for leap years in February.
        if (m == February && y.is_leap()) {
            return day{ 29 };
        }
        return _Last_day_table[(static_cast<unsigned int>(m) - 1) & 0xF];
    }

    ///////////////////////////////////////////////////////////////////////////////

    class year_month_day_last;

    class year_month_day {
    public:
        year_month_day() = default;
        constexpr year_month_day(const year& y, const month& m, const day& d) ELOO_NOEXCEPT
            : mYear(y), mMonth(m), mDay(d) {}
        constexpr year_month_day(const year_month_day_last& ymdl) ELOO_NOEXCEPT;
        constexpr year_month_day(const sys_days& d) ELOO_NOEXCEPT
            : year_month_day{ _Civil_from_days(d.time_since_epoch().count()) } {}
        constexpr explicit year_month_day(const local_days& d) ELOO_NOEXCEPT
            : year_month_day{ _Civil_from_days(d.time_since_epoch().count()) } {}

        template <int = 0>
        constexpr year_month_day& operator += (const months& d) ELOO_NOEXCEPT;
        template <int = 0>
        constexpr year_month_day& operator -= (const months& d) ELOO_NOEXCEPT;
        constexpr year_month_day& operator += (const years& y) ELOO_NOEXCEPT;
        constexpr year_month_day& operator -= (const years& y) ELOO_NOEXCEPT;

        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year year() const ELOO_NOEXCEPT {
            return mYear;
        }
        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr day day() const ELOO_NOEXCEPT {
            return mDay;
        }

        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr operator sys_days() const ELOO_NOEXCEPT {
            return sys_days{ _Days_from_civil() };
        }
        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr explicit operator local_days() const ELOO_NOEXCEPT {
            return local_days{ static_cast<sys_days>(*this).time_since_epoch() };
        }

        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            if (!mYear.ok() || !mMonth.ok()) {
                return false;
            }

            return mDay >= chrono::day{ 1 } && mDay <= _Last_day(mYear, mMonth);
        }

        ELOO_FORCE_INLINE ELOO_NODISCARD constexpr int _Calculate_weekday() const ELOO_NOEXCEPT {
            const int _Day_int = static_cast<int>(static_cast<unsigned int>(mDay));
            const int _Month_int = static_cast<int>(static_cast<unsigned int>(mMonth));

            const int _Era_year = static_cast<int>(mYear) - (_Month_int <= 2);
            const int _Era = (_Era_year >= 0 ? _Era_year : _Era_year - 399) / 400;
            const int _Yoe = _Era_year - _Era * 400;
            const int _Yday_era = ((979 * (_Month_int + (_Month_int > 2 ? -3 : 9)) + 19) >> 5) + _Day_int - 1;
            const int _Doe = ((1461 * _Yoe) >> 2) - _Yoe / 100 + _Yday_era;
            return (_Doe + 3) % 7; // the era began on a Wednesday
        }

    private:
        // _Civil_from_days and _Days_from_civil perform conversions between the dates in the (proleptic) Gregorian
        // calendar and the continuous count of days since 1970-01-01.

        // To simplify the handling of leap days (February 29th), the algorithm below uses a modified calendar
        // internally, in which each year begins on March 1st, while January and February belong to the previous year.
        // We denote the modified year and month number as _Yp and _Mp. We also define modified centuries that begin on
        // each modified year whose _Yp is a multiple of 100.

        // _Mp | Month     | Day of Year
        // --- | --------- | -----------
        //  0  | March     | [  0,  30]
        //  1  | April     | [ 31,  60]
        //  2  | May       | [ 61,  91]
        //  3  | June      | [ 92, 121]
        //  4  | July      | [122, 152]
        //  5  | August    | [153, 183]
        //  6  | September | [184, 213]
        //  7  | October   | [214, 244]
        //  8  | November  | [245, 274]
        //  9  | December  | [275, 305]
        // 10  | January   | [306, 336]
        // 11  | February  | [337, 365] on leap years, [337, 364] on regular years

        // _Yp |  First Day  | Last Day (inclusive) | Leap Year?
        // --- | ----------- | -------------------- | ----------
        //  -4 | -0004-03-01 |     -0003-02-28      |     No
        //  -3 | -0003-03-01 |     -0002-02-28      |     No
        //  -2 | -0002-03-01 |     -0001-02-28      |     No
        //  -1 | -0001-03-01 |      0000-02-29      |     Yes
        //   0 |  0000-03-01 |      0001-02-28      |     No
        //   1 |  0001-03-01 |      0002-02-28      |     No
        //   2 |  0002-03-01 |      0003-02-28      |     No
        //   3 |  0003-03-01 |      0004-02-29      |     Yes

        // _Century |  First Day  | Last Day (inclusive) | Long Century?
        // -------- | ----------- | -------------------- | -------------
        //    -4    | -0400-03-01 |     -0300-02-28      |      No
        //    -3    | -0300-03-01 |     -0200-02-28      |      No
        //    -2    | -0200-03-01 |     -0100-02-28      |      No
        //    -1    | -0100-03-01 |      0000-02-29      |      Yes
        //     0    |  0000-03-01 |      0100-02-28      |      No
        //     1    |  0100-03-01 |      0200-02-28      |      No
        //     2    |  0200-03-01 |      0300-02-28      |      No
        //     3    |  0300-03-01 |      0400-02-29      |      Yes

        // The structure of the modified calendar:
        // 1 ) It has a period of 4 centuries.
        // 2 ) Each calendar period (146097 days) contains 3 regular centuries followed by a long century (36525 days).
        // 3 ) Each regular century (36524 days) contains 24 regular 4-year spans followed by a short 4-year span.
        // 3') Each long century (36525 days) contains 25 regular 4-year spans.
        // 4 ) Each regular 4-year span (1461 days) contains 3 regular years followed by a leap year.
        // 4') Each short 4-year span (1460 days) contains 4 regular years.

        // Formula 1: Compute _Day_of_year of the first day of month _Mp
        //
        //   _Day_of_year = (979 * _Mp + 19) >> 5
        //
        // A more well-known formula is 30 * _Mp + floor((3 * _Mp + 2) / 5) or floor((153 * _Mp + 2) / 5), which is used
        // in Howard Hinnant's paper.
        //
        // The formula above returns the same result for all _Mp in [0, 11].
        // Note that 979 / 2^5 = 30.59375 ~= 30.6 = 153 / 5.

        // Formula 1': Compute _Mp from _Day_of_year
        //
        //   _Mp = (535 * _Day_of_year + 333) >> 14
        //
        // Howard Hinnant's paper uses floor((5 * _Day_of_year + 2) / 153), the inverse of floor((153 * _Mp + 2) / 5) or
        // ceil((153 * _Mp - 2) / 5).
        //
        // The formula above returns the same result for all _Day_of_year in [0, 365].
        // Note that 2^14 / 535 = 30.624... ~= 30.6 = 153 / 5.

        // Formula 2: Compute _Zx of the first day of year _Yp, where _Zx is the continuous count of days since
        // 0000-03-01.
        //
        //   _Zx = ((1461 * _Yp) >> 2) - _Century + (_Century >> 2)
        //
        // Start with multiplying by the number of days in regular years (365), add one day for the leap year in each
        // 4-year span, subtract one day for the short 4-year span in each century, and finally add one day for the long
        // century in each calendar period. This gives us 365 * _Yp + floor(_Yp / 4) - _Century + floor(_Century / 4).

        // Formula 2-1: Compute _Day_of_century of the first day of year _Year_of_century
        //
        //   _Day_of_century = (1461 * _Year_of_century) >> 2
        //
        // Start with multiplying by the number of days in regular years (365), add one day for the leap year in each
        // 4-year span. This gives us 365 * _Year_of_century + floor(_Year_of_century / 4)
        // == floor(1461 * _Year_of_century / 4).

        // Formula 2-1': Compute _Year_of_century from _Day_of_century
        //
        //   _Year_of_century = (91867 * (_Day_of_century + 1)) >> 25
        //
        // The inverse of floor(1461 * _Year_of_century / 4) or ceil((1461 * _Year_of_century - 3) / 4) is
        // floor((4 * _Day_of_century + 3) / 1461).
        //
        // The formula above returns the same result for all _Day_of_century in [0, 36524].
        // Note that 2^25 / 91867 = 365.2501... ~= 365.25 = 1461 / 4.

        // Formula 2-2: Compute _Zx of the first day of century _Century, where _Zx is the continuous count of days
        // since 0000-03-01.
        //
        //   _Zx = (146097 * _Century) >> 2
        //
        // Start with multiplying by the number of days in regular centuries (36524), add one day for the long century
        // in each calendar period. This gives us 36524 * _Century + floor(_Century / 4) = floor(146097 * _Century / 4).

        // Formula 2-2': Compute _Century from _Zx, where _Zx is the continuous count of days since 0000-03-01.
        //
        //   _Century = floor((4 * _Zx + 3) / 146097)
        //
        // This is the inverse of floor(146097 * _Year_of_century / 4) or ceil((146097 * _Year_of_century - 3) / 4)

        // courtesy of Howard Hinnant
        // https://howardhinnant.github.io/date_algorithms.html#civil_from_days
        ELOO_NODISCARD static constexpr year_month_day _Civil_from_days(int _Tp) ELOO_NOEXCEPT {
            static_assert(numeric_limits<unsigned int>::digits >= 32);
            static_assert(numeric_limits<int>::digits >= 26);
            const int _Zx = _Tp + 719468; // Shift epoch to 0000-03-01
            // Formula 2-2'
            const int _Century = (_Zx >= 0 ? 4 * _Zx + 3 : 4 * _Zx - 146093) / 146097;
            // Formula 2-2
            const unsigned int _Day_of_century =
                static_cast<unsigned int>(_Zx - ((146097 * _Century) >> 2)); // [0, 36524]
            // Formula 2-1'
            const unsigned int _Year_of_century = (91867 * (_Day_of_century + 1)) >> 25; // [0, 99]
            const int _Yp = static_cast<int>(_Year_of_century) + _Century * 100; // Where March is the first month
            // Formula 2-1
            const unsigned int _Day_of_year = _Day_of_century - ((1461 * _Year_of_century) >> 2); // [0, 365]
            // Formula 1'
            const unsigned int _Mp = (535 * _Day_of_year + 333) >> 14; // [0, 11]
            // Formula 1
            const unsigned int _Day = _Day_of_year - ((979 * _Mp + 19) >> 5) + 1; // [1, 31]
            const unsigned int _Month = _Mp + (_Mp < 10 ? 3 : static_cast<unsigned int>(-9)); // [1, 12]
            return year_month_day{ chrono::year{_Yp + (_Month <= 2)}, chrono::month{_Month}, chrono::day{_Day} };
        }

        // courtesy of Howard Hinnant
        // https://howardhinnant.github.io/date_algorithms.html#days_from_civil
        ELOO_NODISCARD constexpr days _Days_from_civil() const ELOO_NOEXCEPT {
            static_assert(numeric_limits<unsigned int>::digits >= 18);
            static_assert(numeric_limits<int>::digits >= 26);
            const unsigned int _Mo = static_cast<unsigned int>(mMonth); // [1, 12]
            const int _Yp = static_cast<int>(mYear) - (_Mo <= 2);
            const int _Century = (_Yp >= 0 ? _Yp : _Yp - 99) / 100;
            const unsigned int _Mp = _Mo + (_Mo > 2 ? static_cast<unsigned int>(-3) : 9); // [0, 11]
            // Formula 1
            const int _Day_of_year = static_cast<int>(((979 * _Mp + 19) >> 5) + static_cast<unsigned int>(mDay)) - 1;
            // Formula 2
            return days{ ((1461 * _Yp) >> 2) - _Century + (_Century >> 2) + _Day_of_year - 719468 };
        }

    private:
        chrono::year mYear;
        chrono::month mMonth;
        chrono::day mDay;
    };

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr bool operator == (const year_month_day& lhs, const year_month_day& rhs) ELOO_NOEXCEPT {
        return lhs.year() == rhs.year() && lhs.month() == rhs.month() && lhs.day() == rhs.day();
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr strong_ordering operator <=> (const year_month_day& lhs, const year_month_day& rhs) ELOO_NOEXCEPT {
        auto _Comp = lhs.year() <=> rhs.year();
        if (_Comp != 0) {
            return _Comp;
        }
        _Comp = lhs.month() <=> rhs.month();
        if (_Comp != 0) {
            return _Comp;
        }
        return lhs.day() <=> rhs.day();
    }

    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day operator + (const year_month_day& lhs, const months& rhs) ELOO_NOEXCEPT {
        const auto _Ym = year_month{ lhs.year(), lhs.month() } + rhs;
        return { _Ym.year(), _Ym.month(), lhs.day() };
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day operator + (const months& lhs, const year_month_day& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day operator - (const year_month_day& lhs, const months& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day operator + (const year_month_day& lhs, const years& rhs) ELOO_NOEXCEPT {
        return { lhs.year() + rhs, lhs.month(), lhs.day() };
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day operator + (const years& lhs, const year_month_day& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day operator - (const year_month_day& lhs, const years& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    template <int>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day& year_month_day::operator += (const months& m) ELOO_NOEXCEPT {
        *this = *this + m;
        return *this;
    }
    template <int>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day& year_month_day::operator -= (const months& m) ELOO_NOEXCEPT {
        *this = *this - m;
        return *this;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day& year_month_day::operator += (const years& y) ELOO_NOEXCEPT {
        *this = *this + y;
        return *this;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day& year_month_day::operator -= (const years& y) ELOO_NOEXCEPT {
        *this = *this - y;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////

    class year_month_day_last {
    public:
        constexpr year_month_day_last(const year& y, const month_day_last& mdl) ELOO_NOEXCEPT
            : mYear(y), mMonthDayLast(mdl) {}

        template <int = 0>
        constexpr year_month_day_last& operator += (const months& d) ELOO_NOEXCEPT;
        template <int = 0>
        constexpr year_month_day_last& operator -= (const months& d) ELOO_NOEXCEPT;
        constexpr year_month_day_last& operator += (const years& y) ELOO_NOEXCEPT;
        constexpr year_month_day_last& operator -= (const years& y) ELOO_NOEXCEPT;

        ELOO_NODISCARD constexpr year year() const ELOO_NOEXCEPT {
            return mYear;
        }
        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonthDayLast.month();
        }
        ELOO_NODISCARD constexpr month_day_last month_day_last() const ELOO_NOEXCEPT {
            return mMonthDayLast;
        }
        ELOO_NODISCARD constexpr day day() const ELOO_NOEXCEPT {
            return _Last_day(year(), month());
        }

        ELOO_NODISCARD constexpr operator sys_days() const ELOO_NOEXCEPT {
            return sys_days{ year_month_day{ year(), month(), day() } };
        }
        ELOO_NODISCARD constexpr explicit operator local_days() const ELOO_NOEXCEPT {
            return local_days{ static_cast<sys_days>(*this).time_since_epoch() };
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mYear.ok() && mMonthDayLast.ok();
        }

    private:
        chrono::year mYear;
        chrono::month_day_last mMonthDayLast;
    };

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr bool operator == (const year_month_day_last& lhs, const year_month_day_last& rhs) ELOO_NOEXCEPT {
        return lhs.year() == rhs.year() && lhs.month_day_last() == rhs.month_day_last();
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr strong_ordering operator <=> (const year_month_day_last& lhs, const year_month_day_last& rhs) ELOO_NOEXCEPT {
        const auto _Comp = lhs.year() <=> rhs.year();
        if (_Comp != 0) {
            return _Comp;
        }
        return lhs.month_day_last() <=> rhs.month_day_last();
    }

    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day_last operator + (const year_month_day_last& lhs, const months& rhs) ELOO_NOEXCEPT {
        const auto _Ym = year_month{ lhs.year(), lhs.month() } + rhs;
        return { _Ym.year(), month_day_last{ _Ym.month() } };
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day_last operator + (const months& lhs, const year_month_day_last& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day_last operator - (const year_month_day_last& lhs, const months& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day_last operator + (const year_month_day_last& lhs, const years& rhs) ELOO_NOEXCEPT {
        return { lhs.year() + rhs, lhs.month_day_last() };
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day_last operator + (const years& lhs, const year_month_day_last& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_day_last operator - (const year_month_day_last& lhs, const years& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    template <int>
    ELOO_FORCE_INLINE constexpr year_month_day_last& year_month_day_last::operator += (const months& m) ELOO_NOEXCEPT {
        *this = *this + m;
        return *this;
    }
    template <int>
    ELOO_FORCE_INLINE constexpr year_month_day_last& year_month_day_last::operator -= (const months& m) ELOO_NOEXCEPT {
        *this = *this - m;
        return *this;
    }
    ELOO_FORCE_INLINE constexpr year_month_day_last& year_month_day_last::operator += (const years& y) ELOO_NOEXCEPT {
        *this = *this + y;
        return *this;
    }
    ELOO_FORCE_INLINE constexpr year_month_day_last& year_month_day_last::operator -= (const years& y) ELOO_NOEXCEPT {
        *this = *this - y;
        return *this;
    }

    ELOO_FORCE_INLINE constexpr year_month_day::year_month_day(const year_month_day_last& ymdl) ELOO_NOEXCEPT
        : mYear(ymdl.year()), mMonth(ymdl.month()), mDay(_Last_day(ymdl.year(), ymdl.month())) {}

    ///////////////////////////////////////////////////////////////////////////////

    class year_month_weekday {
    public:
        year_month_weekday() = default;
        constexpr year_month_weekday(const year& y, const month& m, const weekday_indexed& wdi) ELOO_NOEXCEPT
            : mYear(y), mMonth(m), mWeekdayIndexed(wdi) {}
        constexpr year_month_weekday(const sys_days& d) ELOO_NOEXCEPT
            : year_month_weekday{ _Ymwd_from_days(d.time_since_epoch()) } {}
        constexpr year_month_weekday(const local_days& d) ELOO_NOEXCEPT
            : year_month_weekday{ _Ymwd_from_days(d.time_since_epoch()) } {}

        template <int = 0>
        constexpr year_month_weekday& operator += (const months& m) ELOO_NOEXCEPT;
        template <int = 0>
        constexpr year_month_weekday& operator -= (const months& m) ELOO_NOEXCEPT;
        constexpr year_month_weekday& operator += (const years& y) ELOO_NOEXCEPT;
        constexpr year_month_weekday& operator -= (const years& y) ELOO_NOEXCEPT;

        ELOO_NODISCARD constexpr year year() const ELOO_NOEXCEPT {
            return mYear;
        }
        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr weekday weekday() const ELOO_NOEXCEPT {
            return mWeekdayIndexed.weekday();
        }
        ELOO_NODISCARD constexpr unsigned int index() const ELOO_NOEXCEPT {
            return mWeekdayIndexed.index();
        }
        ELOO_NODISCARD constexpr weekday_indexed weekday_indexed() const ELOO_NOEXCEPT {
            return mWeekdayIndexed;
        }

        ELOO_NODISCARD constexpr operator sys_days() const ELOO_NOEXCEPT {
            const sys_days _First = year_month_day{ mYear, mMonth, day{ 1 } };
            const days _Diff = weekday() - chrono::weekday{ _First };
            const days _Days = _Diff + days{ (static_cast<int>(index()) - 1) * 7 };
            return _First + _Days;
        }
        ELOO_NODISCARD constexpr explicit operator local_days() const ELOO_NOEXCEPT {
            return local_days{ static_cast<sys_days>(*this).time_since_epoch() };
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            if (!mYear.ok() || !mMonth.ok() || !mWeekdayIndexed.ok()) {
                return false;
            }
            
            if (index() <= 4) {
                return true;
            }

            // As index() == 5 is not always valid
            // Determine the date of the first weekday and check if + days{28} is <= last day of the month
            const sys_days _First_of_month = year_month_day{ mYear, mMonth, day{ 1 } };
            const days _First_weekday = weekday() - chrono::weekday{ _First_of_month } + days{ 1 };
            const days _Last = _First_weekday + days{ 28 };
            return static_cast<unsigned int>(_Last.count()) <= static_cast<unsigned int>(_Last_day(mYear, mMonth));
        }

    private:
        ELOO_NODISCARD static constexpr year_month_weekday _Ymwd_from_days(days d) ELOO_NOEXCEPT {
            const chrono::year_month_day _Ymd   = sys_days{ d };
            const chrono::weekday _Wd           = sys_days{ d };
            const auto _Idx                     = ((static_cast<unsigned int>(_Ymd.day()) - 1) / 7) + 1;
            return { _Ymd.year(), _Ymd.month(), _Wd[_Idx] };
        }

    private:
        chrono::year mYear;
        chrono::month mMonth;
        chrono::weekday_indexed mWeekdayIndexed;
    };

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr bool operator == (const year_month_weekday& lhs, const year_month_weekday& rhs) ELOO_NOEXCEPT {
        return lhs.year() == rhs.year() && lhs.month() == rhs.month() && lhs.weekday_indexed() == rhs.weekday_indexed();
    }

    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday operator + (const year_month_weekday& lhs, const months& rhs) ELOO_NOEXCEPT {
        const auto _Ym = year_month{ lhs.year(), lhs.month() } + rhs;
        return { _Ym.year(), _Ym.month(), lhs.weekday_indexed() };
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday operator + (const months& lhs, const year_month_weekday& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday operator - (const year_month_weekday& lhs, const months& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday operator + (const year_month_weekday& lhs, const years& rhs) ELOO_NOEXCEPT {
        return { lhs.year() + rhs, lhs.month(), lhs.weekday_indexed() };
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday operator + (const years& lhs, const year_month_weekday& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday operator - (const year_month_weekday& lhs, const years& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    template <int>
    ELOO_FORCE_INLINE constexpr year_month_weekday& year_month_weekday::operator += (const months& m) ELOO_NOEXCEPT {
        *this = *this + m;
        return *this;
    }
    template <int>
    ELOO_FORCE_INLINE constexpr year_month_weekday& year_month_weekday::operator -= (const months& m) ELOO_NOEXCEPT {
        *this = *this - m;
        return *this;
    }
    ELOO_FORCE_INLINE constexpr year_month_weekday& year_month_weekday::operator += (const years& y) ELOO_NOEXCEPT {
        *this = *this + y;
        return *this;
    }
    ELOO_FORCE_INLINE constexpr year_month_weekday& year_month_weekday::operator -= (const years& y) ELOO_NOEXCEPT {
        *this = *this - y;
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////

    class year_month_weekday_last {
    public:
        constexpr year_month_weekday_last(const year& y, const month& m, const weekday_last& wdl) ELOO_NOEXCEPT
            : mYear(y), mMonth(m), mWeekdayLast(wdl) {}

        template <int = 0>
        constexpr year_month_weekday_last& operator += (const months& m) ELOO_NOEXCEPT;
        template <int = 0>
        constexpr year_month_weekday_last& operator -= (const months& m) ELOO_NOEXCEPT;
        constexpr year_month_weekday_last& operator += (const years& y) ELOO_NOEXCEPT;
        constexpr year_month_weekday_last& operator -= (const years& y) ELOO_NOEXCEPT;

        ELOO_NODISCARD constexpr year year() const ELOO_NOEXCEPT {
            return mYear;
        }
        ELOO_NODISCARD constexpr month month() const ELOO_NOEXCEPT {
            return mMonth;
        }
        ELOO_NODISCARD constexpr weekday weekday() const ELOO_NOEXCEPT {
            return mWeekdayLast.weekday();
        }
        ELOO_NODISCARD constexpr weekday_last weekday_last() const ELOO_NOEXCEPT {
            return mWeekdayLast;
        }

        ELOO_NODISCARD constexpr operator sys_days() const ELOO_NOEXCEPT {
            const sys_days _Last        = year_month_day_last{ mYear, month_day_last{ mMonth } };
            const chrono::days _Diff    = chrono::weekday{ _Last } - weekday();
            return _Last - _Diff;
        }
        ELOO_NODISCARD constexpr explicit operator local_days() const ELOO_NOEXCEPT {
            return local_days{ static_cast<sys_days>(*this).time_since_epoch() };
        }

        ELOO_NODISCARD constexpr bool ok() const ELOO_NOEXCEPT {
            return mYear.ok() && mMonth.ok() && mWeekdayLast.ok();
        }

    private:
        chrono::year mYear;
        chrono::month mMonth;
        chrono::weekday_last mWeekdayLast;
    };

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr bool operator == (const year_month_weekday_last& lhs, const year_month_weekday_last& rhs) ELOO_NOEXCEPT {
        return lhs.year() == rhs.year() && lhs.month() == rhs.month() && lhs.weekday_last() == rhs.weekday_last();
    }

    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday_last operator + (const year_month_weekday_last& lhs, const months& rhs) ELOO_NOEXCEPT {
        const auto _Ym = year_month{ lhs.year(), lhs.month() } + rhs;
        return { _Ym.year(), _Ym.month(), lhs.weekday_last() };
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday_last operator + (const months& lhs, const year_month_weekday_last& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    template <int = 0>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday_last operator - (const year_month_weekday_last& lhs, const months& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday_last operator + (const year_month_weekday_last& lhs, const years& rhs) ELOO_NOEXCEPT {
        return { lhs.year() + rhs, lhs.month(), lhs.weekday_last() };
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday_last operator + (const years& lhs, const year_month_weekday_last& rhs) ELOO_NOEXCEPT {
        return rhs + lhs;
    }
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr year_month_weekday_last operator - (const year_month_weekday_last& lhs, const years& rhs) ELOO_NOEXCEPT {
        return lhs + -rhs;
    }

    template <int>
    ELOO_FORCE_INLINE constexpr year_month_weekday_last& year_month_weekday_last::operator += (const months& m) ELOO_NOEXCEPT {
        *this = *this + m;
        return *this;
    }
    template <int>
    ELOO_FORCE_INLINE constexpr year_month_weekday_last& year_month_weekday_last::operator -= (const months& m) ELOO_NOEXCEPT {
        *this = *this - m;
        return *this;
    }
    ELOO_FORCE_INLINE constexpr year_month_weekday_last& year_month_weekday_last::operator += (const years& y) ELOO_NOEXCEPT {
        *this = *this + y;
        return *this;
    }
    ELOO_FORCE_INLINE constexpr year_month_weekday_last& year_month_weekday_last::operator -= (const years& y) ELOO_NOEXCEPT {
        *this = *this - y;
        return *this;
    }


    ///////////////////////////////////////////////////////////////////////////////
    // civil calendar conventional syntax operators
    ///////////////////////////////////////////////////////////////////////////////

    // year_month

    ELOO_NODISCARD constexpr year_month operator / (const year& y, const month& m) ELOO_NOEXCEPT { return year_month{ y, m }; }
    ELOO_NODISCARD constexpr year_month operator / (int y, const month& m) ELOO_NOEXCEPT {
        return year_month{ year{ static_cast<unsigned int>(y) }, m };
    }
    ELOO_NODISCARD constexpr year_month operator / (const year& y, int m) ELOO_NOEXCEPT {
        return year_month{ y, month{ static_cast<unsigned int>(m) } };
    }


    // month_day

    ELOO_NODISCARD constexpr month_day operator / (const month& m, const day& d) ELOO_NOEXCEPT { return month_day{ m, d }; }
    ELOO_NODISCARD constexpr month_day operator / (int m, const day& d) ELOO_NOEXCEPT {
        return month_day{ month{ static_cast<unsigned int>(m) }, d };
    }


    // month_day_last

    ELOO_NODISCARD constexpr month_day_last operator / (const month& m, last_spec) ELOO_NOEXCEPT {
        return month_day_last{ m };
    }
    ELOO_NODISCARD constexpr month_day_last operator / (last_spec, const month& m) ELOO_NOEXCEPT {
        return m / last;
    }
    ELOO_NODISCARD constexpr month_day_last operator / (int m, last_spec) ELOO_NOEXCEPT {
        return month{ static_cast<unsigned int>(m) } / last;
    }
    ELOO_NODISCARD constexpr month_day_last operator / (last_spec, int m) ELOO_NOEXCEPT {
        return month{ static_cast<unsigned int>(m) } / last;
    }


    // month_weekday

    ELOO_NODISCARD constexpr month_weekday operator / (const month& m, const weekday_indexed& wdi) ELOO_NOEXCEPT {
        return month_weekday{ m, wdi };
    }
    ELOO_NODISCARD constexpr month_weekday operator / (int m, const weekday_indexed& wdi) ELOO_NOEXCEPT {
        return month_weekday{ month{ static_cast<unsigned int>(m) }, wdi };
    }
    ELOO_NODISCARD constexpr month_weekday operator / (const month& m, int wdi) ELOO_NOEXCEPT {
        return month_weekday{ m, weekday_indexed{ weekday{ static_cast<unsigned int>(wdi) }, 1 } };
    }


    // month_weekday_last

    ELOO_NODISCARD constexpr month_weekday_last operator / (const month& m, const weekday_last& wdl) ELOO_NOEXCEPT {
        return month_weekday_last{ m, wdl };
    }
    ELOO_NODISCARD constexpr month_weekday_last operator / (int m, const weekday_last& wdl) ELOO_NOEXCEPT {
        return month{ static_cast<unsigned int>(m) } / wdl;
    }


    // year_month_day

    ELOO_NODISCARD constexpr year_month_day operator / (const year_month& ym, const day& d) ELOO_NOEXCEPT {
        return year_month_day{ ym.year(), ym.month(), d };
    }
    ELOO_NODISCARD constexpr year_month_day operator / (const year_month& ym, int d) ELOO_NOEXCEPT {
        return year_month_day{ ym.year(), ym.month(), day{ static_cast<unsigned int>(d) } };
    }
    ELOO_NODISCARD constexpr year_month_day operator / (const year& y, const month_day& md) ELOO_NOEXCEPT {
        return year_month_day{ y, md.month(), md.day() };
    }
    ELOO_NODISCARD constexpr year_month_day operator / (int y, const month_day& md) ELOO_NOEXCEPT {
        return year_month_day{ year{ static_cast<unsigned int>(y) }, md.month(), md.day() };
    }


    // year_month_day_last

    ELOO_NODISCARD constexpr year_month_day_last operator / (const year_month& ym, last_spec) ELOO_NOEXCEPT {
        return year_month_day_last{ ym.year(), month_day_last{ ym.month() } };
    }
    ELOO_NODISCARD constexpr year_month_day_last operator / (last_spec, const year_month& ym) ELOO_NOEXCEPT {
        return ym / last;
    }
    ELOO_NODISCARD constexpr year_month_day_last operator / (const year& y, const month_day_last& mdl) ELOO_NOEXCEPT {
        return year_month_day_last{ y, mdl };
    }
    ELOO_NODISCARD constexpr year_month_day_last operator / (int y, const month_day_last& mdl) ELOO_NOEXCEPT {
        return year_month_day_last{ year{ static_cast<unsigned int>(y) }, mdl };
    }


    // year_month_weekday

    ELOO_NODISCARD constexpr year_month_weekday operator / (const year_month& ym, const weekday_indexed& wdi) ELOO_NOEXCEPT {
        return year_month_weekday{ ym.year(), ym.month(), wdi };
    }
    ELOO_NODISCARD constexpr year_month_weekday operator / (const year& y, const month_weekday mw) ELOO_NOEXCEPT {
        return year_month_weekday{ y, mw.month(), mw.weekday_indexed() };
    }
    ELOO_NODISCARD constexpr year_month_weekday operator / (int y, const month_weekday mw) ELOO_NOEXCEPT {
        return year_month_weekday{ year{ static_cast<unsigned int>(y) }, mw.month(), mw.weekday_indexed() };
    }


    // year_month_weekday_last

    ELOO_NODISCARD constexpr year_month_weekday_last operator / (const year_month& ym, const weekday_last& wdl) ELOO_NOEXCEPT {
        return year_month_weekday_last{ ym.year(), ym.month(), wdl };
    }
    ELOO_NODISCARD constexpr year_month_weekday_last operator / (const year& y, const month_weekday_last mwl) ELOO_NOEXCEPT {
        return year_month_weekday_last{ y, mwl.month(), mwl.weekday_last() };
    }
    ELOO_NODISCARD constexpr year_month_weekday_last operator / (int y, const month_weekday_last mwl) ELOO_NOEXCEPT {
        return year_month_weekday_last{ year{ static_cast<unsigned int>(y) }, mwl.month(), mwl.weekday_last() };
    }


    ///////////////////////////////////////////////////////////////////////////////
    // calendrical constants
    ///////////////////////////////////////////////////////////////////////////////

    inline constexpr weekday Sunday{ 0 };
    inline constexpr weekday Monday{ 1 };
    inline constexpr weekday Tuesday{ 2 };
    inline constexpr weekday Wednesday{ 3 };
    inline constexpr weekday Thursday{ 4 };
    inline constexpr weekday Friday{ 5 };
    inline constexpr weekday Saturday{ 6 };

    inline constexpr month January{ 1 };
    inline constexpr month February{ 2 };
    inline constexpr month March{ 3 };
    inline constexpr month April{ 4 };
    inline constexpr month May{ 5 };
    inline constexpr month June{ 6 };
    inline constexpr month July{ 7 };
    inline constexpr month August{ 8 };
    inline constexpr month September{ 9 };
    inline constexpr month October{ 10 };
    inline constexpr month November{ 11 };
    inline constexpr month December{ 12 };
#else
namespace eloo::helper::chrono {
#endif // defined(ELOO_USE_EXTRA_CHRONO_TYPES)

    ///////////////////////////////////////////////////////////////////////////////
    // convert any std time_point to eastl time_point
    ///////////////////////////////////////////////////////////////////////////////

    namespace sc = ::std::chrono;
    namespace ec = ::eastl::chrono;

    template <class _STDTimePoint, class _EASTLDuration>
    ELOO_FORCE_INLINE ELOO_NODISCARD constexpr ec::time_point<ec::system_clock, _EASTLDuration> std_to_eastl(const _STDTimePoint& stdtp) ELOO_NOEXCEPT {
        // Get nanoseconds in std::chrono form
        const auto sinceEpoch = sc::clock_cast<sc::system_clock>(stdtp).time_since_epoch();
        const auto scns = sc::duration_cast<sc::nanoseconds>(sinceEpoch).count();

        // All eastl::chrono from here
        const auto ecns = ec::nanoseconds(static_cast<long long>(stdns));
        if constexpr (eastl::is_same_v<_EASTLDuration, ec::nanoseconds>) {
            return ec::time_point<ec::system_clock, ec::nanoseconds>(ecns);
        } else {
            return ec::time_point<ec::system_clock, _EASTLDuration>(ec::duration_cast<ec::nanoseconds, _EASTLDuration>(ecns));
        }
    }


    //////////////////////////////////////////////////////////////////////////////
    // now as string
    //////////////////////////////////////////////////////////////////////////////

    ELOO_FORCE_INLINE ELOO_NODISCARD ::eastl::string now_as_string() {
        const auto now = sc::system_clock::now();
        const ::std::time_t t = sc::system_clock::to_time_t(now);
        char buf[64];
        ::std::strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", ::std::localtime(&t));
        return ::eastl::string(buf);
}