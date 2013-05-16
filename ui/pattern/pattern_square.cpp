//
//  pattern_square.cpp
//  VBEngine-cocos2dx
//
//  Created by wonhee jang on 13. 5. 14..
//  Copyright (c) 2013년 vanillabreeze. All rights reserved.
//

#include "pattern_square.h"

void pattern_square::setup_library(library* _lib, VBObjectFile2D* _obj, std::string _name, std::string _where) {
	std::stringstream _stream;
	if(_where != "")
		_stream << _name << "_" << _where;
	else
		_stream << _name;
	_stream  << ".png";
	
	VBString* _str = VBStringInitWithCString(VBStringAlloc(), _stream.str().c_str());
	_lib->bitmap = VBObjectFile2DGetLibraryBitmapByName(_obj, _str);
	VBStringFree(&_str);
	assert(_lib->bitmap != NULL);
	
	_lib->uv = VBAABBCreateWithVertex(_lib->bitmap->uv_info, 4);
	_lib->vtx = VBAABBCreate(0, 0, VBAABBGetWidth(_lib->uv) * tex_grid->getContentSizeInPixels().width, VBAABBGetHeight(_lib->uv) * tex_grid->getContentSizeInPixels().height);
}

pattern_square::margin pattern_square::get_margin() {
	margin _margin;
	
	_margin.l = VBAABBGetWidth(grid.l.vtx);
	_margin.t = VBAABBGetHeight(grid.t.vtx);
	_margin.r = VBAABBGetWidth(grid.r.vtx);
	_margin.b = VBAABBGetHeight(grid.b.vtx);
	_margin.w = size_pattern.width - (_margin.l + _margin.r);
	_margin.h = size_pattern.height - (_margin.t + _margin.b);
	
	return _margin;
}

void pattern_square::update_vtx() {
	drawable_len = 6 * 9;
	if(drawable == NULL) {
		drawable = new ccV3F_C4B_T2F[drawable_len];
		memset(drawable, 0x00, sizeof(ccV3F_C4B_T2F) * drawable_len);
	}
	ccV3F_C4B_T2F* _ptr = drawable;
	
	margin _margin = get_margin();
	
	_ptr = setup_vtx(_ptr, &grid.lt, 	VBAABBCreate(0						,0						,_margin.l 					,_margin.t));
	_ptr = setup_vtx(_ptr, &grid.t, 	VBAABBCreate(_margin.l				,0						,_margin.w 					,_margin.t));
	_ptr = setup_vtx(_ptr, &grid.rt, 	VBAABBCreate(_margin.l + _margin.w	,0						,_margin.r 					,_margin.t));
	
	_ptr = setup_vtx(_ptr, &grid.l, 	VBAABBCreate(0						,_margin.t				,_margin.l 					,_margin.h));
	_ptr = setup_vtx(_ptr, &grid.c, 	VBAABBCreate(_margin.l				,_margin.t				,_margin.w 					,_margin.h));
	_ptr = setup_vtx(_ptr, &grid.r, 	VBAABBCreate(_margin.l + _margin.w	,_margin.t				,_margin.r 					,_margin.h));
	
	_ptr = setup_vtx(_ptr, &grid.lb, 	VBAABBCreate(0						,_margin.t + _margin.h	,_margin.l 					,_margin.b));
	_ptr = setup_vtx(_ptr, &grid.b, 	VBAABBCreate(_margin.l				,_margin.t + _margin.h	,_margin.w 					,_margin.b));
	_ptr = setup_vtx(_ptr, &grid.rb, 	VBAABBCreate(_margin.l + _margin.w	,_margin.t + _margin.h	,_margin.r 					,_margin.b));
}

ccV3F_C4B_T2F* pattern_square::setup_vtx(ccV3F_C4B_T2F* _drawable, library* _lib, VBAABB _aabb) {
	ccV3F_C4B_T2F* _input = _drawable;
	_drawable++;
	
	_drawable->vertices.x = _aabb.l;
	_drawable->vertices.y = -_aabb.t;
	_drawable->texCoords.u = _lib->uv.l;
	_drawable->texCoords.v = _lib->uv.t;
	_drawable++;
	
	_drawable->vertices.x = _aabb.r + _aabb.l;
	_drawable->vertices.y = -_aabb.t;
	_drawable->texCoords.u = _lib->uv.r;
	_drawable->texCoords.v = _lib->uv.t;
	_drawable++;
	
	_drawable->vertices.x = _aabb.l;
	_drawable->vertices.y = -(_aabb.b + _aabb.t);
	_drawable->texCoords.u = _lib->uv.l;
	_drawable->texCoords.v = _lib->uv.b;
	_drawable++;
	
	_drawable->vertices.x = _aabb.r + _aabb.l;
	_drawable->vertices.y = -(_aabb.b + _aabb.t);
	_drawable->texCoords.u = _lib->uv.r;
	_drawable->texCoords.v = _lib->uv.b;
	
	memcpy(_input, _input + 1, sizeof(ccV3F_C4B_T2F));
	memcpy(_input + 5, _input + 4, sizeof(ccV3F_C4B_T2F));
	
	return _input + 6;
}

pattern_square::pattern_square(VBObjectFile2D* _obj, CCTexture2D* _tex, std::string _name, CCSize _size_pattern) : VBModel() {
	tex_grid = _tex;
	size_pattern = CCSizeZero;
	drawable = NULL;
	drawable_len = 0;
	
	setup_library(&grid.lt, _obj, _name, "lt");
	setup_library(&grid.t, _obj, _name, "t");
	setup_library(&grid.rt, _obj, _name, "rt");
	
	setup_library(&grid.l, _obj, _name, "l");
	setup_library(&grid.c, _obj, _name, "");
	setup_library(&grid.r, _obj, _name, "r");
	
	setup_library(&grid.lb, _obj, _name, "lb");
	setup_library(&grid.b, _obj, _name, "b");
	setup_library(&grid.rb, _obj, _name, "rb");
	
	set_size(_size_pattern);
}

pattern_square::~pattern_square() {
	if(drawable)
		delete[] drawable;
}

void pattern_square::set_size(CCSize _size_pattern) {
	if(size_pattern.equals(_size_pattern))
		return;
	size_pattern = _size_pattern;
	update_vtx();
	
}

CCSize pattern_square::get_size() {
	return size_pattern;
}

void pattern_square::draw(void) {
	if(drawable && drawable_len) {
		CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
		
		CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
		
		CC_NODE_DRAW_SETUP();
		
		ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
		
		if (tex_grid != NULL) {
			ccGLBindTexture2D( tex_grid->getName() );
		} else {
			ccGLBindTexture2D(0);
		}
		
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
		
		for(int i = 0; i < drawable_len; i++)
			drawable[i].colors = (ccColor4B){mix_color.r, mix_color.g, mix_color.b, mix_color.a};
		glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(ccV3F_C4B_T2F), &drawable->vertices);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV3F_C4B_T2F), &drawable->texCoords);
		glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV3F_C4B_T2F), &drawable->colors);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, drawable_len);
		
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
		
		CHECK_GL_ERROR_DEBUG();
		
		
#if CC_SPRITE_DEBUG_DRAW == 1
		// draw bounding box
		CCPoint vertices[4]={
			ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
			ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
			ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
			ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
		};
		ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
		// draw texture box
		CCSize s = this->getTextureRect().size;
		CCPoint offsetPix = this->getOffsetPosition();
		CCPoint vertices[4] = {
			ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
			ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
		};
		ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
		
		CC_INCREMENT_GL_DRAWS(1);
		
		CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
	}
}