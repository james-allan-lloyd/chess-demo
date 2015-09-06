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
        boardModel.createPawn(4, 0, Chess.BoardModel.BLACK)
        boardModel.createPawn(4, 7, Chess.BoardModel.WHITE)
        boardModel.createPawn(0, 0, Chess.BoardModel.BLACK)
        boardModel.createPawn(1, 1, Chess.BoardModel.BLACK)
        boardModel.create("Queen", 4, 4, Chess.BoardModel.WHITE)
    }

    Text {
        id: currentPiece
        text: boardView.selectedPiece ? "Selected: " + boardView.selectedPiece.objectName : "Select piece"
        width: 50
        height: 20
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }

    Text {
        id: currentPlayer
        text: boardView.currentPlayer
        anchors.right: parent.right
        anchors.top: parent.top
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

