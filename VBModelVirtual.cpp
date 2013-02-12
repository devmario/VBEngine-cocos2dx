#include "VBModel.h"

bool VBModel::init(void) {
	bool _ret = CCSprite::init();
	
	if(!_ret)
		return false;
	
    is_bitmap = false;
    
    color = mix_color = VBColorRGBALoadIdentity();
    
    is_real_time_animation = false;
    is_animation_update = false;
    is_use_animation = false;
    is_play_loop = false;
    is_play = false;
    frame_rate = 0.0f;
    update_frame = 0.0f;
    cur_frame = 0.0f;
    
    mat = VBMatrix2DWrapperLoadIdentity();
    
    model_parent = NULL;
    
    frame = NULL;
    
    frame_all_allocated_child_models = NULL;
    frame_willFree_child_models = NULL;
    frame_current_key_frame = NULL;
    
    is_exist_empty_frame = false;
    empty_frame = 0.0f;
    
    setAnchorPoint(ccp(0,0));
	
	return true;
}

void VBModel::setTextureRect(const cocos2d::CCRect &rect, bool rotated, const cocos2d::CCSize &untrimmedSize) {
    
    m_bRectRotated = rotated;
    
    setContentSize(untrimmedSize);
    
    m_obRect = CC_RECT_PIXELS_TO_POINTS(rect);
    updateTextureCoords(m_obRect);
    
    CCPoint relativeOffset = m_obUnflippedOffsetPositionFromCenter;
    
    if (m_bFlipX) {
        relativeOffset.x = -relativeOffset.x;
    }
    if (m_bFlipY) {
        relativeOffset.y = -relativeOffset.y;
    }
    
	if(m_pobTexture) {
		m_obOffsetPosition.x = relativeOffset.x + (m_pobTexture->getContentSize().width - m_obRect.size.width) / 2;
		m_obOffsetPosition.y = relativeOffset.y + (m_pobTexture->getContentSize().height - m_obRect.size.height) / 2;
	} else {
		m_obOffsetPosition.x = relativeOffset.x + (0 - m_obRect.size.width) / 2;
		m_obOffsetPosition.y = relativeOffset.y + (0 - m_obRect.size.height) / 2;
	}
    
    if (m_pobBatchNode) {
        setDirty(true);
    } else {
		m_sQuad.bl.vertices = vertex3(0, 0, 0);
		m_sQuad.br.vertices = vertex3(m_obRect.size.width, 0, 0);
		m_sQuad.tl.vertices = vertex3(0, -m_obRect.size.height, 0);
		m_sQuad.tr.vertices = vertex3(m_obRect.size.width, -m_obRect.size.height, 0);
    }
}

void VBModel::updateTextureCoords(const CCRect& rect) {
    CCTexture2D *tex = m_pobBatchNode ? m_pobTextureAtlas->getTexture() : m_pobTexture;
	if(!tex)
		return;
    
	float atlasWidth = (float)tex->getPixelsWide();
	float atlasHeight = (float)tex->getPixelsHigh();
    
	float left, right, top, bottom;
    
    left	= rect.origin.x/atlasWidth;
    right	= left + rect.size.width/atlasWidth;
    top		= rect.origin.y/atlasHeight;
    bottom	= top + rect.size.height/atlasHeight;
    
    m_sQuad.bl.texCoords.u = left;
    m_sQuad.bl.texCoords.v = top;
    m_sQuad.br.texCoords.u = right;
    m_sQuad.br.texCoords.v = top;
    m_sQuad.tl.texCoords.u = left;
    m_sQuad.tl.texCoords.v = bottom;
    m_sQuad.tr.texCoords.u = right;
    m_sQuad.tr.texCoords.v = bottom;
}

CCAffineTransform VBModel::nodeToParentTransform(void) {
	if (m_bTransformDirty) {
        mat = VBMatrix2DWrapperLoadIdentity();
		
        mat = VBMatrix2DWrapperSetPosition(mat, VBVector2DCreate(m_obPosition.x, m_obPosition.y));
        mat = VBMatrix2DWrapperSetScale(mat, VBVector2DCreate(m_fScaleX, m_fScaleY));
		//m_fRotationX, m_fRotationY 적용필요
        if(isnan(m_fRotationX)) {
            mat = VBMatrix2DWrapperSetShear(mat, VBVector2DCreate(-m_fSkewX, -m_fSkewY));
            mat = VBMatrix2DWrapperSetRotation(mat, NAN);
        } else {
            mat = VBMatrix2DWrapperSetRotation(mat, -m_fRotationX);
        }
        mat = VBMatrix2DWrapperSetAnchor(mat, VBVector2DCreate(m_obAnchorPoint.x, m_obAnchorPoint.y));
        mat = VBMatrix2DWrapperUpdate(mat);
        
		m_sTransform = CCAffineTransformMake(mat.mat.m11, mat.mat.m21, mat.mat.m12, mat.mat.m22, mat.mat.m13, mat.mat.m23);
        
		m_bTransformDirty = false;
	}
    
	return m_sTransform;
}