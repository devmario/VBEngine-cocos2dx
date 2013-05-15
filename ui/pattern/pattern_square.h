//
//  pattern_square.h
//  VBEngine-cocos2dx
//
//  Created by wonhee jang on 13. 5. 14..
//  Copyright (c) 2013년 vanillabreeze. All rights reserved.
//

#ifndef __VBEngine_cocos2dx__pattern_square__
#define __VBEngine_cocos2dx__pattern_square__

#include <iostream>
#include "VBModel.h"

class pattern_square : public VBModel {
private:
	CCTexture2D* tex_grid;
	
	CCSize size_pattern;
	
	typedef struct library {
		VBObjectFile2DLibraryBitmap* bitmap;
		VBAABB vtx;
		VBAABB uv;
	} library;
	
	void setup_library(library* _lib, VBObjectFile2D* _obj, std::string _name, std::string _where);
	
	struct {
		library l, t, r, b, lt, lb, rt, rb, c;
	} grid;
	
	int drawable_len;
	ccV3F_C4B_T2F* drawable;
	
	void update_vtx();
	ccV3F_C4B_T2F* setup_vtx(ccV3F_C4B_T2F* _drawable, library* _lib, VBAABB _aabb);
	
public:
	pattern_square(VBObjectFile2D* _obj, CCTexture2D* _tex, std::string _name, CCSize _size_pattern);
	virtual ~pattern_square();
	
	void set_size(CCSize _size_pattern);
	CCSize get_size();
	
	virtual void draw(void);
};

#endif /* defined(__VBEngine_cocos2dx__pattern_square__) */
