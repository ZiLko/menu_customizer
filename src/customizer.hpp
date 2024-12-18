#include "includes.hpp"

class Customizer {

public:

    std::unordered_map<std::string, NodeProperties> currentDefaults;

    static Customizer& get();
    static CCNode* getNode(CCNode*, Node);
    static cocos2d::CCPoint getOriginalPos(std::string);
    static std::unordered_map<std::string, NodeProperties>& getDefaults();
    static void setDefaultsFor(std::string, NodeProperties);
    static NodeProperties getDefaultsFor(std::string);
    static bool hasSavedValue(std::string);

    static void applyLayer(CCNode*, std::vector<Node>);
    static void applyBasics(CCNode*, std::string, NodeProperties, bool);
    static void applyToButton(CCNode*, Node, NodeProperties);
    static void applyToSprite(CCNode*, Node, NodeProperties);
    static void applyToLabel(CCNode*, Node, NodeProperties);
    static void applyToSprite9(CCNode*, Node, NodeProperties);
    static void applyToSlider(CCNode*, Node, NodeProperties);

    static void saveProperties(std::string, NodeProperties);
    static NodeProperties getSavedProperties(std::string);
    static NodeProperties getNodeProperties(CCNode*, Node);
    static NodeProperties getRandomProps();
    static NodeProperties getRandomPropsChill();
    static void setNodeProperties(CCNode*, Node, NodeProperties);

};