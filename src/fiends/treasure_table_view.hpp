#ifndef FIENDS_TREASURE_TABLE_VIEW_HPP_INCLUDED
#define FIENDS_TREASURE_TABLE_VIEW_HPP_INCLUDED


#include "fiends.hpp"


class TreasureTableView : public TView {
    vector<string> treasureTypes;

public:
    explicit TreasureTableView(const TRect &bounds);

    void draw() override;

private:
    void appendLine(char const *source, size_t begin, size_t end);
    void appendLines(char const *source);
    void initializeTable();
};


#endif
