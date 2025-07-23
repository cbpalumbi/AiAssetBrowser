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
        scale: Qt.vector3d(1, 1, 1)
        Model {
            id: nodes_2_
            objectName: "nodes[2]"
            source: "meshes/lod3spShape_mesh.mesh"
            materials: [
                blinn3_fx_material
            ]
        }
        // PerspectiveCamera {
        //     id: nodes_1__camera
        //     objectName: "nodes[1]"
        //     position: Qt.vector3d(400.113, 463.264, -431.078)
        //     rotation: Qt.quaternion(0.347648, -0.121064, 0.878059, 0.305773)
        //     scale: Qt.vector3d(1, 1, 1)
        //     clipNear: 1
        //     fieldOfView: 54.43206024169922
        //     fieldOfViewOrientation: PerspectiveCamera.Horizontal
        // }
    }

    // Animations:
}
