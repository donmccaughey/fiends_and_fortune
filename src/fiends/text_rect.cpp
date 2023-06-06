#include "text_rect.hpp"

extern "C" {
#include "base/base.h"
}


static int
appendLine(vector<string> &lines, char const *source, int begin, int end)
{
    char const *start = source + begin;
    int length = end - begin;
    lines.emplace_back(start, length);
    return length;
}


static int
appendLines(vector<string> &lines, const char *text)
{
    int begin = 0;
    int end = 0;
    int maxLength = 0;
    while (text && text[end]) {
        if ('\n' == text[end]) {
            int length = appendLine(lines, text, begin, end);
            maxLength = max(maxLength, length);
            begin = end + 1;
        }
        ++end;
    }
    if (begin < end) {
        int length = appendLine(lines, text, begin, end);
        maxLength = max(maxLength, length);
    }
    return maxLength;
}


static void
padLines(vector<string> &lines, size_t length)
{
    for(string &line : lines) {
        if (line.length() < length) {
            size_t count = length - line.length();
            line.append(count, ' ');
        }
    }
}


TextRect::TextRect() = default;


TextRect::TextRect(vector<string> const &strings)
{
    int maxLength = 0;
    for (string const &s : strings) {
        int length = appendLines(lines, s.c_str());
        maxLength = max(maxLength, length);
    }
    padLines(lines, maxLength);
}


TextRect::TextRect(struct ptr_array const *array) {
    int maxLength = 0;
    for (int i = 0; i < array->count; ++i) {
        lines.emplace_back((char *)array->elements[i]);
        int length = int(lines[i].length());
        maxLength = max(maxLength, length);
    }
    padLines(lines, maxLength);
}
