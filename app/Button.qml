import QtQuick 2.0

Rectangle {
    property alias text: buttonLabel.text
    signal clicked
    id: root
    color: "grey"

    Text {
        id: buttonLabel
        anchors.centerIn: parent
    }

    MouseArea {
        id: buttonMouseArea
        anchors.fill: parent
        Component.onCompleted: onClicked.connect(root.clicked)
    }
}

