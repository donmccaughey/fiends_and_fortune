#ifndef GEM_H_INCLUDED
#define GEM_H_INCLUDED


struct Dice;


enum GemSize {
  VerySmallGemSize = -2,
  SmallGemSize,
  AverageGemSize,
  LargeGemSize,
  VeryLargeGemSize,
  HugeGemSize
};


enum GemType {
  FakeGemType = 0,
  OrnamentalStoneGemType,
  SemiPreciousStoneGemType,
  FancyStoneGemType,
  PreciousStoneGemType,
  GemStoneGemType,
  JewelStoneGemType
};


struct Gem {
  enum GemSize size;
  enum GemType type;
  int valuePercentModifier;
  int valueRankModifier;
  char *trueDescription;
};


void finalizeGem(struct Gem *gem);

int gemValue_cp(struct Gem *gem);

void generateGem(struct Gem *gem, struct Dice *dice);

void intializeGem(struct Gem *gem);


#endif /* #ifndef GEM_H_INCLUDED */
