#include "includes.hpp"

class Cell : public CCNode {

public:

    Node node;
    CCNode* layer = nullptr;
    geode::Popup<CCNode*, std::vector<Node>>* parent = nullptr;
    CCMenuItemSpriteExtra* editButton = nullptr;

    static Cell* create(Node, geode::Popup<CCNode*, std::vector<Node>>*, CCNode*);
    
    bool init(Node);

    void onEdit(CCObject*);

};

class SelectorLayer : public geode::Popup<CCNode*, std::vector<Node>>, public TextInputDelegate, public ColorPickPopupDelegate, public SliderDelegate {

public:

    std::vector<CCNode*> nodes;
    std::unordered_map<CCNode*, int> ogOpacities;
    float slidingHeight = 0.f;

    ScrollLayer* scroll = nullptr;
    std::vector<Node> selectedNodes;
    Node selectedNode;
    NodeProperties selectedProps;
    CCNode* layer = nullptr;
    std::thread thread;
    bool threading = false;

    CCNode* contentLayer = nullptr;
    CCLabelBMFont* idLabel = nullptr;
    CCMenuItemSpriteExtra* restoreButton = nullptr;
    ColorChannelSprite* colorSprite = nullptr;
    CCScale9Sprite* scrollBg = nullptr;
    CCScale9Sprite* listBg = nullptr;
    GJCommentListLayer* listLayer = nullptr;

    Slider* xOffsetSlider = nullptr;
    Slider* yOffsetSlider = nullptr;
    Slider* scaleSlider = nullptr;
    Slider* xScaleSlider = nullptr;
    Slider* yScaleSlider = nullptr;
    Slider* rotationSlider = nullptr;
    Slider* xRotationSlider = nullptr;
    Slider* yRotationSlider = nullptr;
    Slider* xSkewSlider = nullptr;
    Slider* ySkewSlider = nullptr;
    Slider* xAnchorSlider = nullptr;
    Slider* yAnchorSlider = nullptr;
    Slider* xSizeSlider = nullptr;
    Slider* ySizeSlider = nullptr;
    Slider* opacitySlider = nullptr;

    TextInput* xOffsetInput = nullptr;
    TextInput* yOffsetInput = nullptr;
    TextInput* scaleInput = nullptr;
    TextInput* xScaleInput = nullptr;
    TextInput* yScaleInput = nullptr;
    TextInput* rotationInput = nullptr;
    TextInput* xRotationInput = nullptr;
    TextInput* yRotationInput = nullptr;
    TextInput* xSkewInput = nullptr;
    TextInput* ySkewInput = nullptr;
    TextInput* xAnchorInput = nullptr;
    TextInput* yAnchorInput = nullptr;
    TextInput* xSizeInput = nullptr;
    TextInput* ySizeInput = nullptr;
    TextInput* opacityInput = nullptr;
    TextInput* zOrderInput = nullptr;

    CCMenuItemToggler* visibleToggle = nullptr;
    CCMenuItemToggler* enableToggle = nullptr;
    CCMenuItemToggler* flipXToggle = nullptr;
    CCMenuItemToggler* flipYToggle = nullptr;
    CCMenuItemSpriteExtra* colorButton = nullptr;

    static SelectorLayer* create(CCNode*, std::vector<Node>);
    bool setup(CCNode*, std::vector<Node>) override;

    void addScrollLayer();
    void runFadeIn(CCNode*);
    void runFadeOut(CCNode*);
    void hideBackground(float);
    void showBackground();
    void selectNode(Node);
    void selectAnimation();

    virtual void textChanged(CCTextInputNode*) override;
    virtual void sliderBegan(Slider*) override;
	virtual void sliderEnded(Slider*) override;
    virtual void updateColor(const cocos2d::ccColor4B&) override;

    static float getFloat(TextInput*);
    static std::string getString(float, bool, bool);
    static float getSliderValue(float, float, float);

    void setProperties(NodeProperties);
    NodeProperties getProperties();

    void updateListColors();
    void updateSlider(Slider*, TextInput*, float, float);
    void updateInput(CCObject*, Slider*, TextInput*, float, float, bool);
    void updateSliders();
    void updateInputs(CCObject*);
    void updateNode();
    void updateScale();
    void updateRotation();

    void onCallback();
    void onToggle(CCObject*);
    void openColorPicker(CCObject*);
    void onRestoreAll(CCObject*);
    void onRestoreSelected(CCObject*);
    void onRandom(CCObject*);
    void onSettings(CCObject*);
};