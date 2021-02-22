#include "../Public/PrimitiveComponent.hpp"
namespace ez
{
    void PrimitiveComponent::setMesh(Mesh mesh) 
    {
        _mesh = mesh;
        _mesh.mvpObject.model =ez::rotate(makeIdentity<float, 4>(), ez::radians(0.0f) * 1, Vector3f(0.0f, 0.0f, 1.0f));
        if (_imageResource)
            _mesh.primitive.texture.loadImage(_imageResource);
        // if (_objResource)
        //     _mesh.primitive.loadShape(_objResource->getInfo(), _objResource->getShape());
    }

    void PrimitiveComponent::setImageResource(ImageResource *imageResource)
    {
        _imageResource = imageResource;
        _mesh.primitive.texture.loadImage(_imageResource);
    }

    void PrimitiveComponent::setOBJResource(OBJResource *objResource)
    {
        _objResource = objResource;
        _mesh.primitive.loadShape(_objResource->getInfo(), _objResource->getShape());
    }
}