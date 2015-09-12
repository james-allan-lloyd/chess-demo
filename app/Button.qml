import QtQuick 2.0

Rectangle {
    property alias text: buttonLabel.text
    signal clicked
    id: root
    color: enabled ? "grey" : "lightGrey"

    Text {
        id: buttonLabel
        anchors.centerIn: parent
        color: root.enabled ? "black" : "grey"
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        Component.onCompleted: onClicked.connect(root.clicked)
    }
}

