import QtQuick 2.3
import QtQuick.Window 2.2
import com.mycompany.qmlcomponents 1.0 as Components

Window {
    visible: true
    property var model: Components.Chess

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
}

