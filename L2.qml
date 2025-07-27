import Qt3D.Core 2.15
import Qt3D.Render 2.15
import "MyFunction.js" as MF

Entity {
    property Material material

    Mesh {
        id: l2Mesh
        source: "assets/obj/L.obj"
    }
    Transform {
        id:l2MeshTransform
        matrix: {
            var m = Qt.matrix4x4()
            m.scale(0.2)
            m.rotate(90+m2slider.value, Qt.vector3d(0, 1, 0));
            m.rotate(30, Qt.vector3d(1, 0, 0));
            m.rotate(MF.findL2(pxAngle, pzAngle, pyAngle), Qt.vector3d(0, 1, 0));
            return m
        }
    }
    components: [
        l2Mesh,
        l2MeshTransform,
        material
    ]
}
