#include <billengine.h>

bill::BillEngine::BillEngine() {
    starttime = glutGet(GLUT_ELAPSED_TIME);
    now = starttime;
    set = nullptr;
}

bill::BillEngine::BillEngine(BillSetOfPoints &set) {
    starttime = glutGet(GLUT_ELAPSED_TIME);
    now = starttime;
    this->set = &set;
    this->timestep = timestep;
}

void bill::BillEngine::step() {
    if (set != nullptr) {
        now = glutGet(GLUT_ELAPSED_TIME);
        if ((now - starttime) > 5) {
            set->Go();
            starttime = now;
        }
    }
}
