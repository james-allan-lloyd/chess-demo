import QtQuick 2.3
import QtQuick.Window 2.2
import com.luxoft.Chess 1.0 as Chess

Window {
    visible: true
    title: "Chess 1.0"
    // property var model: Chess.BoardModel
    width: 500
    height: 500

    Chess.BoardModel {
        id: boardModel
    }

    Chess.BoardView {
        id: boardView
        model: boardModel
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("Create pawn")
            boardModel.createPawn(0,0)
        }
    }

    // Text {
    //     text: qsTr("Hello World")
    //     anchors.centerIn: parent
    // }
}

