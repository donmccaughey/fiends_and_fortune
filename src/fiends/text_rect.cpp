#include "text_rect.hpp"

extern "C" {
#include "base/base.h"
}


TextRect::TextRect() = default;


TextRect::TextRect(const char *s)
{
    appendLines(s);
}


TextRect::TextRect(vector<string> const &strings)
{
    for (auto const& s : strings) {
        appendLines(s.c_str());
    }
}


TextRect::TextRect(const struct ptr_array *array) {
    for (int i = 0; i < array->count; ++i) {
        lines.emplace_back((char *)array->elements[i]);
        lineLength = max(int(lines[i].length()), lineLength);
    }
}


void
TextRect::appendLine(const char *source, int begin, int end)
{
    auto start = source + begin;
    auto length = end - begin;
    lines.emplace_back(start, length);
    lineLength = max(length, lineLength);
}


void
TextRect::appendLines(const char *text)
{
    int begin = 0;
    int end = 0;
    while (text && text[end]) {
        if ('\n' == text[end]) {
            appendLine(text, begin, end);
            begin = end + 1;
        }
        ++end;
    }
    if (begin < end) {
        appendLine(text, begin, end);
    }
}
