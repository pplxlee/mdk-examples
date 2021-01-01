import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import MDKPlayer 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property string url


    MDKPlayer {
        id: player
        source: url
        anchors.fill: parent

        Component.onCompleted: player.play()
        onSourceChanged: player.play()

        MouseArea {
            id: mouseArea
            x: 0
            y: 0
            width: 640
            height: 480
            onClicked: player.snapshot()
        }
    }


}
