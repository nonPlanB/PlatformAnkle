import QtQuick 2.15
import Qt3D.Core 2.15
import Qt3D.Render 2.15

Material {
    id: root
    property color ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
    property color diffuseColor: Qt.rgba(0.7, 0.7, 0.9, 1.0)
    property color specularColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
//    property color ambientColor: Qt.rgba(245/255,192/255,173/255, 1.0)

    property real shininess: 150.0
//    property real shininess: 100.0

    parameters: [
        Parameter { name: "ka"; value: Qt.vector3d(root.ambientColor.r, root.ambientColor.g, root.ambientColor.b) },
        Parameter { name: "kd"; value: Qt.vector3d(root.diffuseColor.r, root.diffuseColor.g, root.diffuseColor.b) },
        Parameter { name: "ks"; value: Qt.vector3d(root.specularColor.r, root.specularColor.g, root.specularColor.b) },
        Parameter { name: "shininess"; value: root.shininess }
    ]
}
