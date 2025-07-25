import QtQuick
import QtQuick3D


Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Simple 3D Scene")

    // Create a view and 3D scene
    View3D {
        anchors.fill: parent
        camera: activeCamera
        PerspectiveCamera {
            id: activeCamera
            z: 400
        }

        DirectionalLight {
            color: "white"
        }

        Model {
            x: -100
            source: "file:///C:/Users/Bella/Documents/AssetBrowserProjectAssets/beetle_car/beetle_car.qml"
            materials: PrincipledMaterial {
                baseColor: "red"
            }
        }
        // Model {
        //     x: 100
        //     source: "file:///C:/Users/Bella/Documents/AssetBrowser/meshes/lod3spShape_mesh.mesh"
        //     //scale: Qt.vector3d(100, 100, 100) // adjust scale if tiny
        //     materials: [
        //         PrincipledMaterial {
        //             baseColor: "white"
        //         }
        //     ]
        // }
    }
}
