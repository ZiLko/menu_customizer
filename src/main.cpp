
#include "includes.hpp"
#include "customizer.hpp"
#include "selector_layer.hpp"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>

#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;

template <typename T>
bool isInScene() {
	T* ptr = CCDirector::sharedDirector()->getRunningScene()->getChildByType<T>(0);
	return ptr != nullptr;
}

$execute {
    BindManager::get()->registerBindable({
        "menu"_spr,
        "Open Menu",
        "Opens menu.",
        { Keybind::create(KEY_M, Modifier::Alt) },
        "Menu Customizer",
		false
    });
};

class $modify(MenuLayer) {

	bool init() {
		if (!MenuLayer::init()) return false;

		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {

			if (event->isDown())
				SelectorLayer::create(static_cast<CCNode*>(this), menuLayerNodes)->show();

			return ListenerResult::Propagate;
		}, "menu"_spr);

		if (!Mod::get()->getSettingValue<bool>("disable"))
			Customizer::applyLayer(static_cast<CCNode*>(this), menuLayerNodes);

		return true;
	}
};

class $modify(PauseLayer) {

	void customSetup() {
		PauseLayer::customSetup();

		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
			
			if (event->isDown())
				SelectorLayer::create(static_cast<CCNode*>(this), pauseLayerNodes)->show();

			return ListenerResult::Propagate;
		}, "menu"_spr);

		if (!Mod::get()->getSettingValue<bool>("disable"))
			Customizer::applyLayer(static_cast<CCNode*>(this), pauseLayerNodes);
	}
	
};

class $modify(CreatorLayer) {

	bool init() {
		if (!CreatorLayer::init()) return false;

		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {

			if (event->isDown() && isInScene<CreatorLayer>())
				SelectorLayer::create(static_cast<CCNode*>(this), creatorLayerNodes)->show();

			return ListenerResult::Propagate;
		}, "menu"_spr);

		if (!Mod::get()->getSettingValue<bool>("disable"))
			Customizer::applyLayer(static_cast<CCNode*>(this), creatorLayerNodes);

		return true;
	}
};

class $modify(LevelSearchLayer) {

	bool init(int a) {
		if (!LevelSearchLayer::init(a)) return false;

		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {

			if (event->isDown() && isInScene<LevelSearchLayer>())
				SelectorLayer::create(static_cast<CCNode*>(this), levelSearchLayerNodes)->show();

			return ListenerResult::Propagate;
		}, "menu"_spr);

		if (!Mod::get()->getSettingValue<bool>("disable"))
			Customizer::applyLayer(static_cast<CCNode*>(this), levelSearchLayerNodes);

		return true;
	}
};