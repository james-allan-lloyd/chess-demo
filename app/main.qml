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

    function setupBoard()
    {
        boardModel.clearPieces()
        boardModel.createPawn(0,0)
    }

    Text {
        id: currentPiece
        text: boardView.selectedPiece ? "Selected: " + boardView.selectedPiece.objectName : "Select piece"
        width: 50
        height: 20
        anchors.bottom: parent.bottom
    }

    Rectangle {
        id: startButton
        color: "grey"
        width: 50
        height: 20

        Text {
            id: buttonLabel
            anchors.centerIn: parent
            text: "Start"
        }

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            onClicked: setupBoard()
        }
    }

}

