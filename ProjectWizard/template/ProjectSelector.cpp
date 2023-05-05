#include <scenes/ProjectSelector.h>
#include "SceneTemplate.h"

namespace ProjectSelector {
	Scene* getProjectScene() {
		return new SceneTemplate();
	}
}