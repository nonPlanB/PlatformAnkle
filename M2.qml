import Qt3D.Core 2.15
import Qt3D.Render 2.15

Entity {
    property Material material

    Mesh {
        id: m2Mesh
        source: "assets/obj/M.obj"
    }
    Transform {
        id:m2MeshTransform
        matrix: {
            var m = Qt.matrix4x4();
            m.scale(0.2);
            m.rotate(90, Qt.vector3d(0, 1, 0));
            m.rotate(m2slider.value, Qt.vector3d(0, 1, 0));
            return m;
        }
    }
    components: [
        m2Mesh,
        m2MeshTransform,
        material
    ]
}
