#include "VBModel.h"

VBColorRGBA VBModel::GetColor() {
    return color;
}

void VBModel::SetColor(VBColorRGBA _color) {
    color = _color;
}

unsigned char VBModel::GetAlpha() {
    return color.a;
}

void VBModel::SetAlpha(unsigned char _alpha) {
    color.a = _alpha;
}

void VBModel::SetAlphaRecur(unsigned char _alpha) {
    if (is_bitmap) {
        color.a = _alpha;
    } else if (frame_all_allocated_child_models && VBArrayVectorGetLength(frame_all_allocated_child_models)) {
        color.a = _alpha;
        for (int i=0; i<VBArrayVectorGetLength(frame_all_allocated_child_models); i++) {
            ((VBModel*)VBArrayVectorGetDataAt(frame_all_allocated_child_models, i))->SetAlphaRecur(_alpha);
        }
    }
}

void VBModel::SetBrightness(unsigned char _brightness) {
    color.r = _brightness;
    color.g = _brightness;
    color.b = _brightness;
}

unsigned char VBModel::GetBrightness() {
    return color.r;
}

void VBModel::SetUseMixColor(bool _value) {
	use_mix_color = _value;
}