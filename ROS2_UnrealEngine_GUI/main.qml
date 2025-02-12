import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    maximumHeight: 300
    maximumWidth: 600
    minimumHeight: maximumHeight
    minimumWidth: maximumWidth
    visible: true
    title: qsTr("ROS2 Unreal Engine 5 GUI Application")

    Rectangle{
            id: udrl
            x: 50
            y: 90
            width: 132
            height: 112

            Image {
                id: upper
                x:  50
                y:  0
                source: "/images/up.png"
                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        upper.opacity = 0.5
                        _rosNode.buttonCallback(0)
                    }
                    onReleased: upper.opacity  = 1.0
                }
            }

            Image {
                id: down
                x: 50
                y: 80
                source: "/images/down.png"
                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        down.opacity  = 0.5
                        _rosNode.buttonCallback(1)
                    }
                    onReleased: down.opacity  = 1.0
                }
            }

            Image {
                id: left
                x:  0
                y:  40
                source: "/images/left.png"
                MouseArea{
                    anchors.fill: parent
                    onPressed:  {
                        left.opacity = 0.5
                        _rosNode.buttonCallback(2)
                    }
                    onReleased: left.opacity = 1.0
                }
            }

            Image {
                id: right
                x:  100
                y:  40
                opacity: 1.0
                source: "/images/right.png"
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        right.opacity  = 0.5
                        _rosNode.buttonCallback(3)
                    }
                    onReleased: right.opacity  = 1.0
                }
            }
    }
    Image {
        id: cameraView
        x:  150
        y:  75
        width: parent.width
        height: parent.height / 2
        fillMode: Image.PreserveAspectFit
        source: "image://rosimage"

        Connections {
            target: _rosNode
            onImageReceived: cameraView.source = "image://rosimage?" + Date.now();
        }
    }
}
