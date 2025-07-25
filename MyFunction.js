.pragma library

var ld = 156;//杆长
var bd = -280;//基座y轴
var lf = 0,rg = 360,wu = 8;

// // 将四元数转换为 4x4 矩阵
// function quaternionToMatrix4x4(q) {
//     let w = q.scalar; // 四元数的标量部分 (w)
//     let x = q.x;      // 向量部分的 x
//     let y = q.y;      // 向量部分的 y
//     let z = q.z;      // 向量部分的 z
//     // 归一化四元数
//     let len = Math.sqrt(w * w + x * x + y * y + z * z);
//     if (len !== 0) {
//         w /= len;
//         x /= len;
//         y /= len;
//         z /= len;
//     }
//     // 计算 4x4 旋转矩阵的元素
//     let m11 = 1 - 2 * (y * y + z * z);
//     let m12 = 2 * (x * y - z * w);
//     let m13 = 2 * (x * z + y * w);
//     let m14 = 0;

//     let m21 = 2 * (x * y + z * w);
//     let m22 = 1 - 2 * (x * x + z * z);
//     let m23 = 2 * (y * z - x * w);
//     let m24 = 0;

//     let m31 = 2 * (x * z - y * w);
//     let m32 = 2 * (y * z + x * w);
//     let m33 = 1 - 2 * (x * x + y * y);
//     let m34 = 0;

//     let m41 = 0;
//     let m42 = 0;
//     let m43 = 0;
//     let m44 = 1;
//     // 返回 Matrix4x4 对象
//     return Qt.matrix4x4(
//                 m11, m12, m13, m14,
//                 m21, m22, m23, m24,
//                 m31, m32, m33, m34,
//                 m41, m42, m43, m44
//                 );
// }

// 动平台变换
function transForm(p, x, y, z) {
    var t = Qt.matrix4x4();
    t.rotate(z, Qt.vector3d(0, 0, 1));
    t.rotate(y, Qt.vector3d(0, 1, 0));
    t.rotate(x, Qt.vector3d(1, 0, 0));
    var p1 = t.times(p);
    var p2 = p1.toVector3d();
    return p2;
}

function findM1(p, x, y, z) {
    var pp1 = transForm(p, x, y, z);
    function computeDistance(m1) {
        var b = Qt.vector3d(150 * Math.sin((150 + m1) * Math.PI / 180), bd, 150 * Math.cos((150 + m1) * Math.PI / 180));
        return Math.sqrt(Math.pow(pp1.x - b.x, 2) + Math.pow(pp1.y - b.y, 2) + Math.pow(pp1.z - b.z, 2));
    }
    var left = -120;
    var right = 120;
    var maxIterations = 20;
    for (var i = 0; i < maxIterations; i++) {
        var mid = (left + right) / 2;
        var dist = computeDistance(mid);
        if (Math.abs(dist - ld) < 0.1) {
            return Math.round(mid);
        }
        var distLeft = computeDistance(left);
        if ((distLeft - ld) * (dist - ld) < 0) {
            right = mid;
        } else {
            left = mid;
        }
    }
    console.log("未找到m1解");
    return 0;
}

function findM2(p, x, y, z) {
    var pp2 = transForm(p, x, y, z);
    function computeDistance(m2) {
        var b = Qt.vector3d(150 * Math.sin((270 + m2) * Math.PI / 180), bd, 150 * Math.cos((270 + m2) * Math.PI / 180));
        return Math.sqrt(Math.pow(pp2.x - b.x, 2) + Math.pow(pp2.y - b.y, 2) + Math.pow(pp2.z - b.z, 2));
    }
    var left = -120;
    var right = 120;
    var maxIterations = 20;
    for (var i = 0; i < maxIterations; i++) {
        var mid = (left + right) / 2;
        var dist = computeDistance(mid);
        if (Math.abs(dist - ld) < 0.1) {
            return Math.round(mid);
        }
        var distLeft = computeDistance(left);
        if ((distLeft - ld) * (dist - ld) < 0) {
            right = mid;
        } else {
            left = mid;
        }
    }
    console.log("未找到m2解");
    return 0;
}

// 求解坐标
function findM3(p, x, y, z) {
    var pp3 = transForm(p, x, y, z);
    function computeDistance(m3) {
        var b = Qt.vector3d(150 * Math.sin((30 + m3) * Math.PI / 180), bd, 150 * Math.cos((30 + m3) * Math.PI / 180));
        return Math.sqrt(Math.pow(pp3.x - b.x, 2) + Math.pow(pp3.y - b.y, 2) + Math.pow(pp3.z - b.z, 2));
    }
    var left = -120;
    var right = 120;
    var maxIterations = 20;
    // 二分法
    for (var i = 0; i < maxIterations; i++) {
        var mid = (left + right) / 2;
        var dist = computeDistance(mid);
        if (Math.abs(dist - ld) < 0.1) {
            return Math.round(mid);
        }
        var distLeft = computeDistance(left);
        if ((distLeft - ld) * (dist - ld) < 0) {
            right = mid;
        } else {
            left = mid;
        }
    }
    console.log("未找到m3解");
    return 0;
}

function findL1(x, y, z) {
    var p = Qt.vector4d(-75, -150*Math.sqrt(3), -75*Math.sqrt(3), 1);
    var pp = transForm(p, x, y, z);
    var m = findM1(p, x, y, z);
    var l = Qt.vector4d(150, -150*Math.sqrt(3), 0, 1);
    function computeDistance(a) {
        var t = Qt.matrix4x4();
        t.rotate(-30+m, Qt.vector3d(0, 1, 0));
        t.rotate(30, Qt.vector3d(1, 0, 0));
        t.rotate(a, Qt.vector3d(0, 1, 0));
        var ll = t.times(l);
        var ll1 = ll.toVector3d();
        return Math.sqrt(Math.pow(pp.x - ll1.x, 2) + Math.pow(pp.y - ll1.y, 2) + Math.pow(pp.z - ll1.z, 2));
    }

    for (var i = lf; i < rg; i++) {
        var d = computeDistance(i);
        // console.log("l1:",d,"i=",i)
        if (d < wu) {
            return i;
        }
    }

    console.log("未找到l1解");
    return 0;
}

function findL2(x, y, z) {
    var p = Qt.vector4d(-75, -150*Math.sqrt(3), 75*Math.sqrt(3), 1);
    var pp = transForm(p, x, y, z);
    var m = findM2(p, x, y, z);
    var l = Qt.vector4d(150, -150*Math.sqrt(3), 0, 1);
    function computeDistance(a) {
        var t = Qt.matrix4x4();
        t.rotate(90+m, Qt.vector3d(0, 1, 0));
        t.rotate(30, Qt.vector3d(1, 0, 0));
        t.rotate(a, Qt.vector3d(0, 1, 0));
        var ll = t.times(l);
        var ll1 = ll.toVector3d();
        return Math.sqrt(Math.pow(pp.x - ll1.x, 2) + Math.pow(pp.y - ll1.y, 2) + Math.pow(pp.z - ll1.z, 2));
    }

    for (var i = lf; i < rg; i++) {
        var d = computeDistance(i);
        // console.log("l2::",d,"i==",i)
        if (d < wu) {
            return i;
        }
    }

    console.log("未找到l2解");
    return 0;
}

function findL3(x, y, z) {
    var p = Qt.vector4d(150, -150*Math.sqrt(3), 0, 1);
    var pp = transForm(p, x, y, z);
    var m = findM3(p, x, y, z);
    var l = Qt.vector4d(150, -150*Math.sqrt(3), 0, 1);
    function computeDistance(a) {
        var t = Qt.matrix4x4();
        t.rotate(-150+m, Qt.vector3d(0, 1, 0));
        t.rotate(30, Qt.vector3d(1, 0, 0));
        t.rotate(a, Qt.vector3d(0, 1, 0));
        var ll = t.times(l);
        var ll1 = ll.toVector3d();
        return Math.sqrt(Math.pow(pp.x - ll1.x, 2) + Math.pow(pp.y - ll1.y, 2) + Math.pow(pp.z - ll1.z, 2));
    }

    for (var i = lf; i < rg; i++) {
        var d = computeDistance(i);
        // console.log("l3:::",d,"i===",i)
        if (d < wu) {
            return i;
        }
    }

    console.log("未找到l3解");
    return 0;
}
