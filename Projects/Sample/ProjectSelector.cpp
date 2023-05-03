#include <scenes/ProjectSelector.h>
#include "SceneSample3D.h"

namespace ProjectSelector {
	Scene* getProjectScene() {
		return new SceneSample3D();
	}
}