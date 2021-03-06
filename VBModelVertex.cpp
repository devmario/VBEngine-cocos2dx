#include "VBModel.h"

VBAABB VBModel::GetAABB(bool _world, bool _recursion, CCAffineTransform _t) {
    VBAABB _aabb = VBAABBLoadIndentity();
    CCAffineTransform wt;
    
    if(_world)
        wt = nodeToWorldTransform();
    else {
        wt = nodeToParentTransform();
        wt = CCAffineTransformConcat(wt, _t);
    }
    
    if(is_bitmap) {
        CCPoint tl = CCPointApplyAffineTransform(CCPointMake(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y), wt);
        CCPoint tr = CCPointApplyAffineTransform(CCPointMake(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y), wt);
        CCPoint bl = CCPointApplyAffineTransform(CCPointMake(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y), wt);
        CCPoint br = CCPointApplyAffineTransform(CCPointMake(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y), wt);
        
        VBVector2D vtx[4] = {{tl.x,tl.y}, {tr.x,tr.y}, {bl.x,bl.y}, {br.x,br.y}};
        _aabb = VBAABBMerge(_aabb, VBAABBCreateWithVertex(vtx, 4));
    }
    if(_recursion) {
        if(getChildren()) {
            for(int i = 0; i < getChildren()->count(); i++) {
                _aabb = VBAABBMerge(_aabb, ((VBModel*)getChildren()->objectAtIndex(i))->GetAABB(_world, _recursion, wt));
            }
        }
    }
    return _aabb;
}


int VBModel::GetVertex(int _idx, CCPoint* _vertex, bool _recursion) {
    if(is_bitmap) {
        if(_idx == 0) {
            CCAffineTransform wt = nodeToWorldTransform();
            CCPoint tl = CCPointApplyAffineTransform(ccp(m_sQuad.tl.vertices.x, m_sQuad.tl.vertices.y), wt);
            CCPoint tr = CCPointApplyAffineTransform(ccp(m_sQuad.tr.vertices.x, m_sQuad.tr.vertices.y), wt);
            CCPoint bl = CCPointApplyAffineTransform(ccp(m_sQuad.bl.vertices.x, m_sQuad.bl.vertices.y), wt);
            CCPoint br = CCPointApplyAffineTransform(ccp(m_sQuad.br.vertices.x, m_sQuad.br.vertices.y), wt);
            _vertex[0] = tl;
            _vertex[1] = tr;
            _vertex[2] = bl;
            _vertex[3] = br;
            
            //성공시 -1 리턴
            return -1;
        }
        _idx--;
    }
    
    if(_recursion) {
        if(getChildren()) {
            for(int i = 0; i < getChildren()->count(); i++) {
                _idx = ((VBModel*)getChildren()->objectAtIndex(i))->GetVertex(_idx, _vertex, _recursion);
				if(_idx == -1)
					return _idx;
            }
        }
    }
    
    return _idx;
}

float VBModel::GetWidth(bool _recursion) {
    return VBAABBGetWidth(GetAABB(_recursion));
}

float VBModel::GetHeight(bool _recursion) {
    return VBAABBGetHeight(GetAABB(_recursion));
}
