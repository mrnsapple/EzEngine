
#pragma once

#include "SceneComponent.hpp"
#include "Model.hpp"
#include "ImageResource.hpp"
#include "OBJResource.hpp"
#ifndef PRIMITIVECOMPONENT_HPP
#define PRIMITIVECOMPONENT_HPP
namespace ez{
class PrimitiveComponent : public SceneComponent
{
public:
    //bool isType() override;
    //void Update() override;
    virtual bool OnEvent(Event& event) { return false; };
    virtual bool ShouldCreateRenderState() const  { return true; }
    void setMesh(Mesh mesh);// { _mesh = mesh; }
    Mesh* getMesh() { return &_mesh; }
    void setImageResource   (ImageResource *imageResource);
    void setOBJResource     (OBJResource *objResource);

protected:
    Mesh    _mesh;
    ImageResource *_imageResource;
    OBJResource *_objResource;
};
}
#endif