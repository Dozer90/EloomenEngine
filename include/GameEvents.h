#pragma once

namespace sf
{
template <typename T> class Vector2;
}

namespace ENGINE_NAMESPACE
{
using GameObjectID = uint64_t;

class Event
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Size events parameters (Resized)
    ///
    ////////////////////////////////////////////////////////////
    struct CollisionEvent
    {
        GameObjectID firstObjectID;
        GameObjectID secondObjectID;
        sf::Vector2<double> collisionPoint;
        sf::Vector2<double> collisionDirection;
    };

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the different types of events
    ///
    ////////////////////////////////////////////////////////////
    enum EventType
    {
        Collision,              //!< There was a collision between two game objects (data in event.collision)

        Count                   //!< Keep last -- the total number of event types
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    EventType type; //!< Type of the event

    union
    {
        CollisionEvent        collision;         //!< Collision event parameters (Event::CollisionEvent)
    };
};

} // namespace sf


#endif // SFML_EVENT_HPP


  ////////////////////////////////////////////////////////////
  /// \class sf::Event
  /// \ingroup window
  ///
  /// sf::Event holds all the informations about a system event
  /// that just happened. Events are retrieved using the
  /// sf::Window::pollEvent and sf::Window::waitEvent functions.
  ///
  /// A sf::Event instance contains the type of the event
  /// (mouse moved, key pressed, window closed, ...) as well
  /// as the details about this particular event. Please note that
  /// the event parameters are defined in a union, which means that
  /// only the member matching the type of the event will be properly
  /// filled; all other members will have undefined values and must not
  /// be read if the type of the event doesn't match. For example,
  /// if you received a KeyPressed event, then you must read the
  /// event.key member, all other members such as event.mouseMove
  /// or event.text will have undefined values.
  ///
  /// Usage example:
  /// \code
  /// sf::Event event;
  /// while (window.pollEvent(event))
  /// {
  ///     // Request for closing the window
  ///     if (event.type == sf::Event::Closed)
  ///         window.close();
  ///
  ///     // The escape key was pressed
  ///     if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
  ///         window.close();
  ///
  ///     // The window was resized
  ///     if (event.type == sf::Event::Resized)
  ///         doSomethingWithTheNewSize(event.size.width, event.size.height);
  ///
  ///     // etc ...
  /// }
  /// \endcode
  ///
  ////////////////////////////////////////////////////////////
