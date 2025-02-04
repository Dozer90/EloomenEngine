#pragma once

namespace eloo {
class IComponent {
public:
    virtual void initilize() {};
    virtual void update(double delta) = 0;


};
}