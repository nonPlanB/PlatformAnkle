import Qt3D.Core 2.15
import Qt3D.Render 2.15
import "MyFunction.js" as MF //只能全部大写

Entity {
    property Material material

    Mesh {
        id: pMesh
        source: "assets/obj/P.obj"
    }
    Transform {
        id:pMeshTransform
        property real y: yawslider.value
        property real z: pitchslider.value
        property real x: rollslider.value
        matrix: {
            var m = Qt.matrix4x4();
            m.scale(0.2);
            // var q = MF.quaternionToMatrix4x4(fromEulerAngles(x, y, z));
            // m = m.times(q);
            m.rotate(z, Qt.vector3d(0, 0, 1));
            m.rotate(y, Qt.vector3d(0, 1, 0));
            m.rotate(x, Qt.vector3d(1, 0, 0));
            return m;
        }
    }
    components: [
        pMesh,
        pMeshTransform,
        material
    ]
}
