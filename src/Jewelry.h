#ifndef JEWELRY_H_INCLUDED
#define JEWELRY_H_INCLUDED


#include "Boolean.h"


struct Dice;


enum JewelryForm {
  AnkletJewelryForm = 0,
  ArmBandJewelryForm,
  BeltJewelryForm,
  BoxJewelryForm,
  BraceletJewelryForm,
  BroochJewelryForm,
  BuckleJewelryForm,
  ChainJewelryForm,
  ChaliceJewelryForm,
  ChokerJewelryForm,
  ClaspJewelryForm,
  CofferJewelryForm,
  CollarJewelryForm,
  CombJewelryForm,
  CoronetJewelryForm,
  CrownJewelryForm,
  DecanterJewelryForm,
  DiademJewelryForm,
  EarringJewelryForm,
  FobJewelryForm,
  GobletJewelryForm,
  HeadbandJewelryForm,
  IdolJewelryForm,
  LocketJewelryForm,
  MedalJewelryForm,
  MedallionJewelryForm,
  NecklaceJewelryForm,
  PendantJewelryForm,
  PinJewelryForm,
  OrbJewelryForm,
  RingJewelryForm,
  SceptreJewelryForm,
  SealJewelryForm,
  StatuetteJewelryForm,
  TiaraJewelryForm
};


enum JewelryMaterial {
  FakeJewelryMaterial = 0,
  IvoryJewelryMaterial,
  SilverJewelryMaterial,
  SilverAndGoldJewelryMaterial,
  GoldJewelryMaterial,
  JadeJewelryMaterial,
  CoralJewelryMaterial,
  PlatinumJewelryMaterial,
  SilverWithGemsJewelryMaterial,
  GoldWithGemsJewelryMaterial,
  PlatinumWithGemsJewelryMaterial
};


struct Jewelry {
  Boolean hasGems;
  enum JewelryForm form;
  enum JewelryMaterial material;
  int workmanshipBonus;
  int exceptionalStoneBonus;
  int value_cp;
  char *trueDescription;
};


void finalizeJewelry(struct Jewelry *jewelry);

void generateJewelry(struct Jewelry *jewelry, struct Dice *dice);

void initializeJewelry(struct Jewelry *jewelry);

int jewelryValue_cp(struct Jewelry *jewelry);


#endif /* #ifndef JEWELRY_H_INCLUDED */
