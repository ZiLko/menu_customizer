
#include "includes.hpp"
#include "customizer.hpp"
#include "selector_layer.hpp"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>



class $modify(MenuLayer) {

	bool init() {
		if (!MenuLayer::init()) return false;

		Customizer::applyLayer(static_cast<CCNode*>(this), menuLayerNodes);

		return true;
	}

	void onMoreGames(CCObject*) {
		SelectorLayer::create(static_cast<CCNode*>(this), menuLayerNodes)->show();
	}
};

class $modify(PauseLayer) {

	void customSetup() {
		PauseLayer::customSetup();

		// Customizer::applyLayer(static_cast<CCNode*>(this), pauseLayerNodes);
	}
};