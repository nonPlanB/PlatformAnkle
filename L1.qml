import Qt3D.Core 2.15
import Qt3D.Render 2.15
import "MyFunction.js" as MF

Entity {
    property Material material

    Mesh {
        id: l1Mesh
        source: "assets/obj/L.obj"
    }
    Transform {
        id:l1MeshTransform
        matrix: {
            var m = Qt.matrix4x4()
            m.scale(0.2)
            m.rotate(-30+m1slider.value, Qt.vector3d(0, 1, 0));
            m.rotate(30, Qt.vector3d(1, 0, 0));
            m.rotate(MF.findL1(rollslider.value, yawslider.value, pitchslider.value), Qt.vector3d(0, 1, 0));
            return m
        }
    }
    components: [
        l1Mesh,
        l1MeshTransform,
        material
    ]
}
