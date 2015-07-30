#include "spell.h"

#include <assert.h>
#include <stddef.h>

#include "common/dice.h"
#include "common/fail.h"
#include "common/rnd.h"


char const *
spell_determine(struct rnd *rnd,
                enum spell_type spell_type,
                int spell_level)
{
    assert(spell_level >= 1);
    assert(spell_level <= (spell_type == spell_type_magic_user ? 9 : 7));
    
    struct {
        enum spell_type type;
        int level;
        char const *name;
    } const spells_table[] = {
        {spell_type_clerical, 1, "bless" },
        {spell_type_clerical, 1, "command" },
        {spell_type_clerical, 1, "create water" },
        {spell_type_clerical, 1, "cure light wounds" },
        {spell_type_clerical, 1, "detect evil" },
        {spell_type_clerical, 1, "detect magic" },
        {spell_type_clerical, 1, "light" },
        {spell_type_clerical, 1, "protection from evil" },
        {spell_type_clerical, 1, "purify food & drink" },
        {spell_type_clerical, 1, "remove fear" },
        {spell_type_clerical, 1, "resist cold" },
        {spell_type_clerical, 1, "sanctuary" },
        
        {spell_type_clerical, 2, "augury" },
        {spell_type_clerical, 2, "chant" },
        {spell_type_clerical, 2, "detect charm" },
        {spell_type_clerical, 2, "find traps" },
        {spell_type_clerical, 2, "hold person" },
        {spell_type_clerical, 2, "know alignment" },
        {spell_type_clerical, 2, "resist fire" },
        {spell_type_clerical, 2, "silence 15' radius" },
        {spell_type_clerical, 2, "slow poison" },
        {spell_type_clerical, 2, "snake charm" },
        {spell_type_clerical, 2, "speak with animals" },
        {spell_type_clerical, 2, "spiritual hammer" },
        
        {spell_type_clerical, 3, "animate dead" },
        {spell_type_clerical, 3, "continual light" },
        {spell_type_clerical, 3, "create food & water" },
        {spell_type_clerical, 3, "cure blindness" },
        {spell_type_clerical, 3, "cure disease" },
        {spell_type_clerical, 3, "dispel magic" },
        {spell_type_clerical, 3, "feign death" },
        {spell_type_clerical, 3, "glyph of warding" },
        {spell_type_clerical, 3, "locate object" },
        {spell_type_clerical, 3, "prayer" },
        {spell_type_clerical, 3, "remove curse" },
        {spell_type_clerical, 3, "speak with dead" },
        
        {spell_type_clerical, 4, "cure serious wounds" },
        {spell_type_clerical, 4, "detect lie" },
        {spell_type_clerical, 4, "divination" },
        {spell_type_clerical, 4, "exorcise" },
        {spell_type_clerical, 4, "lower water" },
        {spell_type_clerical, 4, "neutralize poison" },
        {spell_type_clerical, 4, "protection from evil 10' radius" },
        {spell_type_clerical, 4, "speak with plants" },
        {spell_type_clerical, 4, "sticks to snakes" },
        {spell_type_clerical, 4, "tongues" },
        
        {spell_type_clerical, 5, "atonement" },
        {spell_type_clerical, 5, "commune" },
        {spell_type_clerical, 5, "cure critical wounds" },
        {spell_type_clerical, 5, "dispel evil" },
        {spell_type_clerical, 5, "flame strike" },
        {spell_type_clerical, 5, "insect plague" },
        {spell_type_clerical, 5, "plane shift" },
        {spell_type_clerical, 5, "quest" },
        {spell_type_clerical, 5, "raise dead" },
        {spell_type_clerical, 5, "true seeing" },
        
        {spell_type_clerical, 6, "aerial servant" },
        {spell_type_clerical, 6, "animate object" },
        {spell_type_clerical, 6, "blade barrier" },
        {spell_type_clerical, 6, "conjure animals" },
        {spell_type_clerical, 6, "find the path" },
        {spell_type_clerical, 6, "heal" },
        {spell_type_clerical, 6, "part water" },
        {spell_type_clerical, 6, "speak with monsters" },
        {spell_type_clerical, 6, "stone tell" },
        {spell_type_clerical, 6, "word of recall" },
        
        {spell_type_clerical, 7, "astral spell" },
        {spell_type_clerical, 7, "control weather" },
        {spell_type_clerical, 7, "earthquake" },
        {spell_type_clerical, 7, "gate" },
        {spell_type_clerical, 7, "holy (unholy) word" },
        {spell_type_clerical, 7, "regenerate" },
        {spell_type_clerical, 7, "restoration" },
        {spell_type_clerical, 7, "resurrection" },
        {spell_type_clerical, 7, "symbol" },
        {spell_type_clerical, 7, "wind walk" },
        
        {spell_type_drudical, 1, "animal friendship" },
        {spell_type_drudical, 1, "detect magic" },
        {spell_type_drudical, 1, "detect snares & pits" },
        {spell_type_drudical, 1, "entangle" },
        {spell_type_drudical, 1, "faerie fire" },
        {spell_type_drudical, 1, "invisibility to animals" },
        {spell_type_drudical, 1, "locate animals" },
        {spell_type_drudical, 1, "pass without trace" },
        {spell_type_drudical, 1, "predict weather" },
        {spell_type_drudical, 1, "purify water" },
        {spell_type_drudical, 1, "shillelagh" },
        {spell_type_drudical, 1, "speak with animals" },
        
        {spell_type_drudical, 2, "barkskin" },
        {spell_type_drudical, 2, "charm person or mammal" },
        {spell_type_drudical, 2, "create water" },
        {spell_type_drudical, 2, "cure light wounds" },
        {spell_type_drudical, 2, "feign death" },
        {spell_type_drudical, 2, "fire trap" },
        {spell_type_drudical, 2, "heat metal" },
        {spell_type_drudical, 2, "locate plants" },
        {spell_type_drudical, 2, "obscurement" },
        {spell_type_drudical, 2, "produce flame" },
        {spell_type_drudical, 2, "trip" },
        {spell_type_drudical, 2, "warp wood" },
        
        {spell_type_drudical, 3, "call lightning" },
        {spell_type_drudical, 3, "cure disease" },
        {spell_type_drudical, 3, "hold animal" },
        {spell_type_drudical, 3, "neutralize poison" },
        {spell_type_drudical, 3, "plant growth" },
        {spell_type_drudical, 3, "protection from fire" },
        {spell_type_drudical, 3, "pyrotechnics" },
        {spell_type_drudical, 3, "snare" },
        {spell_type_drudical, 3, "stone shape" },
        {spell_type_drudical, 3, "summon insects" },
        {spell_type_drudical, 3, "tree" },
        {spell_type_drudical, 3, "water breathing" },
        
        {spell_type_drudical, 4, "animal summoning I" },
        {spell_type_drudical, 4, "call woodland beings" },
        {spell_type_drudical, 4, "control temperature 10' radius" },
        {spell_type_drudical, 4, "cure serious wounds" },
        {spell_type_drudical, 4, "dispel magic" },
        {spell_type_drudical, 4, "hallucinatory forest" },
        {spell_type_drudical, 4, "hold plant" },
        {spell_type_drudical, 4, "plant door" },
        {spell_type_drudical, 4, "produce fire" },
        {spell_type_drudical, 4, "protection from lightning" },
        {spell_type_drudical, 4, "repel insects" },
        {spell_type_drudical, 4, "speak with plants" },
        
        {spell_type_drudical, 5, "animal growth" },
        {spell_type_drudical, 5, "animal summoning II" },
        {spell_type_drudical, 5, "anti-plant shell" },
        {spell_type_drudical, 5, "commune with nature" },
        {spell_type_drudical, 5, "control winds" },
        {spell_type_drudical, 5, "insect plague" },
        {spell_type_drudical, 5, "pass plant" },
        {spell_type_drudical, 5, "sticks to snakes" },
        {spell_type_drudical, 5, "transmute rock to mud" },
        {spell_type_drudical, 5, "wall of fire" },
        
        {spell_type_drudical, 6, "animal summoning III" },
        {spell_type_drudical, 6, "anti-animal shell" },
        {spell_type_drudical, 6, "conjure fire elemental" },
        {spell_type_drudical, 6, "cure critical wounds" },
        {spell_type_drudical, 6, "feeblemind" },
        {spell_type_drudical, 6, "fire seeds" },
        {spell_type_drudical, 6, "transport via plants" },
        {spell_type_drudical, 6, "turn wood" },
        {spell_type_drudical, 6, "wall of thorns" },
        {spell_type_drudical, 6, "weather summoning" },
        
        {spell_type_drudical, 7, "animate rock" },
        {spell_type_drudical, 7, "chariot of Sustarre" },
        {spell_type_drudical, 7, "confusion" },
        {spell_type_drudical, 7, "conjure earth elemental" },
        {spell_type_drudical, 7, "control weather" },
        {spell_type_drudical, 7, "creeping doom" },
        {spell_type_drudical, 7, "finger of death" },
        {spell_type_drudical, 7, "fire storm" },
        {spell_type_drudical, 7, "reincarnate" },
        {spell_type_drudical, 7, "transmute metal to wood" },
        
        {spell_type_magic_user, 1, "affect normal fires" },
        {spell_type_magic_user, 1, "burning hands" },
        {spell_type_magic_user, 1, "charm person" },
        {spell_type_magic_user, 1, "comprehend languages" },
        {spell_type_magic_user, 1, "dancing lights" },
        {spell_type_magic_user, 1, "detect magic" },
        {spell_type_magic_user, 1, "enlarge" },
        {spell_type_magic_user, 1, "erase" },
        {spell_type_magic_user, 1, "feather fall" },
        {spell_type_magic_user, 1, "find familiar" },
        {spell_type_magic_user, 1, "friends" },
        {spell_type_magic_user, 1, "hold portal" },
        {spell_type_magic_user, 1, "identify" },
        {spell_type_magic_user, 1, "jump" },
        {spell_type_magic_user, 1, "light" },
        {spell_type_magic_user, 1, "magic missile" },
        {spell_type_magic_user, 1, "mending" },
        {spell_type_magic_user, 1, "message" },
        {spell_type_magic_user, 1, "Nystul's magic aura" },
        {spell_type_magic_user, 1, "protection from evil" },
        {spell_type_magic_user, 1, "push" },
        {spell_type_magic_user, 1, "read magic" },
        {spell_type_magic_user, 1, "shield" },
        {spell_type_magic_user, 1, "shocking grasp" },
        {spell_type_magic_user, 1, "sleep" },
        {spell_type_magic_user, 1, "spider climb" },
        {spell_type_magic_user, 1, "Tenser's floating disc" },
        {spell_type_magic_user, 1, "unseen servant" },
        {spell_type_magic_user, 1, "ventriloquism" },
        {spell_type_magic_user, 1, "write" },
        
        {spell_type_magic_user, 2, "audible glamer" },
        {spell_type_magic_user, 2, "continual light" },
        {spell_type_magic_user, 2, "darkness 15' radius" },
        {spell_type_magic_user, 2, "detect evil" },
        {spell_type_magic_user, 2, "detect invisibility" },
        {spell_type_magic_user, 2, "ESP" },
        {spell_type_magic_user, 2, "fools gold" },
        {spell_type_magic_user, 2, "forget" },
        {spell_type_magic_user, 2, "invisibility" },
        {spell_type_magic_user, 2, "knock" },
        {spell_type_magic_user, 2, "Leomund's trap" },
        {spell_type_magic_user, 2, "levitate" },
        {spell_type_magic_user, 2, "locate object" },
        {spell_type_magic_user, 2, "magic mouth" },
        {spell_type_magic_user, 2, "mirror image" },
        {spell_type_magic_user, 2, "pyrotechnics" },
        {spell_type_magic_user, 2, "ray of enfeeblement" },
        {spell_type_magic_user, 2, "rope trick" },
        {spell_type_magic_user, 2, "scare" },
        {spell_type_magic_user, 2, "shatter" },
        {spell_type_magic_user, 2, "stinking cloud" },
        {spell_type_magic_user, 2, "strength" },
        {spell_type_magic_user, 2, "web" },
        {spell_type_magic_user, 2, "wizard lock" },
        
        {spell_type_magic_user, 3, "blink" },
        {spell_type_magic_user, 3, "clairaudience" },
        {spell_type_magic_user, 3, "clairvoyance" },
        {spell_type_magic_user, 3, "dispel magic" },
        {spell_type_magic_user, 3, "explosive runes" },
        {spell_type_magic_user, 3, "feign death" },
        {spell_type_magic_user, 3, "fireball" },
        {spell_type_magic_user, 3, "flame arrow" },
        {spell_type_magic_user, 3, "fly" },
        {spell_type_magic_user, 3, "gust of wind" },
        {spell_type_magic_user, 3, "haste" },
        {spell_type_magic_user, 3, "hold person" },
        {spell_type_magic_user, 3, "infravision" },
        {spell_type_magic_user, 3, "invisibility 10' radius" },
        {spell_type_magic_user, 3, "Leomund's tiny hut" },
        {spell_type_magic_user, 3, "lightning bolt" },
        {spell_type_magic_user, 3, "monster summoning I" },
        {spell_type_magic_user, 3, "phantasmal force" },
        {spell_type_magic_user, 3, "protection from evil 10' radius" },
        {spell_type_magic_user, 3, "protection from normal missiles" },
        {spell_type_magic_user, 3, "slow" },
        {spell_type_magic_user, 3, "suggestion" },
        {spell_type_magic_user, 3, "tongues" },
        {spell_type_magic_user, 3, "water breathing" },
        
        {spell_type_magic_user, 4, "charm monster" },
        {spell_type_magic_user, 4, "confusion" },
        {spell_type_magic_user, 4, "dig" },
        {spell_type_magic_user, 4, "dimension door" },
        {spell_type_magic_user, 4, "enchanted weapon" },
        {spell_type_magic_user, 4, "extension I" },
        {spell_type_magic_user, 4, "fear" },
        {spell_type_magic_user, 4, "fire charm" },
        {spell_type_magic_user, 4, "fire shield" },
        {spell_type_magic_user, 4, "fire trap" },
        {spell_type_magic_user, 4, "fumble" },
        {spell_type_magic_user, 4, "hallucinatory terrain" },
        {spell_type_magic_user, 4, "ice storm" },
        {spell_type_magic_user, 4, "massmorph" },
        {spell_type_magic_user, 4, "minor globe of invulnerability" },
        {spell_type_magic_user, 4, "monster summoning II" },
        {spell_type_magic_user, 4, "plant growth" },
        {spell_type_magic_user, 4, "polymorph other" },
        {spell_type_magic_user, 4, "polymorph self" },
        {spell_type_magic_user, 4, "Rary's mnemonic enhancer" },
        {spell_type_magic_user, 4, "remove curse" },
        {spell_type_magic_user, 4, "wall of fire" },
        {spell_type_magic_user, 4, "wall if ice" },
        {spell_type_magic_user, 4, "wizard eye" },
        
        {spell_type_magic_user, 5, "airy water" },
        {spell_type_magic_user, 5, "animal growth" },
        {spell_type_magic_user, 5, "animate dead" },
        {spell_type_magic_user, 5, "Bigby's interposing hand" },
        {spell_type_magic_user, 5, "cloudkill" },
        {spell_type_magic_user, 5, "conjure elemental" },
        {spell_type_magic_user, 5, "cone of cold" },
        {spell_type_magic_user, 5, "contact other plane" },
        {spell_type_magic_user, 5, "distance distortion" },
        {spell_type_magic_user, 5, "extension II" },
        {spell_type_magic_user, 5, "feeblemind" },
        {spell_type_magic_user, 5, "hold monster" },
        {spell_type_magic_user, 5, "Leomund's secret chest" },
        {spell_type_magic_user, 5, "magic jar" },
        {spell_type_magic_user, 5, "monster summoning III" },
        {spell_type_magic_user, 5, "Mordenkainen's faithful hound" },
        {spell_type_magic_user, 5, "passwall" },
        {spell_type_magic_user, 5, "stone shape" },
        {spell_type_magic_user, 5, "telekinesis" },
        {spell_type_magic_user, 5, "teleport" },
        {spell_type_magic_user, 5, "transmute rock to mud" },
        {spell_type_magic_user, 5, "wall of force" },
        {spell_type_magic_user, 5, "wall of iron" },
        {spell_type_magic_user, 5, "wall of stone" },
        
        {spell_type_magic_user, 6, "anti-magic shell" },
        {spell_type_magic_user, 6, "Bigby's forceful hand" },
        {spell_type_magic_user, 6, "control weather" },
        {spell_type_magic_user, 6, "death spell" },
        {spell_type_magic_user, 6, "disintegrate" },
        {spell_type_magic_user, 6, "enchant an item" },
        {spell_type_magic_user, 6, "extension III" },
        {spell_type_magic_user, 6, "geas" },
        {spell_type_magic_user, 6, "glassee" },
        {spell_type_magic_user, 6, "globe of invulnerability" },
        {spell_type_magic_user, 6, "guards and wards" },
        {spell_type_magic_user, 6, "invisible stalker" },
        {spell_type_magic_user, 6, "legend lore" },
        {spell_type_magic_user, 6, "lower water" },
        {spell_type_magic_user, 6, "monster summoning IV" },
        {spell_type_magic_user, 6, "move earth" },
        {spell_type_magic_user, 6, "Otiluke's freezing sphere" },
        {spell_type_magic_user, 6, "part water" },
        {spell_type_magic_user, 6, "project image" },
        {spell_type_magic_user, 6, "reincarnation" },
        {spell_type_magic_user, 6, "repulsion" },
        {spell_type_magic_user, 6, "spiritwrack" },
        {spell_type_magic_user, 6, "stone to flesh" },
        {spell_type_magic_user, 6, "Tenser's transformation" },
        
        {spell_type_magic_user, 7, "Bigby's grasping hand" },
        {spell_type_magic_user, 7, "cacodemon" },
        {spell_type_magic_user, 7, "charm plants" },
        {spell_type_magic_user, 7, "delayed blast fireball" },
        {spell_type_magic_user, 7, "Drawmij's instant summons" },
        {spell_type_magic_user, 7, "duo-dimension" },
        {spell_type_magic_user, 7, "limited wish" },
        {spell_type_magic_user, 7, "mass invisibility" },
        {spell_type_magic_user, 7, "monster summoning V" },
        {spell_type_magic_user, 7, "Mordenkainen's sword" },
        {spell_type_magic_user, 7, "phase door" },
        {spell_type_magic_user, 7, "power word, stun" },
        {spell_type_magic_user, 7, "reverse gravity" },
        {spell_type_magic_user, 7, "simulacrum" },
        {spell_type_magic_user, 7, "statue" },
        {spell_type_magic_user, 7, "vanish" },
        
        {spell_type_magic_user, 8, "antipathy/sympathy" },
        {spell_type_magic_user, 8, "Bigby's clenched fist" },
        {spell_type_magic_user, 8, "clone" },
        {spell_type_magic_user, 8, "glassteel" },
        {spell_type_magic_user, 8, "incendiary cloud" },
        {spell_type_magic_user, 8, "mass charm" },
        {spell_type_magic_user, 8, "maze" },
        {spell_type_magic_user, 8, "mind blank" },
        {spell_type_magic_user, 8, "monster summoning VI" },
        {spell_type_magic_user, 8, "Otto's irresistible dance" },
        {spell_type_magic_user, 8, "permanency" },
        {spell_type_magic_user, 8, "polymorph any object" },
        {spell_type_magic_user, 8, "power word, blind" },
        {spell_type_magic_user, 8, "Serten's spell immunity" },
        {spell_type_magic_user, 8, "symbol" },
        {spell_type_magic_user, 8, "trap the soul" },
        
        {spell_type_magic_user, 9, "astral spell" },
        {spell_type_magic_user, 9, "Bigby's crushing hand" },
        {spell_type_magic_user, 9, "gate" },
        {spell_type_magic_user, 9, "imprisonment" },
        {spell_type_magic_user, 9, "meteor swarm" },
        {spell_type_magic_user, 9, "monster summoning VII" },
        {spell_type_magic_user, 9, "power word, kill" },
        {spell_type_magic_user, 9, "prismatic sphere" },
        {spell_type_magic_user, 9, "shape change" },
        {spell_type_magic_user, 9, "temporal stasis" },
        {spell_type_magic_user, 9, "time stop" },
        {spell_type_magic_user, 9, "wish" },
        
        {spell_type_illusionist, 1, "audible glamer" },
        {spell_type_illusionist, 1, "change self" },
        {spell_type_illusionist, 1, "color spray" },
        {spell_type_illusionist, 1, "dancing lights" },
        {spell_type_illusionist, 1, "darkness" },
        {spell_type_illusionist, 1, "detect illusion" },
        {spell_type_illusionist, 1, "detect invisibility" },
        {spell_type_illusionist, 1, "gaze reflection" },
        {spell_type_illusionist, 1, "hypnotism" },
        {spell_type_illusionist, 1, "light" },
        {spell_type_illusionist, 1, "phantasmal force" },
        {spell_type_illusionist, 1, "wall of fog" },
        
        {spell_type_illusionist, 2, "blindness" },
        {spell_type_illusionist, 2, "blur" },
        {spell_type_illusionist, 2, "deafness" },
        {spell_type_illusionist, 2, "detect magic" },
        {spell_type_illusionist, 2, "fog cloud" },
        {spell_type_illusionist, 2, "hypnotic pattern" },
        {spell_type_illusionist, 2, "improved phantasmal force" },
        {spell_type_illusionist, 2, "invisibility" },
        {spell_type_illusionist, 2, "magic mouth" },
        {spell_type_illusionist, 2, "mirror image" },
        {spell_type_illusionist, 2, "misdirection" },
        {spell_type_illusionist, 2, "ventriloquism" },
        
        {spell_type_illusionist, 3, "continual darkness" },
        {spell_type_illusionist, 3, "continual light" },
        {spell_type_illusionist, 3, "dispel illusion" },
        {spell_type_illusionist, 3, "fear" },
        {spell_type_illusionist, 3, "hallucinatory terrain" },
        {spell_type_illusionist, 3, "illusionary script" },
        {spell_type_illusionist, 3, "invisibility 10' radius" },
        {spell_type_illusionist, 3, "non-detection" },
        {spell_type_illusionist, 3, "paralyzation" },
        {spell_type_illusionist, 3, "rope trick" },
        {spell_type_illusionist, 3, "spectral force" },
        {spell_type_illusionist, 3, "suggestion" },
        
        {spell_type_illusionist, 4, "confusion" },
        {spell_type_illusionist, 4, "dispel exhaustion" },
        {spell_type_illusionist, 4, "emotion" },
        {spell_type_illusionist, 4, "improved invisibility" },
        {spell_type_illusionist, 4, "massmorph" },
        {spell_type_illusionist, 4, "minor creation" },
        {spell_type_illusionist, 4, "phantasmal killer" },
        {spell_type_illusionist, 4, "shadow monsters" },
        
        {spell_type_illusionist, 5, "chaos" },
        {spell_type_illusionist, 5, "demi-shadow monsters" },
        {spell_type_illusionist, 5, "major creation" },
        {spell_type_illusionist, 5, "maze" },
        {spell_type_illusionist, 5, "projected image" },
        {spell_type_illusionist, 5, "shadow door" },
        {spell_type_illusionist, 5, "shadow magic" },
        {spell_type_illusionist, 5, "summon shadow" },
        
        {spell_type_illusionist, 6, "conjure animals" },
        {spell_type_illusionist, 6, "demi-shadow magic" },
        {spell_type_illusionist, 6, "mass suggestion" },
        {spell_type_illusionist, 6, "permanent illusion" },
        {spell_type_illusionist, 6, "programmed illusion" },
        {spell_type_illusionist, 6, "shades" },
        {spell_type_illusionist, 6, "true sight" },
        {spell_type_illusionist, 6, "veil" },
        
        {spell_type_illusionist, 7, "alter reality" },
        {spell_type_illusionist, 7, "astral spell" },
        {spell_type_illusionist, 7, "prismatic spray" },
        {spell_type_illusionist, 7, "prismatic wall" },
        {spell_type_illusionist, 7, "vision" }
    };
    size_t const spells_table_count = sizeof spells_table
                                    / sizeof spells_table[0];
    
    int total = 0;
    for (int i = 0; i < spells_table_count; ++i) {
        if (   spells_table[i].type == spell_type
            && spells_table[i].level == spell_level)
        {
            ++total;
        }
    }
    assert(total);
    
    int score = dice_roll(dice_make(1, total), rnd, NULL);
    int range = 0;
    for (int i = 0; i < spells_table_count; ++i) {
        if (   spells_table[i].type == spell_type
            && spells_table[i].level == spell_level)
        {
            ++range;
            if (score <= range) return spells_table[i].name;
        }
    }
    fail("Did not match table entry (score=%i, range=%i)", score, range);
    return "(spell)";
}
