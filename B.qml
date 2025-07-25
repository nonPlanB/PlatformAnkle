import Qt3D.Core 2.15
import Qt3D.Render 2.15

Entity {
    property Material material

    Mesh {
        id: bMesh
        source: "assets/obj/B.obj"
    }
    Transform {
        id:bMeshTransform
        matrix: {
            var m = Qt.matrix4x4();
            m.scale(0.2);
            m.rotate(-90, Qt.vector3d(1, 0, 0));
            return m;
        }
    }
    components: [
        bMesh,
        bMeshTransform,
        material
    ]
}
