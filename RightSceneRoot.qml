import QtQuick 2.15
import Qt3D.Core 2.15
import Qt3D.Render 2.15
import Qt3D.Input 2.15
import Qt3D.Extras 2.15

Entity {
    id: rightSceneRoot
    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 65
        nearPlane: 0.1
        farPlane: 1000.0
        // position: Qt.vector3d(70.0, 20.0, -90.0)
        position: Qt.vector3d(70.0, 30.0, 0.0)
        viewCenter: Qt.vector3d(0.0, -70.0, 0.0)
        upVector: Qt.vector3d(0.0, 1.0, 0.0)//y轴向上
    }

    OrbitCameraController {
        camera: camera
        linearSpeed: 100
        lookSpeed: 400
    }

    ShadowMapLight {
        id: light
    }

    components: [
        ShadowMapFrameGraph {
            id: framegraph
            viewCamera: camera
            lightCamera: light.lightCamera
        },
        InputSettings { }
    ]

    AdsEffect {
        id: shadowMapEffect

        shadowTexture: framegraph.shadowTexture
        light: light
    }

    B {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    P {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    M1 {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    M2 {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    M3 {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    L1 {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    L2 {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
    L3 {
        material: AdsMaterial {
            effect: shadowMapEffect
            diffuseColor: Qt.rgba(208/255, 147/255, 121/255, 1.0)
            shininess: 275
        }
    }
}
