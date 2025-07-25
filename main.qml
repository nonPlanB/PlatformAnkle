import QtQuick 2.15
import QtQuick.Scene3D 2.15
import QtQuick.Controls 2.15
import "MyFunction.js" as MF
import QtCharts 2.15
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: root
    width: 1600
    height: 900
    visible: true
    title: "Afoot 上位机"
    property bool mVisible: false
    property real m1Value: 0
    property real m2Value: 0
    property real m3Value: 0
    property var motorData: ({1: [], 2: [], 3: []})
    property int currentIndex: 0
    property int maxPoints: 100
    property font chartTitle: Qt.font({ family: "Calibri", pointSize: 16 })
    property font axisTitle: Qt.font({ family: "Calibri", pointSize: 14 })
    property var chData: ({1: [], 2: [], 3: []})
    Rectangle {
        id: controlsbg
        width: 200
        height: parent.height
        color: "#EDF7FF"
        Column {
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.topMargin: 5
            spacing: 10
            Rectangle {
                id: yaw
                width: 180
                height: 60
                color: "#EDF7FF"
                radius: 12
                Text {
                    id: yawSliderText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "Yaw: " + yawslider.value
                    font.family: "Calibri"
                    color: "black"
                    font.pointSize: 13
                }
                Slider {
                    id: yawslider
                    anchors.fill: parent
                    anchors.topMargin: 30
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    handle.scale: 0.7
                    value: 0
                    from: -30
                    to: 30
                    stepSize: 1
                }
            }
            Rectangle {
                id: pitch
                width: 180
                height: 60
                color: "#EDF7FF"
                radius: 12
                Text {
                    id: pitchSliderText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "Pitch: " + pitchslider.value
                    font.family: "Calibri"
                    color: "black"
                    font.pointSize: 13
                }
                Slider {
                    id: pitchslider
                    anchors.fill: parent
                    anchors.topMargin: 30
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    handle.scale: 0.7
                    value: 0
                    from: -10
                    to: 10
                    stepSize: 1
                }
            }
            Rectangle {
                id: roll
                width: 180
                height: 60
                color: "#EDF7FF"
                radius: 12
                Text {
                    id: rollSliderText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "Roll: " + rollslider.value
                    font.family: "Calibri"
                    color: "black"
                    font.pointSize: 13
                }
                Slider {
                    id: rollslider
                    anchors.fill: parent
                    anchors.topMargin: 30
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    handle.scale: 0.7
                    value: 0
                    from: -10
                    to: 10
                    stepSize: 1
                }
            }
            Rectangle {
                id: m1
                width: 180
                height: 60
                color: "#EDF7FF"
                radius: 12
                visible: mVisible
                Text {
                    id: m1SliderText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "M1: " + m1slider.value
                    font.family: "Calibri"
                    color: "black"
                    font.pointSize: 13
                }
                Slider {
                    id: m1slider
                    anchors.fill: parent
                    anchors.topMargin: 30
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    handle.scale: 0.7
                    enabled: false
                    value: m1Value
                    from: -120
                    to: 120
                    stepSize: 1
                }
            }
            Rectangle {
                id: m2
                width: 180
                height: 60
                color: "#EDF7FF"
                radius: 12
                visible: mVisible
                Text {
                    id: m2SliderText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "M2: " + m2slider.value
                    font.family: "Calibri"
                    color: "black"
                    font.pointSize: 13
                }
                Slider {
                    id: m2slider
                    anchors.fill: parent
                    anchors.topMargin: 30
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    handle.scale: 0.7
                    enabled: false
                    value: m2Value
                    from: -120
                    to: 120
                    stepSize: 1
                }
            }
            Rectangle {
                id: m3
                width: 180
                height: 60
                color: "#EDF7FF"
                radius: 12
                visible: mVisible
                Text {
                    id: m3SliderText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    text: "M3: " + m3slider.value
                    font.family: "Calibri"
                    color: "black"
                    font.pointSize: 13
                }
                Slider {
                    id: m3slider
                    anchors.fill: parent
                    anchors.topMargin: 30
                    anchors.rightMargin: 10
                    anchors.leftMargin: 10
                    handle.scale: 0.7
                    enabled: false
                    value: m3Value
                    from: -120
                    to: 120
                    stepSize: 1
                }
            }
            Binding {
                target: root
                property: "m1Value"
                value: {
                    var p1 = Qt.vector4d(-75, -150*Math.sqrt(3), -75*Math.sqrt(3), 1);
                    return MF.findM1(p1, rollslider.value, yawslider.value, pitchslider.value);
                }
            }
            Binding {
                target: root
                property: "m2Value"
                value: {
                    var p2 = Qt.vector4d(-75, -150*Math.sqrt(3), 75*Math.sqrt(3), 1);
                    return MF.findM2(p2, rollslider.value, yawslider.value, pitchslider.value);
                }
            }
            Binding {
                target: root
                property: "m3Value"
                value: {
                    var p3 = Qt.vector4d(150, -150*Math.sqrt(3), 0, 1);
                    return MF.findM3(p3, rollslider.value, yawslider.value, pitchslider.value);
                }
            }
        }
    }
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 200
        anchors.topMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Connections {
                target: pyHandler
                // onEmgValueReceived:{}已经弃用
                function onEmgValueReceived(channel, value) {
                    root.currentIndex++
                    // 更新缓存
                    if (!chData[channel])
                        chData[channel] = []
                    chData[channel].push({x: root.currentIndex, y: value})
                    if (chData[channel].length > root.maxPoints)
                        chData[channel].shift()
                    var series;
                    if (channel === 1) series = ch1Series;
                    else if (channel === 2) series = ch2Series;
                    else if (channel === 3) series = ch3Series;
                    else return;
                    // 更新 series
                    series.clear()
                    for (var i = 0; i < chData[channel].length; i++)
                        series.append(chData[channel][i].x, chData[channel][i].y);
                    xAxis.min = Math.max(0, root.currentIndex - root.maxPoints + 1);
                    xAxis.max = root.currentIndex;
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ChartView {
                    id: emgChartView
                    anchors.fill: parent
                    title: "EMG"
                    backgroundColor: "#EDF7FF"
                    antialiasing: true
                    legend.visible: true
                    titleFont: chartTitle
                    legend.font: Qt.font({ family: "Calibri", pointSize: 12 })
                    ValueAxis {
                        id: xAxis
                        titleText: "Times"
                        titleFont: axisTitle
                        labelsFont: Qt.font({ family: "Calibri", pointSize: 10 })
                    }

                    ValueAxis {
                        id: yAxis
                        min: -200
                        max: 200
                        titleText: "Values"
                        titleFont: axisTitle
                        labelsFont: Qt.font({ family: "Calibri", pointSize: 10 })
                    }

                    LineSeries {
                        id: ch1Series
                        name: "CH1"
                        axisX: xAxis
                        axisY: yAxis
                    }
                    LineSeries {
                        id: ch2Series
                        name: "CH2"
                        axisX: xAxis
                        axisY: yAxis
                    }
                    LineSeries {
                        id: ch3Series
                        name: "CH3"
                        axisX: xAxis
                        axisY: yAxis
                    }
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ChartView {
                    id: mChartView
                    anchors.fill: parent
                    title: "Motor Angle"
                    backgroundColor: "#EDF7FF"
                    antialiasing: true
                    legend.visible: true
                    titleFont: chartTitle
                    legend.font: Qt.font({ family: "Calibri", pointSize: 12 })
                    ValueAxis {
                        id: xAxis1
                        titleText: "Times"
                        titleFont: axisTitle
                        labelsFont: Qt.font({ family: "Calibri", pointSize: 10 })
                    }
                    ValueAxis {
                        id: yAxis1
                        min: -80
                        max: 80
                        titleText: "Angle"
                        titleFont: axisTitle
                        labelsFont: Qt.font({ family: "Calibri", pointSize: 10 })
                    }
                    LineSeries {
                        id: m1Series
                        name: "M1"
                        axisX: xAxis1
                        axisY: yAxis1
                    }
                    LineSeries {
                        id: m2Series
                        name: "M2"
                        axisX: xAxis1
                        axisY: yAxis1
                    }
                    LineSeries {
                        id: m3Series
                        name: "M3"
                        axisX: xAxis1
                        axisY: yAxis1
                    }
                }
                Timer {
                    id: updateTimer
                    interval: 50
                    running: true
                    repeat: true
                    onTriggered: {
                        root.currentIndex++
                        motorData[1].push({x: root.currentIndex, y: m1slider.value})
                        motorData[2].push({x: root.currentIndex, y: m2slider.value})
                        motorData[3].push({x: root.currentIndex, y: m3slider.value})
                        if (motorData[1].length > root.maxPoints) {
                            motorData[1].shift()
                            motorData[2].shift()
                            motorData[3].shift()
                        }
                        m1Series.clear()
                        m2Series.clear()
                        m3Series.clear()
                        for (var i = 0; i < motorData[1].length; ++i) {
                            m1Series.append(motorData[1][i].x, motorData[1][i].y)
                            m2Series.append(motorData[2][i].x, motorData[2][i].y)
                            m3Series.append(motorData[3][i].x, motorData[3][i].y)
                        }
                        xAxis1.min = Math.max(0, root.currentIndex - root.maxPoints + 1)
                        xAxis1.max = root.currentIndex
                    }

                }
            }
        }
        Scene3D {
            id: rightScene3d
            Layout.fillWidth: true
            Layout.fillHeight: true
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
            RightSceneRoot {
                id: rightRoot
            }
        }
    }
}
