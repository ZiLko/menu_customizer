#pragma once

using namespace geode::prelude;

enum NodeType {
    Button = 1,
    Sprite = 2,
    Label = 3,
    Sprite9 = 4
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
    { "level-name", { "bigFont.fnt" }, {}, NodeType::Label },
    { "background", { "square02b_001.png", false, ccc3(0, 0, 0), 75, {569, 320} }, {}, NodeType::Sprite9 },
    { "normal-progress-bar", { "GJ_progressBar_001.png", false, ccc3(0, 255, 0) }, {}, NodeType::Sprite, {true, {0}, true} },
    { "practice-progress-bar", { "GJ_progressBar_001.png", false, ccc3(0, 255, 255) }, {}, NodeType::Sprite, {true, {0}, true} },
};