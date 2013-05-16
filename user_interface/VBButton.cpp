#include "VBButton.h"

VBButton::VBButton(VBModel* _model,
				   VBButtonProtocol* _protocol,
				   std::string _responder_tag,
				   bool _is_always_event) :
VBResponder(_responder_tag) {
	act_enable = true;
    protocol = _protocol;
	is_always_event = _is_always_event;
	set_model(_model);
}

VBButton::~VBButton() {
}

void VBButton::set_model(VBModel* _model) {
    model = _model;
    model_area = _model->GetChildByInstanceName("area");
    if(!model_area)
        model_area = model;
    touch = NULL;
}

bool VBButton::GetIsDown() {
    return touch != NULL;
}

CCTouch* VBButton::GetTouch() {
    return touch;
}

VBModel* VBButton::GetModel() {
	return model;
}

void VBButton::SetActEnable(bool _enable) {
	act_enable = _enable;
	touch = NULL;
}

void VBButton::TouchBegin(CCTouch* _touch) {
	if(!act_enable)
		return;
	
	VBResponder::TouchBegin(_touch);
    if(!protocol)
        return;
    if(!model)
        return;
	if(is_always_event) {
		touch = _touch;
		protocol->ButtonTouchDown(this, model_area->IsHitByPoint(_touch->getLocationInView()));
	} else {
		if(touch)
			return;
		bool _hit = model_area->IsHitByPoint(_touch->getLocationInView());
		if(_hit)
			touch = _touch;
		protocol->ButtonTouchDown(this, _hit);
	}
}

void VBButton::TouchEnd(CCTouch* _touch) {
	if(!act_enable)
		return;
	
	VBResponder::TouchEnd(_touch);
    if(!protocol)
        return;
    if(!model)
        return;
	if(is_always_event) {
		protocol->ButtonTouchUp(this, model_area->IsHitByPoint(_touch->getLocationInView()));
	} else {
		if(_touch != touch)
			return;
		protocol->ButtonTouchUp(this, model_area->IsHitByPoint(_touch->getLocationInView()));
	}
	touch = NULL;
}