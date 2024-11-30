#include "customizer.hpp"
#include <random>

Customizer& Customizer::get() {
    static Customizer instance;
    return instance;
}

void Customizer::saveProperties(std::string id, NodeProperties props) {
	matjson::Value json;

	json["offset"]["x"] = props.offset.x;
	json["offset"]["y"] = props.offset.y;

	json["scale"]["x"] = props.scale.x;
	json["scale"]["y"] = props.scale.y;

	json["rotation"]["x"] = props.rotation.x;
	json["rotation"]["y"] = props.rotation.y;

	json["skew"]["x"] = props.skew.x;
	json["skew"]["y"] = props.skew.y;

	json["anchor"]["x"] = props.anchor.x;
	json["anchor"]["y"] = props.anchor.y;

	json["size"]["x"] = props.size.x;
	json["size"]["y"] = props.size.y;

	json["opacity"] = props.opacity;

	json["color"]["r"] = props.color.r;
	json["color"]["g"] = props.color.g;
	json["color"]["b"] = props.color.b;
	
	json["visible"] = props.visible;
	json["enabled"] = props.enabled;
	
	json["zOrder"] = props.zOrder;
	json["flipX"] = props.flipX;
	json["flipY"] = props.flipY;

	Mod::get()->setSavedValue<matjson::Value>(id, json);
}

NodeProperties Customizer::getSavedProperties(std::string id) {
	matjson::Value json = Mod::get()->getSavedValue<matjson::Value>(id);
	NodeProperties props = {
        ccp(json["offset"]["x"].asDouble().unwrapOrDefault(), json["offset"]["y"].asDouble().unwrapOrDefault()),
        ccp(json["scale"]["x"].asDouble().unwrapOrDefault(), json["scale"]["y"].asDouble().unwrapOrDefault()),
        ccp(json["rotation"]["x"].asDouble().unwrapOrDefault(), json["rotation"]["y"].asDouble().unwrapOrDefault()),
        ccp(json["skew"]["x"].asDouble().unwrapOrDefault(), json["skew"]["y"].asDouble().unwrapOrDefault()),
        ccp(json["anchor"]["x"].asDouble().unwrapOrDefault(), json["anchor"]["y"].asDouble().unwrapOrDefault()),
        CCSize(json["size"]["x"].asDouble().unwrapOrDefault(), json["size"]["y"].asDouble().unwrapOrDefault()),
        json["opacity"].asDouble().unwrapOrDefault(),
        ccc3(
            json["color"]["r"].asInt().unwrapOrDefault(),
            json["color"]["g"].asInt().unwrapOrDefault(),
            json["color"]["b"].asInt().unwrapOrDefault()
        ),
		json["visible"].asBool().unwrapOr(true),
		json["enabled"].asBool().unwrapOr(true),
		json["zOrder"].asInt().unwrapOrDefault(),
		json["flipX"].asInt().unwrapOr(true),
		json["flipY"].asInt().unwrapOr(true)
    };

	return props;
}

NodeProperties Customizer::getNodeProperties(CCNode* node, Node info) {
	if (!node) return {};
	
	float opacity = 100.f;
	bool disabled = false;
	cocos2d::CCPoint scale = ccp(node->getScaleX(), node->getScaleY());
	cocos2d::ccColor3B color;

	if (CCMenuItemSpriteExtra* btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node); info.type == NodeType::Button) {
		opacity = btn->getOpacity() / 255.f * 100.f;
		color = btn->getColor();
		disabled = btn->isEnabled();
		CCSprite* spr = btn->getChildByType<CCSprite>(0);
		scale = ccp(spr->getScaleX(), spr->getScaleY());
	}

	if (CCLabelBMFont* lbl = typeinfo_cast<CCLabelBMFont*>(node); info.type == NodeType::Label) {
		opacity = lbl->getOpacity() / 255.f * 100.f;
		color = lbl->getColor();
	}

	if (CCSprite* spr = typeinfo_cast<CCSprite*>(node); info.type == NodeType::Sprite) {
		opacity = spr->getOpacity() / 255.f * 100.f;
		color = spr->getColor();
	}

	NodeProperties props = {
		node->getPosition(),
		scale,
		ccp(node->getRotationX(), node->getRotationY()),
		ccp(node->getSkewX(), node->getSkewY()),
		node->getAnchorPoint(),
		node->getContentSize(),
		opacity,
		color,
		node->isVisible(),
		disabled,
		node->getZOrder(),
		node->isFlipX(),
		node->isFlipY()
	};

	return props;
}

NodeProperties Customizer::getRandomPropsChill() {
	std::random_device rd; 
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> offsetDist(-20.f, 20.f); 
    std::uniform_real_distribution<float> scaleDist(0.5f, 1.5f); 
    std::uniform_real_distribution<float> anchorDist(0.3f, 0.7f); 
    std::uniform_real_distribution<float> rotationDist(-20.f, 20.f); 
    std::uniform_real_distribution<float> opacityDist(90.f, 100.f);  
    std::uniform_int_distribution<int> colorDist(200, 255);              

    NodeProperties ret = {
        cocos2d::CCPoint(offsetDist(gen), offsetDist(gen)),
        cocos2d::CCPoint(scaleDist(gen), scaleDist(gen)), 
        cocos2d::CCPoint(rotationDist(gen), rotationDist(gen)),
        cocos2d::CCPoint(scaleDist(gen), scaleDist(gen)),  
        cocos2d::CCPoint(anchorDist(gen), anchorDist(gen)),  
        cocos2d::CCPoint(offsetDist(gen), offsetDist(gen)),  
        opacityDist(gen),                                  
        cocos2d::ccColor3B(colorDist(gen), colorDist(gen), colorDist(gen)), 
        true,                                     
        true,                                     
        100                               
    };

    return ret;
}

NodeProperties Customizer::getRandomProps() {
	std::random_device rd; 
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> offsetDist(-40.f, 40.f); 
    std::uniform_real_distribution<float> scaleDist(-10.f, 10.f); 
    std::uniform_real_distribution<float> anchorDist(0.f, 1.f); 
    std::uniform_real_distribution<float> rotationDist(0.f, 360.f); 
    std::uniform_real_distribution<float> opacityDist(70.f, 100.f);  
    std::uniform_int_distribution<int> colorDist(0, 255);              

    NodeProperties ret = {
        cocos2d::CCPoint(offsetDist(gen), offsetDist(gen)),
        cocos2d::CCPoint(scaleDist(gen), scaleDist(gen)), 
        cocos2d::CCPoint(rotationDist(gen), rotationDist(gen)),
        cocos2d::CCPoint(scaleDist(gen), scaleDist(gen)),  
        cocos2d::CCPoint(anchorDist(gen), anchorDist(gen)),  
        cocos2d::CCPoint(offsetDist(gen), offsetDist(gen)),  
        opacityDist(gen),                                  
        cocos2d::ccColor3B(colorDist(gen), colorDist(gen), colorDist(gen)), 
        true,                                     
        true,      
		100                               
    };

    return ret;
}

CCNode* Customizer::getNode(CCNode* layer, Node info) {
	CCNode* latestNode = layer;
	for (std::string id : info.pathIDs) {
		if (!latestNode) break;
		latestNode = latestNode->getChildByID(id);
	}

	if (!latestNode) return nullptr;

	return latestNode->getChildByID(info.id);
}

void Customizer::setNodeProperties(CCNode* node, Node info, NodeProperties props) {
	switch (info.type) {
		case NodeType::Button: applyToButton(node, info, props); break;
		case NodeType::Sprite: applyToSprite(node, info, props); break;
		case NodeType::Label: applyToLabel(node, info, props); break;
	}
}

void Customizer::setDefaultsFor(std::string id, NodeProperties props) {
	getDefaults()[id] = props;
}

std::unordered_map<std::string, NodeProperties>& Customizer::getDefaults() {
	return get().currentDefaults;
}

NodeProperties Customizer::getDefaultsFor(std::string id) {
	NodeProperties ret = {};
	return getDefaults().contains(id) ? getDefaults().at(id) : ret;
}

cocos2d::CCPoint Customizer::getOriginalPos(std::string id) {
	return getDefaultsFor(id).offset;
}

void Customizer::applyLayer(CCNode* layer, std::vector<Node> nodes) {
    for (Node info : nodes) {
		CCNode* node = getNode(layer, info);
		if (!node) continue;

		setDefaultsFor(info.id, getNodeProperties(node, info));

		if (!Mod::get()->hasSavedValue(info.id)) continue;
		NodeProperties props = getSavedProperties(info.id);
		// NodeProperties props = getRandomPropsChill();

		setNodeProperties(node, info, props);
	}
}

void Customizer::applyBasics(CCNode* node, std::string id, NodeProperties props, bool scale = true) {
	node->setPosition(getOriginalPos(id) + props.offset),
	node->setRotationX(props.rotation.x),
	node->setRotationY(props.rotation.y),
	node->setSkewX(props.skew.x),
	node->setSkewY(props.skew.y),
	node->setAnchorPoint(props.anchor),
	node->setVisible(props.visible),
	node->setZOrder(props.zOrder);
	if (!scale) return;
	node->setScaleX(props.scale.x),
	node->setScaleY(props.scale.y);
}

void Customizer::applyToButton(CCNode* node, Node info, NodeProperties props) {
	CCMenuItemSpriteExtra* button = typeinfo_cast<CCMenuItemSpriteExtra*>(node);
	if (!button) return;

	applyBasics(node, info.id, props, false);
	button->setColor(props.color);
	button->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
	button->setEnabled(props.enabled);

	CCSprite* sprite = button->getChildByType<CCSprite>(0);
	if (!sprite) return;

	sprite->setScaleX(props.scale.x);
	sprite->setScaleY(props.scale.y);
	button->setContentSize({sprite->getContentSize().width * props.scale.x, sprite->getContentSize().height * props.scale.y});
	sprite->setPosition(button->getContentSize() / 2);
	
	if (!info.colors.recursive) return;

	for (int index : info.colors.indexes) {
		if (CCSprite* sprite2 = sprite->getChildByType<CCSprite>(index))
	    	sprite2->setColor(props.color);
	}
}

void Customizer::applyToSprite(CCNode* node, Node info, NodeProperties props) {
	CCSprite* sprite = typeinfo_cast<CCSprite*>(node);
	if (!sprite) return;

	applyBasics(node, info.id, props);
	sprite->setColor(props.color);
	sprite->setOpacity(static_cast<int>(props.opacity / 100.f * 255));

	if (!info.colors.recursive) return;

	for (int index : info.colors.indexes) {
		if (CCSprite* sprite2 = sprite->getChildByType<CCSprite>(index))
	    	sprite2->setColor(props.color);
	}
}

void Customizer::applyToLabel(CCNode* node, Node info, NodeProperties props) {
	CCLabelBMFont* label = typeinfo_cast<CCLabelBMFont*>(node);
	if (!label) return;

    applyBasics(node, info.id, props);
	label->setColor(props.color);
	label->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
}

