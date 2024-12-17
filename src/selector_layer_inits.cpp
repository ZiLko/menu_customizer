#include "selector_layer.hpp" 
#include "customizer.hpp"


SelectorLayer* SelectorLayer::create(CCNode* layer, std::vector<Node> nodes) {
    SelectorLayer* ret = new SelectorLayer();
    if (ret->initAnchored(457, 264, layer, nodes, "GJ_square02.png")) {
        ret->autorelease();
        ret->m_noElasticity = true;
        return ret;
    }

    delete ret;
    return nullptr;
};

Cell* Cell::create(Node node, geode::Popup<CCNode*, std::vector<Node>>* parent, CCNode* layer) {
    Cell* ret = new Cell();
    ret->layer = layer;
    if (ret->init(node)) {
        ret->parent = parent;
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};

bool Cell::init(Node node) {
    this->node = node;
    CCMenu* menu = CCMenu::create();
    menu->setPosition({0, 0});
    addChild(menu);

    CCLabelBMFont* lbl = CCLabelBMFont::create(node.id.c_str(), "bigFont.fnt");
    lbl->setPosition({140, 43});
    lbl->limitLabelWidth(93.f, 0.9f, 0.01f);
    addChild(lbl);
    CCSprite* spr = node.sprite.spriteFrameName ? CCSprite::createWithSpriteFrameName(node.sprite.name.c_str()) : CCSprite::create(node.sprite.name.c_str());

    if (node.type == NodeType::Sprite9) {
        spr = nullptr;
        if (CCScale9Sprite* spr9 = CCScale9Sprite::create(node.sprite.name.c_str())) {
            cocos2d::CCSize size = node.sprite.size;
            float longer = size.width > size.height ? size.width : size.height;
            spr9->setPosition({49, 32.5f});
            spr9->setContentSize(size);
            spr9->setScale(50.f / longer);
            spr9->setColor(node.sprite.color);
            spr9->setOpacity(node.sprite.opacity);
            addChild(spr9);
        }
    }

    if (spr) {
        spr->setPosition({49, 32.5f});
        cocos2d::CCSize size = spr->getContentSize();
        float longer = size.width > size.height ? size.width : size.height;
        spr->setScale(50.f / longer);
        spr->setColor(node.sprite.color);
        spr->setOpacity(node.sprite.opacity);
        addChild(spr);
    } else {
        if (node.id == "geode.loader/geode-button") {
            CircleButtonSprite* spr = CircleButtonSprite::createWithSpriteFrameName(
                "geode.loader/geode-logo-outline-gold.png",
                .95f,
                CircleBaseColor::Green,
                CircleBaseSize::MediumAlt
            );

            spr->setPosition({49, 32.5f});
            cocos2d::CCSize size = spr->getContentSize();
            float longer = size.width > size.height ? size.width : size.height;
            spr->setScale(50.f / longer);
            addChild(spr);
        }
        else if (CCLabelBMFont* realLbl = typeinfo_cast<CCLabelBMFont*>(Customizer::getNode(layer, node)); node.type == NodeType::Label) {
            CCLabelBMFont* lbl = CCLabelBMFont::create(realLbl->getString(), node.sprite.name.c_str());
            lbl->setPosition({49, 32.5f});
            cocos2d::CCSize size = lbl->getContentSize();
            float longer = size.width > size.height ? size.width : size.height;
            lbl->setScale(50.f / longer);
            addChild(lbl);
        }
    }

    ButtonSprite* spr2 = ButtonSprite::create("Edit");
    spr2->setScale(0.6f);
    editButton = CCMenuItemSpriteExtra::create(spr2, this, menu_selector(Cell::onEdit));
    editButton->setPosition({140, 16});
    menu->addChild(editButton);

    return true;
}

bool SelectorLayer::setup(CCNode* layer, std::vector<Node> selectedNodes) {
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    this->layer = layer;
    this->selectedNodes = selectedNodes;
    setTitle("Menu Customizer");

	CCArray* cells = CCArray::create();
    Cell* first = nullptr;

    for (Node node : selectedNodes) {
        if (!Customizer::getNode(layer, node)) continue;
        Cell* cell = Cell::create(node, static_cast<geode::Popup<CCNode*, std::vector<Node>>*>(this), layer);
        if (!first) first = cell;
        cells->addObject(cell);
    }

	ListView* listView = ListView::create(cells, 65, 200, 200);
	contentLayer = static_cast<CCNode*>(listView->m_tableView->getChildren()->objectAtIndex(0));

    updateListColors();

    ButtonSprite* btnSpr = ButtonSprite::create("Restore All");
    btnSpr->setScale(0.412f);
    restoreButton = CCMenuItemSpriteExtra::create(btnSpr, this, menu_selector(SelectorLayer::onRestoreAll));
    restoreButton->setPosition({412, 244});
    if (ButtonSprite* btnSpr = restoreButton->getChildByType<ButtonSprite>(0)) {
        btnSpr->getChildByType<CCScale9Sprite>(0)->setOpacity(120);
        btnSpr->getChildByType<CCLabelBMFont>(0)->setOpacity(120);
    }
    m_buttonMenu->addChild(restoreButton);

    CCSprite* spr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    spr->setScale(0.575f);
    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(SelectorLayer::onSettings));
    btn->setPosition(m_closeBtn->getPosition() + ccp(37, 0));
    m_buttonMenu->addChild(btn);

	listLayer = GJCommentListLayer::create(listView, "", ccc4(255, 255, 255, 0), 200, 200, true);
	listLayer->setID("list-layer");
    listLayer->setPosition({20, m_size.height / 2 - 115});
    listLayer->setZOrder(1);
	m_mainLayer->addChild(listLayer);

	listLayer->setUserObject("dont-correct-borders", cocos2d::CCBool::create(true));

    CCSprite* topBorder = listLayer->getChildByType<CCSprite>(1);
	CCSprite* bottomBorder = listLayer->getChildByType<CCSprite>(0);
	CCSprite* rightBorder = listLayer->getChildByType<CCSprite>(3);
	CCSprite* leftBorder = listLayer->getChildByType<CCSprite>(2);

    topBorder->setScaleX(0.563f);
	topBorder->setScaleY(0.88f);

	bottomBorder->setScaleX(0.563f);
	bottomBorder->setScaleY(0.88f);

	rightBorder->setScaleX(0.8f);
	rightBorder->setScaleY(6.8f);
	rightBorder->setPositionX(201);

	leftBorder->setScaleX(0.8f);
	leftBorder->setScaleY(6.8f);
	leftBorder->setPositionX(-1);

    listBg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
	listBg->setOpacity(75);
    listBg->setColor(ccc3(0, 0, 0));
	listBg->setPosition(listLayer->getPosition());
	listBg->setContentSize({ 200.f, 201.f });
    listBg->setAnchorPoint({0, 0});
	listBg->setID("background");
	m_mainLayer->addChild(listBg);

    scrollBg = CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
	scrollBg->setOpacity(80);
    scrollBg->setColor(ccc3(0, 0, 0));
	scrollBg->setPosition({335, 117});
	scrollBg->setContentSize({ 200.f, 201.f });
	m_mainLayer->addChild(scrollBg);

    idLabel = CCLabelBMFont::create("play-button", "goldFont.fnt");
	idLabel->setPosition({335, 206});
    idLabel->setScale(0.575f);
    m_mainLayer->addChild(idLabel);

    if (first) first->onEdit(nullptr);

    return true;
}

void SelectorLayer::addScrollLayer() {
    scroll = ScrollLayer::create({200, 174});
	scroll->setPosition({235, 18});
    scroll->m_contentLayer->setContentHeight(174);
    m_mainLayer->addChild(scroll);

    float yPos = 168;

    CCLabelBMFont* lbl = CCLabelBMFont::create("Position:", "goldFont.fnt");
    lbl->setPosition({9, yPos});
    lbl->setScale(0.35f);
    lbl->setAnchorPoint({0, 0.5f});
    scroll->m_contentLayer->addChild(lbl);

    yPos -= 15;

    lbl = CCLabelBMFont::create("X Offset", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    xOffsetSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    xOffsetSlider->m_delegate = this;
	xOffsetSlider->setPosition({102, yPos});
	xOffsetSlider->setAnchorPoint({ 0.f, 0.f });
	xOffsetSlider->setScale(0.37f);
	xOffsetSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(xOffsetSlider);

    xOffsetInput = TextInput::create(50, "x", "bigFont.fnt");
    xOffsetInput->getInputNode()->setMaxLabelLength(9);
    xOffsetInput->getInputNode()->setAllowedChars("-0123456789.");
    xOffsetInput->getInputNode()->setDelegate(this);
    xOffsetInput->setPosition({171, yPos});
    xOffsetInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(xOffsetInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Y Offset", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    yOffsetSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    yOffsetSlider->m_delegate = this;
	yOffsetSlider->setPosition({102, yPos});
	yOffsetSlider->setAnchorPoint({ 0.f, 0.f });
	yOffsetSlider->setScale(0.37f);
	yOffsetSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(yOffsetSlider);

    yOffsetInput = TextInput::create(50, "y", "bigFont.fnt");
    yOffsetInput->getInputNode()->setMaxLabelLength(9);
    yOffsetInput->getInputNode()->setAllowedChars("-0123456789.");
    yOffsetInput->getInputNode()->setDelegate(this);
    yOffsetInput->setPosition({171, yPos});
    yOffsetInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(yOffsetInput);

    yPos -= 21;

    lbl = CCLabelBMFont::create("Scale:", "goldFont.fnt");
    lbl->setPosition({9, yPos});
    lbl->setScale(0.35f);
    lbl->setAnchorPoint({0, 0.5f});
    scroll->m_contentLayer->addChild(lbl);

    yPos -= 15;

    lbl = CCLabelBMFont::create("Scale", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.375f);
    scroll->m_contentLayer->addChild(lbl);

    scaleSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    scaleSlider->m_delegate = this;
	scaleSlider->setPosition({102, yPos});
	scaleSlider->setAnchorPoint({ 0.f, 0.f });
	scaleSlider->setScale(0.37f);
	scaleSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(scaleSlider);

    scaleInput = TextInput::create(50, "x", "bigFont.fnt");
    scaleInput->getInputNode()->setMaxLabelLength(9);
    scaleInput->getInputNode()->setAllowedChars("-0123456789.");
    scaleInput->getInputNode()->setDelegate(this);
    scaleInput->setPosition({171, yPos});
    scaleInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(scaleInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Scale X", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    xScaleSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    xScaleSlider->m_delegate = this;
	xScaleSlider->setPosition({102, yPos});
	xScaleSlider->setAnchorPoint({ 0.f, 0.f });
	xScaleSlider->setScale(0.37f);
	xScaleSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(xScaleSlider);

    xScaleInput = TextInput::create(50, "x", "bigFont.fnt");
    xScaleInput->getInputNode()->setMaxLabelLength(9);
    xScaleInput->getInputNode()->setAllowedChars("-0123456789.");
    xScaleInput->getInputNode()->setDelegate(this);
    xScaleInput->setPosition({171, yPos});
    xScaleInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(xScaleInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Scale Y", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    yScaleSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    yScaleSlider->m_delegate = this;
	yScaleSlider->setPosition({102, yPos});
	yScaleSlider->setAnchorPoint({ 0.f, 0.f });
	yScaleSlider->setScale(0.37f);
	yScaleSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(yScaleSlider);

    yScaleInput = TextInput::create(50, "y", "bigFont.fnt");
    yScaleInput->getInputNode()->setMaxLabelLength(9);
    yScaleInput->getInputNode()->setAllowedChars("-0123456789.");
    yScaleInput->getInputNode()->setDelegate(this);
    yScaleInput->setPosition({171, yPos});
    yScaleInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(yScaleInput);

    yPos -= 21;

    lbl = CCLabelBMFont::create("Rotation:", "goldFont.fnt");
    lbl->setPosition({9, yPos});
    lbl->setScale(0.35f);
    lbl->setAnchorPoint({0, 0.5f});
    scroll->m_contentLayer->addChild(lbl);

    yPos -= 15;

    lbl = CCLabelBMFont::create("Rotation", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    rotationSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    rotationSlider->m_delegate = this;
	rotationSlider->setPosition({102, yPos});
	rotationSlider->setAnchorPoint({ 0.f, 0.f });
	rotationSlider->setScale(0.37f);
	rotationSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(rotationSlider);

    rotationInput = TextInput::create(50, "r", "bigFont.fnt");
    rotationInput->getInputNode()->setMaxLabelLength(9);
    rotationInput->getInputNode()->setAllowedChars("-0123456789.");
    rotationInput->getInputNode()->setDelegate(this);
    rotationInput->setPosition({171, yPos});
    rotationInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(rotationInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Rotation X", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.275f);
    scroll->m_contentLayer->addChild(lbl);

    xRotationSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    xRotationSlider->m_delegate = this;
	xRotationSlider->setPosition({102, yPos});
	xRotationSlider->setAnchorPoint({ 0.f, 0.f });
	xRotationSlider->setScale(0.37f);
	xRotationSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(xRotationSlider);

    xRotationInput = TextInput::create(50, "x", "bigFont.fnt");
    xRotationInput->getInputNode()->setMaxLabelLength(9);
    xRotationInput->getInputNode()->setAllowedChars("-0123456789.");
    xRotationInput->getInputNode()->setDelegate(this);
    xRotationInput->setPosition({171, yPos});
    xRotationInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(xRotationInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Rotation Y", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.275f);
    scroll->m_contentLayer->addChild(lbl);

    yRotationSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    yRotationSlider->m_delegate = this;
	yRotationSlider->setPosition({102, yPos});
	yRotationSlider->setAnchorPoint({ 0.f, 0.f });
	yRotationSlider->setScale(0.37f);
	yRotationSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(yRotationSlider);

    yRotationInput = TextInput::create(50, "y", "bigFont.fnt");
    yRotationInput->getInputNode()->setMaxLabelLength(9);
    yRotationInput->getInputNode()->setAllowedChars("-0123456789.");
    yRotationInput->getInputNode()->setDelegate(this);
    yRotationInput->setPosition({171, yPos});
    yRotationInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(yRotationInput);

    yPos -= 21;

    lbl = CCLabelBMFont::create("Skew:", "goldFont.fnt");
    lbl->setPosition({9, yPos});
    lbl->setScale(0.35f);
    lbl->setAnchorPoint({0, 0.5f});
    scroll->m_contentLayer->addChild(lbl);

    yPos -= 15;

    lbl = CCLabelBMFont::create("Skew X", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    xSkewSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    xSkewSlider->m_delegate = this;
	xSkewSlider->setPosition({102, yPos});
	xSkewSlider->setAnchorPoint({ 0.f, 0.f });
	xSkewSlider->setScale(0.37f);
	xSkewSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(xSkewSlider);

    xSkewInput = TextInput::create(50, "x", "bigFont.fnt");
    xSkewInput->getInputNode()->setMaxLabelLength(9);
    xSkewInput->getInputNode()->setAllowedChars("-0123456789.");
    xSkewInput->getInputNode()->setDelegate(this);
    xSkewInput->setPosition({171, yPos});
    xSkewInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(xSkewInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Skew Y", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.325f);
    scroll->m_contentLayer->addChild(lbl);

    ySkewSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    ySkewSlider->m_delegate = this;
	ySkewSlider->setPosition({102, yPos});
	ySkewSlider->setAnchorPoint({ 0.f, 0.f });
	ySkewSlider->setScale(0.37f);
	ySkewSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(ySkewSlider);

    ySkewInput = TextInput::create(50, "y", "bigFont.fnt");
    ySkewInput->getInputNode()->setMaxLabelLength(9);
    ySkewInput->getInputNode()->setAllowedChars("-0123456789.");
    ySkewInput->getInputNode()->setDelegate(this);
    ySkewInput->setPosition({171, yPos});
    ySkewInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(ySkewInput);

    yPos -= 21;

    lbl = CCLabelBMFont::create("Anchor Point:", "goldFont.fnt");
    lbl->setPosition({9, yPos});
    lbl->setScale(0.35f);
    lbl->setAnchorPoint({0, 0.5f});
    scroll->m_contentLayer->addChild(lbl);

    yPos -= 15;

    lbl = CCLabelBMFont::create("Anchor X", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.3f);
    scroll->m_contentLayer->addChild(lbl);

    xAnchorSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    xAnchorSlider->m_delegate = this;
	xAnchorSlider->setPosition({102, yPos});
	xAnchorSlider->setAnchorPoint({ 0.f, 0.f });
	xAnchorSlider->setScale(0.37f);
	xAnchorSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(xAnchorSlider);

    xAnchorInput = TextInput::create(50, "x", "bigFont.fnt");
    xAnchorInput->getInputNode()->setMaxLabelLength(9);
    xAnchorInput->getInputNode()->setAllowedChars("-0123456789.");
    xAnchorInput->getInputNode()->setDelegate(this);
    xAnchorInput->setPosition({171, yPos});
    xAnchorInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(xAnchorInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Anchor Y", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.3f);
    scroll->m_contentLayer->addChild(lbl);

    yAnchorSlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    yAnchorSlider->m_delegate = this;
	yAnchorSlider->setPosition({102, yPos});
	yAnchorSlider->setAnchorPoint({ 0.f, 0.f });
	yAnchorSlider->setScale(0.37f);
	yAnchorSlider->setValue(1.f);
	scroll->m_contentLayer->addChild(yAnchorSlider);

    yAnchorInput = TextInput::create(50, "y", "bigFont.fnt");
    yAnchorInput->getInputNode()->setMaxLabelLength(9);
    yAnchorInput->getInputNode()->setAllowedChars("-0123456789.");
    yAnchorInput->getInputNode()->setDelegate(this);
    yAnchorInput->setPosition({171, yPos});
    yAnchorInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(yAnchorInput);

    if (selectedNode.type == NodeType::Sprite9) {
        yPos -= 21;

        lbl = CCLabelBMFont::create("Content Size:", "goldFont.fnt");
        lbl->setPosition({9, yPos});
        lbl->setScale(0.35f);
        lbl->setAnchorPoint({0, 0.5f});
        scroll->m_contentLayer->addChild(lbl);

        yPos -= 15;

        lbl = CCLabelBMFont::create("Size X", "bigFont.fnt");
        lbl->setPosition({8, yPos});
        lbl->setAnchorPoint({0, 0.5f});
        lbl->setScale(0.325f);
        scroll->m_contentLayer->addChild(lbl);

        xSizeSlider = Slider::create(
            this,
            menu_selector(SelectorLayer::updateInputs),
            1.f
        );
        xSizeSlider->m_delegate = this;
        xSizeSlider->setPosition({102, yPos});
        xSizeSlider->setAnchorPoint({ 0.f, 0.f });
        xSizeSlider->setScale(0.37f);
        xSizeSlider->setValue(1.f);
        scroll->m_contentLayer->addChild(xSizeSlider);

        xSizeInput = TextInput::create(50, "x", "bigFont.fnt");
        xSizeInput->getInputNode()->setMaxLabelLength(9);
        xSizeInput->getInputNode()->setAllowedChars("-0123456789.");
        xSizeInput->getInputNode()->setDelegate(this);
        xSizeInput->setPosition({171, yPos});
        xSizeInput->setScale(0.625f);
        scroll->m_contentLayer->addChild(xSizeInput);

        yPos -= 27;

        lbl = CCLabelBMFont::create("Size Y", "bigFont.fnt");
        lbl->setPosition({8, yPos});
        lbl->setAnchorPoint({0, 0.5f});
        lbl->setScale(0.325f);
        scroll->m_contentLayer->addChild(lbl);

        ySizeSlider = Slider::create(
            this,
            menu_selector(SelectorLayer::updateInputs),
            1.f
        );
        ySizeSlider->m_delegate = this;
        ySizeSlider->setPosition({102, yPos});
        ySizeSlider->setAnchorPoint({ 0.f, 0.f });
        ySizeSlider->setScale(0.37f);
        ySizeSlider->setValue(1.f);
        scroll->m_contentLayer->addChild(ySizeSlider);

        ySizeInput = TextInput::create(50, "y", "bigFont.fnt");
        ySizeInput->getInputNode()->setMaxLabelLength(9);
        ySizeInput->getInputNode()->setAllowedChars("-0123456789.");
        ySizeInput->getInputNode()->setDelegate(this);
        ySizeInput->setPosition({171, yPos});
        ySizeInput->setScale(0.625f);
        scroll->m_contentLayer->addChild(ySizeInput);
    }

    yPos -= 21;

    lbl = CCLabelBMFont::create("Other:", "goldFont.fnt");
    lbl->setPosition({9, yPos});
    lbl->setScale(0.35f);
    lbl->setAnchorPoint({0, 0.5f});
    scroll->m_contentLayer->addChild(lbl);

    yPos -= 15;

    lbl = CCLabelBMFont::create("Visible", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.375f);
    scroll->m_contentLayer->addChild(lbl);

    visibleToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(SelectorLayer::onToggle)
    );
    visibleToggle->setScale(0.5f);
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint({0, 0});
    menu->setPosition({102, yPos});
    scroll->m_contentLayer->addChild(menu);
    menu->addChild(visibleToggle);

    if (selectedNode.type == NodeType::Button) {
        yPos -= 27;

        lbl = CCLabelBMFont::create("Enable", "bigFont.fnt");
        lbl->setPosition({8, yPos});
        lbl->setAnchorPoint({0, 0.5f});
        lbl->setScale(0.375f);
        scroll->m_contentLayer->addChild(lbl);

        enableToggle = CCMenuItemToggler::create(
            CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
            this,
            menu_selector(SelectorLayer::onToggle)
        );
        enableToggle->setScale(0.5f);

        CCMenu* menu = CCMenu::create();
        menu->setAnchorPoint({0, 0});
        menu->setPosition({102, yPos});
        scroll->m_contentLayer->addChild(menu);
        menu->addChild(enableToggle);
    }

    yPos -= 27;

    lbl = CCLabelBMFont::create("Flip X", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.375f);
    scroll->m_contentLayer->addChild(lbl);

    flipXToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(SelectorLayer::onToggle)
    );
    flipXToggle->setScale(0.5f);

    menu = CCMenu::create();
    menu->setAnchorPoint({0, 0});
    menu->setPosition({102, yPos});
    scroll->m_contentLayer->addChild(menu);
    menu->addChild(flipXToggle);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Flip Y", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.375f);
    scroll->m_contentLayer->addChild(lbl);

    flipYToggle = CCMenuItemToggler::create(
        CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
        CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
        this,
        menu_selector(SelectorLayer::onToggle)
    );
    flipYToggle->setScale(0.5f);

    menu = CCMenu::create();
    menu->setAnchorPoint({0, 0});
    menu->setPosition({102, yPos});
    scroll->m_contentLayer->addChild(menu);
    menu->addChild(flipYToggle);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Z Order", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.355f);
    scroll->m_contentLayer->addChild(lbl);

    zOrderInput = TextInput::create(50, "z", "bigFont.fnt");
    zOrderInput->getInputNode()->setMaxLabelLength(9);
    zOrderInput->getInputNode()->setAllowedChars("-0123456789");
    zOrderInput->getInputNode()->setDelegate(this);
    zOrderInput->setPosition({102, yPos});
    zOrderInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(zOrderInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Opacity", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.355f);
    scroll->m_contentLayer->addChild(lbl);

    opacitySlider = Slider::create(
		this,
		menu_selector(SelectorLayer::updateInputs),
		1.f
	);
    opacitySlider->m_delegate = this;
	opacitySlider->setPosition({102, yPos});
	opacitySlider->setAnchorPoint({ 0.f, 0.f });
	opacitySlider->setScale(0.37f);
	opacitySlider->setValue(1.f);
	scroll->m_contentLayer->addChild(opacitySlider);

    opacityInput = TextInput::create(50, "%", "bigFont.fnt");
    opacityInput->getInputNode()->setMaxLabelLength(9);
    opacityInput->getInputNode()->setAllowedChars("-0123456789.");
    opacityInput->getInputNode()->setDelegate(this);
    opacityInput->setPosition({171, yPos});
    opacityInput->setScale(0.625f);
    scroll->m_contentLayer->addChild(opacityInput);

    yPos -= 27;

    lbl = CCLabelBMFont::create("Color", "bigFont.fnt");
    lbl->setPosition({8, yPos});
    lbl->setAnchorPoint({0, 0.5f});
    lbl->setScale(0.375);
    scroll->m_contentLayer->addChild(lbl);

    colorSprite = ColorChannelSprite::create();
    colorSprite->setColor(ccc3(255, 255, 255));
    colorSprite->setScale(0.5f);

    colorButton = CCMenuItemSpriteExtra::create(
        colorSprite,
        this,
        menu_selector(SelectorLayer::openColorPicker)
    );

    menu = CCMenu::create();
    menu->setAnchorPoint({0, 0});
    menu->setPosition({102, yPos});
    scroll->m_contentLayer->addChild(menu);
    menu->addChild(colorButton);

    yPos -= 33;

    ButtonSprite* spr = ButtonSprite::create("Restore");
    spr->setScale(0.45f);

    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(SelectorLayer::onRestoreSelected)
    );

    menu = CCMenu::create();
    menu->setAnchorPoint({0, 0});
    menu->setPosition({134, yPos});
    scroll->m_contentLayer->addChild(menu);
    menu->addChild(btn);

    spr = ButtonSprite::create("Random");
    spr->setScale(0.45f);

    btn = CCMenuItemSpriteExtra::create(
        spr,
        this,
        menu_selector(SelectorLayer::onRandom)
    );

    menu = CCMenu::create();
    menu->setAnchorPoint({0, 0});
    menu->setPosition({66, yPos});
    scroll->m_contentLayer->addChild(menu);
    menu->addChild(btn);

    scroll->m_scrollLimitBottom = abs(yPos) + 20;
}