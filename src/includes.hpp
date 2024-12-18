#pragma once

using namespace geode::prelude;

enum NodeType {
    Button = 1,
    Sprite = 2,
    Label = 3,
    Sprite9 = 4,
    Sliderr = 5
};

struct ColorSettings {
    bool recursive = false;
    std::vector<int> indexes = {};
    bool ignoreParent = false;
};

struct NodeSprite {
    std::string name;
    bool spriteFrameName = true;
    cocos2d::ccColor3B color = ccc3(255, 255, 255);
    int opacity = 255;
    cocos2d::CCSize size = {0, 0};
    cocos2d::CCPoint scale = {1, 1};
    bool grayScale = false;
    bool flipY = false;
};

struct Node {
    std::string id;
    NodeSprite sprite;
    std::vector<std::string> pathIDs;
    NodeType type = NodeType::Button;
    ColorSettings colors;
};

struct NodeProperties {
    cocos2d::CCPoint offset = ccp(0, 0);
    cocos2d::CCPoint scale = ccp(0, 0);
    cocos2d::CCPoint rotation = ccp(0, 0);
    cocos2d::CCPoint skew = ccp(0, 0);
    cocos2d::CCPoint anchor = ccp(0, 0);
    cocos2d::CCPoint size = ccp(0, 0);
    double opacity = 0.f;
    cocos2d::ccColor3B color = ccc3(255, 255, 255);
    bool visible = true;
    bool enabled = true;
    int64_t zOrder = 0;
    bool flipX = false;
    bool flipY = false;
};

const std::vector<Node> menuLayerNodes = {
    { "play-button", { "GJ_playBtn_001.png" }, {"main-menu"} },
    { "icon-kit-button", { "GJ_garageBtn_001.png" }, {"main-menu"} },
    { "editor-button", { "GJ_creatorBtn_001.png" }, {"main-menu"} },
    { "achievements-button", { "GJ_achBtn_001.png" }, {"bottom-menu"} },
    { "settings-button", { "GJ_optionsBtn_001.png" }, {"bottom-menu"} },
    { "stats-button", { "GJ_statsBtn_001.png" }, {"bottom-menu"} },
    { "newgrounds-button", { "GJ_ngBtn_001.png" }, {"bottom-menu"} },
    { "geode.loader/geode-button", {}, {"bottom-menu"}, NodeType::Button, {true, {0}} },
    { "profile-button", { "GJ_profileButton_001.png" }, {"profile-menu"} },
    { "daily-chest-button", { "GJ_dailyRewardBtn_001.png" }, {"right-side-menu"} },
    { "robtop-logo-button", { "robtoplogo_small.png" }, {"social-media-menu"} },
    { "facebook-button", { "gj_fbIcon_001.png" }, {"social-media-menu"} },
    { "twitter-button", { "gj_twIcon_001.png" }, {"social-media-menu"} },
    { "youtube-button", { "gj_ytIcon_001.png" }, {"social-media-menu"} },
    { "discord-button", { "gj_discordIcon_001.png" }, {"social-media-menu"} },
    { "twitch-button", { "gj_twitchIcon_001.png" }, {"social-media-menu"} },
    { "more-games-button", { "GJ_moreGamesBtn_001.png" }, {"more-games-menu"} },
    { "close-button", { "GJ_closeBtn_001.png" }, {"close-menu"} },
    { "main-title", { "GJ_logo_001.png" }, {}, NodeType::Sprite},
    { "player-username", { "goldFont.fnt" }, {}, NodeType::Label }
};

const std::vector<Node> pauseLayerNodes = {
    { "play-button", { "GJ_playBtn2_001.png" }, {"center-button-menu"} },
    { "practice-button", { "GJ_practiceBtn_001.png" }, {"center-button-menu"} },
    { "exit-button", { "GJ_menuBtn_001.png" }, {"center-button-menu"} },
    { "retry-button", { "GJ_replayBtn_001.png" }, {"center-button-menu"} },
    { "options-button", { "GJ_optionsBtn_001.png" }, {"right-button-menu"} },
    { "music-slider", { "slider.png"_spr, false }, {}, NodeType::Sliderr },
    { "sfx-slider", { "slider.png"_spr, false }, {}, NodeType::Sliderr },
    { "background", { "square02b_001.png", false, ccc3(0, 0, 0), 75, {569, 320} }, {}, NodeType::Sprite9 },
    { "normal-progress-bar", { "GJ_progressBar_001.png", false, ccc3(0, 255, 0) }, {}, NodeType::Sprite, {true, {0}, true} },
    { "practice-progress-bar", { "GJ_progressBar_001.png", false, ccc3(0, 255, 255) }, {}, NodeType::Sprite, {true, {0}, true} },
    { "level-name", { "bigFont.fnt" }, {}, NodeType::Label },
    { "music-label", { "bigFont.fnt" }, {}, NodeType::Label },
    { "sfx-label", { "bigFont.fnt" }, {}, NodeType::Label },
    { "normal-progress-label", { "bigFont.fnt" }, {}, NodeType::Label },
    { "practice-progress-label", { "bigFont.fnt" }, {}, NodeType::Label },
    { "normal-mode-label", { "bigFont.fnt" }, {}, NodeType::Label },
    { "practice-mode-label", { "bigFont.fnt" }, {}, NodeType::Label }
};

const std::vector<Node> creatorLayerNodes = {
    { "create-button", { "GJ_createBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "saved-button", { "GJ_savedBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "scores-button", { "GJ_highscoreBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "quests-button", { "GJ_challengeBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "versus-button", { .name = "GJ_versusBtn_001.png", .color = ccc3(175, 175, 175), .grayScale = true }, { "creator-buttons-menu" }, NodeType::Button },
    { "map-button", { .name = "GJ_mapBtn_001.png", .color = ccc3(175, 175, 175), .grayScale = true }, { "creator-buttons-menu" }, NodeType::Button },
    { "daily-button", { "GJ_dailyBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "weekly-button", { "GJ_weeklyBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "event-button", { "GJ_eventBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "gauntlets-button", { "GJ_gauntletsBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "featured-button", { "GJ_featuredBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "lists-button", { "GJ_listsBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "paths-button", { "GJ_pathsBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "map-packs-button", { "GJ_mapPacksBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "search-button", { "GJ_searchBtn_001.png" }, { "creator-buttons-menu" }, NodeType::Button },
    { "background", { .name = "GJ_gradientBG.png", .spriteFrameName = false, .color = ccc3(0, 102, 255), .scale = {19.3f, 1.031f} }, {}, NodeType::Sprite },
    { "top-left-corner", { .name = "GJ_sideArt_001.png", .flipY = true }, {}, NodeType::Sprite },
    { "bottom-left-corner", { "GJ_sideArt_001.png" }, {}, NodeType::Sprite },
    { "exit-button", { "GJ_arrow_03_001.png" }, { "exit-menu" }, NodeType::Button },
    { "vault-button", { "GJ_lockGray_001.png" }, { "top-right-menu" }, NodeType::Button },
    { "treasure-room-button", { "secretDoorBtn_closed_001.png" }, { "bottom-right-menu" }, NodeType::Button },
};

const std::vector<Node> levelSearchLayerNodes = {
    { "background", { .name = "GJ_gradientBG.png", .spriteFrameName = false, .color = ccc3(0, 102, 255), .scale = {19.3f, 1.031f} }, {}, NodeType::Sprite },
    { "level-search-bg", { "square02b_001.png", false, ccc3(0, 56, 141), 255, {365, 40} }, {}, NodeType::Sprite9 },
    { "level-search-bar-bg", { "square02b_001.png", false, ccc3(0, 39, 98), 255, {210, 30} }, {}, NodeType::Sprite9 },
    { "quick-search-bg", { "square02b_001.png", false, ccc3(0, 46, 117), 255, {365, 115} }, {}, NodeType::Sprite9 },
    { "length-filters-bg", { "square02b_001.png", false, ccc3(0, 31, 79), 255, {365, 35} }, {}, NodeType::Sprite9 },
    { "search-level-button", { "GJ_longBtn06_001.png" }, { "search-button-menu" }, NodeType::Button },
    { "search-user-button", { "GJ_longBtn05_001.png" }, { "search-button-menu" }, NodeType::Button },
    { "clear-search-button", { "GJ_longBtn07_001.png" }, { "search-button-menu" }, NodeType::Button },
    { "quick-search-title", { "bigFont.fnt" }, {}, NodeType::Label },
};