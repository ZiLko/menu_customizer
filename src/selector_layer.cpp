#include "selector_layer.hpp" 
#include "customizer.hpp"
#include <Geode/ui/GeodeUI.hpp>

void Cell::onEdit(CCObject*) {
    static_cast<SelectorLayer*>(parent)->selectNode(node);
    static_cast<SelectorLayer*>(parent)->updateListColors();
    if (editButton) editButton->setEnabled(false);

    CCLayerColor* bg = getParent()->getChildByType<CCLayerColor>(0);
    cocos2d::ccColor3B ogCol = bg->getColor();
    cocos2d::ccColor3B newCol = ccc3(58, 62, 131);

    CCTintTo* col1 = CCTintTo::create(0.6f, newCol.r, newCol.g, newCol.b);
    CCTintTo* col2 = CCTintTo::create(0.8f, ogCol.r + 20, ogCol.g + 20, ogCol.b + 20);
    CCSequence* colorSequence = CCSequence::create(col1, col2, nullptr);
    CCRepeatForever* repeatAction = CCRepeatForever::create(colorSequence);

    bg->runAction(repeatAction);
}

void SelectorLayer::selectAnimation() {
    CCTintTo* col1 = CCTintTo::create(0.01f, 40, 40, 40);
    CCTintTo* col2 = CCTintTo::create(0.42f, 0, 0, 0);
    CCSequence* flash = CCSequence::create(col1, col2, nullptr);
    scrollBg->runAction(flash);
    CCFadeTo* fade1 = CCFadeTo::create(0.01f, 150);
    CCFadeTo* fade2 = CCFadeTo::create(0.2f, 255);
    flash = CCSequence::create(fade1, fade2, nullptr);
    idLabel->runAction(flash);
}

void SelectorLayer::selectNode(Node node) {
    if (scroll) scroll->removeFromParentAndCleanup(true);
    scroll = nullptr;

    xOffsetSlider = nullptr;
    yOffsetSlider = nullptr;
    scaleSlider = nullptr;
    xScaleSlider = nullptr;
    yScaleSlider = nullptr;
    rotationSlider = nullptr;
    xRotationSlider = nullptr;
    yRotationSlider = nullptr;
    xSkewSlider = nullptr;
    ySkewSlider = nullptr;
    xAnchorSlider = nullptr;
    yAnchorSlider = nullptr;
    xSizeSlider = nullptr;
    ySizeSlider = nullptr;
    opacitySlider = nullptr;

    xOffsetInput = nullptr;
    yOffsetInput = nullptr;
    xScaleInput = nullptr;
    yScaleInput = nullptr;
    rotationInput = nullptr;
    xRotationInput = nullptr;
    yRotationInput = nullptr;
    xSkewInput = nullptr;
    ySkewInput = nullptr;
    xAnchorInput = nullptr;
    xScaleSlider = nullptr;
    yAnchorInput = nullptr;
    xSizeInput = nullptr;
    ySizeInput = nullptr;
    opacityInput = nullptr;

    visibleToggle = nullptr;
    enableToggle = nullptr;
    colorButton = nullptr;

    selectedNode = node;
    selectedProps = Customizer::getSavedProperties(node.id);
    if (!Customizer::hasSavedValue(node.id)) {
        selectedProps = Customizer::getNodeProperties(Customizer::getNode(layer, node), node);
        selectedProps.offset = ccp(0, 0);
    }

    if (idLabel) {
        idLabel->setString(node.id.c_str());
        idLabel->limitLabelWidth(165.f, 0.575f, 0.01f);
    }

    selectAnimation();
    addScrollLayer();
    setProperties(selectedProps);

    nodes.clear();
    nodes.push_back(static_cast<CCNode*>(m_bgSprite));
    nodes.push_back(static_cast<CCNode*>(m_title));
    nodes.push_back(static_cast<CCNode*>(scrollBg));
    nodes.push_back(static_cast<CCNode*>(listBg));
    nodes.push_back(static_cast<CCNode*>(restoreButton));
    nodes.push_back(static_cast<CCNode*>(m_closeBtn));
    nodes.push_back(static_cast<CCNode*>(idLabel));
    nodes.push_back(static_cast<CCNode*>(listLayer));
    nodes.push_back(static_cast<CCNode*>(scroll->m_contentLayer));
}

void SelectorLayer::onCallback() {
    showBackground();
}

void SelectorLayer::textChanged(CCTextInputNode* node) {
    updateSliders();
    hideBackground(node->getParent()->getPositionY());

    this->stopAllActions();
    this->runAction(CCSequence::create(CCDelayTime::create(1.f), CCCallFunc::create(this, callfunc_selector(SelectorLayer::onCallback)), nullptr));

    if (node == scaleInput->getInputNode()) {
        updateSlider(xScaleSlider, scaleInput, -3.f, 3.f);
        updateSlider(yScaleSlider, scaleInput, -3.f, 3.f);
        xScaleInput->setString(scaleInput->getString());
        yScaleInput->setString(scaleInput->getString());
    }

    if (node == xScaleInput->getInputNode()) {
        scaleSlider->setValue(xScaleSlider->getValue());
        scaleInput->setString(xScaleInput->getString());
    }

    if (node == rotationInput->getInputNode()) {
        updateSlider(rotationSlider, rotationInput, 0.f, 360.f);
        updateSlider(xRotationSlider, rotationInput, 0.f, 360.f);
        updateSlider(yRotationSlider, rotationInput, 0.f, 360.f);
        xRotationInput->setString(rotationInput->getString());
        yRotationInput->setString(rotationInput->getString());
    }

    if (node == xRotationInput->getInputNode()) {
        rotationSlider->setValue(xRotationSlider->getValue());
        rotationInput->setString(xRotationInput->getString());
    }

    updateNode();
}

void SelectorLayer::runFadeIn(CCNode* node) {
    int opacity = 255;
    if (ogOpacities.contains(node))
        opacity = ogOpacities.at(node);

    for (CCNode* child : CCArrayExt<CCNode*>(node->getChildren())) {
        if (node == static_cast<CCNode*>(scroll->m_contentLayer) && child->getPositionY() == slidingHeight)
            continue;
        runFadeIn(child);
    }

    node->runAction(CCFadeTo::create(0.2f, opacity));
}

void SelectorLayer::runFadeOut(CCNode* node) {
    if (!ogOpacities.contains(node)) {
        int ogOpacity = 255;
        if (CCRGBAProtocol* rgb = typeinfo_cast<CCRGBAProtocol*>(node))
            ogOpacity = rgb->getOpacity();
        ogOpacities[node] = ogOpacity;
    }

    for (CCNode* child : CCArrayExt<CCNode*>(node->getChildren())) {
        if (node == static_cast<CCNode*>(scroll->m_contentLayer) && child->getPositionY() == slidingHeight)
            continue;
        runFadeOut(child);
    }

    node->runAction(CCFadeTo::create(0.2f, 0));
}

void SelectorLayer::hideBackground(float y) {
    listLayer->m_list->m_tableView->m_contentLayer->runAction(
        CCSequence::create(
            CCDelayTime::create(0.2f),
            CCHide::create(),
            nullptr
        )
    );
    slidingHeight = y;
    for (CCNode* node : nodes) {
        runFadeOut(node);
    }
}

void SelectorLayer::showBackground() {
    listLayer->m_list->m_tableView->m_contentLayer->stopAllActions();
    listLayer->m_list->m_tableView->m_contentLayer->runAction(CCShow::create());
    for (CCNode* node : nodes) {
        runFadeIn(node);
    }
}

void SelectorLayer::sliderBegan(Slider* slider) {
    hideBackground(slider->getPositionY());
}

void SelectorLayer::sliderEnded(Slider*) {
    showBackground();
}

float SelectorLayer::getFloat(TextInput* input) {
    if (!input) return 0.f;
    return geode::utils::numFromString<float>(input->getString()).unwrapOr(0.f);
}

std::string SelectorLayer::getString(float value, bool integer = false, bool autoi = true) {
    if (autoi) integer = value - static_cast<int>(value) == 0.f;
    return integer ? std::to_string(static_cast<int>(std::round(value))) : fmt::format("{:.2f}", value);
}

float SelectorLayer::getSliderValue(float value, float min, float max) {
    value = std::clamp(value, min, max);
    return (value - min) / (max - min);
}

NodeProperties SelectorLayer::getProperties() {
    return {
        ccp(getFloat(xOffsetInput), getFloat(yOffsetInput)),
        ccp(getFloat(xScaleInput), getFloat(yScaleInput)),
        ccp(getFloat(xRotationInput), getFloat(yRotationInput)),
        ccp(getFloat(xSkewInput), getFloat(ySkewInput)),
        ccp(getFloat(xAnchorInput), getFloat(yAnchorInput)),
        ccp(getFloat(xSizeInput), getFloat(ySizeInput)),
        (double)getFloat(opacityInput),
        colorSprite->getColor(),
        visibleToggle ? visibleToggle->isToggled() : true,
        enableToggle ? enableToggle->isToggled() : true,
        static_cast<int>(getFloat(zOrderInput)),
        flipXToggle ? flipXToggle->isToggled() : false,
        flipYToggle ? flipYToggle->isToggled() : false
    };
}

void SelectorLayer::setProperties(NodeProperties props) {
    if (xOffsetInput) xOffsetInput->setString(getString(props.offset.x).c_str());
    if (yOffsetInput) yOffsetInput->setString(getString(props.offset.y).c_str());
    if (scaleInput) scaleInput->setString(getString(props.scale.x).c_str());
    if (xScaleInput) xScaleInput->setString(getString(props.scale.x).c_str());
    if (yScaleInput) yScaleInput->setString(getString(props.scale.y).c_str());
    if (rotationInput) rotationInput->setString(getString(props.rotation.x).c_str());
    if (xRotationInput) xRotationInput->setString(getString(props.rotation.x).c_str());
    if (yRotationInput) yRotationInput->setString(getString(props.rotation.y).c_str());
    if (xSkewInput) xSkewInput->setString(getString(props.skew.x).c_str());
    if (ySkewInput) ySkewInput->setString(getString(props.skew.y).c_str());
    if (xAnchorInput) xAnchorInput->setString(getString(props.anchor.x).c_str());
    if (yAnchorInput) yAnchorInput->setString(getString(props.anchor.y).c_str());
    if (xSizeInput) xSizeInput->setString(getString(props.size.x).c_str());
    if (ySizeInput) ySizeInput->setString(getString(props.size.y).c_str());
    if (opacityInput) opacityInput->setString(getString(props.opacity).c_str());
    if (visibleToggle) visibleToggle->toggle(props.visible);
    if (enableToggle) enableToggle->toggle(props.enabled);
    if (colorSprite) colorSprite->setColor(props.color);
    if (zOrderInput) zOrderInput->setString(std::to_string(props.zOrder).c_str());
    if (flipXToggle) flipXToggle->toggle(props.flipX);
    if (flipYToggle) flipYToggle->toggle(props.flipY);

    updateSliders();
}

void SelectorLayer::updateListColors() {
    cocos2d::ccColor3B color = ccc3(72, 91, 181);
	cocos2d::ccColor3B color1 = ccc3(std::max(0, color.r - 70), std::max(0, color.g - 70), std::max(0, color.b - 70));
	cocos2d::ccColor3B color2 = ccc3(std::max(0, color.r - 55), std::max(0, color.g - 55), std::max(0, color.b - 55));

	int it = 0;
    for (GenericListCell* cell : CCArrayExt<GenericListCell*>(contentLayer->getChildren())) {
        cell->m_backgroundLayer->stopAllActions();
		cell->m_backgroundLayer->setColor((it % 2 == 0) ? color1 : color2);
        it++;
        if (Cell* cell2 = cell->getChildByType<Cell>(0))
            cell2->editButton != nullptr ? cell2->editButton->setEnabled(true) : void();
	}
}

void SelectorLayer::updateSlider(Slider* slider, TextInput* input, float min, float max) {
    if (!slider || !input) return;
    float value = getFloat(input);
    slider->setValue(getSliderValue(value, min, max));
    slider->updateBar();
}

void SelectorLayer::updateScale() {
    CCObject* obj = static_cast<CCObject*>(scaleSlider->getThumb());
    updateInput(obj, scaleSlider, xScaleInput, -3.f, 3.f, false);
    updateInput(obj, scaleSlider, yScaleInput, -3.f, 3.f, false);
    updateSlider(xScaleSlider, xScaleInput, -3.f, 3.f);
    updateSlider(yScaleSlider, yScaleInput, -3.f, 3.f);
}

void SelectorLayer::updateRotation() {
    CCObject* obj = static_cast<CCObject*>(rotationSlider->getThumb());
    updateInput(obj, rotationSlider, xRotationInput, 0.f, 360.f, true);
    updateInput(obj, rotationSlider, yRotationInput, 0.f, 360.f, true);
    updateSlider(xRotationSlider, xRotationInput, 0.f, 360.f);
    updateSlider(yRotationSlider, yRotationInput, 0.f, 360.f);
}

void SelectorLayer::updateInput(CCObject* obj, Slider* slider, TextInput* input, float min, float max, bool integer = false) {
    if (!slider || !input) return;
    if (static_cast<SliderThumb*>(obj) != slider->getThumb()) return;

    float value = slider->getValue();
    float inputValue = min + value * (max - min);

    std::string string = getString(inputValue, integer, false);
    input->setString(string.c_str());

    if (input == scaleInput) updateScale();
    if (input == rotationInput) updateRotation();

    if (slider == xScaleSlider) {
        scaleSlider->setValue(xScaleSlider->getValue());
        scaleInput->setString(xScaleInput->getString());
    }
    if (slider == xRotationSlider) {
        rotationSlider->setValue(xRotationSlider->getValue());
        rotationInput->setString(xRotationInput->getString());
    }
}

void SelectorLayer::updateNode() {
    CCNode* node = Customizer::getNode(layer, selectedNode);
    selectedProps = getProperties();
    Customizer::setNodeProperties(node, selectedNode, selectedProps);
    Customizer::saveProperties(selectedNode.id, selectedProps);
}

void SelectorLayer::updateSliders() {
    updateSlider(xOffsetSlider, xOffsetInput, -500.f, 500.f);
    updateSlider(yOffsetSlider, yOffsetInput, -500.f, 500.f);
    updateSlider(scaleSlider, scaleInput, -3.f, 3.f);
    updateSlider(xScaleSlider, xScaleInput, -3.f, 3.f);
    updateSlider(yScaleSlider, yScaleInput, -3.f, 3.f);
    updateSlider(rotationSlider, rotationInput, 0.f, 360.f);
    updateSlider(xRotationSlider, xRotationInput, 0.f, 360.f);
    updateSlider(yRotationSlider, yRotationInput, 0.f, 360.f);
    updateSlider(xSkewSlider, xSkewInput, -30.f, 30.f);
    updateSlider(ySkewSlider, ySkewInput, -30.f, 30.f);
    updateSlider(xAnchorSlider, xAnchorInput, 0.f, 1.f);
    updateSlider(yAnchorSlider, yAnchorInput, 0.f, 1.f);
    updateSlider(xSizeSlider, xSizeInput, -500.f, 500.f);
    updateSlider(ySizeSlider, ySizeInput, -500.f, 500.f);
    updateSlider(opacitySlider, opacityInput, 0.f, 100.f);
}

void SelectorLayer::updateInputs(CCObject* obj) {
    updateInput(obj, xOffsetSlider, xOffsetInput, -500.f, 500.f, true);
    updateInput(obj, yOffsetSlider, yOffsetInput, -500.f, 500.f, true);
    updateInput(obj, scaleSlider, scaleInput, -3.f, 3.f);
    updateInput(obj, xScaleSlider, xScaleInput, -3.f, 3.f);
    updateInput(obj, yScaleSlider, yScaleInput, -3.f, 3.f);
    updateInput(obj, rotationSlider, rotationInput, 0.f, 360.f, true);
    updateInput(obj, xRotationSlider, xRotationInput, 0.f, 360.f, true);
    updateInput(obj, yRotationSlider, yRotationInput, 0.f, 360.f, true);
    updateInput(obj, xSkewSlider, xSkewInput, -30.f, 30.f);
    updateInput(obj, ySkewSlider, ySkewInput, -30.f, 30.f);
    updateInput(obj, xAnchorSlider, xAnchorInput, 0.f, 1.f);
    updateInput(obj, yAnchorSlider, yAnchorInput, 0.f, 1.f);
    updateInput(obj, xSizeSlider, xSizeInput, -500.f, 500.f, true);
    updateInput(obj, ySizeSlider, ySizeInput, -500.f, 500.f, true);
    updateInput(obj, opacitySlider, opacityInput, 0.f, 100.f, true);

    updateNode();
}

void SelectorLayer::updateColor(const cocos2d::ccColor4B&) {
    updateNode();
}

void SelectorLayer::onToggle(CCObject*) {
    Loader::get()->queueInMainThread([this] {
        this->updateNode();
    });
}

void SelectorLayer::openColorPicker(CCObject*) {
    ColorPickPopup* popup = ColorPickPopup::create(colorSprite->getColor());
    popup->setColorTarget(colorSprite);
    popup->setDelegate(this);
    popup->show();
}

void SelectorLayer::onRestoreAll(CCObject*) {
    geode::createQuickPopup(
        "Warning",
        "<cr>Restore</c> all defaults?",
        "Cancel", "Yes",
        [this](auto, bool btn2) {
            if (!btn2) return;
            selectAnimation();
            onRestoreSelected(nullptr);
            for (Node info : selectedNodes) {
                CCNode* node = Customizer::getNode(layer, info);
                NodeProperties props = Customizer::getDefaultsFor(info.id);
                props.offset = ccp(0, 0);
                Customizer::setNodeProperties(node, info, props);
                Customizer::saveProperties(info.id, props);
                matjson::Value empty;
                Mod::get()->setSavedValue(info.id, empty);
            }
        }
    );
}

void SelectorLayer::onRestoreSelected(CCObject*) {
    selectAnimation();
    NodeProperties props = Customizer::getDefaultsFor(selectedNode.id);
    props.offset = ccp(0, 0);
    setProperties(props);
    updateNode();
    matjson::Value empty;
    Mod::get()->setSavedValue(selectedNode.id, empty);
    hideBackground(1.f);
    this->runAction(CCSequence::create(CCDelayTime::create(0.8f), CCCallFunc::create(this, callfunc_selector(SelectorLayer::onCallback)), nullptr));
}

void SelectorLayer::onRandom(CCObject*) {
    NodeProperties props = Customizer::getRandomProps();

    setProperties(props);
    updateNode();

    hideBackground(1.f);
    this->runAction(CCSequence::create(CCDelayTime::create(0.8f), CCCallFunc::create(this, callfunc_selector(SelectorLayer::onCallback)), nullptr));
}

void SelectorLayer::onSettings(CCObject*) {
	geode::openSettingsPopup(Mod::get(), false);
}