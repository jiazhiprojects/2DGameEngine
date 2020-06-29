#include "./Animation.h"

Animation::Animation() {

}

Animation::Animation(unsigned int index, unsigned int numFrames, unsigned int animSpeed) {
    this->index = index;
    this->numFrames = numFrames;
    this->animSpeed = animSpeed;
}