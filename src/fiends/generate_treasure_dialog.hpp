#ifndef FIENDS_GENERATE_TREASURE_DIALOG_HPP_INCLUDED
#define FIENDS_GENERATE_TREASURE_DIALOG_HPP_INCLUDED


class TDialog;


struct GenerateTreasureDialog {
    char treasureType[2];
};


TDialog *
newGenerateTreasureDialog();


#endif
