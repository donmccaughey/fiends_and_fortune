#ifndef FIENDS_TEXT_RECT_HPP_INCLUDED
#define FIENDS_TEXT_RECT_HPP_INCLUDED


#include "fiends.hpp"


struct ptr_array;


class TextRect {
    vector<string> lines;

public:
    TextRect();
    explicit TextRect(vector<string> const &strings);
    explicit TextRect(struct ptr_array const *array);
    TextRect(TextRect &&textRect) noexcept;

    TextRect &operator=(TextRect &&textRect) noexcept;

    int height() const { return int(lines.size()); }
    int width() const { return lines.empty() ? 0 : int(lines[0].length()); }

    string const &operator[](int i) const { return lines[i]; }
};


#endif
