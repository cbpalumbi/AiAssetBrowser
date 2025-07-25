import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick3D 6.9

Item {
    id: root
    width: 640
    height: 480

    // New property to be set from C++
    property url modelComponentSource: ""

    View3D {
        id: view3d
        anchors.fill: parent
        environment: SceneEnvironment {
            backgroundMode: SceneEnvironment.Color
            clearColor: "#ffffff"
        }

        PerspectiveCamera {
            id: camera
            //position: Qt.vector3d(0, 0, 600) // back on the Z-axis
            z: 400
        }

        DirectionalLight {
            rotation: Qt.vector3d(-45, 0, 0)
        }

        Node {
            id: modelContainer
        }

        MouseArea {
            id: orbitControls
            anchors.fill: parent
            property real lastX: 0
            property real lastY: 0
            property real yaw: 0
            property real pitch: 0
            property real distance: 800

            onPressed: {
                lastX = mouse.x
                lastY = mouse.y
            }

            onPositionChanged: {
                const dx = mouse.x - lastX
                const dy = mouse.y - lastY
                lastX = mouse.x
                lastY = mouse.y

                yaw -= dx * 0.5
                pitch = Math.max(-90, Math.min(90, pitch - dy * 0.5))
                updateCamera()
            }

            onWheel: {
                distance = Math.max(100, distance - wheel.angleDelta.y)
                updateCamera()
            }

            function updateCamera() {
                const rYaw = yaw * Math.PI / 180
                const rPitch = pitch * Math.PI / 180

                const x = distance * Math.cos(rPitch) * Math.sin(rYaw)
                const y = distance * Math.sin(rPitch)
                const z = distance * Math.cos(rPitch) * Math.cos(rYaw)

                camera.position = Qt.vector3d(x, y, z)
                camera.lookAt(Qt.vector3d(0, 0, 0))
            }

            Component.onCompleted: updateCamera()
        }

        Loader {
           id: modelLoader
           source: modelComponentSource
           asynchronous: false
           onLoaded: {
               if (item && item instanceof Node) {
                   item.parent = modelContainer
                   item.position = Qt.vector3d(0, 0, 0)
               } else {
                   console.warn("ModelViewer.qml: Loaded item is not a Node")
               }
           }
       }
    }
}
