#include "strings.h"

#include <sys/types.h>

#if GAME_VER >= VER_C3_DEMO_NTSC_U
char *StringCompleted = "COMPLETED";
char *StringQuit_Flight = "QUIT";
char *StringTimeIsUp = "TIME IS UP";
char *StringCrashed = "CRASHED";
char *StringAllInOne = "ALL IN ONE";
char *StringTotal = "TOTAL";
char *StringPressButton = "PRESS   BUTTON";
int StringPressButton_ButtonIndex = 6;
char *StringTryAgain = "TRY AGAIN?";
char *StringYes_Flight = "YES";
char *StringNo_Flight = "NO";
char *StringTimeAttack = "TIME ATTACK";
char *StringBestTime = "BEST TIME";
char *StringYourTime = "YOUR TIME";
char *StringNewRecord = "NEW RECORD";
const char StringCopters[] = "COPTERS";
const char StringBoats[] = "BOATS";
const char StringLights[] = "LIGHTS";
const char StringRings[] = "RINGS";
const char StringChests[] = "CHESTS";
const char StringPlanes[] = "PLANES";
const char StringArches[] = "ARCHES";
const char StringBarrels[] = "BARRELS";
char g_GameOverTextRot[8] = {107, 96, 85, 78, 50, 43, 32, 21};
short g_GameOverRotY = 176;
short g_GameOverRotZ = 1024;
short g_GameOverSkyboxRotY = 0x0F50;
short g_GameOverSkyboxRotZ = 0x0884;
u_char g_GameOverLetterDisplayTicks[8] = {0, 8, 16, 24, 72, 80, 88, 96};
char *StringPressStart = "PRESS START";
u_char StringPressStartX = 174;
char *StringInTheWorldOfDragons = "IN THE WORLD OF DRAGONS...";
u_char StringInTheWorldOfDragonsX = 81;
u_char StringInTheWorldOfDragonsRevealTick = 140;
char *StringTheAdventureBegins = "THE ADVENTURE BEGINS...";
u_char StringTheAdventureBeginsX = 102;
u_char StringTheAdventureBeginsRevealTick = 143;
char *StringTheAdventureContinues = "THE ADVENTURE CONTINUES...";
u_char StringTheAdventureContinuesX = 81;
u_char StringTheAdventureContinuesRevealTick = 140;
char *StringEnteringDemoMode = "ENTERING DEMO MODE...";
u_char StringEnteringDemoModeX = 116;
u_char StringEnteringDemoModeRevealTick = 145;
char *StringDemoMode = "DEMO MODE";
u_char StringDemoModeX = 191;
char *StringReturningHome = "RETURNING HOME...";
u_char StringReturningHomeX = 127;
char *StringConfronting = "CONFRONTING";
char *StringEntering_Level = "ENTERING";
char *StringEntering_World = "ENTERING";
char *StringWorld = "WORLD";
char *StringEnteringGnastysWorld = "ENTERING GNASTY'S WORLD";
char *StringTreasureFound = "TREASURE FOUND";
u_char StringTreasureFoundXOffset = 70;
char *StringTotalTreasure = "TOTAL TREASURE";
u_char StringTotalTreasureXOffset = 54;
char *StringPaused = "PAUSED";
u_char StringPausedX = 186;
char *StringContinue_Pause = "CONTINUE";
u_char StringContinue_PauseX = 199;
char *StringOptions = "OPTIONS";
u_char StringOptionsX = 207;
char *StringInventory = "INVENTORY";
u_char StringInventoryX = 191;
char *StringQuit_Pause = "QUIT";
u_char StringQuit_PauseX = 231;
char *StringExitLevel = "EXIT LEVEL";
u_char StringExitLevelX = 183;
char *StringQuitGame = "QUIT GAME";
u_char StringQuitGameX = 191;
char *StringSoundEffects = "SOUND EFFECTS";
u_char StringSoundEffectsX = 112;
char *StringMusicVolume = "MUSIC VOLUME";
u_char StringMusicVolumeX = 126;
char *StringSpeakerSetup = "SPEAKER SETUP";
u_char StringSpeakerSetupX = 112;
char *StringVibration = "VIBRATION";
u_char StringVibrationX = 168;
char *StringCamera = "CAMERA";
u_char StringCameraX = 213;
char *StringScreenAdjust = "SCREEN ADJUST";
u_char StringScreenAdjustX = 168;
char *StringHorizontal = "HORIZONTAL";
u_char StringHorizontalX = 143;
char *StringVertical = "VERTICAL";
u_char StringVerticalX = 173;
char *StringDone = "DONE";
u_char StringDoneX = 234;
char *StringOn = "ON";
char *StringOff = "OFF";
char *StringStereo = "STEREO";
char *StringMono = "MONO";
char *StringActive = "ACTIVE";
char *StringPassive = "PASSIVE";
char *StringYes_Pause = "YES";
u_char StringYes_PauseX = 192;
char *StringNo_Pause = "NO";
u_char StringNo_PauseX = 40;
char *StringHiSpyro = "HI SPYRO,";
char *StringSaveGame = "SAVE GAME";
char *StringReplayDragon = "REPLAY DRAGON";
char *StringContinue_Fairy = "CONTINUE";
char *StringNoSaveFile = "NO SAVE FILE";
u_char StringNoSaveFileXOffset = 62;
char *StringPleaseRestartWith = "PLEASE RESTART WITH";
u_char StringPleaseRestartWithXOffset = 16;
char *StringAMemoryCardTo = "A MEMORY CARD TO";
u_char StringAMemoryCardToXOffset = 37;
char *StringEnableSaveGames = "ENABLE SAVE GAMES";
u_char StringEnableSaveGamesXOffset = 30;
char *StringSaving = "SAVING...";
u_char StringSavingXOffset = 64;
char *StringNoMemoryCard = "NO MEMORY CARD";
u_char StringNoMemoryCardXOffset = 51;
char *StringPleaseReplace = "PLEASE REPLACE";
u_char StringPleaseReplaceXOffset = 49;
char *StringTheSpyro = "THE SPYRO";
u_char StringTheSpyroXOffset = 84;
char *StringMemoryCard = "MEMORY CARD.";
u_char StringMemoryCardXOffset = 63;
char *StringSaveError = "SAVE ERROR";
u_char StringSaveErrorXOffset = 63;
char *StringPleaseCheckThat = "PLEASE CHECK THAT";
u_char StringPleaseCheckThatXOffset = 16;
char *StringTheMemoryCard = "THE MEMORY CARD";
u_char StringTheMemoryCardXOffset = 30;
char *StringIsStillInPlace = "IS STILL IN PLACE";
u_char StringIsStillInPlaceXOffset = 16;
char *StringSaveFailed = "SAVE FAILED";
u_char StringSaveFailedXOffset = 51;
char *StringRetry = "RETRY";
u_char StringRetryXOffset = 92;
char *StringAbort = "ABORT";
u_char StringAbortXOffset = 92;
char *StringSaveSuccessful = "SAVE SUCCESSFUL";
u_char StringSaveSuccessfulXOffset = 23;
char *StringMemoryCard1 = "MEMORY CARD 1";
char *StringMemoryCard2 = "MEMORY CARD 2";
u_char StringMemoryCard2Width = 169;
char *StringTheBalloonist = "THE BALLOONIST";
char *StringWhereToSpyro = "WHERE TO, SPYRO?";
char *StringNeedALift = "NEED A LIFT?";
char *StringAreYouReadyToGo = "ARE YOU READY TO GO?";
char *StringWouldYouLikeToGo = "WOULD YOU LIKE TO GO?";
char *StringGoTo = "GO TO ";
char *StringStayHere = "STAY HERE";
char *StringRescued = "RESCUED";
#endif

/* Dragon names */
char *g_DragonNames[80] = {
    "SILVUS",  /* Index 0 (Unused) */
    "ARGUS",   /* Index 1 */
    "DELBIN",  /* Index 2 */
    "NESTOR",  /* Index 3 */
    "LINDAR",  /* Index 4 */
    "GILDAS",  /* Index 5 */
    "ASTOR",   /* Index 6 */
    "GAVIN",   /* Index 7 */
    "OSWIN",   /* Index 8 */
    "DARIUS",  /* Index 9 */
    "ALBAN",   /* Index 10 */
    "NILS",    /* Index 11 */
    "DEVLIN",  /* Index 12 */
    "TOMAS",   /* Index 13 */
    "ALVAR",   /* Index 14 */
    "NEVIN",   /* Index 15 */
    "MAGNUS",  /* Index 16 */
    "GUNNAR",  /* Index 17 */
    "TITAN",   /* Index 18 */
    "BORIS",   /* Index 19 */
    "MAXIMOS", /* Index 20 */
    "CONAN",   /* Index 21 */
    "IVOR",    /* Index 22 */
    "THOR",    /* Index 23 */
    "MARCO",   /* Index 24 */
    "ENZO",    /* Index 25 */
    "HALVOR",  /* Index 26 */
    "GALE",    /* Index 27 (Unused) */
    "ANDOR",   /* Index 28 */
    "RAGNAR",  /* Index 29 */
    "ULRIC",   /* Index 30 */
    "TODOR",   /* Index 31 */
    "ASHER",   /* Index 32 */
    "TRONDO",  /* Index 33 */
    "COSMOS",  /* Index 34 */
    "ZANTOR",  /* Index 35 */
    "BOLDAR",  /* Index 36 */
    "ZANE",    /* Index 37 */
    "ELDRID",  /* Index 38 */
    "KELVIN",  /* Index 39 */
    "ZANDER",  /* Index 40 */
    "CYRUS",   /* Index 41 */
    "AJAX",    /* Index 42 */
    "CEDRIC",  /* Index 43 */
    "HEXUS",   /* Index 44 */
    "JARVIS",  /* Index 45 */
    "LUCAS",   /* Index 46 */
    "ALTAIR",  /* Index 47 */
    "FINLAY",  /* Index 48 (Unused, also last name in Tabloid, rest DRAGON X) */
    "JETHRO",  /* Index 49 (Unused) */
    "BRUNO",   /* Index 50 */
    "CLEETUS", /* Index 51 */
    "CLAUDE",  /* Index 52 */
    "CYPRIN",  /* Index 53 */
    "ROSCO",   /* Index 54 */
    "DAMON",   /* Index 55 */
    "BUBBA",   /* Index 56 */
    "ZEKE",    /* Index 57 */
    "ISAAK",   /* Index 58 */
    "LYLE",    /* Index 59 */
    "JED",     /* Index 60 */
    "SADIKI",  /* Index 61 */
    "ZIKOMO",  /* Index 62 */
    "MAZI",    /* Index 63 */
    "LATEEF",  /* Index 64 */
    "KASIYA",  /* Index 65 */
    "AZIZI",   /* Index 66 */
    "BAKARI",  /* Index 67 */
    "APARA",   /* Index 68 */
    "OBASI",   /* Index 69 */
    "USENI",   /* Index 70 */
    "BARUTI",  /* Index 71 */
    "MUDADA",  /* Index 72*/
    "KOSOKO",  /* Index 73*/
    "COPANO",  /* Index 74*/
    "LUTALO",  /* Index 75*/
    "UNIKA",   /* Index 76 */
    "RASHIDI", /* Index 77 (Unused) */
    "REVILO",  /* Index 78 */
    "DRAGON X", /* Index 79 */
};

char *g_HomeworldNames[] = {
    "ARTISANS",       /**/
    "PEACE KEEPERS",  /**/
    "MAGIC CRAFTERS", /**/
    "BEAST MAKERS",   /**/
    "DREAM WEAVERS",  /**/
    "GNASTY'S WORLD", /**/
#if GAME_VER == VER_NTSC_U
    "THIGH MASTERS"   /**/
#endif
};

// Length is in Mobys, so spaces aren't included
#if GAME_VER == VER_NTSC_U || defined(NON_MATCHING)
char g_HomeworldNamesLength[7] = {
    sizeof("ARTISANS") - 1,      /**/
    sizeof("PEACEKEEPERS") - 1,  /**/
    sizeof("MAGICCRAFTERS") - 1, /**/
    sizeof("BEASTMAKERS") - 1,   /**/
    sizeof("DREAMWEAVERS") - 1,  /**/
    sizeof("MACHINISTS") - 1,    /* SKELETON: Gnasty's World, oversight */
    sizeof("THIGHMASTERS") - 1,  /**/
};

#endif

// Level name table
char *g_LevelNames[] = {
    /* Artisans */
    "HOME",         /**/
    "STONE HILL",   /**/
    "DARK HOLLOW",  /**/
    "TOWN SQUARE",  /**/
    "TOASTY",       /**/
    "SUNNY FLIGHT", /**/

    /* Peace Keepers */
    "HOME",         /**/
    "DRY CANYON",   /**/
    "CLIFF TOWN",   /**/
    "ICE CAVERN",   /**/
    "DOCTOR SHEMP", /**/
    "NIGHT FLIGHT", /**/

    /* Magic Crafters */
    "HOME",           /**/
    "ALPINE RIDGE",   /**/
    "HIGH CAVES",     /**/
    "WIZARD PEAK",    /**/
    "BLOWHARD",       /**/
    "CRYSTAL FLIGHT", /**/

    /* Beast Makers */
    "HOME",            /**/
    "TERRACE VILLAGE", /**/
    "MISTY BOG",       /**/
    "TREE TOPS",       /**/
    "METALHEAD",       /**/
    "WILD FLIGHT",     /**/

    /* Dream Weavers */
    "HOME",           /**/
    "DARK PASSAGE",   /**/
    "LOFTY CASTLE",   /**/
    "HAUNTED TOWERS", /**/
    "JACQUES",        /**/
    "ICY FLIGHT",     /**/

    /* Gnasty's World */
    "GNORC GNEXUS",    /**/
    "GNORC COVE",      /**/
    "TWILIGHT HARBOR", /**/
    "GNASTY GNORC",    /**/
    "GNASTY'S LOOT",   /**/
#if GAME_VER == VER_NTSC_U
    "A",               /* Placeholder for flight level */
#endif

    /* Entry 36 */
    "RETURN HOME" /**/
};

/***********************************************/
/* Balloonist stuff */
/***********************************************/
char *g_BalloonistNames[6] = {
    "MARCO",   /* Artisans */
    "GOSNOLD", /* Peace Keepers */
    "TUCO",    /* Magic Crafters */
    "CRAY",    /* Beast Makers */
    "AMOS",    /* Dream Weavers */
    "HAK"      /* Gnasty's World */
};

#define IDX(idx) (idx * 3)

#if GAME_VER == VER_NTSC_U || defined(NON_MATCHING)
char g_BalloonistDialogueIndices[40] = {
    IDX(0),  IDX(1),  IDX(2),  IDX(3),  IDX(4),  IDX(5),  IDX(6),  IDX(7),
    IDX(8),  IDX(9),  IDX(10), IDX(11), IDX(12), IDX(13), IDX(14), IDX(15),
    IDX(16), IDX(17), IDX(18), IDX(19), IDX(20), IDX(21), IDX(22), IDX(23),
    IDX(24), IDX(25), IDX(26), IDX(27), IDX(28), IDX(29), IDX(30), IDX(30),
    IDX(30), IDX(30), IDX(31), IDX(32), IDX(33), IDX(34), IDX(35), IDX(36)};

#endif

#undef IDX

char *g_BalloonistDialogue[] = {
    // 0
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "THERE ARE MANY MORE", "WORLDS FOR YOU TO", "EXPLORE, SPYRO.",
#else
    "IF YOU PROVE YOUR", "WORTH BY RESCUING", "10 DRAGONS...",
#endif

    // 1
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "I CAN TAKE YOU TO SEE THEM", "IN THE FULL VERSION OF", "SPYRO THE DRAGON.",
#else
    "THEN YOU MAY USE", "THIS BALLOON TO FLY", "TO A NEW WORLD.",
#endif

    // 2
    "COME BACK TO SEE ME", "AFTER YOU HAVE", "RESCUED 10 DRAGONS.",

    // 3
    "WOW!  I SEE YOU HAVE", "BEEN BUSY RESCUING", "DRAGONS, SPYRO.",

    // 4
    "WELL DONE SPYRO!", "I DIDN'T THINK YOU", "COULD DO IT.",

    // 5
    "YOU MAY TRAVEL TO", "THE PEACE KEEPERS", "WORLD IF YOU LIKE.",

    // 6
    "THE MAGIC CRAFTERS", "WORLD IS FAR AWAY", "AND VERY DANGEROUS.",

    // 7
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "MAYBE IF YOU FIND", "1200 STOLEN TREASURES", "I'LL SHOW IT TO YOU.",
#else
    "MAYBE IF YOU FIND", "1200 STOLEN TREASURE", "I'LL SHOW IT TO YOU.",
#endif

    // 8
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "I'M STILL WAITING", "FOR YOU TO FIND", "1200 STOLEN TREASURES.",
#else
    "I'M STILL WAITING", "FOR YOU TO FIND THAT", "1200 STOLEN TREASURE.",
#endif

    // 9
    "I CAN'T BELIEVE HOW", "FAST YOU FOUND SO", "MUCH STOLEN TREASURE.",

    // 10
    "NICE JOB SPYRO!", "ONE DAY YOU MIGHT", "BECOME A REAL DRAGON.",

    // 11
    "HOP ABOARD FOR THE", "MAGIC CRAFTERS WORLD", "IF YOU ARE READY.",

    // 12
    "THOSE SNEAKY THIEVES", "ARE HIDING OUT ALL", "OVER THE PLACE.",

    // 13
    "DO ME A FAVOR AND", "RECOVER 5 DRAGON EGGS", "FROM THEM, WOULD YOU?",

    // 14
    "HEY SPYRO, MAYBE I", "CAN HELP YOU RECOVER", "THOSE 5 DRAGON EGGS.",

    // 15
    "THANK YOU, SPYRO, FOR", "RECOVERING SO MANY OF", "OUR DRAGON EGGS.",

    // 16
    "GREAT WORK SPYRO!", "YOU MUST BE FASTER", "THAN YOU LOOK.",

    // 17
    "HOP ABOARD FOR THE", "BEAST MAKERS WORLD", "IF YOU ARE READY.",

    // 18
    "I FEEL SORRY FOR ALL", "OF THE DRAGONS STILL", "TRAPPED IN CRYSTAL.",

    // 19
    "MAYBE ONCE YOU RESCUE", "50 DRAGONS THEN I'LL", "FEEL A LITTLE BETTER.",

    // 20
    "RESCUE 50 DRAGONS SO", "I'LL BE ABLE TO SLEEP", "BETTER AT NIGHT.",

    // 21
    "I FEEL MUCH BETTER NOW", "THAT SO MANY DRAGONS", "HAVE BEEN RESCUED.",

    // 22
    "WOW, THAT WAS QUICK!", "I BARELY HAD A CHANCE", "TO PATCH THAT LEAK.",

    // 23
    "JUMP IN THE BALLOON", "AND I'LL SHOW YOU THE", "DREAM WEAVERS WORLD.",

    // 24
    "I'M TIRED OF RENTING", "BALLOONS. THE FAIRIES", "GET ALL THE GOOD JOBS.",

    // 25
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "COME SEE ME AGAIN WHEN", "YOU HAVE RECOVERED", "6000 STOLEN TREASURES.",
#else
    "COME SEE ME AGAIN WHEN", "YOU HAVE RECOVERED", "6000 STOLEN TREASURE.",
#endif

    // 26
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "I'M STILL WAITING", "FOR YOU TO FIND", "6000 STOLEN TREASURES.",
#else
    "I'M STILL WAITING", "FOR YOU TO FIND THAT", "6000 STOLEN TREASURE.",
#endif

    // 27
    "NICE JOB COLLECTING SO", "MUCH STOLEN TREASURE.", "BUT BEWARE, SPYRO.",

    // 28
    "WELL DONE, SPYRO!", "I HOPE YOU KNOW WHAT", "LIES AHEAD OF YOU.",

    // 29
    "GNASTY'S WORLD IS NOT", "THE MOST FRIENDLY", "PLACE YOU WILL FIND.",

    // 30
    "I THINK I SAW A THIEF", "RUNNING AROUND IN", "STONE HILL.",

    // 31
    "MY FRIEND TOLD ME", "ABOUT A PESKY THIEF", "IN DRY CANYON.",

    // 32
    "I'VE HEARD A RUMOR OF", "A THIEF HIDING OUT", "IN CLIFF TOWN.",

    // 33
    "THERE ARE A COUPLE OF", "THIEVES RIGHT HERE IN", "MAGIC CRAFTERS HOME.",

    // 34
    "I BET YOU WOULD DO", "WELL TO HUNT FOR EGGS", "ON WIZARD PEAK.",

    // 35
#if GAME_VER >= VER_C3_DEMO_NTSC_U
    "THE ENTRANCE PORTAL", "IS JUST UP THE HILL", "BEHIND YOU"
#else
    "THE ENTRANCE PORTAL", "IS JUST UP THE HILL", "BEHIND YOU."
#endif

    /**/
};
