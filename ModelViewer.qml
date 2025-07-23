import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick3D 6.9

Item {
    id: root
    width: 640
    height: 480

    // New property to be set from C++
    property url modelSource: ""

    View3D {
        anchors.fill: parent
        environment: SceneEnvironment {
            backgroundMode: SceneEnvironment.Color
            clearColor: "#2b2b2b"
        }

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 0, 600)
        }

        DirectionalLight {
            rotation: Qt.vector3d(-45, 0, 0)
        }

        Model {
            id: assetModel
            source: "#Sphere"
            scale: Qt.vector3d(100, 100, 100)
        }
    }
}
