
#include "includes.hpp"
#include "customizer.hpp"
#include "selector_layer.hpp"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>

#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;

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
