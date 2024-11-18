#ifndef RRC_PS5_CTRL_STATE_HPP_
#define RRC_PS5_CTRL_STATE_HPP_

struct GamepadState
{
    int l_x;
    int l_y;
    int r_x;
    int r_y;
    unsigned l_2_trig;
    unsigned r_2_trig;
    bool l_1;
    bool r_1;
    bool l_2;
    bool r_2;
    bool l_3;
    bool r_3;
    bool triangle;
    bool circle;
    bool cross;
    bool square;
    bool options;
    bool share;
};

#endif // RRC_PS5_CTRL_STATE_HPP_