#include "streaming/streaming.h"
#include "filesystem/filestream.h"
#include "utility/observer.h"

#include "datatypes/float2.h"
#include "logging.h"
#include "memory.h"

#include <fstream>
#include <stdexcept>
#include <source_location>
#include <sstream>
#include <type_traits>
#include <utility>
#include <string>
#include <memory>

void check_condition(bool ok, const char* expression,
    std::source_location location = std::source_location::current()) {
    if (!ok) {
        throw std::runtime_error(std::string(location.file_name()) + ":" +
            std::to_string(location.line()) + ": Failed: " + expression);
    }
}

#define ensure(expression) check_condition(bool(expression), #expression)

template <typename T>
std::string describe(const T& value) {
    if constexpr (std::is_same_v<T, std::nullptr_t>) {
        return "nullptr";
    } else if constexpr (std::is_pointer_v<T> &&
        !std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, char>) {
        std::ostringstream text;
        text << static_cast<const void*>(value);
        return text.str();
    } else if constexpr (requires(std::ostream& out) { out << value; }) {
        std::ostringstream text;
        text << value;
        return text.str();
    } else {
        return "<unprintable value>";
    }
}

// Separate types allow comparisons such as size_t/int, pointer/nullptr,
// and std::string/string literal without changing the call sites.
template <typename Actual, typename Expected>
void expect(const Actual& a, const Expected& b,
    std::source_location location = std::source_location::current()) {
    if (!(a == b)) {
        throw std::runtime_error(std::string(location.file_name()) + ":" +
            std::to_string(location.line()) + ": Failed: Expected " +
            describe(b) + " but got " + describe(a));
    }
}

template <typename Exception, typename Function>
void expect_throws(Function&& function,
    std::source_location location = std::source_location::current()) {
    try {
        std::forward<Function>(function)();
    } catch (const Exception&) {
        return;
    } catch (...) {
        check_condition(false, "expected exception type; got a different exception", location);
    }
    check_condition(false, "expected exception; nothing was thrown", location);
}

struct copy_failure_value {
    int value;
    bool failOnCopy = false;

    explicit copy_failure_value(int number, bool fail = false)
        : value(number), failOnCopy(fail) {}

    copy_failure_value(const copy_failure_value &other)
        : value(other.value), failOnCopy(other.failOnCopy) {
        if (failOnCopy) {
            throw std::runtime_error("Deliberate copy failure");
        }
    }

    copy_failure_value(copy_failure_value &&) noexcept = default;
    copy_failure_value &operator=(copy_failure_value &&) noexcept = default;
};


// CTest runs this executable. Helpers stop at the first failure; main prints
// the diagnostic and returns a failing exit code.

int run_checks() {
    using namespace sge;

    // Check failure diagnostics without deliberately failing the test run.
    expect_throws<std::runtime_error>([] { ensure(false); });
    expect_throws<std::runtime_error>([] { expect(1, 2); });

    /* ID test */
    {
        std::fputs("id test\n", stderr);

        dense_storage<int> storageA;
        dense_storage<int> storageB;
        const auto invalidHandle = dense_storage<int>::INVALID_HANDLE;

        auto idA1 = storageA.create(42);
        ensure(storageA.is_valid(idA1));
        ensure(!storageA.is_valid(invalidHandle));
        expect(storageA.try_get(invalidHandle), nullptr);

        const int *value = storageA.try_get(idA1);
        ensure(value != nullptr);
        expect(*value, 42);

        auto idA2 = storageA.create(7);
        ensure(idA1 != idA2);

        auto idB1 = storageB.create(29);
        ensure(!storageB.is_valid(idA1));
        ensure(idA1 != idB1);
        expect(storageA.try_get(idB1), nullptr);

        const auto &readOnlyStorage = storageA;
        ensure(readOnlyStorage.try_get(idA1) != nullptr);
    }
    {
        dense_storage<int> storage;
        const auto invalidHandle = dense_storage<int>::INVALID_HANDLE;

        auto a = storage.create(42);
        auto b = storage.create(7);
        auto c = storage.create(99);

        ensure(storage.remove(b));
        expect(storage.size(), 2);

        ensure(!storage.is_valid(b));
        expect(storage.try_get(b), nullptr);
        ensure(!storage.remove(b)); // Reject double deletion

        auto d = storage.create(123);
        expect(storage.size(), 3);
        ensure(d != b);
        ensure(!storage.is_valid(b));
        expect(storage.try_get(b), nullptr);

        const int *newValue = storage.try_get(d);
        ensure(newValue != nullptr);
        expect(*newValue, 123);

        const int *movedValue = storage.try_get(c);
        ensure(movedValue != nullptr);
        expect(*movedValue, 99);  // Same handle, new location

        ensure(storage.is_valid(a));
        ensure(!storage.remove(invalidHandle));

        ensure(storage.remove(d));
        ensure(storage.remove(c)); // Delete the last element
        ensure(storage.remove(a)); // Delete the only element
        expect(storage.size(), 0);
    }
    {
        dense_storage<int> storage;

        auto a = storage.create(10);
        auto b = storage.create(20);
        auto c = storage.create(30);

        ensure(storage.remove(b));

        for (int &value : storage.values()) {
            value += 5;
        }

        expect(storage.values().size(), 2);

        const int *valueA = storage.try_get(a);
        const int *valueC = storage.try_get(c);

        ensure(valueA != nullptr);
        ensure(valueC != nullptr);

        expect(*valueA, 15);
        expect(*valueC, 35);
        ensure(!storage.is_valid(b));
    }
    {
        dense_storage<int> storage;

        auto a = storage.create(10);
        auto b = storage.create(20);
        auto c = storage.create(30);

        ensure(storage.remove(b));
        ensure(storage.remove(a));

        // Two free slots now exist.
        auto d = storage.create(40);
        auto e = storage.create(50);

        ensure(!storage.is_valid(a));
        ensure(!storage.is_valid(b));

        expect(storage.size(), 3);

        const int *valueC = storage.try_get(c);
        const int *valueD = storage.try_get(d);
        const int *valueE = storage.try_get(e);

        ensure(valueC != nullptr);
        ensure(valueD != nullptr);
        ensure(valueE != nullptr);

        expect(*valueC, 30);
        expect(*valueD, 40);
        expect(*valueE, 50);

        // Empty the storage, then use it again.
        ensure(storage.remove(c));
        ensure(storage.remove(d));
        ensure(storage.remove(e));
        expect(storage.size(), 0);

        auto f = storage.create(60);

        ensure(!storage.is_valid(c));
        ensure(!storage.is_valid(d));
        ensure(!storage.is_valid(e));

        const int *valueF = storage.try_get(f);
        ensure(valueF != nullptr);
        expect(*valueF, 60);
        expect(storage.values().size(), 1);
    }
    {
        dense_storage<std::string> storage;

        // Long strings exercise allocated character storage.
        const std::string original(256, 'A');
        const std::string survivor(256, 'B');

        auto a = storage.create(original);
        auto b = storage.create(survivor);

        ensure(storage.remove(a)); // Moves B into A's position.

        ensure(!storage.is_valid(a));

        const std::string *moved = storage.try_get(b);
        ensure(moved != nullptr);
        expect(*moved, survivor);

        auto c = storage.create(std::string(256, 'C'));

        ensure(c != a);
        ensure(!storage.is_valid(a));

        for (std::string &value : storage.values()) {
            value += "!";
        }

        // Retrieve again after creation could have reallocated storage.
        const std::string *updatedB = storage.try_get(b);
        const std::string *updatedC = storage.try_get(c);

        ensure(updatedB != nullptr);
        ensure(updatedC != nullptr);

        expect(*updatedB, survivor + "!");
        expect(*updatedC, std::string(256, 'C') + "!");
    }
    {
        dense_storage<copy_failure_value> storage;

        auto a = storage.create(copy_failure_value{ 10 });
        const copy_failure_value failing{ 99, true };

        // Failure while allocating a new slot.
        expect_throws<std::runtime_error>([&] {
            storage.create(failing);
        });

        expect(storage.size(), 1);

        const auto *valueA = storage.try_get(a);
        ensure(valueA != nullptr);
        expect(valueA->value, 10);

        auto b = storage.create(copy_failure_value{ 20 });
        ensure(storage.remove(a));

        // Failure while reusing a free slot.
        expect_throws<std::runtime_error>([&] {
            storage.create(failing);
        });

        expect(storage.size(), 1);
        ensure(!storage.is_valid(a));

        // The failed attempt must leave the free slot usable.
        auto c = storage.create(copy_failure_value{ 30 });

        const auto *valueB = storage.try_get(b);
        const auto *valueC = storage.try_get(c);

        ensure(valueB != nullptr);
        ensure(valueC != nullptr);

        expect(valueB->value, 20);
        expect(valueC->value, 30);
        expect(storage.size(), 2);
        ensure(!storage.is_valid(a));
    }
    {
        std::weak_ptr<int> lifetime;

        {
            dense_storage<std::shared_ptr<int>> storage;

            auto resource = std::make_shared<int>(42);
            lifetime = resource;

            auto handle = storage.create(resource);
            resource.reset(); // Storage now holds the only strong reference.

            ensure(!lifetime.expired());

            ensure(storage.remove(handle));

            // Removing the value must release its resource.
            ensure(lifetime.expired());
        }

        // Also check cleanup when the storage itself is destroyed.
        {
            dense_storage<std::shared_ptr<int>> storage;

            auto resource = std::make_shared<int>(99);
            lifetime = resource;

            storage.create(resource);
            resource.reset();

            ensure(!lifetime.expired());
        }

        ensure(lifetime.expired());
    }
    {
        dense_storage<float2::values> storage;

        auto a = storage.create(float2::values{ 1.0f, 2.0f });
        auto b = storage.create(float2::values{ 3.0f, 4.0f });

        ensure(storage.remove(a));

        const auto *value = storage.try_get(b);
        ensure(value != nullptr);

        expect(value->x(), 3.0f);
        expect(value->y(), 4.0f);
    }
    {
        auto a = float2::create(1.0f, 2.0f);
        auto b = float2::create(3.0f, 4.0f);

        ensure(float2::release(a));

        float x = 0.0f;
        float y = 0.0f;

        ensure(float2::try_get_values(b, x, y));
        expect(x, 3.0f);
        expect(y, 4.0f);

        auto c = float2::create(5.0f, 6.0f);

        ensure(!float2::is_valid(a));
        ensure(!float2::try_get_values(a, x, y));

        expect_throws<std::out_of_range>([&] {
            float2::x(a);
        });

        float2::x(c) = 7.0f;
        expect(float2::const_x(c), 7.0f);
        expect(float2::const_y(c), 6.0f);

        ensure(float2::release(b));
        ensure(float2::release(c));
    }
    {
        struct moving_body {
            float2::values position;
            float2::values velocity;
        };

        dense_storage<moving_body> bodies;

        auto a = bodies.create({
            {10.0f, 0.0f}, // Position
            { 2.0f, 0.0f}  // Velocity
            });

        auto b = bodies.create({
            {0.0f, 10.0f},
            {0.0f, -4.0f}
            });

        auto c = bodies.create({
            {100.0f, 100.0f},
            { 10.0f,  10.0f}
            });

        ensure(bodies.remove(c));

        const float dt = 0.5f;

        for (auto &body : bodies.values()) {
            body.position.x() += body.velocity.x() * dt;
            body.position.y() += body.velocity.y() * dt;
        }

        const auto *bodyA = bodies.try_get(a);
        const auto *bodyB = bodies.try_get(b);

        ensure(bodyA != nullptr);
        ensure(bodyB != nullptr);

        expect(bodyA->position.x(), 11.0f);
        expect(bodyA->position.y(), 0.0f);

        expect(bodyB->position.x(), 0.0f);
        expect(bodyB->position.y(), 8.0f);

        expect(bodies.size(), 2);
        ensure(!bodies.is_valid(c));
    }

    /* Stream test */
    std::fputs("stream test\n", stderr);

    streaming::iostream stream(0);
    ensure(bool(stream << uint32_t{42}));

    uint32_t value = 0;
    ensure(bool(stream >> value));
    expect(value, 42);

    /* Observer test */
    std::fputs("observer test\n", stderr);

    observer<int> event;
    int observed = 0;
    auto subscription = event.subscribe([&](int v) { observed = v; });
    event.broadcast(7);
    expect(observed, 7);

    event.unsubscribe(subscription);
    event.broadcast(9);
    expect(observed, 7);

    /* File write test */
    {
        std::fputs("file test\n", stderr);
        filesystem::ofstream file("regression-output.tmp");
        ensure(bool(file.write("hello", 5)));
        ensure(bool(file.flush()));
    }

    /* File read test */
    {
        std::ifstream file("regression-output.tmp", std::ios::binary);
        std::string text;
        std::getline(file, text);
        expect(text, "hello");
        filesystem::ifstream input("regression-output.tmp");
    }
    std::filesystem::remove("regression-output.tmp");

    /* Logging test */
    std::fputs("logger test\n", stderr);
    logging::logger log("regression");
    log.log_info("value {}", 42);

    return 0;
}

int main() {
    try {
        return run_checks();
    } catch (const std::exception& e) {
        std::fprintf(stderr, "%s\n", e.what());
        return 1;
    }
}
