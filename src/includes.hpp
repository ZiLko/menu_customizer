#pragma once

using namespace geode::prelude;

enum NodeType {
    Button = 1,
    Sprite = 2,
    Label = 3
};

struct ColorSettings {
    bool recursive = false;
    std::vector<int> indexes = {};
};

struct NodeSprite {
    std::string name;
    cocos2d::ccColor3B color = ccc3(255, 255, 255);
    int opacity = 255;
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
    { "main-title", { "GJ_logo_001.png" }, {}, NodeType::Sprite },
    { "player-username", {}, {}, NodeType::Label }
};

const std::vector<Node> pauseLayerNodes = {
    { "normal-progress-bar", { "" }, {}, NodeType::Sprite, {true, {0}} },
};