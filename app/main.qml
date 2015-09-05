import QtQuick 2.3
import QtQuick.Window 2.2
import com.luxoft.Chess 1.0 as Chess

Window {
    visible: true
    property var model: Chess.Chess

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

