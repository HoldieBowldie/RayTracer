#ifndef INTERVAL_H
#define INTERVAL_H

class interval {

public:

    float min, max;

    //Default interval is empty
    interval() : min(+infinity), max(-infinity) {}

    interval(float _min, float _max) : min(_min), max(_max) {}

    bool contains(float x) const {
        return min <= x && x <= max;
    }

    bool surrounds(float x) const {
        return min < x && x < max;
    }

    float clamp(float x) const {
        if (x < min) {
            return min;
        }
        if (x > max) {
            return max;
        }
        return x;
    }

    static const interval empty, universe;

};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif