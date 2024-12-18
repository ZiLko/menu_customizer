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
		json["flipX"].asBool().unwrapOr(true),
		json["flipY"].asBool().unwrapOr(true)
    };

	return props;
}

NodeProperties Customizer::getNodeProperties(CCNode* node, Node info) {
	if (!node) return {};
	
	float opacity = 100.f;
	bool disabled = false;
	bool flipX = false;
	bool flipY = false;
	cocos2d::CCPoint scale = ccp(node->getScaleX(), node->getScaleY());
	cocos2d::ccColor3B color;

	if (CCMenuItemSpriteExtra* btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
		opacity = btn->getOpacity() / 255.f * 100.f;
		color = btn->getColor();
		disabled = btn->isEnabled();
		CCSprite* spr = btn->getChildByType<CCSprite>(0);
		scale = ccp(spr->getScaleX(), spr->getScaleY());
	}

	else if (CCLabelBMFont* lbl = typeinfo_cast<CCLabelBMFont*>(node)) {
		opacity = lbl->getOpacity() / 255.f * 100.f;
		color = lbl->getColor();
	}

	else if (CCSprite* spr = typeinfo_cast<CCSprite*>(node)) {
		opacity = spr->getOpacity() / 255.f * 100.f;
		flipX = spr->isFlipX();
		flipY = spr->isFlipY();

		if (info.colors.recursive && info.colors.ignoreParent && !info.colors.indexes.empty())
			spr = spr->getChildByType<CCSprite>(info.colors.indexes[0]);

		color = spr->getColor();
	}

	else if (CCScale9Sprite* spr = typeinfo_cast<CCScale9Sprite*>(node)) {
		opacity = spr->getOpacity() / 255.f * 100.f;
		color = spr->getColor();
	}

	else if (Slider* slider = typeinfo_cast<Slider*>(node)) {
		if (CCSprite* spr = slider->getChildByType<CCSprite>(0)) {
			opacity = spr->getOpacity() / 255.f * 100.f;
			color = spr->getColor();
		}
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
		flipX,
		flipY
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
		case NodeType::Sprite9: applyToSprite9(node, info, props); break;
		case NodeType::Sliderr: applyToSlider(node, info, props); break;
	}
}

void Customizer::setDefaultsFor(std::string id, NodeProperties props) {
	getDefaults()[id] = props;
}

std::unordered_map<std::string, NodeProperties>& Customizer::getDefaults() {
	return get().currentDefaults;
}

NodeProperties Customizer::getDefaultsFor(std::string id) {
	NodeProperties ret;
	return getDefaults().contains(id) ? getDefaults().at(id) : ret;
}

cocos2d::CCPoint Customizer::getOriginalPos(std::string id) {
	return getDefaultsFor(id).offset;
}

bool Customizer::hasSavedValue(std::string id) {
	if (!Mod::get()->hasSavedValue(id)) return false;
	matjson::Value empty;
	return Mod::get()->getSavedValue<matjson::Value>(id) != empty;
}

void Customizer::applyLayer(CCNode* layer, std::vector<Node> nodes) {
    for (Node info : nodes) {
		CCNode* node = getNode(layer, info);
		if (!node) continue;

		setDefaultsFor(info.id, getNodeProperties(node, info));

		if (!hasSavedValue(info.id)) continue;
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
	button->setEnabled(props.enabled);

	std::vector<CCSprite*> sprites;
	sprites.push_back(button->getChildByType<CCSprite>(0)); 
	if (CCSprite* sprite = sprites[0]) {
		sprite->setScaleX(props.scale.x);
		sprite->setScaleY(props.scale.y);
		button->setContentSize({sprite->getContentSize().width * props.scale.x, sprite->getContentSize().height * props.scale.y});
		sprite->setPosition(button->getContentSize() / 2);

		sprites.push_back(sprite->getChildByType<CCSprite>(0));
		if (CCLabelBMFont* lbl = sprite->getChildByType<CCLabelBMFont>(0)) {
			for (CCNode* node : CCArrayExt<CCNode*>(lbl->getChildren())) {
				sprites.push_back(typeinfo_cast<CCSprite*>(node));
			}
		}
	}

	for (CCSprite* sprite : sprites) {
		if (!sprite) continue;
		sprite->setColor(props.color);
		sprite->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
		sprite->setFlipX(props.flipX);
		sprite->setFlipY(props.flipY);
		
		if (!info.colors.recursive) continue;

		for (int index : info.colors.indexes) {
			if (CCSprite* sprite2 = sprite->getChildByType<CCSprite>(index))
				sprite2->setColor(props.color);
		}
	}
}

void Customizer::applyToSprite(CCNode* node, Node info, NodeProperties props) {
	CCSprite* sprite = typeinfo_cast<CCSprite*>(node);
	if (!sprite) return;

	applyBasics(node, info.id, props);

	sprite->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
	sprite->setFlipX(props.flipX);
	sprite->setFlipY(props.flipY);

	if (!info.colors.ignoreParent)
		sprite->setColor(props.color);

	if (!info.colors.recursive) return;

	for (int index : info.colors.indexes) {
		if (CCSprite* sprite2 = sprite->getChildByType<CCSprite>(index))
	    	sprite2->setColor(props.color);
	}
}

void Customizer::applyToLabel(CCNode* node, Node info, NodeProperties props) {
	CCLabelBMFont* label = typeinfo_cast<CCLabelBMFont*>(node);
	if (!label) return;

	if (props.flipX) props.scale.x *= -1;
	if (props.flipY) props.scale.y *= -1;

    applyBasics(node, info.id, props);
	label->setColor(props.color);
	label->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
}

void Customizer::applyToSprite9(CCNode* node, Node info, NodeProperties props) {
	CCScale9Sprite* sprite = typeinfo_cast<CCScale9Sprite*>(node);
	if (!sprite) return;

	if (props.flipX) props.scale.x *= -1;
	if (props.flipY) props.scale.y *= -1;
	applyBasics(node, info.id, props);

	sprite->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
	sprite->setContentSize(props.size);

	if (!info.colors.ignoreParent)
		sprite->setColor(props.color);

	if (!info.colors.recursive) return;

	for (int index : info.colors.indexes) {
		if (CCSprite* sprite2 = sprite->getChildByType<CCSprite>(index))
	    	sprite2->setColor(props.color);
	}
}

void Customizer::applyToSlider(CCNode* node, Node info, NodeProperties props) {
	Slider* slider = typeinfo_cast<Slider*>(node);
	if (!slider) return;

	if (props.flipX) props.scale.x *= -1;
	if (props.flipY) props.scale.y *= -1;
	applyBasics(node, info.id, props);

	CCSprite* sprites[4] = {
		slider->getChildByType<CCSprite>(0),
		nullptr,
		nullptr,
		nullptr
	};
	if (SliderTouchLogic* logic = slider->getChildByType<SliderTouchLogic>(0)) {
		if (SliderThumb* thumb = logic->getChildByType<SliderThumb>(0)) {
			sprites[2] = thumb->getChildByType<CCSprite>(0);
			sprites[3] = thumb->getChildByType<CCSprite>(1);
		}
	}
	if (sprites[0])
		sprites[1] = sprites[0]->getChildByType<CCSprite>(0);

	for (int i = 0; i < 4; i++) {
		if (!sprites[i]) continue;
		sprites[i]->setColor(props.color);
		sprites[i]->setOpacity(static_cast<int>(props.opacity / 100.f * 255));
	}
}