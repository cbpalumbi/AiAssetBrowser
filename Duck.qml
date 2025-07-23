import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    property url textureData: "maps/textureData.png"
    Texture {
        id: _0_texture
        minFilter: Texture.Nearest
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData
    }
    PrincipledMaterial {
        id: blinn3_fx_material
        objectName: "blinn3-fx"
        baseColorMap: _0_texture
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Node {
        id: nodes_0_
        objectName: "nodes[0]"
        //scale: Qt.vector3d(100, 100, 100)
        Model {
            id: nodes_2_
            objectName: "nodes[2]"
            source: "meshes/lod3spShape_mesh.mesh"
            materials: [
                blinn3_fx_material
            ]
        }
    }

    // Animations:
}
