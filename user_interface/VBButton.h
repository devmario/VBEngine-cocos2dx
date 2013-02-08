#ifndef VBButton_h
#define VBButton_h

#include "VBModel.h"
#include "VBResponder.h"

class VBButton;

class VBButtonProtocol {
public:
    virtual void ButtonTouchDown(VBButton* _button, bool _is_inside) = 0;
    virtual void ButtonTouchUp(VBButton* _button, bool _is_inside) = 0;
};

class VBButton : public VBResponder {
protected:
    VBButtonProtocol* protocol;
    VBModel* model;
    VBModel* model_area;
    CCTouch* touch;
	bool is_always_event;
	bool act_enable;
    
public:
    VBButton(VBModel* _model,
			 VBButtonProtocol* _protocol,
			 std::string _responder_tag = "",
			 bool _is_always_event = false);
    virtual ~VBButton();
	
	void SetActEnable(bool _enable) {
		act_enable = _enable;
	}
	
	bool GetActEnable() {
		return act_enable;
	}
    
    bool GetIsDown();
    CCTouch* GetTouch();
	VBModel* GetModel();
    
    virtual void TouchBegin(CCTouch* _touch);
    virtual void TouchEnd(CCTouch* _touch);
};

#endif