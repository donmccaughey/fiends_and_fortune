#ifndef FIENDS_TEXT_RECT_HPP_INCLUDED
#define FIENDS_TEXT_RECT_HPP_INCLUDED


#include "fiends.hpp"


struct ptr_array;


class TextRect {
    vector<string> lines;
    int lineLength{};

public:
    TextRect();
    explicit TextRect(char const *s);
    explicit TextRect(vector<string> const &strings);
    explicit TextRect(struct ptr_array const *array);

    int height() const { return int(lines.size()); }
    int width() const { return lineLength; }

    string const &operator[](int i) const { return lines[i]; }

private:
    void appendLine(char const *source, int begin, int end);
    void appendLines(char const *text);
};


#endif
